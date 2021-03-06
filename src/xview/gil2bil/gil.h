/*
 * Structures to map the GIL files in memory.
 */

struct gil {
	int type;		/* e.g. :base-window */ */
	char *name;		/* e.g., baseFrame */
	struct gil *owner;
	int width;		/* e.g. 679 */
	int height;		/* e.g. 702 */
	char *background-color;	/* e.g. "blue" */
	char *foreground-color;	/* e.g. "green" */
	char *label;		/* e.g. "Licensing: Client Information" */
	int label-type;		/* :string or :icon */
	int initial-state;	/* :open */
	int show-footer;	/* t */
	int resizable;		/* nil */
	char *icon-file;	/* e.g. "xgenlice.icon" */
	char *icon-label;	/* e.g. "XGenLicense" */
	char *icon-mask-file;	/* "" */
	char *event-handler;	/* nil */
	char *user-data;	/* () */
	char *actions;		/* () */
};
