#ifndef JKS_ARRAY_H
# define JKS_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

# include <stdint.h>
# include <stdbool.h>

typedef void (*jks_array_destructor_t)(void *data);

typedef struct jks_array_iterator_s
{
	void *data;
	uint32_t data_size;
} jks_array_iterator_t;

typedef struct jks_array_s
{
	void *data;
	uint32_t size;
	uint32_t capacity;
	uint32_t data_size;
	jks_array_destructor_t destructor;
} jks_array_t;

void jks_array_init(jks_array_t *array, uint32_t data_size, jks_array_destructor_t destructor);
void jks_array_destroy(jks_array_t *array);

bool jks_array_resize(jks_array_t *array, uint32_t size);
void *jks_array_grow(jks_array_t *array, uint32_t size);
bool jks_array_reserve(jks_array_t *array, uint32_t capacity);
bool jks_array_shrink(jks_array_t *array);

static inline void *jks_array_get(const jks_array_t *array, uint32_t offset)
{
	return ((uint8_t*)array->data) + array->data_size * offset;
}

#define JKS_ARRAY_GET(array, offset, type) (&((type*)(array)->data)[offset])

/* data can be NULL to let the data unitialized */
void *jks_array_push_front(jks_array_t *array, const void *data);
void *jks_array_push_back(jks_array_t *array, const void *data);
void *jks_array_push(jks_array_t *array, const void *data, uint32_t offset);

bool jks_array_erase(jks_array_t *array, uint32_t offset);

jks_array_iterator_t jks_array_iterator_begin(const jks_array_t *array);
jks_array_iterator_t jks_array_iterator_end(const jks_array_t *array);
jks_array_iterator_t jks_array_iterator_find(const jks_array_t *array, uint32_t offset);

static inline void *jks_array_iterator_get(const jks_array_iterator_t *iterator)
{
	return iterator->data;
}

void jks_array_iterator_erase(jks_array_t *array, const jks_array_iterator_t *iterator);
bool jks_array_iterator_is_end(const jks_array_t *array, const jks_array_iterator_t *iterator);
void jks_array_iterator_next(jks_array_iterator_t *iterator);

#define JKS_ARRAY_FOREACH(iterator, array) \
	for (jks_array_iterator_t iterator = jks_array_iterator_begin(array); !jks_array_iterator_is_end(array, &iterator); jks_array_iterator_next(&iterator))

#ifdef __cplusplus
}
#endif

#endif
