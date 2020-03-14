/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/*
$ v /usr/include/X11/keysymdef.h /usr/include/X11/XF86keysym.h
*/

/* appearance */
static const char font[]            = "Inconsolata 12";
static const char normbordercolor[] = "#444444";
/*static const char normbgcolor[]     = "#222222";*/
static const char normbgcolor[]     = "#000000";
/*static const char normfgcolor[]     = "#bbbbbb";*/
static const char normfgcolor[]     = "#777777";
static const char selbordercolor[]  = "#005577";
/*static const char selbgcolor[]      = "#005577";*/
static const char selbgcolor[]      = "#000000";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */
static const Bool statusmarkup      = True;

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            True,        -1 },
	/* Disabled so that it doesn't go to tag 1<<8 */
	/*{ "Firefox",  NULL,       NULL,       1 << 8,       False,       -1 },*/
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
/*
static char dmenumon[2] = "0"; * component of dmenucmd, manipulated in spawn() *
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
*/
static const char *termcmd_tmux[] = { "urxvt", "-e", "tmux", NULL };
static const char *termcmd[] = { "urxvt", NULL };
static const char *browsercmd[] = { "firefox", NULL };
static const char *browsercmd_incognito[] = { "firefox", "--private-window", NULL };

static const char *slockcmd[] = { "slock", NULL };
static const char *logoutcmd[] = { "dmenu-file", "~/Code/dotfiles/menus/logoff.txt", NULL };
static const char *appscmd[] = { "dmenu-file", "~/Code/dotfiles/menus/apps.txt", NULL };
static const char *passcmd[] = { "dmenu-pass", NULL };
static const char *bmuxcmd[] = { "dmenu-bmux", NULL };

/* media commands */
static const char *vol_mute[] = { "dm-volume", "mute", NULL };
static const char *vol_down[] = { "dm-volume", "down", NULL };
static const char *vol_up[] = { "dm-volume", "up", NULL };
static const char *vol_down_small[] = { "dm-volume", "mod", "-1", NULL };
static const char *vol_up_small[] = { "dm-volume", "mod", "+1", NULL };
static const char *vol_mic[] = { "dm-volume", "mic", NULL };

static const char *bright_down[] = { "dm-backlight", "down", NULL };
static const char *bright_up[] = { "dm-backlight", "up", NULL };
static const char *bright_down_small[] = { "dm-backlight", "down", "minor", NULL };
static const char *bright_up_small[] = { "dm-backlight", "up", "minor", NULL };
static const char *toggle_multihead[] = { "dm-multihead", NULL };

static const char *cmus_prev[] = { "cmus-remote", "--prev", NULL };
static const char *cmus_next[] = { "cmus-remote", "--next", NULL };
static const char *cmus_pause[] = { "cmus-remote", "--pause", NULL };

/* Function to shift the current view to the left/right.
 * http://lists.suckless.org/dev/att-7590/shiftview.c
 */
void
shiftview(const Arg *arg) {
	Arg shifted;
	if(arg->i > 0) { // left circular shift
		shifted.ui = (selmon->tagset[selmon->seltags] << arg->i)
		   | (selmon->tagset[selmon->seltags] >> (LENGTH(tags) - arg->i));
	} else {
		shifted.ui = selmon->tagset[selmon->seltags] >> (- arg->i)
		   | selmon->tagset[selmon->seltags] << (LENGTH(tags) + arg->i);
	}
	view(&shifted);
}

static Key keys[] = {
	/* modifier                     key            function        argument */
	{ MODKEY,                       XK_p,          spawn,          {.v = appscmd } },
	{ MODKEY|ShiftMask,             XK_p,          spawn,          {.v = passcmd } },
	{ MODKEY,                       XK_Escape,     spawn,          {.v = slockcmd} },
	{ MODKEY|ShiftMask,             XK_Escape,     spawn,          {.v = logoutcmd} },
	{ MODKEY,                       XK_semicolon,  spawn,          {.v = termcmd_tmux } },
	{ MODKEY|ShiftMask,             XK_semicolon,  spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_apostrophe, spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_apostrophe, spawn,          {.v = browsercmd_incognito } },
	{ MODKEY,                       XK_b,          togglebar,      {0} },
	{ MODKEY,                       XK_j,          focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,          incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,     zoom,           {0} },
	{ MODKEY,                       XK_Tab,        view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,          killclient,     {0} },
	{ MODKEY,                       XK_t,          setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,          spawn,          {.v = bmuxcmd} },
	{ MODKEY,                       XK_f,          setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,          setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,      togglefloating, {0} },
	/* Show all tags at once. Annoying to hit accedentally when aiming for meta-9. */
	/*{ MODKEY,                       XK_0,          view,           {.ui = ~0 } },*/
	/*{ MODKEY|ShiftMask,             XK_0,          tag,            {.ui = ~0 } },*/
	{ MODKEY,                       XK_comma,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                          0)
	TAGKEYS(                        XK_2,                          1)
	TAGKEYS(                        XK_3,                          2)
	TAGKEYS(                        XK_4,                          3)
	TAGKEYS(                        XK_5,                          4)
	TAGKEYS(                        XK_6,                          5)
	TAGKEYS(                        XK_7,                          6)
	TAGKEYS(                        XK_8,                          7)
	TAGKEYS(                        XK_9,                          8)
	{ MODKEY,                       XK_Left,       shiftview,      {.i = -1} },
	{ MODKEY,                       XK_Right,      shiftview,      {.i = +1} },
	{ MODKEY,                       XK_Up,         shiftview,      {.i = -3} },
	{ MODKEY,                       XK_Down,       shiftview,      {.i = +3} },
	{ MODKEY|ShiftMask,             XK_q,          quit,           {0} },

	{ 0,                            XK_F1,                    spawn, {.v = vol_mute } },
	{ 0,                            XK_F2,                    spawn, {.v = vol_down } },
	{ 0,                            XK_F3,                    spawn, {.v = vol_up } },
	{ ShiftMask,                    XK_F2,                    spawn, {.v = vol_down_small } },
	{ ShiftMask,                    XK_F3,                    spawn, {.v = vol_up_small } },

	{ 0,                            XF86XK_AudioMute,         spawn, {.v = vol_mute } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn, {.v = vol_down } },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn, {.v = vol_up } },
	{ ShiftMask,                    XF86XK_AudioLowerVolume,  spawn, {.v = vol_down_small } },
	{ ShiftMask,                    XF86XK_AudioRaiseVolume,  spawn, {.v = vol_up_small } },
	{ 0,                            XF86XK_AudioMicMute,      spawn, {.v = vol_mic } },
	{ 0,                            XF86XK_MonBrightnessDown, spawn, {.v = bright_down } },
	{ 0,                            XF86XK_MonBrightnessUp,   spawn, {.v = bright_up } },
	{ ShiftMask,                    XF86XK_MonBrightnessDown, spawn, {.v = bright_down_small } },
	{ ShiftMask,                    XF86XK_MonBrightnessUp,   spawn, {.v = bright_up_small } },
	{ 0,                            XF86XK_Explorer,          spawn, {.v = toggle_multihead } }, /* Lenovo T430/T440 */
	{ 0,                            XF86XK_Display,           spawn, {.v = toggle_multihead } }, /* Lenovo X1 Carbon 6th gen */

	{ MODKEY,                       XK_Home,                  spawn, {.v = cmus_prev } },
	{ MODKEY,                       XK_End,                   spawn, {.v = cmus_next } },
	{ MODKEY,                       XK_Delete,                spawn, {.v = cmus_pause } },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

