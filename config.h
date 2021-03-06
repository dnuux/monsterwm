/* see LICENSE for copyright and license */

#ifndef CONFIG_H
#define CONFIG_H

/** modifiers **/
#define MOD1            Mod1Mask    /* ALT key */
#define MOD4            Mod4Mask    /* Super/Windows key */
#define CONTROL         ControlMask /* Control key */
#define SHIFT           ShiftMask   /* Shift key */

/** generic settings **/
#define MASTER_SIZE     0.52
#define SHOW_PANEL      True      /* show panel by default on exec */
#define TOP_PANEL       True      /* False means panel is on bottom */
#define PANEL_HEIGHT    18        /* 0 for no space for panel, thus no panel */
#define DEFAULT_MODE    TILE      /* initial layout/mode: TILE MONOCLE BSTACK GRID FLOAT */
#define ATTACH_ASIDE    True      /* False means new window is master */
#define FOLLOW_WINDOW   False     /* follow the window when moved to a different desktop */
#define FOLLOW_MONITOR  False     /* follow the window when moved to a different monitor */
#define FOLLOW_MOUSE    False     /* focus the window the mouse just entered */
#define CLICK_TO_FOCUS  True      /* focus an unfocused window when clicked  */
#define FOCUS_BUTTON    Button3   /* mouse button to be used along with CLICK_TO_FOCUS */
#define BORDER_WIDTH    2         /* window border width */
#define FOCUS           "#ff950e" /* focused window border color   */
#define UNFOCUS         "#444444" /* unfocused window border color */
#define INFOCUS         "#9c3885" /* focused window border color on unfocused monitor */
#define MINWSZ          50        /* minimum window size in pixels */
#define DEFAULT_MONITOR 0         /* the monitor to focus initially */
#define DEFAULT_DESKTOP 1         /* the desktop to focus initially */
#define DESKTOPS        4         /* number of desktops - edit DESKTOPCHANGE keys to suit */

struct ml {
    int m; /* monitor that the desktop in on  */
    int d; /* desktop which properties follow */
    struct {
        int mode;  /* layout mode for desktop d of monitor m    */
        int masz;  /* incread or decrease master area in px     */
        Bool sbar; /* whether or not to show panel on desktop d */
    } dl;
};

/**
 * define initial values for each monitor and dekstop properties
 *
 * in the example below:
 * - the first desktop (0) on the first monitor (0) will have
 *   tile layout, with its master area increased by 50px and
 *   the panel will be visible.
 * - the third desktop (2) on the second monitor (1) will have
 *   grid layout, with no changes to its master area and
 *   the panel will be hidden.
 */
static const struct ml init[] = { \
    /* monitor  desktop   mode  masz  sbar   */
    {     0,       0,   { TILE, -180, True  } },
    {     0,       1,   { TILE,  300, True  } },
    {     0,       2,   { TILE, -150, True  } },
    {     1,       0,   { TILE,    0, False } },
};

/**
 * open applications to specified monitor and desktop
 * with the specified properties.
 * if monitor is negative, then current is assumed
 * if desktop is negative, then current is assumed
 */
static const AppRule rules[] = { \
    /*  class     monitor  desktop  follow  float */
    { "mpv",         0,      -1,    False,  True  },
    { "sxiv",        0,      -1,    False,  True  },
    { "Navigator",   0,       1,    False,  False },
    { "zathura",     0,       2,    False,  False },
    { "rtorrent",    0,       3,    False,  False },
    { "ncmpcpp",     0,       0,    False,  False },
    { "htop",        0,       0,    False,  False },
    { "vifm",        0,       0,    False,  False },
};

/**
 * custom commands
 * must always end with ', NULL };'
 */
#define MENUFONT "-*-dejavu sans-medium-r-normal--10-*-*-*-*-*-iso8859-1"
static const char *termcmd[] = { "urxvtc", NULL };
static const char *menucmd[] = { "dmenu_run", "-i", "-fn", MENUFONT, NULL };
static const char *filecmd[] = { "/home/elias/bin/flaunch", "file", "-i", "-fn", MENUFONT, NULL };
static const char *dircmd[]  = { "/home/elias/bin/flaunch", "dir", "-i", "-fn", MENUFONT, NULL };
static const char *warecmd[] = { "urxvtc", "-name", "rtorrent", "-e", "rtorrent", NULL };
#undef MENUFONT

/* audio volume */
static const char *voltogglecmd[]  = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *voldowncmd[]    = { "amixer", "-q", "sset", "Master", "4-", "unmute", NULL };
static const char *volupcmd[]      = { "amixer", "-q", "sset", "Master", "4+", "unmute", NULL };

static const char *cmustogglecmd[] = { "mpc", "toggle", "-q", NULL };
static const char *cmusprevcmd[]   = { "mpc", "prev", "-q", NULL };
static const char *cmusnextcmd[]   = { "mpc", "next", "-q", NULL };

#define MONITORCHANGE(K,N) \
    {  MOD1,             K,              change_monitor, {.i = N}}, \
    {  MOD1|ShiftMask,   K,              client_to_monitor, {.i = N}},

#define DESKTOPCHANGE(K,N) \
    {  MOD4,             K,              change_desktop, {.i = N}}, \
    {  MOD4|ShiftMask,   K,              client_to_desktop, {.i = N}},

/**
 * keyboard shortcuts
 */
static Key keys[] = {
    /* modifier          key            function           argument */
    {  MOD4,             XK_j,          next_win,          {NULL}},
    {  MOD4,             XK_k,          prev_win,          {NULL}},
    {  MOD4|SHIFT,       XK_j,          move_down,         {NULL}},
    {  MOD4|SHIFT,       XK_k,          move_up,           {NULL}},
    {  MOD4|SHIFT,       XK_c,          killclient,        {NULL}},
    {  MOD4,             XK_h,          resize_master,     {.i = -10}},
    {  MOD4,             XK_l,          resize_master,     {.i = +10}},
    {  MOD4,             XK_o,          resize_stack,      {.i = -20}},
    {  MOD4,             XK_p,          resize_stack,      {.i = +20}},
    {  0,                0x1008ff11,    spawn,             {.com = voldowncmd}},
    {  0,                0x1008ff13,    spawn,             {.com = volupcmd}},
    {  0,                0x1008ff12,    spawn,             {.com = voltogglecmd}},
    {  0,                0x1008ff14,    spawn,             {.com = cmustogglecmd}},
    {  0,                0x1008ff16,    spawn,             {.com = cmusprevcmd}},
    {  0,                0x1008ff17,    spawn,             {.com = cmusnextcmd}},
    {  MOD4|SHIFT,       XK_Return,     spawn,             {.com = termcmd}},
    {  MOD4,             XK_v,          spawn,             {.com = menucmd}},
    {  MOD4,             XK_f,          spawn,             {.com = filecmd}},
    {  MOD4,             XK_d,          spawn,             {.com = dircmd}},
    {  MOD4|SHIFT,       XK_r,          spawn,             {.com = warecmd}},
    {  MOD4|SHIFT,       XK_t,          switch_mode,       {.i = TILE}},
    {  MOD4|SHIFT,       XK_m,          switch_mode,       {.i = MONOCLE}},
    {  MOD4|SHIFT,       XK_b,          switch_mode,       {.i = BSTACK}},
    {  MOD4|SHIFT,       XK_g,          switch_mode,       {.i = GRID}},
    {  MOD4|SHIFT,       XK_f,          switch_mode,       {.i = FLOAT}},
    {  MOD4,             XK_Return,     swap_master,       {NULL}},
    {  MOD4,             XK_b,          togglepanel,       {NULL}},
    {  MOD4,             XK_Tab,        last_desktop,      {NULL}},
    {  MOD4|CONTROL,     XK_q,          quit,              {NULL}},
    {  MOD1,             XK_j,          moveresize,        {.v = (int []){   0,  25,   0,   0 }}}, /* move down  */
    {  MOD1,             XK_k,          moveresize,        {.v = (int []){   0, -25,   0,   0 }}}, /* move up    */
    {  MOD1,             XK_l,          moveresize,        {.v = (int []){  25,   0,   0,   0 }}}, /* move right */
    {  MOD1,             XK_h,          moveresize,        {.v = (int []){ -25,   0,   0,   0 }}}, /* move left  */
    {  MOD1|SHIFT,       XK_j,          moveresize,        {.v = (int []){   0,   0,   0,  25 }}}, /* height grow   */
    {  MOD1|SHIFT,       XK_k,          moveresize,        {.v = (int []){   0,   0,   0, -25 }}}, /* height shrink */
    {  MOD1|SHIFT,       XK_l,          moveresize,        {.v = (int []){   0,   0,  25,   0 }}}, /* width grow    */
    {  MOD1|SHIFT,       XK_h,          moveresize,        {.v = (int []){   0,   0, -25,   0 }}}, /* width shrink  */
       DESKTOPCHANGE(    XK_1,                              0)
       DESKTOPCHANGE(    XK_2,                              1)
       DESKTOPCHANGE(    XK_3,                              2)
       DESKTOPCHANGE(    XK_4,                              3)
       MONITORCHANGE(    XK_1,                              0)
       MONITORCHANGE(    XK_2,                              1)
};

/**
 * mouse shortcuts
 */
static Button buttons[] = {
    {  MOD4,    Button1,     mousemotion,   {.i = MOVE}},
    {  MOD4,    Button3,     mousemotion,   {.i = RESIZE}},
};
#endif

/* vim: set expandtab ts=4 sts=4 sw=4 : */
