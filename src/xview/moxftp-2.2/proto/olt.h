static void Context_help P_(());
static void WM_ACTION_cb P_((Widget, XtPointer, XtPointer));
static void WM_DELETE_UNREGISTER P_((Widget));
static void clear_line_act P_((Widget, XEvent*, String*, Cardinal*));
static Widget create_scrolled_text P_((Widget, String, Arg*, Cardinal));
static void delete_char_act P_((Widget, XEvent*, String*, Cardinal*));
static void delete_view P_((Widget));
static void delete_view_file_cb P_((Widget, XtPointer, XtPointer));
static void dispatch_act P_((Widget, XEvent*, String*, Cardinal*));
static void draw_sep_cb P_((Widget, XEvent*, Region));
static Widget findwidget P_((Widget, int, int));
static void help_act P_((Widget, XEvent*, String*, Cardinal*));
static void help_cb P_((Widget, char*, caddr_t));
static void help_context_f P_(());
static void insert_char_act P_((Widget, XEvent*, String*, Cardinal*));
static void insert_char_cb P_((Widget, XtPointer, XtPointer));
static void my_popup_act P_((Widget, XEvent *, String *, Cardinal *));
static void set_gateway_cb P_((Widget, caddr_t, caddr_t));
static void set_retry_cb P_((Widget, caddr_t, caddr_t));
static void set_view_file_cb P_((Widget, XtPointer, XtPointer));
static void set_width_cb P_((Widget, XtPointer, XtPointer));
static void take_over_cb P_((Widget, XtPointer, XtPointer));
