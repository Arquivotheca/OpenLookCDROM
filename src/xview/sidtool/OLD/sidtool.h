
/*  @(#)sidtool.h 1.7 91/08/19
 *
 *  Definitions used by the sidtool program.
 *
 *  Screen design and original implementation
 *               Copyright (C) 1981, 1982, 1983 - Brad A. Myers
 *
 *  Current implementation
 *               Copyright (C) 1991 Rich Burridge
 *
 *  Permission is given to distribute these sources, as long as the
 *  copyright messages are not removed, and no monies are exchanged.
 *
 *  No responsibility is taken for any errors on inaccuracies inherent
 *  either to the comments or the code of this program, but if reported
 *  to me, then an attempt will be made to fix them.
 */

#include <setjmp.h>
#include <string.h>

/* For all function declarations, if ANSI then use a prototype. */

#if  defined(__STDC__)
#define P(args)  args
#else  /* ! __STDC__ */
#define P(args)  ()
#endif  /* STDC */

#define  SPEED          3                    /* Speed of this Sun. */
#define  S_NAME         "sidtool.hs"         /* Default highscore filename. */

typedef  int            BOOLEAN ;

char *getenv() ;

#define  CLOSE         (void) close       /* To make lint happy. */
#define  DOINC         (void) doinc
#define  FCLOSE        (void) fclose
#define  FGETS         (void) fgets
#define  FPRINTF       (void) fprintf
#define  FREE          (void) free
#define  GETHOSTNAME   (void) gethostname
#define  MEMSET        (void) memset
#define  READ          (void) read
#define  SSCANF        (void) sscanf
#define  SETJMP        (void) setjmp
#define  SPRINTF       (void) sprintf
#define  SRAND         (void) srand
#define  STRCAT        (void) strcat
#define  STRCPY        (void) strcpy
#define  WRITE         (void) write

#define  EQUAL(a, b)   !strncmp(a, b, strlen(b))

#define  SQUARE        26         /* Size of each square of the maze. */
#define  XBASE         45         /* X start of maze. */
#define  YBASE         15         /* Y start of maze. */

/* Set the function to be used for characters. */
#define  SCHRFUNC(f)  (sfunc = f)

/* Determine the bug index, POKEY to SHADOW. */
#define  GIND(x) ((x) - &bugs[0])

#define  AUTO_SCORE     5         /* Player number to update in auto mode. */
#define  FRUIT_SCORE    6
#define  HIGH_SCORE     7

#define  BORDER_SIZE    7         /* Frame border width (in pixels). */
#define  BSPACE         8         /* Backspace used by getline. */
#define  BUTXOFF        30        /* X offset of buttons. */
#define  BUTYOFF        10        /* Y offset of buttons. */
#define  CATCHUP        3         /* Amount screen catches up blue bug. */
#define  CR             13
#define  DEF_PLAYERS    1         /* Default number of players. */
#define  DEF_SKILL      5         /* Default skill level. */
#define  DEL            127       /* Used to start the sid tool game. */
#define  ENKEY          01652     /* Encode key used in highscore file. */
#define  FALSE          0
#define  FRUITMX        13        /* X maze position of fruit. */
#define  FRUITMY        16        /* Y maze position of fruit. */
#define  GOFFSET        SQUARE / 2 - 3     /* Offset of bugs and screen. */
#define  INC            argc-- ; argv++ ;
#define  LPAUSE         20000     /* Default pause length (microseconds). */
#define  MAXIMAGES      61        /* Maximum number of icon images. */
#define  MAXNUMPLAYERS  4         /* Number of players allowed. */
#define  MAXLINE        80        /* Maximum string length. */
#define  MINMOVE        20
#define  SLUG_MSECS     5         /* Default main loop time (millisecs). */
#define  SWIDTH         768       /* Maximum screen width. */
#define  SHEIGHT        900       /* Maximum screen height. */
#define  TRUE           1
#define  XSIZE          26        /* Number of squares in x. */
#define  YSIZE          28        /* Number of squares in y. */

enum font_type { BOLDFONT, NORMALFONT } ;   /* Font types used by sidtool. */

enum rop_type { RRPL, RXOR, RCLR, RSET, RINV } ;   /* Rasterop types. */

/* States for the sidtool automation. */

enum sid_type { STARTUP,  INITGAME,  PLAY,      DOPLAY,
                MAKEPLAY, DOREST,    HIGHSCORE, DOCREDIT,
                MOVELEFT, MOVERIGHT, DOLEAVE,   RESETGAME } ;

enum but_type { BUT_HELP,  BUT_PROPS, BUT_SCORES, BUT_QUIT,
                BUT_NEW,   BUT_STOP,  BUT_CONT,   BUT_APPLY,
                BUT_RESET } ;

enum choice_type { C_PLAY } ;

/* Sidtool images. */

enum icon_type { CURCIRCLE, CORNER,    CORNER1,   CORNER2,
                 CORNER3,   BDOT,      SDOT,      CIRCLES,
                 CIRCLES1,  CIRCLES2,  CIRCLES3,  CIRCLES4,
                 CIRCLES5,  CIRCLES6,  CIRCLES7,  CIRCLES8,
                 CIRCLES9,  CIRCLES10, CIRCLES11, CIRCLES12,
                 CIRCLES13, CIRCLES14, CIRCLES15, BUGPICS,
                 BUGPICS1,  BUGPICS2,  BUGPICS3,  BUGPICS4,
                 BUGPICS5,  BUGPICS6,  BUGPICS7,  BLUEBUG,
                 BLUEBUG1,  BLUEPICS,  BLUEPICS1, EYES,
                 EYES1,     EYES2,     EYES3,     EXPLODE,
                 EXPLODE1,  EXPLODE2,  EXPLODE3,  EXPLODE4,
                 EXPLODE5,  EXPLODE6,  EXPLODE7,  EXPLODE8,
                 FRUIT,     FRUIT1,    FRUIT2,    FRUIT3,
                 FRUIT4,    FRUIT5,    FRUIT6,    FRUIT7, 
                 FRUIT8,    RJSTICK,   UJSTICK,   LJSTICK,
                 DJSTICK } ;

enum corner_type { UR, RD, DL, LU } ;                 /* Corners. */

enum dot_type { NODOT, BIGDOT, SMALLDOT } ;           /* Dotsize. */

enum dir_type { RIGHT, UP, LEFT, DOWN } ;             /* Direction. */

enum ghost_type { POKEY, BASHFUL, SPEEDY, SHADOW } ;  /* Ghostnames. */

enum image_type { CNTR1,    CNTR2,   CNTR3, CNTR4,    /* Panel images. */
                  CURFRUIT } ;

enum mes_type { M_PLAYER1, M_PLAYER2,                 /* Message types. */
                M_PLAYER3, M_PLAYER4 } ;

enum res_type { R_PLAYERS, R_SKILL, R_PAUSE, R_SLUG } ;  /* Resource types. */

enum sound_type { S_DIE, S_EATBIGDOT, S_EATBLUE,         /* Sound types. */
                  S_EATFRUIT, S_ERROR, S_NEWSCREEN } ;

enum win_type { W_PROPS, W_SCORES } ;                 /* Window types. */

enum prop_type { P_SKILL, P_PLAYERS } ;      /* Property window types. */

struct scorerec
         {
           char who[MAXLINE] ;
           int score ;
         } ;

struct bugrec
         {
           enum dir_type dir ;
           BOOLEAN eyesonly ;      /* Going to box. */
           BOOLEAN enteringbox ;   /* Going down into box. */
           BOOLEAN inbox ;         /* Inside or leaving. */
           BOOLEAN intunnel ;
           int delay ;
           int scrx, scry ;
           int mx, my ;
           int bluetime ;          /* If zero then not blue. */
           int boxtime ;           /* Countdown until leave. */
           int count ;             /* Incremented every tic. */
           int pic ;               /* 0 or 1. */
         } ;

struct startrec
         {
           int x, y, time ;
         } ;

char *get_resource P((enum res_type)) ;

enum dir_type dorandomdir P((enum dir_type, int, int, int, int,
                             int *, int *, int *, int *, int)) ;
enum dir_type reversedir P((enum dir_type)) ;

int checkinc P((enum dir_type, int, int)) ;
int checkcollision P((int, int, int *)) ;
int doinc P((enum dir_type, int, int, int, int, int *, int *, int *, int *)) ;
int gcentered P((int, int)) ;
int get_choice P((enum choice_type)) ;
int get_int_resource P((enum res_type, int *)) ;
int get_value P((enum prop_type)) ;
int main P((int, char **)) ;
int randomrange P(()) ;

void activate_button P((enum but_type, int)) ;
void activate_choice P((enum choice_type, int)) ;
void activate_message P((enum mes_type, int)) ;
void blinkpause P(()) ;
void blt_mem P((enum icon_type, int, int, int, int, enum rop_type,
                enum icon_type src, int, int)) ;
void blt_mem_to_scrn P((int, int, int, int, enum rop_type,
                        enum icon_type, int, int)) ;
void blt_scrn P((int, int, int, int, enum rop_type)) ;
void changebugs P(()) ;
void changeplayers P((int)) ;
void clear_screen P(()) ;
void cmdbool P((int, char *, char *)) ;
void cmdint P((int, int, char *, char *)) ;
void cmdstr P((char *, char *, char *)) ;
void destroyblue P((struct bugrec *)) ;
void destroyfruit P(()) ;
void do_flush P(()) ;
void do_startup P(()) ;
void docredits P(()) ;
void dohelp P(()) ;
void doplay P(()) ;
void doupdate P(()) ;
void draw_joystick P((enum dir_type)) ;
void draw_text P((int, int, enum font_type, char *)) ;
void drawbox P((int, int)) ;
void drawbug P((struct bugrec *)) ;
void drawcir P((enum icon_type, int, int)) ;
void drawcorner P((enum corner_type, int, int)) ;
void drawdot P((enum dot_type, int, int)) ;
void drawline P((int, int, int)) ;
void drawmaze P(()) ;
void erasebugs P(()) ;
void explodecircle P((int, int)) ;
void fixexits P(()) ;
void get_icons P(()) ;
void get_options P(()) ;
void geths P((struct scorerec *, int)) ;
void getparam P((char *, char **, char *)) ;
void handlecollision P((struct bugrec *)) ;
void headto P((int, int, int, int, int, int, enum dir_type *,
               int *, int *, int *, int *)) ;
void initgame P(()) ;
void init_options P(()) ;
void init_rops P(()) ;
void initialize P(()) ;
void load_image P((enum icon_type, unsigned char *)) ;
void load_resources P(()) ;
void longpause P((int)) ;
void make_play P(()) ;
void make_sound P((enum sound_type)) ;
void move_left P(()) ;
void move_right P(()) ;
void newbugs P((int)) ;
void next_state P(()) ;
void ppause P((int)) ;
void play P(()) ;
void position_popup P((enum win_type)) ;
void process_button P((char *)) ;
void puths P((struct scorerec, int)) ;
void read_resources P(()) ;
void readallhighscores P(()) ;
void removecircle P(()) ;
void resetmaze P(()) ;
void restore_screen P(()) ;
void save_cmdline P((char *)) ;
void savescorefile P(()) ;
void set_choice P((enum choice_type, int)) ;
void set_cursor P((int)) ;
void set_highscore P((int)) ;
void set_image P((enum image_type, enum icon_type, enum rop_type)) ;
void set_label P((enum but_type)) ;
void set_list P((int)) ;
void set_score P((int, int)) ;
void set_timer P((int)) ;
void set_value P((enum prop_type, int)) ;
void setdots P((int)) ;
void show_window P((enum win_type, int)) ;
void showplayerscore P((int)) ;
void transpt P((int, int, int *, int *)) ;
void untranspt P((int, int, int *, int *)) ;
void updatebugs P(()) ;
void updatefruit P(()) ;
void updatescore P((int)) ;
void warp_mouse P(()) ;
void write_cmdline P(()) ;
void writehighscore P(()) ;
