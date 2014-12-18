/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "util.h"

enum {
	ABSOLUTE = 0,
	RELATIVE = 1
};

static xcb_connection_t *conn;
static xcb_screen_t     *scr;

static void usage (char *);
static void warp_cursor (int, int, int);

static void
usage (char *name)
{
	fprintf(stderr, "For more details see %s(1)\n", name);
	exit(1);
}

static void
warp_cursor (int x, int y, int mode)
{
	xcb_warp_pointer(conn, XCB_NONE, mode ? XCB_NONE : scr->root,
			0, 0, 0, 0, x, y);
}

int
main (int argc, char **argv)
{
	int mode = ABSOLUTE;

	if (argc != 4)
		usage(argv[0]);

	init_xcb(&conn);
	get_screen(conn, &scr);

	switch (argv[1][0]) {
		case 'a': mode = ABSOLUTE;
			  break;
		case 'r': mode = RELATIVE;
			  break;
	}

	warp_cursor(atoi(argv[2]), atoi(argv[3]), mode);
	
	xcb_flush(conn);

	kill_xcb(&conn);
	return 0;
}
