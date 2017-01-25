/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "arg.h"
#include "util.h"

enum {
	ABSOLUTE = 0,
	RELATIVE = 1
};

static xcb_screen_t *scr;
static xcb_connection_t *conn;

static void usage(char *);
static void spot_cursor(int, uint32_t);
static void warp_cursor(int, int, int);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s [-ar <x> <y>] [wid]\n", name);
	exit(1);
}

static void
spot_cursor(int mode, uint32_t win)
{
	xcb_query_pointer_reply_t *r;
	xcb_query_pointer_cookie_t c;

	c = xcb_query_pointer(conn, win);
	r = xcb_query_pointer_reply(conn, c, NULL);

	if (r == NULL)
		errx(1, "cannot retrieve pointer position");

	if (r->child != XCB_NONE) {
		printf("%d %d\n", r->win_x, r->win_y);
	} else {
		printf("%d %d\n", r->root_x, r->root_y);
	}
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
	uint32_t win;

	ARGBEGIN {
		case 'a': mode = ABSOLUTE;
			  break;
		case 'r': mode = RELATIVE;
			  break;
		default : usage(argv0);
	} ARGEND;

	init_xcb(&conn);
	get_screen(conn, &scr);

	switch (argc) {
		case 0:
		case 1:
			win = argc > 0 ? strtoul(*argv, NULL, 16) : scr->root;
			spot_cursor(mode, win);
			break;
		case 2:
			warp_cursor(atoi(argv[0]), atoi(argv[1]), mode);
			break;
		default:
			usage(argv0);
	}
	
	xcb_aux_sync(conn);

	kill_xcb(&conn);
	return 0;
}
