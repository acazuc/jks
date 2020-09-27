#include "hash_table.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>

#define REHASH_LOW_THRESHOLD .25
#define REHASH_LOW_SIZE .5

#define REHASH_HIGH_THRESHOLD .5
#define REHASH_HIGH_SIZE 2

void jks_hash_table_init(jks_hash_table_t *hash_table, uint32_t data_size, jks_hash_table_destructor_t destructor)
{
	jks_array_init(&hash_table->array, sizeof(jks_list_t), (jks_array_destructor_t)jks_list_destroy);
	hash_table->size = 0;
	hash_table->data_size = data_size;
	hash_table->destructor = destructor;
}

void jks_hash_table_destroy(jks_hash_table_t *hash_table)
{
	jks_array_destroy(&hash_table->array);
}

static jks_list_t *get_bucket(jks_array_t *array, uint32_t hash)
{
	uint32_t bucket = hash & array->size;
	return jks_array_get(array, bucket);
}

static bool rehash(jks_hash_table_t *hash_table, uint32_t size)
{
	jks_array_t new_array;
	jks_array_init(&new_array, sizeof(jks_list_t), (jks_array_destructor_t)jks_list_destroy);
	if (!jks_array_resize(&new_array, size))
		return false;
	JKS_ARRAY_FOREACH(iterator, &new_array)
	{
		jks_list_init(jks_array_iterator_get(&iterator), hash_table->data_size, hash_table->destructor);
	}
	JKS_HASH_TABLE_FOREACH(iterator, hash_table)
	{
		uint32_t hash = jks_hash_table_iterator_get_hash(&iterator);
		jks_list_t *bucket = get_bucket(&new_array, hash);
		void *data = jks_list_push_back(bucket, jks_hash_table_iterator_get(&iterator));
		if (!data)
		{
			jks_array_destroy(&new_array);
			return false;
		}
		*(uint32_t*)data = hash;
		memmove((uint8_t*)data + sizeof(uint32_t), jks_hash_table_iterator_get(&iterator), hash_table->data_size);
	}
	jks_array_destroy(&hash_table->array);
	hash_table->array = new_array;
	return true;
}

bool jks_hash_table_reserve(jks_hash_table_t *hash_table, uint32_t capacity)
{
	if (hash_table->array.size >= capacity)
		return true;
	return rehash(hash_table, capacity);
}

void *jks_hash_table_get(jks_hash_table_t *hash_table, uint32_t hash)
{
	if (!hash_table->size)
		return NULL;
	jks_list_t *bucket = get_bucket(&hash_table->array, hash);
	if (!bucket)
		return NULL;
	JKS_LIST_FOREACH(iterator, bucket)
	{
		void *data = jks_list_iterator_get(&iterator);
		uint32_t h = *(uint32_t*)data;
		if (h != hash)
			continue;
		return ((uint8_t*)data) + sizeof(uint32_t);
	}
	return NULL;
}

void *jks_hash_table_set(jks_hash_table_t *hash_table, uint32_t hash, void *data)
{
	jks_list_t *bucket = get_bucket(&hash_table->array, hash);
	if (bucket)
	{
		JKS_LIST_FOREACH(iterator, bucket)
		{
			void *iter_data = jks_list_iterator_get(&iterator);
			uint32_t h = *(uint32_t*)iter_data;
			if (h != hash)
				continue;
			void *dst = (uint8_t*)iter_data + sizeof(uint32_t);
			if (data)
				memmove(dst, data, hash_table->data_size);
			return dst;
		}
	}
	if (hash_table->size + 1 > hash_table->array.size * REHASH_HIGH_THRESHOLD)
	{
		uint32_t rehash_size = hash_table->array.size * REHASH_HIGH_SIZE;
		if (rehash_size == 0)
			rehash_size = 32;
		if (!rehash(hash_table, rehash_size))
			return NULL;
		bucket = get_bucket(&hash_table->array, hash);
		if (!bucket)
			return NULL;
	}
	void *list_data = jks_list_push_back(bucket, NULL);
	if (!list_data)
		return NULL;
	*(uint32_t*)list_data = hash;
	void *dst = (uint8_t*)list_data + sizeof(uint32_t);
	if (data)
		memmove(dst, data, hash_table->data_size);
	hash_table->size++;
	return dst;
}

bool jks_hash_table_erase(jks_hash_table_t *hash_table, uint32_t hash)
{
	if (!hash_table->size)
		return true;
	jks_list_t *bucket = get_bucket(&hash_table->array, hash);
	JKS_LIST_FOREACH(iterator, bucket)
	{
		void *data = jks_list_iterator_get(&iterator);
		uint32_t h = *(uint32_t*)data;
		if (h != hash)
			continue;
		jks_list_iterator_erase(bucket, &iterator);
		hash_table->size--;
		break;
	}
	if (hash_table->size < hash_table->array.size * REHASH_LOW_THRESHOLD)
		return rehash(hash_table, hash_table->array.size * REHASH_LOW_SIZE);
	return true;
}

jks_hash_table_iterator_t jks_hash_table_iterator_begin(jks_hash_table_t *hash_table)
{
	jks_hash_table_iterator_t iterator;
	JKS_ARRAY_FOREACH(array_iter, &hash_table->array)
	{
		jks_list_t *list = jks_array_iterator_get(&array_iter);
		iterator.list_iterator = jks_list_iterator_begin(list);
		if (!jks_list_iterator_is_end(list, &iterator.list_iterator))
		{
			iterator.array_iterator = array_iter;
			return iterator;
		}
	}
	return jks_hash_table_iterator_end(hash_table);
}

jks_hash_table_iterator_t jks_hash_table_iterator_end(jks_hash_table_t *hash_table)
{
	jks_hash_table_iterator_t iterator;
	iterator.array_iterator = jks_array_iterator_end(&hash_table->array);
	return iterator;
}

jks_hash_table_iterator_t jks_hash_table_iterator_find(jks_hash_table_t *hash_table, uint32_t hash)
{
	if (!hash_table->size)
		return jks_hash_table_iterator_end(hash_table);
	uint32_t bucket_id = hash & hash_table->array.size;
	jks_list_t *bucket = jks_array_get(&hash_table->array, bucket_id);
	JKS_LIST_FOREACH(list_iterator, bucket)
	{
		void *data = jks_list_iterator_get(&list_iterator);
		uint32_t h = *(uint32_t*)data;
		if (h == hash)
		{
			jks_hash_table_iterator_t iterator;
			jks_array_iterator_find(&hash_table->array, bucket_id);
			iterator.list_iterator = list_iterator;
			return iterator;
		}
	}
	return jks_hash_table_iterator_end(hash_table);
}

void *jks_hash_table_iterator_get(jks_hash_table_iterator_t *iterator)
{
	return (uint8_t*)jks_list_iterator_get(&iterator->list_iterator) + sizeof(uint32_t);
}

uint32_t jks_hash_table_iterator_get_hash(jks_hash_table_iterator_t *iterator)
{
	return *(uint32_t*)jks_list_iterator_get(&iterator->list_iterator);
}

void jks_hash_table_iterator_erase(jks_hash_table_t *hash_table, jks_hash_table_iterator_t *iterator)
{
	jks_list_t *list = jks_array_iterator_get(&iterator->array_iterator);
	jks_list_iterator_erase(list, &iterator->list_iterator);
	hash_table->size--;
}

bool jks_hash_table_iterator_is_end(jks_hash_table_t *hash_table, jks_hash_table_iterator_t *iterator)
{
	return jks_array_iterator_is_end(&hash_table->array, &iterator->array_iterator);
}

void jks_hash_table_iterator_next(jks_hash_table_t *hash_table, jks_hash_table_iterator_t *iterator)
{
	jks_list_t *list = jks_array_iterator_get(&iterator->array_iterator);
	jks_list_iterator_next(&iterator->list_iterator);
	if (!jks_list_iterator_is_end(list, &iterator->list_iterator))
		return;
	jks_array_iterator_next(&iterator->array_iterator);
	while (!jks_array_iterator_is_end(&hash_table->array, &iterator->array_iterator))
	{
		jks_list_t *list = jks_array_iterator_get(&iterator->array_iterator);
		iterator->list_iterator = jks_list_iterator_begin(list);
		if (!jks_list_iterator_is_end(list, &iterator->list_iterator))
			return;
		jks_array_iterator_next(&iterator->array_iterator);
	}
}
