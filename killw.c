/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

#include "util.h"

static xcb_connection_t *conn;

static void usage(char *);

static void
usage (char *name)
{
	fprintf(stderr, "usage: %s <wid> [wid...]\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	if (argc < 2)
		usage(argv[0]);

	init_xcb(&conn);

	/* assume remaining arguments are windows */
	while (*argv)
		xcb_kill_client(conn, strtoul(*argv++, NULL, 16));

	xcb_flush(conn);

	kill_xcb(&conn);

	return 0;
}
