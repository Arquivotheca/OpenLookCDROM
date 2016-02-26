/*                                            Utilities for the Authorization parts of libwww
             COMMON PARTS OF AUTHORIZATION MODULE TO BOTH SERVER AND BROWSER
                                             
   This module is the interface to the common parts of Access Authorization (AA) package
   for both server and browser. Important to know about memory allocation:
   
   Routines in this module use dynamic allocation, but free automatically all the memory
   reserved by them.
   
   Therefore the caller never has to (and never should) free() any object returned by
   these functions.
   
   Therefore also all the strings returned by this package are only valid until the next
   call to the same function is made. This approach is selected, because of the nature of
   access authorization: no string returned by the package needs to be valid longer than
   until the next call.
   
   This also makes it easy to plug the AA package in: you don't have to ponder whether to
   free() something here or is it done somewhere else (because it is always done somewhere
   else).
   
   The strings that the package needs to store are copied so the original strings given as
   parameters to AA functions may be freed or modified with no side effects.
   
   Also note: The AA package does not free() anything else than what it has itself
   allocated.
   
 */

#ifndef HTAAUTIL_H
#define HTAAUTIL_H

#include "HTUtils.h"            /* BOOL, PARAMS, ARGS */
#include "HTList.h"
#include "tcp.h"

#ifdef SHORT_NAMES
#define HTAASenu        HTAAScheme_enum
#define HTAASnam        HTAAScheme_name
#define HTAAteMa        HTAA_templateMatch
#define HTAAmaPT        HTAA_makeProtectionTemplate
#define HTAApALi        HTAA_parseArgList
#endif /*SHORT_NAMES*/


/*

Default filenames

 */
#ifndef PASSWD_FILE
#define PASSWD_FILE     "/home2/luotonen/passwd"
#endif

#ifndef GROUP_FILE
#define GROUP_FILE      "/home2/luotonen/group"
#endif

#define ACL_FILE_NAME   ".www_acl"


/*
** Numeric constants
*/
#define MAX_USERNAME_LEN        16      /* @@ Longest allowed username    */
#define MAX_PASSWORD_LEN        4*13    /* @@ Longest allowed password    */
                                        /* (encrypted, so really only 4*8)*/
#define MAX_METHODNAME_LEN      12      /* @@ Longest allowed method name */
#define MAX_FIELDNAME_LEN       16      /* @@ Longest field name in       */
                                        /* protection setup file          */
#define MAX_PATHNAME_LEN        80      /* @@ Longest passwd/group file   */
                                        /* patname to allow               */

/*
** Helpful macros
*/
#define FREE(x) if (x) {free(x); x=NULL;}


/*
** Access Authorization failure reasons
*/
typedef enum {
    HTAA_OK,            /* 200 OK                               */
    HTAA_OK_GATEWAY,    /* 200 OK, acting as a gateway          */
    HTAA_OK_REDIRECT,   /* 302 OK, redirected                   */
    HTAA_NO_AUTH,       /* 401 Unauthorized, not authenticated  */
    HTAA_NOT_MEMBER,    /* 401 Unauthorized, not authorized     */
    HTAA_IP_MASK,       /* 403 Forbidden by IP mask             */
    HTAA_BY_RULE,       /* 403 Forbidden by rule                */
    HTAA_NO_ACL,        /* 403 Forbidden, ACL non-existent      */
    HTAA_NO_ENTRY,      /* 403 Forbidden, no ACL entry          */
    HTAA_SETUP_ERROR,   /* 403 Forbidden, server setup error    */
    HTAA_DOTDOT,        /* 403 Forbidden, URL with /../ illegal */
    HTAA_HTBIN,         /* 403 Forbidden, /htbin not enabled    */
    HTAA_INVALID_REDIRECT,
                        /* 403 Forbidden, bad redirection setup */
    HTAA_NOT_ALLOWED,   /* 403 Forbidden, dangerous method must */
                        /*                be explicitly allowed */
    HTAA_NOT_FOUND,     /* 404 Not found, or read protected     */
    HTAA_MULTI_FAILED   /* 404 No suitable presentation found   */
} HTAAFailReason;


/*

Datatype definitions

  HTAASCHEME
  
   The enumeration HTAAScheme represents the possible authentication schemes used by the
   WWW Access Authorization.
   
 */

typedef enum {
    HTAA_UNKNOWN,
    HTAA_NONE,
    HTAA_BASIC,
    HTAA_PUBKEY,
    HTAA_KERBEROS_V4,
    HTAA_KERBEROS_V5,
    HTAA_MAX_SCHEMES /* THIS MUST ALWAYS BE LAST! Number of schemes */
} HTAAScheme;

/*

Authentication Schemes

 */

/* PUBLIC                                               HTAAScheme_enum()
**              TRANSLATE SCHEME NAME TO A SCHEME ENUMERATION
** ON ENTRY:
**      name            is a string representing the scheme name.
**
** ON EXIT:
**      returns         the enumerated constant for that scheme.
*/
PUBLIC HTAAScheme HTAAScheme_enum PARAMS((CONST char* name));


/* PUBLIC                                               HTAAScheme_name()
**                      GET THE NAME OF A GIVEN SCHEME
** ON ENTRY:
**      scheme          is one of the scheme enum values:
**                      HTAA_NONE, HTAA_BASIC, HTAA_PUBKEY, ...
**
** ON EXIT:
**      returns         the name of the scheme, i.e.
**                      "none", "basic", "pubkey", ...
*/
PUBLIC char *HTAAScheme_name PARAMS((HTAAScheme scheme));


/* PUBLIC                                           HTAA_templateCaseMatch()
**              STRING COMPARISON FUNCTION FOR FILE NAMES
**                 WITH ONE WILDCARD * IN THE TEMPLATE (Case Insensitive)
** NOTE:
**      This is essentially the same code as in HTAA_templateMatch, but
**      it compares case insensitive (for VMS). Reason for this routine
**      is that HTAA_templateMatch gets called from several places, also
**      there where a case sensitive match is needed, so one cannot just
**      change the HTAA_templateMatch routine for VMS.
**
** ON ENTRY:
**      template        is a template string to match the file name
**                      agaist, may contain a single wildcard
**                      character * which matches zero or more
**                      arbitrary characters.
**      filename        is the filename (or pathname) to be matched
**                      agaist the template.
**
** ON EXIT:
**      returns         YES, if filename matches the template.
**                      NO, otherwise.
*/
PUBLIC BOOL HTAA_templateCaseMatch PARAMS((CONST char * template,
                                           CONST char * filename));


/* PUBLIC                                       HTAA_makeProtectionTemplate()
**              CREATE A PROTECTION TEMPLATE FOR THE FILES
**              IN THE SAME DIRECTORY AS THE GIVEN FILE
**              (Used by server if there is no fancier way for
**              it to tell the client, and by browser if server
**              didn't send WWW-ProtectionTemplate: field)
** ON ENTRY:
**      docname is the document pathname (from URL).
**
** ON EXIT:
**      returns a template matching docname, and other files
**              files in that directory.
**
**              E.g.  /foo/bar/x.html  =>  /foo/bar/ *
**                                                  ^
**                              Space only to prevent it from
**                              being a comment marker here,
**                              there really isn't any space.
*/
PUBLIC char *HTAA_makeProtectionTemplate PARAMS((CONST char * docname));
/*

MIME Argument List Parser

 */


/* PUBLIC                                               HTAA_parseArgList()
**              PARSE AN ARGUMENT LIST GIVEN IN A HEADER FIELD
** ON ENTRY:
**      str     is a comma-separated list:
**
**                      item, item, item
**              where
**                      item ::= value
**                             | name=value
**                             | name="value"
**
**              Leading and trailing whitespace is ignored
**              everywhere except inside quotes, so the following
**              examples are equal:
**
**                      name=value,foo=bar
**                       name="value",foo="bar"
**                        name = value ,  foo = bar
**                         name = "value" ,  foo = "bar"
**
** ON EXIT:
**      returns a list of name-value pairs (actually HTAssocList*).
**              For items with no name, just value, the name is
**              the number of order number of that item. E.g.
**              "1" for the first, etc.
*/
PUBLIC HTList *HTAA_parseArgList PARAMS((char * str));

/*

 */

#endif  /* NOT HTAAUTIL_H */
/*

   End of file HTAAUtil.h.  */
