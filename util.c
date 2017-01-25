#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

#include "util.h"

void
init_xcb(xcb_connection_t **con)
{
	*con = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(*con))
		errx(1, "unable connect to the X server");
}

void
kill_xcb(xcb_connection_t **con)
{
	if (*con)
		xcb_disconnect(*con);
}

void
get_screen(xcb_connection_t *con, xcb_screen_t **scr)
{
	*scr = xcb_setup_roots_iterator(xcb_get_setup(con)).data;
	if (*scr == NULL)
		errx(1, "unable to retrieve screen informations");
}

int
exists(xcb_connection_t *con, xcb_window_t w)
{
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(con, w);
	r = xcb_get_window_attributes_reply(con, c, NULL);

	if (r == NULL)
		return 0;

	free(r);
	return 1;
}

int
mapped(xcb_connection_t *con, xcb_window_t w)
{
	int ms;
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(con, w);
	r = xcb_get_window_attributes_reply(con, c, NULL);

	if (r == NULL)
		return 0;

	ms = r->map_state;

	free(r);
	return ms == XCB_MAP_STATE_VIEWABLE;
}

int
ignore(xcb_connection_t *con, xcb_window_t w)
{
	int or;
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(con, w);
	r = xcb_get_window_attributes_reply(con, c, NULL);

	if (r == NULL)
		return 0;

	or = r->override_redirect;

	free(r);
	return or;
}

int
get_windows(xcb_connection_t *con, xcb_window_t w, xcb_window_t **l)
{
	uint32_t childnum = 0;
	xcb_query_tree_cookie_t c;
	xcb_query_tree_reply_t *r;

	c = xcb_query_tree(con, w);
	r = xcb_query_tree_reply(con, c, NULL);
	if (r == NULL)
		errx(1, "0x%08x: no such window", w);

	*l = malloc(sizeof(xcb_window_t) * r->children_len);
	memcpy(*l, xcb_query_tree_children(r),
			sizeof(xcb_window_t) * r->children_len);

	childnum = r->children_len;

	free(r);
	return childnum;
}
