/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

#include "arg.h"
#include "util.h"

static xcb_connection_t *conn;

static void usage(char *);

static void
usage (char *name)
{
	fprintf(stderr, "usage: %s [-p] <wid> [wid...]\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	int parent = 0;
	char *argv0;

	if (argc < 2)
		usage(argv[0]);

	ARGBEGIN{
	case 'p': parent=1; break;
	default: usage(argv0);
	}ARGEND;

	init_xcb(&conn);

	/* assume remaining arguments are windows */
	while (*argv) {
		if (parent) {
			/* kills the client whose WID belongs to */
			xcb_kill_client(conn, strtoul(*argv++, NULL, 16));
		} else {
			/* destroy the given window and all its children */
			xcb_destroy_window(conn, strtoul(*argv++, NULL, 16));
		}
	}

	xcb_aux_sync(conn);
	kill_xcb(&conn);

	return 0;
}
