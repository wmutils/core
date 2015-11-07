/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "arg.h"
#include "wmlib.h"

xcb_connection_t *conn;
xcb_screen_t     *scrn;

static void usage(char *name);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <-sc ...> <wid> [wid...]\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	char *argv0;
	int color,width;

	color = width = -1;

	if (argc < 2)
		usage(argv[0]);

	ARGBEGIN {
	case 's':
		width = strtoul(EARGF(usage(argv0)), NULL, 10);
		break;
	case 'c':
		color = strtoul(EARGF(usage(argv0)), NULL, 16);
		break;
	default:
		usage(argv0);
		/* NOTREACHED */
	} ARGEND

	init_xcb(&conn);

	/* assume remaining arguments are windows */
	while (*argv)
		set_border(width, color, strtoul(*argv++, NULL, 16));

	xcb_flush(conn);

	kill_xcb(&conn);

	return 0;
}
