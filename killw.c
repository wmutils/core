#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

#include "util.h"

static xcb_connection_t *conn;

int
main(int argc, char **argv)
{
	init_xcb(&conn);

	/* assume remaining arguments are windows */
	while (*argv)
		xcb_kill_client(conn, strtoul(*argv++, NULL, 16));

	xcb_flush(conn);

	kill_xcb(&conn);

	return 0;
}
