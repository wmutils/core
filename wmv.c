/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#include "util.h"

static xcb_connection_t *conn;
static xcb_screen_t *scr;

static void usage(char *);
static void move(xcb_window_t, int, int);
static void center_pointer(xcb_window_t);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <x> <y> <win>", name);
	exit(1);
}

static void
center_pointer(xcb_window_t win)
{
	uint32_t values[1];
	xcb_get_geometry_reply_t *geom;
	geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), NULL);

	if (!geom)
		errx(1, "center_pointer: missing geometry!");

	xcb_warp_pointer(conn, XCB_NONE, win, 0, 0, 0, 0,
			(geom->width  + (geom->border_width * 2)) / 2,
			(geom->height + (geom->border_width * 2)) / 2);

	values[0] = XCB_STACK_MODE_ABOVE;
	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, values);

	free(geom);
}

static void
move(xcb_window_t win, int x, int y)
{
	uint32_t values[2];
	int real;
	xcb_get_geometry_reply_t *geom;

	if (!win || win == scr->root)
		return;

	geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), NULL);
	if (!geom)
		return;

	values[0] = x ? geom->x + x : geom->x;
	values[1] = y ? geom->y + y : geom->y;

	if (x)
	{
		real = geom->width + (geom->border_width * 2);
		if (geom->x + x < 1)
			values[0] = 0;
		if (geom->x + x > scr->width_in_pixels - real)
			values[0] = scr->width_in_pixels - real;
	}

	if (y)
	{
		real = geom->height + (geom->border_width * 2);
		if (geom->y + y < 1)
			values[1] = 0;
		if (geom->y + y > scr->height_in_pixels - real)
			values[1] = scr->height_in_pixels - real;
	}

	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_X
			| XCB_CONFIG_WINDOW_Y, values);

	center_pointer(win);
	free(geom);
}

int
main(int argc, char **argv)
{
	xcb_window_t win;

	if (argc != 4)
		usage(argv[0]);

	init_xcb(&conn);

	scr = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
	win = scr->root;

	win = strtoul(argv[3], NULL, 16);
	if (!win)
		errx(1, "invalid win");

	move(win, atoi(argv[1]), atoi(argv[2]));
	xcb_flush(conn);

	kill_xcb(&conn);

	return 0;
}
