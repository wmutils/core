/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>

static xcb_connection_t *conn;

static void xcbinit(void);
static void cleanup(void);
static xcb_window_t focuswindow(void);

static void
xcbinit(void)
{
	conn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(conn))
		errx(1, "xcb_connect");
}

static void
cleanup(void)
{
	if (conn != NULL)
		xcb_disconnect(conn);
}

static xcb_window_t
focuswindow(void)
{
	xcb_window_t w = 0;
	xcb_get_input_focus_cookie_t c;
	xcb_get_input_focus_reply_t *r;

	c = xcb_get_input_focus(conn);
	r = xcb_get_input_focus_reply(conn, c, NULL);
	if (r == NULL)
		errx(1, "xcb_get_input_focus");

	w = r->focus;
	free(r);
	return w;
}

int
main(int argc, char **argv)
{
	atexit(cleanup);
	xcbinit();

	printf("0x%08x\n", focuswindow());
	return 0;
}
