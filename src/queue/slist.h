#ifndef JKS_SLIST_H
# define JKS_SLIST_H

#define JKS_SLIST_HEAD(name, type) \
struct name \
{ \
	type *first; \
}

#define JKS_SLIST_ENTRY(type) \
struct \
{ \
	type *next; \
}

#define JKS_SLIST_INIT(head) \
do \
{ \
	JKS_SLIST_FIRST(head) = NULL; \
} while (0)

#define JKS_SLIST_HEAD_INITIALIZER() \
{NULL}

#define JKS_SLIST_EMPTY(head) \
(JKS_SLIST_FIRST(head) == NULL)

#define JKS_SLIST_FIRST(head) \
((head)->first)

#define JKS_SLIST_NEXT(elm, field) \
((elm)->field.next)

#define JKS_SLIST_FOREACH(var, head, field) \
for ((var) = JKS_SLIST_FIRST(head); (var); (var) = JKS_SLIST_NEXT(var, field))

#define JKS_SLIST_FOREACH_FROM(var, head, field) \
for ((var) = (var) ? (var) : JKS_SLIST_FIRST(head); (var); (var) = JKS_SLIST_NEXT(var, field))

#define JKS_SLIST_FOREACH_SAFE(var, head, field, tvar) \
for ((var) = JKS_SLIST_FIRST(head); (var) && ((tvar) = JKS_SLIST_NEXT(var, field), 1); (var) = (tvar))

#define JKS_SLIST_FOREACH_FROM_SAFE(var, head, field, tvar) \
for ((var) = (var) ? (var) : JKS_SLIST_FIRST(head); (var) && ((tvar) = JKS_SLIST_NEXT(var, field), 1); (var) = (tvar))

#define JKS_SLIST_INSERT_HEAD(head, elm, field) \
do \
{ \
	JKS_SLIST_NEXT(elm, field) = JKS_SLIST_FIRST(head); \
	JKS_SLIST_FIRST(head) = (elm); \
} while (0)

#define JKS_SLIST_INSERT_AFTER(slistelm, elm, field) \
do \
{ \
	JKS_SLIST_NEXT(elm, field) = JKS_SLIST_NEXT(slistelm, field); \
	JKS_SLIST_NEXT(slistelm, field) = (elm); \
} while (0)

#define JKS_SLIST_CONCAT(head1, head2, type, field) \
do \
{ \
	if (JKS_SLIST_EMPTY(head2)) \
		break; \
	if (JKS_SLIST_EMPTY(head1)) \
	{ \
		JKS_SLIST_FIRST(head1) = JKS_SLIST_FIRST(head2); \
		JKS_SLIST_INIT(head2); \
		break; \
	} \
	type *var; \
	type *tvar; \
	JKS_SLIST_FOREACH_SAFE(var, head1, field, tvar) \
	{ \
		if (tvar == NULL) \
		{ \
			JKS_SLIST_NEXT(var, field) = JKS_SLIST_FIRST(head2); \
			JKS_SLIST_INIT(head2); \
			break; \
		} \
	} \
} while (0)

#define JKS_SLIST_REMOVE_HEAD(head, field) \
JKS_SLIST_FIRST(head) = JKS_SLIST_NEXT(JKS_SLIST_FIRST(head), field)

#define JKS_SLIST_REMOVE_AFTER(elm, field) \
JKS_SLIST_NEXT(elm, field) = JKS_SLIST_NEXT(JKS_SLIST_NEXT(elm, field), field)

#define JKS_SLIST_REMOVE(head, elm, type, field) \
do \
{ \
	if (JKS_SLIST_FIRST(head) == (elm)) \
	{ \
		JKS_SLIST_REMOVE_HEAD(head, field); \
		break; \
	} \
	type *var; \
	type *tvar; \
	JKS_SLIST_FOREACH_SAFE(var, head, field, tvar) \
	{ \
		if ((elm) == tvar) \
		{ \
			JKS_SLIST_REMOVE_AFTER(var, field); \
			break; \
		} \
	} \
} while (0)

#endif
