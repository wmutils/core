/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "arg.h"
#include "wmlib.h"

xcb_connection_t *conn;
xcb_screen_t     *scrn;

static void usage(char *);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s [-ar <x> <y>] [wid]\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	char *argv0;
	int mode = ABSOLUTE, x, y;
	uint32_t win;

	ARGBEGIN {
		case 'a': mode = ABSOLUTE;
			  break;
		case 'r': mode = RELATIVE;
			  break;
		default : usage(argv0);
	} ARGEND;

	init_xcb(&conn);
	get_screen(conn, &scrn);

	switch (argc) {
		case 0:
		case 1:
			win = argc > 0 ? strtoul(*argv, NULL, 16) : scrn->root;
			if (get_cursor(mode, win, &x, &y))
				printf("%d %d\n", x, y);
			break;
		case 2:
			set_cursor(atoi(argv[0]), atoi(argv[1]), mode);
			break;
		default:
			usage(argv0);
	}
	
	xcb_flush(conn);

	kill_xcb(&conn);
	return 0;
}
