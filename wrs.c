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

#include "util.h"

static xcb_connection_t *conn;
static xcb_screen_t *scr;

static void resize(xcb_window_t, int, int);

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

	if ((r->x + r->width + 2*r->border_width + x) > scr->width_in_pixels)
		x = scr->width_in_pixels - (
				r->x + r->width + (2*r->border_width));

	if ((r->y + r->height + 2*r->border_width + y) > scr->height_in_pixels)
		y = scr->height_in_pixels - (
				r->y + r->height + (2*r->border_width));

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
	int x, y;
	if (argc < 4)
		errx(1, "usage: %s <x> <y> <wid> [wid..]", argv[0]);

	init_xcb(&conn);
	get_screen(conn, &scr);

	x = atoi(*(++argv));
	y = atoi(*(++argv));

	while (*argv)
		resize(strtoul(*argv++, NULL, 16), x, y);

	xcb_flush(conn);

	kill_xcb(&conn);

	return 0;
}
