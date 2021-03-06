/*
 * olxvman_stubs.c - Notify and event callback function stubs.
 * This file was generated by `gxv' from `olxvman.G'.
 * DO NOT EDIT BY HAND.
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/panel.h>
#include <xview/textsw.h>
#include <xview/xv_xrect.h>
#include <xview/notice.h>
#include <gdd.h>
#include "olxvman_ui.h"
#include "globals.h"

Notify_value	DestroyProcess();

/*
 * Instance XV_KEY_DATA key.  An instance is a set of related
 * user interface objects.  A pointer to an object's instance
 * is stored under this key in every object.  This must be a
 * global variable.
 */
Attr_attribute	INSTANCE;

void
  main(argc, argv)
int		argc;
char		**argv;
{
    /*
     * Initialize XView.
     */
    xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, 0);
    INSTANCE = xv_unique_key();
    
    /*
     * Initialize user interface components.
     */
    OLXVMan = olxvman_OLXVMan_objects_initialize(NULL, NULL);
    SearchPopup = olxvman_SearchPopup_objects_initialize(NULL, OLXVMan->OLXVMan);

    /*
     * Set Destroy function for window.
     */
    notify_interpose_destroy_func (OLXVMan->OLXVMan, DestroyProcess);

    /*
     * Set up manual page text windows.
     */
    InitManpages ();
    MakeManpagePopup ();

    /*
     * Get the section list.
     */
    if ((sections = Man()) == 0)
      PrintError ("There are no manual sections to display, check your MANPATH.");
    
    /*
     * Build the scrolling lists.
     */
    BuildSectionList ();
    BuildPageList (1);
    man_pages_shown = 0;
    
    /*
     * Turn control over to XView.
     */
    xv_main_loop(OLXVMan->OLXVMan);
    exit(0);
}

InitManpages ()
{
    int		i;

    for (i = 0; i < NUMMANPAGES; i++)
      ManualPages[i] = NULL;

    CurrentManualPage = 0;
}

MakeManpagePopup ()
{
    int		i;
    char	FoundNew = 0;

    for (i = 0; i < NUMMANPAGES; i++)
    {
	if (ManualPages[i] == NULL)
	{
	    FoundNew = 1;
	    CurrentManualPage = i;
	    break;
	}
    }

    if (!FoundNew)
      return;

    ManualPages[CurrentManualPage] = olxvman_ManualPagePopup_objects_initialize (NULL,
							  OLXVMan->OLXVMan);
    xv_set (ManualPages[CurrentManualPage]->ManualPageText, 
	    TEXTSW_MEMORY_MAXIMUM, 200000, NULL);
}

BuildSectionList ()
{
    int		sect;

    for (sect = 0; sect < sections; sect++)
    {
	xv_set (OLXVMan->ManualSections,
		PANEL_LIST_STRING, sect, manual[sect].blabel,
		PANEL_LIST_CLIENT_DATA, sect, (caddr_t)(sect + 1),
		NULL);
    }

    xv_set (OLXVMan->ManualSections,
	    PANEL_LIST_SELECT, 0, TRUE, NULL);
}

BuildPageList (section_number)
int	section_number;
{
    int		page;
    int		sect_index = section_number - 1;
    char	*man_name;

    xv_set (OLXVMan->ManualPages, XV_SHOW, FALSE, NULL);
    for (page = 0; page < manual[sect_index].nentries; page++)
    {
	man_name = CreateManpageName (manual[sect_index].entries[page]);
	xv_set (OLXVMan->ManualPages,
		PANEL_LIST_STRING, page, man_name,
		PANEL_LIST_CLIENT_DATA, page, (caddr_t)page,
		NULL);
	free (man_name);
    }
    xv_set (OLXVMan->ManualPages, XV_SHOW, TRUE, NULL);
    CurrentSection = sect_index;
}

void
RemovePageList (section_number)
int	section_number;
{
    int		page;
    int		sect_index = section_number - 1;

    xv_destroy (OLXVMan->ManualPages);
    OLXVMan->ManualPages = olxvman_OLXVMan_ManualPages_create (OLXVMan, OLXVMan->controls1);
    return;
}

void
  PutUpManpage (file, label)
FILE	*file;
char	*label;
{
    Xv_opaque cText = ManualPages[CurrentManualPage]->ManualPageText;
    Xv_opaque cPopup = ManualPages[CurrentManualPage]->ManualPagePopup;
    char	label_buff[50];
    
    if ((int)xv_get (cPopup, FRAME_CMD_PUSHPIN_IN) == TRUE)
    {
	MakeManpagePopup ();
	cText = ManualPages[CurrentManualPage]->ManualPageText;
	cPopup = ManualPages[CurrentManualPage]->ManualPagePopup;
    }
    else
      textsw_reset (cText, 0, 0);
    OpenFile (cText, file);
    xv_set (cPopup, XV_SHOW, TRUE, NULL);
    sprintf (label_buff, "Manual page for %s.", label);
    xv_set (cPopup, XV_LABEL, label_buff, NULL);
}

void
  CleanUpManpages ()
{
    int i;
    
    for (i = 0; i < NUMMANPAGES; i++)
      if (ManualPages[i] != NULL)
	textsw_reset (ManualPages[i]->ManualPageText, 0, 0);
}

Notify_value
  DestroyProcess (client, status)
Notify_client client;
Destroy_status status;
{
    switch (status)
    {
      case DESTROY_CHECKING:
	break;
	
      case DESTROY_CLEANUP:
	CleanUpManpages ();
	xv_destroy_safe (OLXVMan->OLXVMan);
	exit (0);
	break;
	
      case DESTROY_SAVE_YOURSELF:
	break;
	
      default:
	break;
    }
    
    return (NOTIFY_DONE);
}

/*
 * Menu handler for `OptionMenu (Search...)'.
 */
Menu_item
  SearchHandler(item, op)
Menu_item	item;
Menu_generate	op;
{
    olxvman_OLXVMan_objects * ip = (olxvman_OLXVMan_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
    
    switch (op) {
      case MENU_DISPLAY:
	break;
	
      case MENU_DISPLAY_DONE:
	break;
	
      case MENU_NOTIFY:
	xv_set (SearchPopup->SearchPopup, XV_SHOW, TRUE, NULL);
	break;
	
      case MENU_NOTIFY_DONE:
	break;
    }
    return item;
}

/*
 * Menu handler for `OptionMenu (Show Version)'.
 */
Menu_item
  ShowVersionHandler(item, op)
Menu_item	item;
Menu_generate	op;
{
    olxvman_OLXVMan_objects * ip = (olxvman_OLXVMan_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
    int		x;
    int		y;
    
    switch (op) {
      case MENU_DISPLAY:
	break;
	
      case MENU_DISPLAY_DONE:
	break;
	
      case MENU_NOTIFY:
	x = xv_get (ip->OptionButton, XV_X, NULL);
	y = xv_get (ip->OptionButton, XV_Y, NULL);
	x += xv_get (ip->OptionButton, XV_WIDTH, NULL) / 2;
	y += xv_get (ip->OptionButton, XV_HEIGHT, NULL) / 2;
	notice_prompt (ip->controls1, NULL,
		       NOTICE_MESSAGE_STRINGS, XVMAN_VERSION, NULL,
		       NOTICE_BUTTON,	"Ok", 101,
		       NOTICE_FOCUS_XY,	x, y,
		       NULL);
	break;
	
      case MENU_NOTIFY_DONE:
	break;
    }
    return item;
}

/*
 * Menu handler for `OptionMenu (Print Current Page)'.
 */
Menu_item
  PrintHandler(item, op)
Menu_item	item;
Menu_generate	op;
{
    olxvman_OLXVMan_objects * ip = (olxvman_OLXVMan_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
    int	x,y;
    
    switch (op) {
      case MENU_DISPLAY:
	break;
	
      case MENU_DISPLAY_DONE:
	break;
	
      case MENU_NOTIFY:
	if (CurrentEntry < 0 || CurrentSection < 0)
	{
	    x = xv_get (ip->OptionButton, XV_X, NULL);
	    y = xv_get (ip->OptionButton, XV_Y, NULL);
	    x += xv_get (ip->OptionButton, XV_WIDTH, NULL) / 2;
	    y += xv_get (ip->OptionButton, XV_HEIGHT, NULL) / 2;
	    notice_prompt (ip->controls1, NULL,
			   NOTICE_MESSAGE_STRINGS, "There is no manual page selected!", NULL,
			   NOTICE_BUTTON,	"Ok", 101,
			   NOTICE_FOCUS_XY,	x, y,
			   NULL);
	    return item;
	}
	PrintFormat (manual[CurrentSection].entries[CurrentEntry]);
	break;
	
      case MENU_NOTIFY_DONE:
	break;
    }
    return item;
}

/*
 * Menu handler for `OptionMenu (Quit)'.
 */
Menu_item
  QuitHandler(item, op)
Menu_item	item;
Menu_generate	op;
{
    olxvman_OLXVMan_objects * ip = (olxvman_OLXVMan_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
    int	i;
    
    switch (op) {
      case MENU_DISPLAY:
	break;
	
      case MENU_DISPLAY_DONE:
	break;
	
      case MENU_NOTIFY:
	CleanUpManpages ();
	xv_destroy_safe (ip->OLXVMan);
	exit (0);
	break;
	
      case MENU_NOTIFY_DONE:
	break;
    }
    return item;
}

/*
 * Notify callback function for `ManualSections'.
 */
int
  ManualSectionNotify(item, string, client_data, op, event)
Panel_item	item;
char		*string;
Xv_opaque	client_data;
Panel_list_op	op;
Event		*event;
{
    olxvman_OLXVMan_objects	*ip = (olxvman_OLXVMan_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
    
    switch(op) {
      case PANEL_LIST_OP_DESELECT:
	RemovePageList ((int)client_data);
	break;
	
      case PANEL_LIST_OP_SELECT:
	xv_set (OLXVMan->OLXVMan, FRAME_BUSY, TRUE, NULL);
	BuildPageList ((int)client_data);
	xv_set (OLXVMan->OLXVMan, FRAME_BUSY, FALSE, NULL);
	break;
	
      case PANEL_LIST_OP_VALIDATE:
	fprintf(stderr, "olxvman: ManualSectionNotify: PANEL_LIST_OP_VALIDATE: %s\n",string);
	break;
	
      case PANEL_LIST_OP_DELETE:
	fprintf(stderr, "olxvman: ManualSectionNotify: PANEL_LIST_OP_DELETE: %s\n",string);
	break;
    }
    return XV_OK;
}

/*
 * Notify callback function for `ManualPages'.
 */
int
  ManualPageNotify(item, string, client_data, op, event)
Panel_item	item;
char		*string;
Xv_opaque	client_data;
Panel_list_op	op;
Event		*event;
{
    olxvman_OLXVMan_objects	*ip = (olxvman_OLXVMan_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
    Xv_opaque cText = ManualPages[CurrentManualPage]->ManualPageText;
    Xv_opaque cPopup = ManualPages[CurrentManualPage]->ManualPagePopup;
    FILE	*file;
    
    switch(op) {
      case PANEL_LIST_OP_DESELECT:
	break;
	
      case PANEL_LIST_OP_SELECT:
	file = FindManualFile (CurrentSection, (int)client_data);
	PutUpManpage (file, CreateManpageName (manual[CurrentSection].entries[(int)client_data]));
	CurrentEntry = (int)client_data;
	fclose (file);
	break;
	
      case PANEL_LIST_OP_VALIDATE:
	fprintf(stderr, "olxvman: ManualPageNotify: PANEL_LIST_OP_VALIDATE: %s\n",string);
	break;
	
      case PANEL_LIST_OP_DELETE:
	fprintf(stderr, "olxvman: ManualPageNotify: PANEL_LIST_OP_DELETE: %s\n",string);
	break;
    }
    return XV_OK;
}

/*
 * Notify callback function for `ManualButton'.
 */
void
  SearchNotify(item, event)
Panel_item	item;
Event		*event;
{
    olxvman_SearchPopup_objects	*ip = (olxvman_SearchPopup_objects *) xv_get(item, XV_KEY_DATA, INSTANCE);
    char	tValue[100];
    FILE	*file;

    strcpy (tValue, (char *)xv_get (SearchPopup->SearchField, PANEL_VALUE, NULL));

    if (item == SearchPopup->ManualButton)
      file = DoSearch (tValue, MANUAL);
    else
      file = DoSearch (tValue, APROPOS);

    if (file != NULL)
    {
	PutUpManpage (file, tValue);
	fclose (file);
    }
}

/*
 * Notify callback function for Manual pinning functions.
 */
void
  ManualPagePinned (frame)
Frame	frame;
{
    int		i;
    
    for (i = 0; i < NUMMANPAGES; i++)
    {
	if (ManualPages[i] != NULL && ManualPages[i]->ManualPagePopup == frame)
	{
	    /* If this is the current manual page, then don't destroy it. */
	    if (i != CurrentManualPage)
	    {
		textsw_reset (ManualPages[i]->ManualPageText, 0, 0);
		xv_destroy (ManualPages[i]->ManualPageText);
		xv_destroy (ManualPages[i]->ManualPagePopup);
		ManualPages[i] = NULL;
	    }
	    else
	      xv_set (frame, XV_SHOW, FALSE, NULL);
	}
    }
}
