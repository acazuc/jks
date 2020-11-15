#ifndef JKS_LIST_H
# define JKS_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

# include <stdint.h>
# include <stdbool.h>

typedef void (*jks_list_destructor_t)(void *data);

typedef struct jks_list_item_s jks_list_item_t;

struct jks_list_item_s
{
	jks_list_item_t *prev;
	jks_list_item_t *next;
	char data[];
};

typedef struct jks_list_iterator_s
{
	jks_list_item_t *item;
} jks_list_iterator_t;

typedef struct jks_list_h
{
	jks_list_item_t *head;
	jks_list_item_t *tail;
	uint32_t size;
	uint32_t data_size;
	jks_list_destructor_t destructor;
} jks_list_t;

void jks_list_init(jks_list_t *list, uint32_t data_size, jks_list_destructor_t destructor);
void jks_list_destroy(jks_list_t *list);

bool jks_list_resize(jks_list_t *list, uint32_t size);

void *jks_list_get(const jks_list_t *list, uint32_t offset);

void *jks_list_push_front(jks_list_t *list, void *data);
void *jks_list_push_back(jks_list_t *list, void *data);
void *jks_list_push(jks_list_t *list, void *data, uint32_t offset);

bool jks_list_erase(jks_list_t *list, uint32_t offset);

jks_list_iterator_t jks_list_iterator_begin(const jks_list_t *list);
jks_list_iterator_t jks_list_iterator_end(const jks_list_t *list);
jks_list_iterator_t jks_list_iterator_find(const jks_list_t *list, uint32_t offset);

void *jks_list_iterator_get(const jks_list_iterator_t *iterator);
void jks_list_iterator_erase(jks_list_t *list, const jks_list_iterator_t *iterator);
bool jks_list_iterator_is_end(const jks_list_t *list, const jks_list_iterator_t *iterator);
void jks_list_iterator_next(jks_list_iterator_t *iterator);

#define JKS_LIST_FOREACH(iterator, list) \
	for (jks_list_iterator_t iterator = jks_list_iterator_begin(list); !jks_list_iterator_is_end(list, &iterator); jks_list_iterator_next(&iterator))

#ifdef __cplusplus
}
#endif

#endif
