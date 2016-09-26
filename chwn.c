/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

#include "arg.h"
#include "util.h"

static xcb_connection_t *conn;

static void usage(char *);
static void set_title(xcb_connection_t *, xcb_window_t, char *);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <name> <wid>\n", name);
	exit(1);
}

void
set_title(xcb_connection_t *con, xcb_window_t w, char *name)
{
	xcb_change_property(con, XCB_PROP_MODE_REPLACE, w, XCB_ATOM_WM_NAME,
		XCB_ATOM_STRING, 8, strlen(name), name);
	xcb_flush(con);
}

int
main(int argc, char **argv)
{
	if (argc != 3)
		usage(argv[0]);

	init_xcb(&conn);

	set_title(conn, strtoul(argv[2], NULL, 16), argv[1]);

	kill_xcb(&conn);

	return 0;
}
