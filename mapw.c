/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>
#include <wm.h>

#include "arg.h"

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
	case 'm':
		mapflag = MAP;
		break;
	case 'u':
		mapflag = UNMAP;
		break;
	case 't':
		mapflag = TOGGLE;
		break;
	default:
		usage(argv0);
		/* NOTREACHED */
	} ARGEND

	if (argc < 1 || mapflag == 0)
		usage(argv0);

	init_xcb(&conn);

	while (*argv) {
		w = strtoul(*argv++, NULL, 16);
		remap(w, mapflag);
	}

	kill_xcb(&conn);

	return 0;
}
