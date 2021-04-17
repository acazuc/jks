#ifndef JKS_HMAP_H
# define JKS_HMAP_H

#ifdef __cplusplus
extern "C" {
#endif

# include <stdint.h>
# include <stdbool.h>

typedef union
{
	uint64_t u64;
	uint32_t u32;
	uint16_t u16;
	uint8_t u8;
	void *ptr;
} jks_hmap_key_t;

#define JKS_HMAP_KEY_PTR(v) (jks_hmap_key_t){.ptr = v}
#define JKS_HMAP_KEY_U64(v) (jks_hmap_key_t){.u64 = v}
#define JKS_HMAP_KEY_U32(v) (jks_hmap_key_t){.u32 = v}
#define JKS_HMAP_KEY_U16(v) (jks_hmap_key_t){.u16 = v}
#define JKS_HMAP_KEY_U8(v) (jks_hmap_key_t){.u8 = v}
#define JKS_HMAP_KEY_STR(v) (jks_hmap_key_t){.ptr = v}

typedef void (*jks_hmap_destructor_t)(jks_hmap_key_t key, void *value);
typedef uint32_t (*jks_hmap_hash_fn_t)(jks_hmap_key_t key);
typedef int (*jks_hmap_cmp_fn_t)(const jks_hmap_key_t k1, const jks_hmap_key_t k2);

typedef struct jks_hmap_bucket_s jks_hmap_bucket_t;
typedef struct jks_hmap_object_s jks_hmap_object_t;

typedef struct jks_hmap_iterator_s
{
	jks_hmap_bucket_t *bucket;
	jks_hmap_object_t *object;
} jks_hmap_iterator_t;

typedef struct jks_hmap_s
{
	jks_hmap_bucket_t *buckets;
	uint32_t buckets_count;
	uint32_t size;
	uint32_t value_size;
	jks_hmap_destructor_t destructor;
	jks_hmap_hash_fn_t hash_fn;
	jks_hmap_cmp_fn_t cmp_fn;
} jks_hmap_t;

void jks_hmap_init(jks_hmap_t *hmap, uint32_t value_size, jks_hmap_destructor_t destructor, jks_hmap_hash_fn_t hash_fn, jks_hmap_cmp_fn_t cmp_fn);
void jks_hmap_destroy(jks_hmap_t *hmap);

bool jks_hmap_reserve(jks_hmap_t *hmap, uint32_t capacity);

void *jks_hmap_get(jks_hmap_t *hmap, jks_hmap_key_t key);
void *jks_hmap_set(jks_hmap_t *hmap, jks_hmap_key_t key, void *value);
bool jks_hmap_erase(jks_hmap_t *hmap, jks_hmap_key_t key);

jks_hmap_iterator_t jks_hmap_iterator_begin(const jks_hmap_t *hmap);
jks_hmap_iterator_t jks_hmap_iterator_end(const jks_hmap_t *hmap);
jks_hmap_iterator_t jks_hmap_iterator_find(const jks_hmap_t *hmap, jks_hmap_key_t key);

jks_hmap_key_t jks_hmap_iterator_get_key(const jks_hmap_iterator_t *iterator);
void *jks_hmap_iterator_get_value(const jks_hmap_iterator_t *iterator);
void jks_hmap_iterator_erase(jks_hmap_t *hmap, const jks_hmap_iterator_t *iterator);
bool jks_hmap_iterator_is_end(const jks_hmap_t *hmap, const jks_hmap_iterator_t *iterator);
void jks_hmap_iterator_next(const jks_hmap_t *hmap, jks_hmap_iterator_t *iterator);

uint32_t jks_hmap_hash_uint32(const jks_hmap_key_t key);
int jks_hmap_cmp_uint32(const jks_hmap_key_t k1, const jks_hmap_key_t k2);
uint32_t jks_hmap_hash_uint64(const jks_hmap_key_t key);
int jks_hmap_cmp_uint64(const jks_hmap_key_t k1, const jks_hmap_key_t k2);
uint32_t jks_hmap_hash_string(const jks_hmap_key_t key);
int jks_hmap_cmp_string(const jks_hmap_key_t k1, const jks_hmap_key_t k2);
uint32_t jks_hmap_hash_ptr(const jks_hmap_key_t key);
int jks_hmap_cmp_ptr(const jks_hmap_key_t k1, const jks_hmap_key_t k2);

#define JKS_HMAP_FOREACH(iterator, hmap) \
	for (jks_hmap_iterator_t iterator = jks_hmap_iterator_begin(hmap); !jks_hmap_iterator_is_end(hmap, &iterator); jks_hmap_iterator_next(hmap, &iterator))

#ifdef __cplusplus
}
#endif

#endif
