/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "util.h"

static xcb_connection_t *conn;
static xcb_screen_t *scr;

static void usage(char *);
static void resize(xcb_window_t, int, int);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <x> <y> <wid> [wid..]", name);
	exit(1);
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

	free(r);
}

int
main(int argc, char **argv)
{
	int x, y;
	if (argc < 4)
		usage(argv[0]);

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
