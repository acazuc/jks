#include "hash_table.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void test_hash_table()
{
	jks_hash_table_t hash_table;
	jks_hash_table_init(&hash_table, sizeof(uint32_t), NULL);
	for (int i = 0; i < 500; ++i)
	{
		uint32_t val = 1000 + i;
		assert(jks_hash_table_set(&hash_table, i, &val));
	}
	assert(hash_table.size == 500);
	uint32_t old_size = hash_table.array.size;
	assert(jks_hash_table_reserve(&hash_table, 2000));
	assert(hash_table.array.size != old_size);
	for (int i = 0; i < 500; ++i)
	{
		void *data;
		assert((data = jks_hash_table_get(&hash_table, i)));
		assert(*(uint32_t*)data == 1000u + i);
	}
	uint32_t i = 0;
	JKS_HASH_TABLE_FOREACH(iter, &hash_table)
	{
		assert(jks_hash_table_iterator_get_hash(&iter) == i);
		assert(*(uint32_t*)jks_hash_table_iterator_get(&iter) == 1000 + i);
		++i;
	}
	assert(i == 500);
	jks_hash_table_destroy(&hash_table);
	printf("[OK] test_hash_table\n");
}
