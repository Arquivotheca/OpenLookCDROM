#ifndef	taskinfo_HEADER
#define	taskinfo_HEADER

/*
 * taskinfo_ui.h - User interface object and function declarations.
 * This file was generated by `gxv' from `taskinfo.G'.
 * DO NOT EDIT BY HAND.
 */

extern Attr_attribute	INSTANCE;


typedef struct {
	Xv_opaque	taskinfoPopup;
	Xv_opaque	control;
	Xv_opaque	name;
	Xv_opaque	options;
	Xv_opaque	description;
	Xv_opaque	duration;
	Xv_opaque	floatTime;
	Xv_opaque	startDateLabel;
	Xv_opaque	endDateLabel;
	Xv_opaque	plannedStartDate;
	Xv_opaque	plannedEndDate;
	Xv_opaque	actualStartDate;
	Xv_opaque	actualEndDate;
	Xv_opaque	dependenciesButton;
	Xv_opaque	forecastStartDate;
	Xv_opaque	forecastEndDate;
	Xv_opaque	earliestStartDate;
	Xv_opaque	earliestEndDate;
	Xv_opaque	resourcesButton;
	Xv_opaque	latestStartDate;
	Xv_opaque	latestEndDate;
	Xv_opaque	applyButton;
	Xv_opaque	resetButton;
} taskinfo_taskinfoPopup_objects;

extern taskinfo_taskinfoPopup_objects	*taskinfo_taskinfoPopup_objects_initialize(taskinfo_taskinfoPopup_objects *, Xv_opaque);

extern Xv_opaque	taskinfo_taskinfoPopup_taskinfoPopup_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_control_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_name_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_options_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_description_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_duration_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_floatTime_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_startDateLabel_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_endDateLabel_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_plannedStartDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_plannedEndDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_actualStartDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_actualEndDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_dependenciesButton_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_forecastStartDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_forecastEndDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_earliestStartDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_earliestEndDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_resourcesButton_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_latestStartDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_latestEndDate_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_applyButton_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
extern Xv_opaque	taskinfo_taskinfoPopup_resetButton_create(taskinfo_taskinfoPopup_objects *, Xv_opaque);
#endif
