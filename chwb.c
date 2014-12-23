/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "arg.h"
#include "util.h"

static xcb_connection_t *conn;

static void usage      	(const char *);
static void setborder  	(int, int, xcb_window_t);
static void helper		(const char *);


static void
usage (const char *name)
{
	fprintf(stderr, "usage: %s <-sc ...> <wid> [wid...]\n", name);
	exit(1);
}

static void
helper(const char *name)
{
	fprintf(stdout,
			"usage: %s <-sc ...> <wid> [wid...]\n"
			"change window border\n\n"
			"\t-s width\t\tset width border\n"
			"\t-c color\t\tset color border\n",
			name);
	exit(0);
}

static void
setborder (int bw, int color, xcb_window_t win)
{
	uint32_t values[1];
	int mask;

	if (bw != -1) {
		values[0] = bw;
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
main (int argc, char **argv)
{
	char *argv0;
	int color,bw;

	init_xcb(&conn);

	color = bw = -1;

	if (argc < 2)
		usage(argv[0]);

	ARGBEGIN {
		case 's':
			bw = strtoul(EARGF(usage(argv0)), NULL, 10);
			break;
		case 'c':
			color = strtoul(EARGF(usage(argv0)), NULL, 16);
			break;
		case 'h':
			helper(argv0);
	} ARGEND

	/* assume remaining arguments are windows */
	while (*argv)
		setborder(bw, color, strtoul(*argv++, NULL, 16));

	xcb_flush(conn);

	kill_xcb(&conn);

	return 0;
}
