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
#include <err.h>

static xcb_connection_t *conn;

static void cleanup(void);
static void resize(xcb_window_t, int, int);

static void
xcbinit(void)
{
	conn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(conn))
		errx(1, "unable to connect to the X server");
}

static void
cleanup(void)
{
	if (conn)
		xcb_disconnect(conn);
}

static void
resize(xcb_window_t w, int x, int y)
{
	uint32_t val[3];
	uint32_t mask = XCB_CONFIG_WINDOW_WIDTH
	              | XCB_CONFIG_WINDOW_HEIGHT
	              | XCB_CONFIG_WINDOW_STACK_MODE;
	xcb_get_geometry_cookie_t c;
	xcb_get_geometry_reply_t *r;

	c = xcb_get_geometry(conn, w);
	r = xcb_get_geometry_reply(conn, c, NULL);

	if (r == NULL)
		return;

	val[0] = r->width  + x;
	val[1] = r->height + y;
	val[2] = XCB_STACK_MODE_ABOVE;

	xcb_configure_window(conn, w, mask, val);

	xcb_warp_pointer(conn, XCB_NONE, w, 0, 0, 0, 0, r->width  + x,
			r->height + y);

	free(r);
}

int
main(int argc, char **argv)
{
	int i;

	if (argc < 4)
		errx(1, "usage: %s <x> <y> <wid> [wid..]", argv[0]);

	atexit(cleanup);
	xcbinit();

	for (i=3; i<argc; i++)
		resize(strtoul(argv[i], NULL, 16), atoi(argv[1]), atoi(argv[2]));

	xcb_flush(conn);

	return 0;
}
