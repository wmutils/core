/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>

#include "wmlib.h"

xcb_connection_t *conn;
xcb_screen_t     *scrn;

static xcb_window_t focus_window(void);

static xcb_window_t
focus_window(void)
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
	init_xcb(&conn);

	printf("0x%08x\n", focus_window());

	kill_xcb(&conn);
	return 0;
}
