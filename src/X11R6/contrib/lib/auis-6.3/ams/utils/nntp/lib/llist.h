/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
#include <sys/types.h>

struct llist {
	struct llist	*l_next;
	caddr_t		l_item;
	unsigned	l_len;
};

extern void		l_free();
extern struct llist	*l_alloc();

#ifndef NULL
#define NULL	0
#endif /* NULL */
