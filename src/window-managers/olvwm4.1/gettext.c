#ifdef IDENT
#ident	"@(#)gettext.c	1.10	93/06/28 SMI"
#endif

/*
 *      (c) Copyright 1989 Sun Microsystems, Inc.
 */

/*
 *      Sun design patents pending in the U.S. and foreign countries. See
 *      LEGAL_NOTICE file for terms of the license.
 */

#include "gettext.h"

#ifdef SYSV
#define bzero(a,b) memset(a,0,b)
#endif

char *malloc(), *strdup();
char * dgettext();
char *_gettext();
char *in_path();
char *fgets(), *getenv();
caddr_t mmap(), calloc();

#ifdef NOT
static struct domain_binding *firstbind=0, *lastbind=0;

static void
initbindinglist()
{
    if (! firstbind) {
	firstbind =
	    (struct domain_binding *) malloc(sizeof(struct domain_binding));
	firstbind->domain_name = strdup("");
	firstbind->binding = strdup(DEFAULT_BINDING);
	firstbind->nextdomain = (struct domain_binding *) 0;
	lastbind = firstbind;
    }
}

static char *
lookupdefbind(domain_name)
    char	*domain_name;
{
    static char *binding = NULL; /* psuedo static ptr for return value */
    static int bindinglen = 0;
    char *bindptr = firstbind->binding;
    char *current_locale;
    char chartmp;
    char *bindtmptr, pathtmp[MAXPATHLEN], bindtmp[MAXPATHLEN];
    int newlen;

    struct stat statbuf;

    bindtmptr = bindtmp;

    current_locale = setlocale(LC_MESSAGES, NULL);

    while (chartmp = *bindptr++)  {
	switch (chartmp) {

	    case BINDINGLISTDELIM:
		*bindtmptr = '\0';
		strcpy(pathtmp, bindtmp);
		strcat(pathtmp, "/");
		strcat(pathtmp, current_locale);
		strcat(pathtmp, "/LC_MESSAGES/");
		strcat(pathtmp, domain_name);
		strcat(pathtmp, ".mo");
		
		if (stat(pathtmp, &statbuf)) {
		    bindtmptr = bindtmp;
		    break;
		}

		if ((newlen = strlen(bindtmp)) > bindinglen) {
		    bindinglen = newlen;
		    if (binding) {
			free (binding);
		    }
		    binding = malloc(newlen+1);
		    strcpy(binding, bindtmp);
		}
		return(binding);

	    default:
		*bindtmptr++ = chartmp;
		break;
	}
    }
    /*
     * NOT FOUND, return NULL
    */
    
    return (NULL);
}

char *
bindtextdomain(domain_name, binding)
    char    *domain_name;
    unsigned char *binding;
{

    struct domain_binding *bind;
    char *lastpath;
    unsigned char pathtmp[MAXPATHLEN+1];
    int newlen;

    pathtmp[0] = '\0';

    /* Initialize list */
    if (! firstbind) {
	initbindinglist();
    }
    
    if (!domain_name) {
	return (NULL);
    }

    if (*domain_name == '\0') {
	if (!binding) {
	    /* query, add COOKIE to binding
	     * return new binding cookie
	    */
	    pathtmp[0] = (unsigned char) COOKIE;
	    pathtmp[1] = '\0';
	    strcat(pathtmp, firstbind->binding);
	    return (strdup(pathtmp));
	} else if (binding[0] == COOKIE) {
	    /* result of a previous query,
	     * restore old binding
	    */
	    firstbind->binding = strdup(binding+1);
	    free(binding);
	    return (NULL);
	} else {
	    /* add binding to default binding list
	    */
	    strcat(pathtmp, firstbind->binding);
	    free(firstbind->binding);
	    strcat(pathtmp, binding);
	    strcat(pathtmp, "\n");
	    firstbind->binding = strdup(pathtmp);
	    return (NULL);
	}
    }
    
    /* linear search for binding, rebind if found, add if not */
    bind = firstbind;
    while (bind) {
	if (!strcmp(domain_name, bind->domain_name)) {
	    if (!binding) {
		return(bind->domain_name);
	    }
	    if (bind->domain_name) {
		free(bind->domain_name);
	    }
	    if (bind->binding) {
		free(bind->binding);
	    }
	    
	    bind->domain_name = strdup(domain_name);
	    bind->binding = strdup(binding);
	    return (bind->binding);
	}
	bind = bind->nextdomain;
    }
    
    /* Not found in list, add it to the end */

    if (!binding) {
	return (NULL);
    }
    lastbind = bind = lastbind->nextdomain =
	(struct domain_binding *) malloc(sizeof(struct domain_binding));
    bind->domain_name = strdup(domain_name);
    bind->binding = strdup(binding);
    bind->nextdomain = 0;
    return (bind->binding);
}

static char *
findtextdomain(domain_name)
    char *domain_name;
{
    struct domain_binding *bind;
    char *tmptr;

    bind = firstbind;

    if (!bind) {
        initbindinglist();
        return (lookupdefbind(domain_name));
    }

    while (bind) {
	if (!strcmp(domain_name, bind->domain_name)) { 
		return (bind->binding);
	}
	bind = bind->nextdomain;
    }

    /* not found, look for binding in default binding list */
    return (lookupdefbind(domain_name));
}
 
    

char *
textdomain(domain_name)
    char    *domain_name;
{
	
    static int entered = 0;

    static char	current_domain[MAX_DOMAIN_LENGTH + 1];

    if (! entered) {
	strcpy(current_domain, DEFAULT_DOMAIN);
 	entered = 1;
    }

    if (domain_name == NULL) {
	return(current_domain);
    }

    if (strlen(domain_name) > MAX_DOMAIN_LENGTH) {
	return(NULL);
    }

    if (*domain_name == '\0') {
	strcpy(current_domain, DEFAULT_DOMAIN);
    } else {
	strcpy(current_domain, domain_name);
    }

    return(current_domain);
}

char *
gettext(msg_id)
char *msg_id;
{
    return (dgettext(NULL, msg_id));
}


char *
dgettext(domain_name, msg_id)
    char    *domain_name;
    char    *msg_id;
{
    char msgfile[MAXPATHLEN+1];

    char *current_locale;
    char *current_domain;
    char *current_binding;
    char *msgptr, *openwinhome = NULL;

    struct stat statbuf;
    int	fd = -1;
    caddr_t addr;

    int   msg_inc;

    static int	top_list = 0;			/* top of message_so list */
    static int	first_free = 0;			/* first free entry in list */
    static int	last_entry_seen = 0;		/* try this one first */
    static struct message_so messages_so[MAX_MSG];
    static struct message_so  cur_mess_so;	/* holds current message domain */


    current_locale = setlocale(LC_MESSAGES, NULL);

    if (domain_name == NULL) {
        current_domain = textdomain(NULL);
    } else if (strlen(domain_name) > MAX_DOMAIN_LENGTH) {
        return(msg_id);
    } else if (*domain_name == '\0') {
        current_domain = DEFAULT_DOMAIN;
    } else {
        current_domain = domain_name;
    }

    /* check to see if textdomain has changed	*/


    bzero(msgfile, sizeof(msgfile));
    if (current_binding = findtextdomain(current_domain)) {
	strcpy(msgfile, current_binding);
	strcat(msgfile, "/");
	strcat(msgfile, current_locale);
	strcat(msgfile, "/LC_MESSAGES/");
	strcat(msgfile, current_domain);
	strcat(msgfile, ".mo");
    } else {
	return(msg_id);
    }

    msg_inc = top_list;
    while (msg_inc < first_free) {
	if (!strcmp(msgfile, messages_so[msg_inc].message_so_path)) {
	    if (messages_so[msg_inc].fd != -1 &&
		messages_so[msg_inc].mess_file_info !=
		    (struct struct_mo_info *) -1) {
		cur_mess_so = messages_so[msg_inc];
		last_entry_seen = msg_inc;
		return (_gettext(cur_mess_so, msg_id));
	    } else {
		return(msg_id);
	    }
	}
	msg_inc++;
    }

    /*
       been though entire queue and not found 
       open new entry if there is space.
    */

    if (msg_inc == MAX_MSG) {
	return (msg_id);		/* not found and no more space */
    }
    if (first_free == MAX_MSG) {
	return (msg_id);		/* no more space		*/
    }

    /*
     * There is an available entry in the queue, so make a
     * message_so for it and put it on the queue, 
     * return msg_id if message file isn't opened -or-
     * mmap'd correctly
    */

/*
    if ((fd = open(msgfile, O_RDONLY)) == -1) {
	return (msg_id);
    } else if (fstat(fd, &statbuf) == -1) {
	close(fd);
	return (msg_id);
    } else if ((addr =
	mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0))
	    == (caddr_t) -1 ) {
	close(fd);
	return (msg_id);
    }
    close(fd);
*/

    fd = open(msgfile, O_RDONLY);

    messages_so[first_free].fd = fd;
    messages_so[first_free].message_so_path = strdup(msgfile);

    if (fd == -1) {
	first_free++;
	close(fd);
	return (msg_id);
    }

    fstat(fd, &statbuf);
    addr = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);

    messages_so[first_free].mess_file_info = (struct struct_mo_info *) addr;
    if (addr == (caddr_t) -1) {
	first_free++;
	return (msg_id);
    }

    messages_so[first_free].message_list =
	(struct message_struct *) &messages_so[first_free].mess_file_info[1];
    messages_so[first_free].msg_ids =
	(char *) &messages_so[first_free].message_list[messages_so[first_free].mess_file_info->message_count];
    messages_so[first_free].msgs =
	(char *) messages_so[first_free].msg_ids + messages_so[first_free].mess_file_info->string_count_msgid;
    cur_mess_so = messages_so[first_free];
    first_free++;


    /* return pointer to message */

    return (_gettext(cur_mess_so,msg_id));
}


char *_gettext( messages, key_string )
    struct message_so messages;
        char *key_string ; 
{
    register int check;
    register int val;
    check = messages.mess_file_info->message_mid;
    for (;;) {
	if ((val=strcmp(key_string,
	    messages.msg_ids+messages.message_list[check].msgid_offset)) < 0) {
	    if (messages.message_list[check].less == -99) {
		return (key_string);
	    } else {
		check = messages.message_list[check].less;
	    }
	} else if (val > 0) {
	    if (messages.message_list[check].more == -99) {
                return (key_string);
	    } else {
		check = messages.message_list[check].more;
            }
	} else {
	    return (messages.msgs+messages.message_list[check].msg_offset);
        } /* if ((val= ... */
    } /* for (;;) */
}

#endif
