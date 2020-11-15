#include "slist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define PROTECT(...) __VA_ARGS__

#define ASSERT_CHAIN(tmp, head, field, values_nb, values) \
i = 0; \
JKS_SLIST_FOREACH(tmp, head, field) \
{ \
	static int tmp_values[values_nb] = values; \
	assert(tmp->value == tmp_values[i]); \
	i++; \
}

#define ASSERT_CHAIN_SAFE(tmp, head, field, tmp2, values_nb, values) \
i = 0; \
JKS_SLIST_FOREACH_SAFE(tmp, head, field, tmp2) \
{ \
	static int tmp_values[values_nb] = values; \
	assert(tmp->value == tmp_values[i]); \
	if (i == values_nb - 1) \
		assert(tmp2 == NULL); \
	else \
		assert(tmp2->value == tmp_values[i + 1]); \
	i++; \
}

#define ASSERT_CHAIN_FROM(tmp, head, field, values_nb, values) \
i = 0; \
JKS_SLIST_FOREACH_FROM(tmp, head, field) \
{ \
	static int tmp_values[values_nb] = values; \
	assert(tmp->value == tmp_values[i]); \
	i++; \
}

#define ASSERT_CHAIN_FROM_SAFE(tmp, head, field, tmp2, values_nb, values) \
i = 0; \
JKS_SLIST_FOREACH_FROM_SAFE(tmp, head, field, tmp2) \
{ \
	static int tmp_values[values_nb] = values; \
	assert(tmp->value == tmp_values[i]); \
	if (i == values_nb - 1) \
		assert(tmp2 == NULL); \
	else \
		assert(tmp2->value == tmp_values[i + 1]); \
	i++; \
}

typedef struct item_s item_t;

struct item_s
{
	int value;
	JKS_SLIST_ENTRY(item_t) chain;
};

typedef JKS_SLIST_HEAD(head_s, item_t) head_t;

int main()
{
	item_t values[50]; /* Dummy values to pick */
	for (int i = 0; i < sizeof(values) / sizeof(*values); ++i)
		values[i].value = i;
	item_t *tmp, *tmp2;
	int i;

	head_t head = JKS_SLIST_HEAD_INITIALIZER();
	head_t head2 = JKS_SLIST_HEAD_INITIALIZER();

	/* INIT
	 * HEAD_INITIALIZER
	 * EMPTY
	 */
	assert(head.first == NULL);
	head.first = (void*)50;
	assert(head.first != NULL);
	JKS_SLIST_INIT(&head);
	assert(head.first == NULL);
	assert(JKS_SLIST_EMPTY(&head));
	head.first = (void*)50;
	assert(!JKS_SLIST_EMPTY(&head));
	JKS_SLIST_INIT(&head);

	/* INSERT_HEAD
	 * INSERT_AFTER
	 */
	JKS_SLIST_INSERT_HEAD(&head, &values[0], chain);
	assert(!JKS_SLIST_EMPTY(&head));
	assert(JKS_SLIST_FIRST(&head)->value == 0);
	JKS_SLIST_INSERT_HEAD(&head, &values[1], chain);
	assert(JKS_SLIST_FIRST(&head)->value == 1);
	assert(JKS_SLIST_NEXT(JKS_SLIST_FIRST(&head), chain) != NULL);
	assert(JKS_SLIST_NEXT(JKS_SLIST_FIRST(&head), chain)->value == 0);
	JKS_SLIST_INSERT_AFTER(JKS_SLIST_FIRST(&head), &values[2], chain);
	assert(JKS_SLIST_NEXT(JKS_SLIST_FIRST(&head), chain)->value == 2);

	/* FOREACH
	 * FOREACH_SAFE
	 * FOREACH_FROM
	 * FOREACH_FROM_SAFE
	 */
	JKS_SLIST_INSERT_HEAD(&head, &values[3], chain);
	ASSERT_CHAIN(tmp, &head, chain, 4, PROTECT({3, 1, 2, 0}));
	ASSERT_CHAIN_SAFE(tmp, &head, chain, tmp2, 4, PROTECT({3, 1, 2, 0}));
	tmp = JKS_SLIST_FIRST(&head);
	ASSERT_CHAIN_FROM(tmp, &head, chain, 4, PROTECT({3, 1, 2, 0}));
	tmp = NULL;
	ASSERT_CHAIN_FROM(tmp, &head, chain, 4, PROTECT({3, 1, 2, 0}));
	tmp = JKS_SLIST_NEXT(JKS_SLIST_FIRST(&head), chain);
	ASSERT_CHAIN_FROM(tmp, &head, chain, 3, PROTECT({1, 2, 0}));
	tmp = JKS_SLIST_FIRST(&head);
	ASSERT_CHAIN_FROM_SAFE(tmp, &head, chain, tmp2, 4, PROTECT({3, 1, 2, 0}));
	tmp = NULL;
	ASSERT_CHAIN_FROM_SAFE(tmp, &head, chain, tmp2, 4, PROTECT({3, 1, 2, 0}));
	tmp = JKS_SLIST_NEXT(JKS_SLIST_FIRST(&head), chain);
	ASSERT_CHAIN_FROM_SAFE(tmp, &head, chain, tmp2, 3, PROTECT({1, 2, 0}));

	/* CONCAT
	 */
	JKS_SLIST_INSERT_HEAD(&head2, &values[12], chain);
	JKS_SLIST_INSERT_HEAD(&head2, &values[10], chain);
	JKS_SLIST_INSERT_HEAD(&head2, &values[11], chain);
	assert(!JKS_SLIST_EMPTY(&head2));
	JKS_SLIST_CONCAT(&head, &head2, item_s, chain);
	assert(JKS_SLIST_EMPTY(&head2));
	ASSERT_CHAIN(tmp, &head, chain, 7, PROTECT({3, 1, 2, 0, 11, 10, 12}));

	/* REMOVE_HEAD
	 * REMOVE_AFTER
	 * REMOVE
	 */
	JKS_SLIST_REMOVE_HEAD(&head, chain);
	ASSERT_CHAIN(tmp, &head, chain, 6, PROTECT({1, 2, 0, 11, 10, 12}));
	JKS_SLIST_REMOVE_AFTER(JKS_SLIST_FIRST(&head), chain);
	ASSERT_CHAIN(tmp, &head, chain, 5, PROTECT({1, 0, 11, 10, 12}));
	JKS_SLIST_REMOVE_AFTER(JKS_SLIST_NEXT(JKS_SLIST_NEXT(JKS_SLIST_FIRST(&head), chain), chain), chain);
	ASSERT_CHAIN(tmp, &head, chain, 4, PROTECT({1, 0, 11, 12}));
	JKS_SLIST_REMOVE(&head, JKS_SLIST_NEXT(JKS_SLIST_NEXT(JKS_SLIST_NEXT(JKS_SLIST_FIRST(&head), chain), chain), chain) ,chain);
	ASSERT_CHAIN(tmp, &head, chain, 3, PROTECT({1, 0, 11}));
	JKS_SLIST_REMOVE_AFTER(JKS_SLIST_NEXT(JKS_SLIST_FIRST(&head), chain), chain);
	ASSERT_CHAIN(tmp, &head, chain, 2, PROTECT({1, 0}));
	JKS_SLIST_REMOVE_AFTER(JKS_SLIST_FIRST(&head), chain);
	ASSERT_CHAIN(tmp, &head, chain, 1, PROTECT({1}));
	JKS_SLIST_REMOVE_HEAD(&head, chain);
	assert(JKS_SLIST_EMPTY(&head));
	printf("ok\n");
	return 0;
}
