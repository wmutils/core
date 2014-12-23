/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "util.h"

static xcb_connection_t *conn;

static void usage (char *name);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <wid>\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	xcb_window_t win;


	if (argc != 2)
		usage(argv[0]);

	init_xcb(&conn);

	win = strtoul(argv[1], NULL, 16);
	if (!win)
		errx(1, "could not get focused window");

	xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, win,
			XCB_CURRENT_TIME);

	xcb_flush(conn);

	kill_xcb(&conn);
	return 0;
}
