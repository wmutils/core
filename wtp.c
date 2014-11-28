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

static xcb_connection_t	*conn;

static void cleanup    (void);
static void usage      (char *name);
static void teleport   (xcb_window_t, int, int, int, int);
static  int get_border (xcb_window_t win);

static void usage(char *name)
{
	fprintf(stderr, "usage: %s <x> <y> <w> <h> <wid>\n", name);
	exit(1);
}

static void
cleanup (void)
{
	if (conn)
		xcb_disconnect(conn);
}

static int
get_border (xcb_window_t win)
{
	xcb_get_geometry_reply_t *geom;
	geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), NULL);

	if (!geom)
		errx(1, "failed to get geometry");

	return geom->border_width;
}

static void
teleport (xcb_window_t win, int x, int y, int w, int h)
{
	uint32_t values[2];
	int mode;
	
	mode = get_border(win);
	if (mode < 0)
		return;

	values[0] = x;
	values[1] = y;

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_X
			| XCB_CONFIG_WINDOW_Y, values);

	values[0] = mode ? w - mode - 1 : w;
	values[1] = mode ? h - mode - 1 : h;

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_WIDTH
			| XCB_CONFIG_WINDOW_HEIGHT, values);
}

int
main (int argc, char **argv)
{
	atexit(cleanup);
	xcb_window_t win;

	if (argc != 6)
		usage(argv[0]);

	if (xcb_connection_has_error(conn = xcb_connect(NULL, NULL)))
		errx(1, "error connecting to X");

	win = strtoul(argv[5], NULL, 16);
	if (!win)
		errx(1, "cannot get window");

	teleport(win, atoi(argv[1]), atoi(argv[2]),
			atoi(argv[3]), atoi(argv[4]));
	xcb_flush(conn);

	return 0;
}
