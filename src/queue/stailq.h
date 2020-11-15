#ifndef JKS_STAILQ_H
# define JKS_STAILQ_H

#define JKS_STAILQ_HEAD(name, type) \
struct name \
{ \
	type *first; \
	type *last; \
}

#define JKS_STAILQ_ENTRY(type) \
struct \
{ \
	type *next; \
}

#define JKS_STAILQ_INIT(head) \
do \
{ \
	JKS_STAILQ_FIRST(head) = NULL; \
	JKS_STAILQ_LAST(head) = NULL; \
} while (0)

#define JKS_STAILQ_HEAD_INITIALIZER() \
{NULL, NULL}

#define JKS_STAILQ_EMPTY(head) \
(JKS_STAILQ_FIRST(head) == NULL)

#define JKS_STAILQ_FIRST(head) \
((head)->first)

#define JKS_STAILQ_LAST(head) \
((head)->last)

#define JKS_STAILQ_NEXT(elm, field) \
((elm)->field.next)

#define JKS_STAILQ_FOREACH(var, head, field) \
for ((var) = JKS_STAILQ_FIRST(head); (var); (var) = JKS_STAILQ_NEXT(var, field))

#define JKS_STAILQ_FOREACH_FROM(var, head, field) \
for ((var) = (var) ? (var) : JKS_STAILQ_FIRST(head); (var); (var) = JKS_STAILQ_NEXT(var, field))

#define JKS_STAILQ_FOREACH_SAFE(var, head, field, tvar) \
for ((var) = JKS_STAILQ_FIRST(head); (var) && ((tvar) = JKS_STAILQ_NEXT(var, field), 1); (var) = (tvar))

#define JKS_STAILQ_FOREACH_FROM_SAFE(var, head, field, tvar) \
for ((var) = (var) ? (var) : JKS_STAILQ_FIRST(head); (var) && ((tvar) = JKS_STAILQ_NEXT(var, field), 1); (var) = (tvar))

#define JKS_STAILQ_INSERT_HEAD(head, elm, field) \
do \
{ \
	JKS_STAILQ_NEXT(elm, field) = JKS_STAILQ_FIRST(head); \
	if (JKS_STAILQ_EMPTY(head)) \
		JKS_STAILQ_LAST(head) = (elm); \
	JKS_STAILQ_FIRST(head) = (elm); \
} while (0)

#define JKS_STAILQ_INSERT_TAIL(head, elm, field) \
do \
{ \
	JKS_STAILQ_NEXT(elm, field) = NULL; \
	if (JKS_STAILQ_EMPTY(head)) \
	{ \
		JKS_STAILQ_FIRST(head) = (elm); \
		JKS_STAILQ_LAST(head) = (elm); \
	} \
	else \
		JKS_STAILQ_NEXT(JKS_STAILQ_LAST(head), field) = (elm); \
} while (0)

#define JKS_STAILQ_INSERT_AFTER(head, listelm, elm, field) \
do \
{ \
	JKS_STAILQ_NEXT(elm, field) = JKS_STAILQ_NEXT(listelm, field); \
	JKS_STAILQ_NEXT(listelm, field) = (elm); \
	if (JKS_STAILQ_LAST(head) == (listelm)) \
		JKS_STAILQ_LAST(head) = (elm); \
} while (0)

#define JKS_STAILQ_CONCAT(head1, head2, type, field) \
do \
{ \
	if (JKS_STAILQ_EMPTY(head2)) \
		break; \
	if (JKS_STAILQ_EMPTY(head1)) \
	{ \
		JKS_STAILQ_FIRST(head1) = JKS_STAILQ_FIRST(head2); \
		JKS_STAILQ_LAST(head1) = JKS_STAILQ_LAST(head2); \
		JKS_STAILQ_INIT(head2); \
		break; \
	} \
	JKS_STAILQ_NEXT(JKS_STAILQ_LAST(head1), field) = JKS_STAILQ_FIRST(head2); \
	JKS_STAILQ_INIT(head2); \
} while (0)

#define JKS_STAILQ_REMOVE_HEAD(head, field) \
do \
{ \
	JKS_STAILQ_FIRST(head) = JKS_STAILQ_NEXT(JKS_STAILQ_FIRST(head), field); \
	if (JKS_STAILQ_FIRST(head) == NULL) \
		JKS_STAILQ_LAST(head) = NULL; \
} while (0)

#define JKS_STAILQ_REMOVE_AFTER(head, elm, field) \
do \
{ \
	if (JKS_STAILQ_LAST(head) == JKS_STAILQ_NEXT(elm, field)) \
		JKS_STAILQ_LAST(head) = (elm); \
	JKS_STAILQ_NEXT(elm, field) = JKS_STAILQ_NEXT(JKS_STAILQ_NEXT(elm, field), field); \
} while (0)

#define JKS_STAILQ_REMOVE(head, elm, type, field) \
do \
{ \
	if (JKS_STAILQ_FIRST(head) == (elm)) \
	{ \
		JKS_STAILQ_REMOVE_HEAD(head, field); \
		break; \
	} \
	type *var; \
	type *tvar; \
	JKS_STAILQ_FOREACH_SAFE(var, head, field, tvar) \
	{ \
		if ((elm) == tvar) \
		{ \
			JKS_STAILQ_REMOVE_AFTER(var, field); \
			break; \
		} \
	} \
} while (0)

#endif
