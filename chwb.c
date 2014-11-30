/**
*      Copyright (c) 2014, Broseph <dcat (at) iotek (dot) org>
*
*      Permission to use, copy, modify, and/or distribute this software for any
*      purpose with or without fee is hereby granted, provided that the above
*      copyright notice and this permission notice appear in all copies.
*
*      THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
*      WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
*      MERCHANTABILITY AND FITNESS IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
*      ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
*      WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
*      ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
*      OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**/

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "arg.h"

static xcb_connection_t *conn;

static void cleanup    (void);
static void usage      (char *name);
static void setborder  (int, int, xcb_window_t);

static void
cleanup (void)
{
	if (conn)
		xcb_disconnect(conn);
}

static void
usage (char *name)
{
	fprintf(stderr, "usage: %s <-sc ...> <wid> [wid...]\n", name);
	exit(1);
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
	atexit(cleanup);

	if (xcb_connection_has_error(conn = xcb_connect(NULL, NULL)))
		errx(1, "error connecting to X");

	color = bw = -1;

	if (argc < 2)
		usage(argv[0]);

	ARGBEGIN {
		case 's':
			bw = strtoul(ARGF(), NULL, 10);
			break;
		case 'c':
			color = strtoul(ARGF(), NULL, 16);
			break;
		case 'h':
			usage(argv0);
	} ARGEND

	/* assume remaining arguments are windows */
	while (*argv)
		setborder(bw, color, strtoul(*argv++, NULL, 16));


	xcb_flush(conn);

	return 0;
}
