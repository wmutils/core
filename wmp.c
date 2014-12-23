/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "arg.h"
#include "util.h"

enum {
	ABSOLUTE = 0,
	RELATIVE = 1
};

static xcb_connection_t *conn;
static xcb_screen_t *scr;

static void usage(char *);
static void warp_cursor(int, int, int);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s -ar <x> <y>\n", name);
	exit(1);
}

static void
warp_cursor(int x, int y, int mode)
{
	xcb_warp_pointer(conn, XCB_NONE, mode ? XCB_NONE : scr->root,
			0, 0, 0, 0, x, y);
}

int
main(int argc, char **argv)
{
	char *argv0;
	int mode = ABSOLUTE;

	ARGBEGIN {
		case 'a': mode = ABSOLUTE;
			  break;
		case 'r': mode = RELATIVE;
			  break;
		default : usage(argv0);
	} ARGEND;

	if (argc != 2)
		usage(argv0);

	init_xcb(&conn);
	get_screen(conn, &scr);

	warp_cursor(atoi(argv[0]), atoi(argv[1]), mode);
	
	xcb_flush(conn);

	kill_xcb(&conn);
	return 0;
}
