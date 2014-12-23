/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "arg.h"
#include "util.h"

static xcb_connection_t *conn;

static void usage(char *name);
static void set_border(int, int, xcb_window_t);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <-sc ...> <wid> [wid...]\n", name);
	exit(1);
}

static void
set_border(int width, int color, xcb_window_t win)
{
	uint32_t values[1];
	int mask;

	if (width != -1) {
		values[0] = width;
		mask = XCB_CONFIG_WINDOW_BORDER_WIDTH;
		xcb_configure_window(conn, win, mask, values);

		xcb_flush(conn);
	}

	if (color != -1) {
		values[0] = color;
		mask = XCB_CW_BORDER_PIXEL;
		xcb_change_window_attributes(conn, win, mask, values);

		xcb_flush(conn);
	}
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
