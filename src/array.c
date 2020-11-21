#include "array.h"
#include <string.h>
#include <stdlib.h>

void jks_array_init(jks_array_t *array, uint32_t data_size, jks_array_destructor_t destructor)
{
	array->data = NULL;
	array->size = 0;
	array->capacity = 0;
	array->data_size = data_size;
	array->destructor = destructor;
}

void jks_array_destroy(jks_array_t *array)
{
	if (array->destructor)
	{
		for (uint32_t i = 0; i < array->size; ++i)
			array->destructor(jks_array_get(array, i));
	}
	free(array->data);
}

static uint32_t npot(uint32_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

bool jks_array_resize(jks_array_t *array, uint32_t size)
{
	if (array->size == size)
		return true;
	if (size >= array->capacity)
	{
		uint32_t new_capacity = size;
		if (array->capacity * 2 > new_capacity)
			new_capacity = array->capacity * 2;
		if (!jks_array_reserve(array, npot(new_capacity)))
			return false;
	}
	if (size < array->size)
	{
		if (array->destructor)
		{
			for (uint32_t i = size; i < array->size; ++i)
				array->destructor(jks_array_get(array, i));
		}
	}
	array->size = size;
	return true;
}

void *jks_array_grow(jks_array_t *array, uint32_t size)
{
	if (!size)
		return NULL;
	if (!jks_array_resize(array, array->size + size))
		return NULL;
	return jks_array_get(array, array->size - size);
}

bool jks_array_reserve(jks_array_t *array, uint32_t capacity)
{
	if (array->capacity >= capacity)
		return true;
	void *new_data = realloc(array->data, capacity * array->data_size);
	if (!new_data)
		return false;
	array->data = new_data;
	array->capacity = capacity;
	return true;
}

bool jks_array_shrink(jks_array_t *array)
{
	if (!array->size)
	{
		free(array->data);
		array->data = NULL;
		array->capacity = 0;
		return true;
	}
	void *new_data = realloc(array->data, array->size * array->data_size);
	if (!new_data)
		return false;
	array->data = new_data;
	array->capacity = array->size;
	return true;
}

void *jks_array_push_front(jks_array_t *array, void *data)
{
	if (!jks_array_resize(array, array->size + 1))
		return NULL;
	void *dst = jks_array_get(array, 0);
	memmove(jks_array_get(array, 1), dst, (array->size - 1) * array->data_size);
	if (data)
		memmove(dst, data, array->data_size);
	return dst;
}

void *jks_array_push_back(jks_array_t *array, void *data)
{
	if (!jks_array_resize(array, array->size + 1))
		return NULL;
	void *dst = jks_array_get(array, array->size - 1);
	if (data)
		memmove(dst, data, array->data_size);
	return dst;
}

void *jks_array_push(jks_array_t *array, void *data, uint32_t offset)
{
	if (!jks_array_resize(array, array->size + 1))
		return NULL;
	void *dst = jks_array_get(array, offset);
	memmove(jks_array_get(array, offset + 1), dst, (array->size - offset - 1) * array->data_size);
	if (data)
		memmove(dst, data, array->data_size);
	return dst;
}

bool jks_array_erase(jks_array_t *array, uint32_t offset)
{
	if (offset >= array->size)
		return false;
	if (array->destructor)
		array->destructor(jks_array_get(array, offset));
	memmove(jks_array_get(array, offset), jks_array_get(array, offset + 1), (array->size - offset - 1) * array->data_size);
	array->size--;
	return true;
}

jks_array_iterator_t jks_array_iterator_begin(const jks_array_t *array)
{
	jks_array_iterator_t iter = {array->data, array->data_size};
	return iter;
}

jks_array_iterator_t jks_array_iterator_end(const jks_array_t *array)
{
	jks_array_iterator_t iter = {(uint8_t*)array->data + array->size * array->data_size, array->data_size};
	return iter;
}

jks_array_iterator_t jks_array_iterator_find(const jks_array_t *array, uint32_t offset)
{
	if (offset >= array->size)
		return jks_array_iterator_end(array);
	jks_array_iterator_t iter = {(uint8_t*)array->data + offset * array->data_size, array->data_size};
	return iter;
}

void *jks_array_iterator_get(const jks_array_iterator_t *iterator)
{
	return iterator->data;
}

void jks_array_iterator_erase(jks_array_t *array, const jks_array_iterator_t *iterator)
{
	if (array->destructor)
		array->destructor(iterator->data);
	uint32_t offset = ((uint8_t*)iterator->data - (uint8_t*)array->data) / array->data_size;
	memmove(iterator->data, (uint8_t*)iterator->data + array->data_size, (array->size - offset - 1) * array->data_size);
	array->size--;
}

bool jks_array_iterator_is_end(const jks_array_t *array, const jks_array_iterator_t *iterator)
{
	return iterator->data == (uint8_t*)array->data + array->size * array->data_size;
}

void jks_array_iterator_next(jks_array_iterator_t *iterator)
{
	iterator->data = (uint8_t*)iterator->data + iterator->data_size;
}
