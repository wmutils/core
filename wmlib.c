#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wmlib.h"

	int
init_xcb()
{
	conn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(conn))
		return 0;
	return 1;
}

	int
kill_xcb()
{
	if (conn) {
		xcb_disconnect(conn);
		return 1;
	}
	return 0;
}

	int
is_alive(xcb_window_t w)
{
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(conn, w);
	r = xcb_get_window_attributes_reply(conn, c, NULL);

	if (r == NULL)
		return 0;

	free(r);
	return 1;
}

	int
is_mapped(xcb_window_t w)
{
	int ms;
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(conn, w);
	r = xcb_get_window_attributes_reply(conn, c, NULL);

	if (r == NULL)
		return 0;

	ms = r->map_state;

	free(r);
	return ms == XCB_MAP_STATE_VIEWABLE;
}

	int
is_ignored(xcb_window_t wid)
{
	int or;
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(conn, wid);
	r = xcb_get_window_attributes_reply(conn, c, NULL);

	if (r == NULL)
		return 0;

	or = r->override_redirect;

	free(r);
	return or;
}

	int
get_screen()
{
	scrn = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	if (scrn == NULL)
		return 0;
	return 1;
}

	int
get_windows(xcb_window_t w, xcb_window_t **l)
{
	uint32_t childnum = 0;
	xcb_query_tree_cookie_t c;
	xcb_query_tree_reply_t *r;

	c = xcb_query_tree(conn, w);
	r = xcb_query_tree_reply(conn, c, NULL);
	if (r == NULL)
		return -1;

	*l = malloc(sizeof(xcb_window_t) * r->children_len);
	memcpy(*l, xcb_query_tree_children(r),
			sizeof(xcb_window_t) * r->children_len);

	childnum = r->children_len;

	free(r);
	return childnum;
}

	int
get_attribute(xcb_window_t w, int attr)
{
	xcb_get_geometry_cookie_t c;
	xcb_get_geometry_reply_t *r;

	c = xcb_get_geometry(conn, w);
	r = xcb_get_geometry_reply(conn, c, NULL);

	if (r == NULL)
		return -1;

	switch (attr) {
		case ATTR_X: attr = r->x; break;
		case ATTR_Y: attr = r->y; break;
		case ATTR_W: attr = r->width; break;
		case ATTR_H: attr = r->height; break;
		case ATTR_B: attr = r->border_width; break;
	}

	free(r);
	return attr;
}

	int
get_cursor(int mode, uint32_t wid, int *x, int *y)
{
	xcb_query_pointer_reply_t *r;
	xcb_query_pointer_cookie_t c;

	c = xcb_query_pointer(conn, wid);
	r = xcb_query_pointer_reply(conn, c, NULL);

	if (r == NULL)
		return 0;

	if (r->child != XCB_NONE) {
		*x = r->win_x;
		*y = r->win_y;
	} else {
		*x = r->root_x;
		*y = r->root_y;
	}

	return 1;
}

	int
set_border(int width, int color, xcb_window_t win)
{
	uint32_t values[1];
	int mask, retval = 0;
	/* change width if > 0 */
	if (width > -1) {
		values[0] = width;
		mask = XCB_CONFIG_WINDOW_BORDER_WIDTH;
		xcb_configure_window(conn, win, mask, values);
		retval++;
	}

	/* change color if > 0 */
	if (color > -1) {
		values[0] = color;
		mask = XCB_CW_BORDER_PIXEL;
		xcb_change_window_attributes(conn, win, mask, values);
		retval++;
	}

	xcb_flush(conn);
	return retval;
}

	int
set_cursor(int x, int y, int mode)
{
	xcb_warp_pointer(conn, XCB_NONE, mode ? XCB_NONE : scrn->root,
			0, 0, 0, 0, x, y);
	return 1;
}

	int
is_listable(xcb_window_t w, int mask)
{
	if ((mask & LIST_ALL)
		|| (!is_mapped (w) && mask & LIST_HIDDEN)
		|| ( is_ignored(w) && mask & LIST_IGNORE)
		|| ( is_mapped (w) && !is_ignored(w) && mask == 0))
		return 1;

	return 0;
}

	int
teleport(xcb_window_t wid, int x, int y, int w, int h)
{
	uint32_t values[4];
	uint32_t mask =   XCB_CONFIG_WINDOW_X
	                | XCB_CONFIG_WINDOW_Y
	                | XCB_CONFIG_WINDOW_WIDTH
	                | XCB_CONFIG_WINDOW_HEIGHT;
	values[0] = x;
	values[1] = y;
	values[2] = w;
	values[3] = h;
	xcb_configure_window(conn, wid, mask, values);

	xcb_flush(conn);
	return 1;
}

	int
move(xcb_window_t wid, int mode, int x, int y)
{
	int curx, cury, curw, curh, curb;

	if (!is_mapped(wid) || wid == scrn->root)
		return -1;
	
	curb = get_attribute(wid, ATTR_B);
	curx = get_attribute(wid, ATTR_X);
	cury = get_attribute(wid, ATTR_Y);
	curw = get_attribute(wid, ATTR_W);
	curh = get_attribute(wid, ATTR_H);

	if (mode == ABSOLUTE) {
		x -= curx + curw /2;
		y -= cury + curh /2;
	} else {
		x += curx;
		y += cury;
	}

	/* the following prevent windows from moving off the screen */
	if (x < 0)
		x = 0;
	else if (x > scrn->width_in_pixels - curw - 2*curb)
		x = scrn->width_in_pixels - curw - 2*curb;

	if (y < 0)
		y = 0;
	else if (y > scrn->height_in_pixels - curh - 2*curb)
		y = scrn->height_in_pixels - curh - 2*curb;

	teleport(wid, x, y, curw, curh);
	return 1;
}

	int
resize(xcb_window_t wid, int mode, int w, int h)
{
	int curx, cury, curw, curh, curb;

	if (!is_mapped(wid) || wid == scrn->root)
		return -1;
	
	curb = get_attribute(wid, ATTR_B);
	curx = get_attribute(wid, ATTR_X);
	cury = get_attribute(wid, ATTR_Y);
	curw = get_attribute(wid, ATTR_W);
	curh = get_attribute(wid, ATTR_H);

	if (mode == ABSOLUTE) {
		w -= curx + 2*curb;
		h -= cury + 2*curb;
	} else {
		w += curw;
		h += curh;
	}

	/*
	 * The following prevent windows from growing out of the screen, or
	 * having a negative size
	 */
	if (w < 0)
		w = curw;
	if (curx + w >  scrn->width_in_pixels)
		w = scrn->width_in_pixels - curx - 2*curb;

	if (h < 0)
		h = cury;
	if (cury + h > scrn->height_in_pixels)
		h = scrn->height_in_pixels - cury - 2*curb;

	teleport(wid, curx, cury, w, h);
	return 1;
}

	int
restack(xcb_window_t wid, uint32_t mode)
{
	uint32_t values[1] = { mode };
	xcb_configure_window(conn, wid, XCB_CONFIG_WINDOW_STACK_MODE, values);
	xcb_flush(conn);
	return 1;
}

	int
set_focus(xcb_window_t wid)
{
	xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, wid,
	                    XCB_CURRENT_TIME);
	xcb_flush(conn);
	return 1;
}
