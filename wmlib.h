#ifndef __WMLIB_H__
#define __WMLIB_H__

extern xcb_connection_t *conn;
extern xcb_screen_t     *scrn;

enum {
	LIST_HIDDEN = 1 << 0,
	LIST_IGNORE = 1 << 1,
	LIST_ALL    = 1 << 2
};

enum {
	MAP     = 1 << 0,
	UNMAP   = 1 << 1,
	TOGGLE  = 1 << 2
};

enum {
	ATTR_W = 1 << 0,
	ATTR_H = 1 << 1,
	ATTR_X = 1 << 2,
	ATTR_Y = 1 << 3,
	ATTR_B = 1 << 4,
	ATTR_M = 1 << 5,
	ATTR_I = 1 << 6,
	ATTR_MAX
};

enum {
	ABSOLUTE = 0,
	RELATIVE = 1
};

int init_xcb();
int kill_xcb();

int is_alive(xcb_window_t wid);
int is_ignored(xcb_window_t wid);
int is_listable(xcb_window_t wid, int mask);
int is_mapped(xcb_window_t wid);

int get_screen();
int get_windows(xcb_window_t wid, xcb_window_t **list);
int get_attribute(xcb_window_t, int);
int get_cursor(int mode, uint32_t wid, int *x, int *y);

int set_border(int width, int color, xcb_window_t wid);
int set_focus(xcb_window_t wid);
int set_cursor(int x, int y, int mode);

int teleport(xcb_window_t wid, int w, int h, int x, int y);
int move(xcb_window_t wid, int mode, int x, int y);
int resize(xcb_window_t wid, int mode, int w, int h);
int restack(xcb_window_t wid, uint32_t mode);

#endif
