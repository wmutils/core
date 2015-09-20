/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

#include "arg.h"
#include "wmlib.h"

xcb_connection_t *conn;
xcb_screen_t     *scrn;

static void usage(char *);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s [-h] [-mut <wid> [wid..]]\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	int mapflag = 0;
	xcb_window_t w = 0;
	char *argv0;

	ARGBEGIN {
		case 'm': mapflag = MAP;    break;
		case 'u': mapflag = UNMAP;  break;
		case 't': mapflag = TOGGLE; break;
		default : usage(argv0);
	} ARGEND;

	if (argc < 1 || mapflag == 0)
		usage(argv0);

	init_xcb(&conn);

	while (*argv) {
		w = strtoul(*argv++, NULL, 16);

		switch (mapflag) {
		case MAP:
			xcb_map_window(conn, w);
			break;
		case UNMAP:
			xcb_unmap_window(conn, w);
			break;
		case TOGGLE:
			if (is_mapped(conn, w))
				xcb_unmap_window(conn, w);
			else
				xcb_map_window(conn, w);
			break;
		}
	}
	xcb_flush(conn);

	kill_xcb(&conn);

	return 0;
}
