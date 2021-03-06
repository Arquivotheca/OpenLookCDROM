/*  xvset 0.8--graphical interface for functions of xset.
    Copyright (C) 1994 Brian L. Brush <brush@acomp.usf.edu>,
    223 S. Riverhills Dr., Tampa, FL 33617-7240, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <xview/xview.h>
#include <xview/panel.h>

#define	VERSION	0.8

Frame frame;
Panel panel;
Display *dpy;
Panel_slider_item mouse_accel_slider,
	mouse_thresh_slider,
	bell_vol_slider,
	bell_pitch_slider,
	bell_dur_slider,
	saver_timeout_slider,
	saver_cycle_slider;
Panel_choice_item saver_blank, saver_exposure;
Panel_item repeat_check_box;

void
apply(Panel_item item, Event *event)
{
	int mouse_accel = (int)xv_get(mouse_accel_slider, PANEL_VALUE);
	int mouse_thresh = (int)xv_get(mouse_thresh_slider, PANEL_VALUE);
	int saver_timeout = (int)xv_get(saver_timeout_slider, PANEL_VALUE);
	int saver_cycle = (int)xv_get(saver_cycle_slider, PANEL_VALUE);
	int saver_blanking = (int)xv_get(saver_blank, PANEL_VALUE);
	int saver_exposures = (int)xv_get(saver_exposure, PANEL_VALUE);
	XKeyboardControl xkeyboardcontrol;

	xkeyboardcontrol.bell_percent = (int)xv_get(bell_vol_slider, PANEL_VALUE);
	xkeyboardcontrol.bell_pitch = (int)xv_get(bell_pitch_slider, PANEL_VALUE);
	xkeyboardcontrol.bell_duration = (int)xv_get(bell_dur_slider, PANEL_VALUE);
	xkeyboardcontrol.auto_repeat_mode = (int)xv_get(repeat_check_box, PANEL_VALUE);

	XSetScreenSaver(dpy, saver_timeout, saver_cycle, saver_blanking, saver_exposures);
	XChangePointerControl(dpy, (Bool)1, (Bool)1, mouse_accel, 1, mouse_thresh);
	XChangeKeyboardControl(dpy, 142, &xkeyboardcontrol);
}

void
defaults(Panel_item item, Event *event)
{
	xv_set(mouse_accel_slider, PANEL_VALUE, 2, XV_NULL);
	xv_set(mouse_thresh_slider, PANEL_VALUE, 4, XV_NULL);
	xv_set(bell_vol_slider, PANEL_VALUE, 50, XV_NULL);
	xv_set(bell_pitch_slider, PANEL_VALUE, 400, XV_NULL);
	xv_set(bell_dur_slider, PANEL_VALUE, 100, XV_NULL);
	xv_set(repeat_check_box, PANEL_VALUE, 1, XV_NULL);
	xv_set(saver_timeout_slider, PANEL_VALUE, 600, XV_NULL);
	xv_set(saver_cycle_slider, PANEL_VALUE, 600, XV_NULL);
	xv_set(saver_blank, PANEL_VALUE, 1, XV_NULL);
	xv_set(saver_exposure, PANEL_VALUE, 1, XV_NULL);
}

main(int argc, char *argv[])
{
    XKeyboardState xkeyboardstate;
    int panel_value_x, timeout, interval, prefer_blanking, allow_exposures, accel_numerator, accel_denominator, threshold;
    extern void exit();
    char version_info[]="xvset 0.8, Copyright (C) 1994 Brian L. Brush\nxvset comes with ABSOLUTELY NO WARRANTY.  This is free software, and you are\nwelcome to redistribute it under certain conditions.  See the file \"COPYING\"\nfor details.";


    (void) xv_init(XV_INIT_ARGC_PTR_ARGV, &argc, argv, NULL);

    if (argc > 1)
	{
		if (strcmp(argv[1], "-v") == 0)
		{
			puts(version_info);
			exit(0);
		}
		else
		{
			printf("unknown option %s\n", argv[1]);
			exit(1);
		}
	}
    frame = (Frame)xv_create((int)NULL, FRAME_CMD,
	FRAME_LABEL,		"Server Settings",
	FRAME_CMD_DEFAULT_PIN_STATE,	FRAME_CMD_PIN_IN,
	NULL);
    dpy = (Display *)xv_get(frame, XV_DISPLAY);
    XGetKeyboardControl(dpy, &xkeyboardstate);
    XGetScreenSaver(dpy, &timeout, &interval, &prefer_blanking, &allow_exposures);
    XGetPointerControl(dpy, &accel_numerator, &accel_denominator, &threshold);
    panel = (Panel)xv_get(frame, FRAME_CMD_PANEL);
    xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, NULL);
    mouse_accel_slider = xv_create(panel, PANEL_SLIDER,
	XV_HELP_DATA,		"xvset:mouse_acceleration",
        PANEL_LABEL_STRING,     "    Mouse Acceleration: ",
        PANEL_VALUE,            accel_numerator,
        PANEL_MIN_VALUE,        0,
        PANEL_MAX_VALUE,        10,
        PANEL_TICKS,            10,
        NULL);
    panel_value_x = (int)xv_get(mouse_accel_slider, PANEL_VALUE_X);
    mouse_thresh_slider = xv_create(panel, PANEL_SLIDER,
	XV_HELP_DATA,		"xvset:mouse_threshold",
	PANEL_LABEL_STRING,	"Mouse Threshold: ",
	PANEL_VALUE,		threshold,
	PANEL_MIN_VALUE,	0,
	PANEL_MAX_VALUE,	10,
	PANEL_TICKS,		10,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    bell_vol_slider = xv_create(panel, PANEL_SLIDER,
	XV_HELP_DATA,		"xvset:bell_volume",
	PANEL_LABEL_STRING,	"Bell Volume: ",
	PANEL_VALUE,		xkeyboardstate.bell_percent,
	PANEL_MIN_VALUE,	0,
	PANEL_MAX_VALUE,	100,
	PANEL_TICKS,		10,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    bell_pitch_slider = xv_create(panel, PANEL_SLIDER,
	XV_HELP_DATA,		"xvset:bell_pitch",
	PANEL_LABEL_STRING,	"Bell Pitch: ",
	PANEL_VALUE,		xkeyboardstate.bell_pitch,
	PANEL_MIN_VALUE,	0,
	PANEL_MAX_VALUE,	1000,
	PANEL_TICKS,		10,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    bell_dur_slider = xv_create(panel, PANEL_SLIDER,
	XV_HELP_DATA,		"xvset:bell_duration",
	PANEL_LABEL_STRING,	"Bell Duration: ",
	PANEL_VALUE,		xkeyboardstate.bell_duration,
	PANEL_MIN_VALUE,	0,
	PANEL_MAX_VALUE,	1000,
	PANEL_TICKS,		10,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    repeat_check_box = xv_create(panel, PANEL_CHECK_BOX,
	XV_HELP_DATA,		"xvset:auto-repeat",
	PANEL_LABEL_STRING,	"Auto-repeat: ",
	PANEL_VALUE,		xkeyboardstate.global_auto_repeat,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    saver_timeout_slider = xv_create(panel, PANEL_SLIDER,
	PANEL_LABEL_STRING,	"Screen Saver Timeout: ",
	XV_HELP_DATA,		"xvset:screen_saver_timeout",
	PANEL_VALUE,		timeout,
	PANEL_MIN_VALUE,	0,
	PANEL_MAX_VALUE,	6000,
	PANEL_TICKS,		10,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    saver_cycle_slider = xv_create(panel, PANEL_SLIDER,
	XV_HELP_DATA,		"xvset:screen_saver_cycle",
	PANEL_LABEL_STRING,	"Screen Saver Cycle: ",
	PANEL_VALUE,		interval,
	PANEL_MIN_VALUE,	0,
	PANEL_MAX_VALUE,	6000,
	PANEL_TICKS,		10,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    saver_blank = xv_create(panel, PANEL_CHOICE,
	XV_HELP_DATA,		"xvset:blanking",
	PANEL_LABEL_STRING,	"Blanking: ",
	PANEL_CHOICE_STRINGS,	"Noblank", "Blank", NULL,
	PANEL_VALUE,		prefer_blanking,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    saver_exposure = xv_create(panel, PANEL_CHOICE,
	XV_HELP_DATA,		"xvset:exposures",
	PANEL_LABEL_STRING,	"Exposures: ",
	PANEL_CHOICE_STRINGS,	"Noexpose", "Expose", NULL,
	PANEL_VALUE,		allow_exposures,
	PANEL_VALUE_X,		panel_value_x,
	NULL);
    xv_create(panel, PANEL_BUTTON,
	XV_HELP_DATA,		"xvset:apply",
	PANEL_LABEL_STRING,	"Apply",
	PANEL_NOTIFY_PROC,	apply,
	XV_X,			98,
	NULL);
    xv_set(panel, PANEL_LAYOUT, PANEL_HORIZONTAL, NULL);
    xv_create(panel, PANEL_BUTTON,
	XV_HELP_DATA,		"xvset:defaults",
	PANEL_LABEL_STRING,	"Defaults",
	PANEL_NOTIFY_PROC,	defaults,
	XV_X,			206,
	NULL);
    window_fit(panel);
    window_fit(frame);
    xv_main_loop(frame);
    exit(0);
}
