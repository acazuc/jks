#ifndef JKS_LIST_H
# define JKS_LIST_H

#define JKS_LIST_HEAD(name, type) \
struct name \
{ \
	type *first; \
}

#define JKS_LIST_ENTRY(type) \
struct \
{ \
	type *next; \
	type *prev; \
}

#define JKS_LIST_INIT(head) \
do \
{ \
	JKS_LIST_FIRST(head) = NULL; \
} while (0)

#define JKS_LIST_HEAD_INITIALIZER() \
{NULL}

#define JKS_LIST_EMPTY(head) \
(JKS_LIST_FIRST(head) == NULL)

#define JKS_LIST_FIRST(head) \
((head)->first)

#define JKS_LIST_NEXT(elm, field) \
((elm)->field.next)

#define JKS_LIST_PREV(elm, field) \
((elm)->field.prev)

#define JKS_LIST_FOREACH(var, head, field) \
for ((var) = JKS_LIST_FIRST(head); (var); (var) = JKS_LIST_NEXT(var, field))

#define JKS_LIST_FOREACH_FROM(var, head, field) \
for ((var) = (var) ? (var) : JKS_LIST_FIRST(head); (var); (var) = JKS_LIST_NEXT(var, field))

#define JKS_LIST_FOREACH_SAFE(var, head, field, tvar) \
for ((var) = JKS_LIST_FIRST(head); (var) && ((tvar) = JKS_LIST_NEXT(var, field), 1); (var) = (tvar))

#define JKS_LIST_FOREACH_FROM_SAFE(var, head, field, tvar) \
for ((var) = (var) ? (var) : JKS_LIST_FIRST(head); (var) && ((tvar) = JKS_LIST_NEXT(var, field), 1); (var) = (tvar))

#define JKS_LIST_INSERT_HEAD(head, elm, field) \
do \
{ \
	JKS_LIST_NEXT(elm, field) = JKS_LIST_FIRST(head); \
	JKS_LIST_PREV(elm, field) = NULL; \
	if (!JKS_LIST_EMPTY(head)) \
		JKS_LIST_PREV(JKS_LIST_FIRST(head), field) = (elm); \
	JKS_LIST_FIRST(head) = (elm); \
} while (0)

#define JKS_LIST_INSERT_AFTER(listelm, elm, field) \
do \
{ \
	if (JKS_LIST_NEXT(listelm, field)) \
		JKS_LIST_PREV(JKS_LIST_NEXT(listelm, field), field) = (elm); \
	JKS_LIST_NEXT(elm, field) = JKS_LIST_NEXT(listelm, field); \
	JKS_LIST_PREV(elm, field) = (listelm); \
	JKS_LIST_NEXT(listelm, field) = (elm); \
} while (0)

#define JKS_LIST_INSERT_BEFORE(listelm, elm, field) \
do \
{ \
	JKS_LIST_PREV(elm, field) = JKS_LIST_PREV(listelm, field); \
	JKS_LIST_NEXT(elm, field) = (listelm); \
	JKS_LIST_NEXT(JKS_LIST_PREV(listelm, field), field) = (elm); \
	JKS_LIST_PREV(listelm, field) = (elm); \
} while (0)

#define JKS_LIST_CONCAT(head1, head2, type, field) \
do \
{ \
	if (JKS_LIST_EMPTY(head2)) \
		break; \
	if (JKS_LIST_EMPTY(head1)) \
	{ \
		JKS_LIST_FIRST(head1) = JKS_LIST_FIRST(head2); \
		JKS_LIST_INIT(head2); \
		break; \
	} \
	type *var; \
	type *tvar; \
	JKS_LIST_FOREACH_SAFE(var, head1, field, tvar) \
	{ \
		if (tvar == NULL) \
		{ \
			JKS_LIST_NEXT(var, field) = JKS_LIST_FIRST(head2); \
			JKS_LIST_PREV(JKS_LIST_FIRST(head2), field) = tvar; \
			JKS_LIST_INIT(head2); \
			break; \
		} \
	} \
} while (0)

#define JKS_LIST_REMOVE(elm, field) \
do \
{ \
	if (JKS_LIST_NEXT(elm, field) != NULL) \
		JKS_LIST_PREV(JKS_LIST_NEXT(elm, field), field) = JKS_LIST_PREV(elm); \
	JKS_LIST_NEXT(JKS_LIST_PREV(elm, field), field) = JKS_LIST_NEXT(elm, field); \
} while (0)

#endif
