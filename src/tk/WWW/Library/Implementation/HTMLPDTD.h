/*                                          The HTML PLUS DTD -- software interface in libwww
                            HTML PLUS DTD - SOFTWARE INTERFACE
                                             
   SGML purists should excuse the use of the term "DTD" in this file to represent
   DTD-related information which is not exactly a DTD itself.
   
   The C modular structure doesn't work very well here, as the dtd is partly in the .h and
   partly in the .c which are not very independent.  Tant pis.
   
   There are a couple of HTML-specific utility routines also defined.
   
 */
#ifndef HTMLDTD_H
#define HTMLDTD_H

#include "HTUtils.h"
#include "SGML.h"

/*

Element Numbers

 */

/*

   Must Match all tables by element! These include tables in HTMLPDTD.c and code in HTML.c
   .
   
   Differences from Internet Draft 00: HTML, H7,PLAINTEXT LISTINGand XMP left in.
   
 */
typedef enum _HTMLElement {
        HTML_A,
        HTML_ABBREV,
        HTML_ABSTRACT,
        HTML_ACRONYM,
        HTML_ADDED,
        HTML_ADDRESS,
        HTML_ARG,
        HTML_B,
        HTML_BASE,
        HTML_BLOCKQUOTE,
        HTML_BODY,
        HTML_BOX,
        HTML_BR,
        HTML_BYLINE,
        HTML_CAPTION,
        HTML_CHANGED,
        HTML_CITE,
        HTML_CMD,
        HTML_CODE,
        HTML_COMMENT,
        HTML_DD,
        HTML_DFN,
        HTML_DIR,
        HTML_DL,
        HTML_DT,
        HTML_EM,
        HTML_FIG,
        HTML_FOOTNOTE,
        HTML_FORM,
        HTML_H1,
        HTML_H2,
        HTML_H3,
        HTML_H4,
        HTML_H5,
        HTML_H6,
        HTML_H7,
        HTML_HEAD,
        HTML_HR,
        HTML_HTML,
        HTML_HTMLPLUS,
        HTML_I,
        HTML_IMAGE,
        HTML_IMG,
        HTML_INPUT,
        HTML_ISINDEX,
        HTML_KBD,
        HTML_L,
        HTML_LI,
        HTML_LINK,
        HTML_LISTING,
        HTML_LIT,
        HTML_MARGIN,
        HTML_MATH,
        HTML_MENU,
        HTML_NEXTID,
        HTML_NOTE,
        HTML_OL,
        HTML_OPTION,
        HTML_OVER,
        HTML_P,
        HTML_PERSON,
        HTML_PLAINTEXT,
        HTML_PRE,
        HTML_Q,
        HTML_QUOTE,
        HTML_RENDER,
        HTML_REMOVED,
        HTML_S,
        HTML_SAMP,
        HTML_SELECT,
        HTML_STRONG,
        HTML_SUB,
        HTML_SUP,
        HTML_TAB,
        HTML_TABLE,
        HTML_TD,
        HTML_TEXTAREA,
        HTML_TH,
        HTML_TITLE,
        HTML_TR,
        HTML_TT,
        HTML_U,
        HTML_UL,
        HTML_VAR,
        HTML_XMP }
HTMLElement;

#define HTMLP_ELEMENTS 85

/*

Attribute numbers

 */

/*

   Identifier is HTML_<element>_<attribute>. These must match the tables in HTMLPDTD.c !
   
 */
#define HTML_A_EFFECT           0
#define HTML_A_HREF             1
#define HTML_A_ID               2
#define HTML_A_METHODS          3
#define HTML_A_NAME             4
#define HTML_A_PRINT            5
#define HTML_A_REL              6
#define HTML_A_REV              7
#define HTML_A_SHAPE            8
#define HTML_A_TITLE            9
#define HTML_A_ATTRIBUTES       10

#define HTML_BASE_ATTRIBUTES    1
#define HTML_FORM_ATTRIBUTES    5

#define HTML_FIG_ATTRIBUTES     6

#define HTML_GEN_ATTRIBUTES     3

#define HTML_HTMLPLUS_ATTRIBUTES        2

#define HTML_IMAGE_ATTRIBUTES   5

#define HTML_CHANGED_ATTRIBUTES 2
#define HTML_DL_ATTRIBUTES      3

#define DL_COMPACT 0

#define HTML_IMG_ALIGN          0
#define HTML_IMG_ALT            1
#define HTML_IMG_ISMAP          2       /* Obsolete but supported */
#define HTML_IMG_SEETHRU        3

#define HTML_IMG_SRC            4

#define HTML_IMG_ATTRIBUTES     5

#define HTML_INPUT_ALIGN        0
#define HTML_INPUT_CHECKED      1
#define HTML_INPUT_DISABLED     2
#define HTML_INPUT_ERROR        3
#define HTML_INPUT_MAX          4
#define HTML_INPUT_MIN          5
#define HTML_INPUT_NAME         6
#define HTML_INPUT_SIZE         7
#define HTML_INPUT_SRC          8
#define HTML_INPUT_TYPE         9
#define HTML_INPUT_VALUE        10
#define HTML_INPUT_ATTRIBUTES   11

#define HTML_L_ATTRIBUTES       4

#define HTML_LI_ATTRIBUTES      4
#define HTML_LIST_ATTRIBUTES    4

#define HTML_LINK_ATTRIBUTES    5

#define HTML_ID_ATTRIBUTE       1

#define HTML_NEXTID_ATTRIBUTES  1
#define HTML_NEXTID_N 0

#define HTML_NOTE_ATTRIBUTES    4

#define HTML_OPTION_ATTRIBUTES  3

/* #define HTML_PRE_WIDTH               0
  #define HTML_PRE_ATTRIBUTES   1
*/
#define HTML_RENDER_ATTRIBUTES  2

#define HTML_SELECT_ATTRIBUTES  4
#define HTML_TAB_ATTRIBUTES     2
#define HTML_TABLE_ATTRIBUTES   4
#define HTML_TD_ATTRIBUTES      4

#define HTML_TEXTAREA_ATTRIBUTES        6

#define HTML_TH_ATTRIBUTES      4

#define HTML_UL_ATTRIBUTES      6

extern CONST SGML_dtd HTMLP_dtd;


/*

Start anchor element

   It is kinda convenient to have a particulr routine for starting an anchor element, as
   everything else for HTML is simple anyway.
   
  ON ENTRY
  
   targetstream poinst to a structured stream object.
   
   name and href point to attribute strings or are NULL if the attribute is to be omitted.
   
 */
extern void HTStartAnchor PARAMS((
                HTStructured * targetstream,
                CONST char *    name,
                CONST char *    href));




/*

Specify next ID to be used

   This is anoter convenience routine, for specifying the next ID to be used by an editor
   in the series z1. z2,...
   
 */
extern void HTNextID PARAMS((HTStructured * targetStream, int n));

#endif /* HTMLDTD_H */

/*

   End of module definition  */
