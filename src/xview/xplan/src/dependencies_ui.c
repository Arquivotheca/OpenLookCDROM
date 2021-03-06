/*
 * dependencies_ui.c - User interface object initialization functions.
 * This file was generated by `gxv' from `dependencies.G'.
 * DO NOT EDIT BY HAND.
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>
#include <xview/scrollbar.h>
#include <xview/svrimage.h>
#include <xview/termsw.h>
#include <xview/text.h>
#include <xview/tty.h>
#include <xview/xv_xrect.h>
#include "dependencies_ui.h"

/*
 * Create object `insertMenu' in the specified instance.
 */
Xv_opaque
dependencies_insertMenu_create(caddr_t ip, Xv_opaque owner)
{
	extern Menu_item	dependency_insert_before(Menu_item, Menu_generate);
	extern Menu_item	dependency_insert_after(Menu_item, Menu_generate);
	extern Menu_item	dependency_insert_top(Menu_item, Menu_generate);
	extern Menu_item	dependency_insert_bottom(Menu_item, Menu_generate);
	Xv_opaque	obj;
	
	obj = xv_create(XV_NULL, MENU_COMMAND_MENU,
		XV_KEY_DATA, INSTANCE, ip,
		MENU_ITEM,
			XV_KEY_DATA, INSTANCE, ip,
			MENU_STRING, "Before",
			MENU_GEN_PROC, dependency_insert_before,
			NULL,
		MENU_ITEM,
			XV_KEY_DATA, INSTANCE, ip,
			MENU_STRING, "After",
			MENU_GEN_PROC, dependency_insert_after,
			NULL,
		MENU_ITEM,
			XV_KEY_DATA, INSTANCE, ip,
			MENU_STRING, "Top",
			MENU_GEN_PROC, dependency_insert_top,
			NULL,
		MENU_ITEM,
			XV_KEY_DATA, INSTANCE, ip,
			MENU_STRING, "Bottom",
			MENU_GEN_PROC, dependency_insert_bottom,
			NULL,
		MENU_DEFAULT, 5,
		MENU_GEN_PIN_WINDOW, owner, "",
		NULL);
	return obj;
}

/*
 * Initialize an instance of object `dependenciesPopup'.
 */
dependencies_dependenciesPopup_objects *
dependencies_dependenciesPopup_objects_initialize(dependencies_dependenciesPopup_objects *ip, Xv_opaque owner)
{
	if (!ip && !(ip = (dependencies_dependenciesPopup_objects *) calloc(1, sizeof (dependencies_dependenciesPopup_objects))))
		return (dependencies_dependenciesPopup_objects *) NULL;
	if (!ip->dependenciesPopup)
		ip->dependenciesPopup = dependencies_dependenciesPopup_dependenciesPopup_create(ip, owner);
	if (!ip->control)
		ip->control = dependencies_dependenciesPopup_control_create(ip, ip->dependenciesPopup);
	if (!ip->dependenciesLabel)
		ip->dependenciesLabel = dependencies_dependenciesPopup_dependenciesLabel_create(ip, ip->control);
	if (!ip->dependencyList)
		ip->dependencyList = dependencies_dependenciesPopup_dependencyList_create(ip, ip->control);
	if (!ip->insertButton)
		ip->insertButton = dependencies_dependenciesPopup_insertButton_create(ip, ip->control);
	if (!ip->deleteButton)
		ip->deleteButton = dependencies_dependenciesPopup_deleteButton_create(ip, ip->control);
	if (!ip->dependencyField)
		ip->dependencyField = dependencies_dependenciesPopup_dependencyField_create(ip, ip->control);
	return ip;
}

/*
 * Create object `dependenciesPopup' in the specified instance.
 */
Xv_opaque
dependencies_dependenciesPopup_dependenciesPopup_create(dependencies_dependenciesPopup_objects *ip, Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, FRAME_CMD,
		XV_KEY_DATA, INSTANCE, ip,
		XV_WIDTH, 359,
		XV_HEIGHT, 326,
		XV_LABEL, "Dependencies",
		XV_SHOW, FALSE,
		FRAME_SHOW_FOOTER, TRUE,
		FRAME_SHOW_RESIZE_CORNER, TRUE,
		FRAME_CMD_PUSHPIN_IN, TRUE,
		NULL);
	xv_set(xv_get(obj, FRAME_CMD_PANEL), WIN_SHOW, FALSE, NULL);
	return obj;
}

/*
 * Create object `control' in the specified instance.
 */
Xv_opaque
dependencies_dependenciesPopup_control_create(dependencies_dependenciesPopup_objects *ip, Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL,
		XV_KEY_DATA, INSTANCE, ip,
		XV_HELP_DATA, "dependencies:control",
		XV_X, 0,
		XV_Y, 0,
		XV_WIDTH, WIN_EXTEND_TO_EDGE,
		XV_HEIGHT, WIN_EXTEND_TO_EDGE,
		WIN_BORDER, FALSE,
		NULL);
	return obj;
}

/*
 * Create object `dependenciesLabel' in the specified instance.
 */
Xv_opaque
dependencies_dependenciesPopup_dependenciesLabel_create(dependencies_dependenciesPopup_objects *ip, Xv_opaque owner)
{
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_MESSAGE,
		XV_KEY_DATA, INSTANCE, ip,
		XV_X, 16,
		XV_Y, 16,
		PANEL_LABEL_STRING, "Dependencies",
		PANEL_LABEL_BOLD, TRUE,
		NULL);
	return obj;
}

/*
 * Create object `dependencyList' in the specified instance.
 */
Xv_opaque
dependencies_dependenciesPopup_dependencyList_create(dependencies_dependenciesPopup_objects *ip, Xv_opaque owner)
{
	extern int		dependency_list_notify(Panel_item, char *, Xv_opaque, Panel_list_op, Event *, int);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_LIST,
		XV_KEY_DATA, INSTANCE, ip,
		XV_HELP_DATA, "dependencies:dependencyList",
		XV_X, 16,
		XV_Y, 40,
		PANEL_LIST_WIDTH, 300,
		PANEL_LIST_DISPLAY_ROWS, 10,
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, TRUE,
		PANEL_CHOOSE_ONE, TRUE,
		PANEL_CHOOSE_NONE, TRUE,
		PANEL_NOTIFY_PROC, dependency_list_notify,
		NULL);
	return obj;
}

/*
 * Create object `insertButton' in the specified instance.
 */
Xv_opaque
dependencies_dependenciesPopup_insertButton_create(dependencies_dependenciesPopup_objects *ip, Xv_opaque owner)
{
	extern Xv_opaque	dependencies_insertMenu_create(caddr_t, Xv_opaque);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, ip,
		XV_HELP_DATA, "dependencies:insertButton",
		XV_X, 88,
		XV_Y, 264,
		PANEL_LABEL_STRING, "Insert",
		PANEL_ITEM_MENU, dependencies_insertMenu_create((caddr_t) ip, ip->dependenciesPopup),
		NULL);
	return obj;
}

/*
 * Create object `deleteButton' in the specified instance.
 */
Xv_opaque
dependencies_dependenciesPopup_deleteButton_create(dependencies_dependenciesPopup_objects *ip, Xv_opaque owner)
{
	extern void		dependency_delete(Panel_item, Event *);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_BUTTON,
		XV_KEY_DATA, INSTANCE, ip,
		XV_HELP_DATA, "dependencies:deleteButton",
		XV_X, 184,
		XV_Y, 264,
		PANEL_LABEL_STRING, "Delete",
		PANEL_NOTIFY_PROC, dependency_delete,
		NULL);
	return obj;
}

/*
 * Create object `dependencyField' in the specified instance.
 */
Xv_opaque
dependencies_dependenciesPopup_dependencyField_create(dependencies_dependenciesPopup_objects *ip, Xv_opaque owner)
{
	extern Panel_setting	dependency_enter_callback(Panel_item, Event *);
	Xv_opaque	obj;
	
	obj = xv_create(owner, PANEL_TEXT,
		XV_KEY_DATA, INSTANCE, ip,
		XV_HELP_DATA, "dependencies:dependencyField",
		XV_X, 16,
		XV_Y, 304,
		PANEL_VALUE_DISPLAY_LENGTH, 30,
		PANEL_VALUE_STORED_LENGTH, 60,
		PANEL_LABEL_STRING, "Dependency:",
		PANEL_LAYOUT, PANEL_HORIZONTAL,
		PANEL_READ_ONLY, FALSE,
		PANEL_NOTIFY_PROC, dependency_enter_callback,
		NULL);
	return obj;
}

