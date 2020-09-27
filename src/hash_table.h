#ifndef JKS_HASH_TABLE_H
# define JKS_HASH_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

# include "array.h"
# include "list.h"

typedef void (*jks_hash_table_destructor_t)(void *data);

typedef struct jks_hash_table_iterator_s
{
	jks_array_iterator_t array_iterator;
	jks_list_iterator_t list_iterator;
} jks_hash_table_iterator_t;

typedef struct jks_hash_table_s
{
	jks_array_t array;
	uint32_t data_size;
	uint32_t size;
	jks_hash_table_destructor_t destructor;
} jks_hash_table_t;

void jks_hash_table_init(jks_hash_table_t *hash_table, uint32_t data_size, jks_hash_table_destructor_t destructor);
void jks_hash_table_destroy(jks_hash_table_t *hash_table);

bool jks_hash_table_reserve(jks_hash_table_t *hash_table, uint32_t capacity);

void *jks_hash_table_get(jks_hash_table_t *hash_table, uint32_t hash);
void *jks_hash_table_set(jks_hash_table_t *hash_table, uint32_t hash, void *data);
bool jks_hash_erase(jks_hash_table_t *hash_table, uint32_t hash);

jks_hash_table_iterator_t jks_hash_table_iterator_begin(jks_hash_table_t *hash_table);
jks_hash_table_iterator_t jks_hash_table_iterator_end(jks_hash_table_t *hash_table);
jks_hash_table_iterator_t jks_hash_table_iterator_find(jks_hash_table_t *hash_table, uint32_t hash);

void *jks_hash_table_iterator_get(jks_hash_table_iterator_t *iterator);
uint32_t jks_hash_table_iterator_get_hash(jks_hash_table_iterator_t *iterator);
void jks_hash_table_iterator_erase(jks_hash_table_t *hash_table, jks_hash_table_iterator_t *iterator);
bool jks_hash_table_iterator_is_end(jks_hash_table_t *hash_table, jks_hash_table_iterator_t *iterator);
void jks_hash_table_iterator_next(jks_hash_table_t *hash_table, jks_hash_table_iterator_t *iterator);

#define JKS_HASH_TABLE_FOREACH(iterator, hash_table) \
	for (jks_hash_table_iterator_t iterator = jks_hash_table_iterator_begin(hash_table); !jks_hash_table_iterator_is_end(hash_table, &iterator); jks_hash_table_iterator_next(hash_table, &iterator))

#ifdef __cplusplus
}
#endif

#endif
