/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "util.h"

static xcb_connection_t *conn;

static void usage(char *name);
static void teleport(xcb_window_t, int, int, int, int);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <x> <y> <w> <h> <wid>\n", name);
	exit(1);
}

static void
teleport(xcb_window_t win, int x, int y, int w, int h)
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

	xcb_configure_window(conn, win, mask, values);
}

int
main(int argc, char **argv)
{
	xcb_window_t win;

	if (argc != 6)
		usage(argv[0]);

	init_xcb(&conn);

	win = strtoul(argv[5], NULL, 16);
	if (!win)
		errx(1, "cannot get window");

	teleport(win, atoi(argv[1]), atoi(argv[2]),
			atoi(argv[3]), atoi(argv[4]));
	xcb_aux_sync(conn);

	kill_xcb(&conn);

	return 0;
}
