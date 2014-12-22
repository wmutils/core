/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

#include "arg.h"
#include "util.h"

char *argv0;
static xcb_connection_t *conn;

static void usage(void);
static void setoverride(xcb_window_t, int);

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-sr] <wid> [wid..]\n", argv0);
	exit(1);
}

static void
setoverride(xcb_window_t w, int or)
{
	uint32_t mask = XCB_CW_OVERRIDE_REDIRECT;
	uint32_t val[] = { or };

	xcb_change_window_attributes(conn, w, mask, val);
}

int
main(int argc, char **argv)
{
	int setflag = 0;

	ARGBEGIN {
		case 's': setflag = 1; break;
		case 'r': setflag = 0; break;
		default: usage();
	} ARGEND;

	init_xcb(&conn);

	while (*argv)
		setoverride(strtoul(*argv++, NULL, 16), setflag);

	xcb_flush(conn);

	kill_xcb(&conn);

	return 0;
}
