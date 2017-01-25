/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
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
	uint32_t values[3];
	uint16_t curr_width;
	int mask;
	xcb_get_geometry_reply_t *geom;

	if (width != -1) {
		geom = xcb_get_geometry_reply(conn, xcb_get_geometry(conn, win), NULL);
		if (!geom)
			return;

		/* Windows track position based on the top left corner of the border.
		 * To make the border move instead of the window, we move the window up and left
		 * by the amount the border would have shifted it down and right.*/
		curr_width = geom->border_width;
		values[0] = geom->x + curr_width - width;
		values[1] = geom->y + curr_width - width;
		values[2] = width;

		mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_BORDER_WIDTH ;
		xcb_configure_window(conn, win, mask, values);

		xcb_aux_sync(conn);

		free(geom);
	}

	if (color != -1) {
		values[0] = color;
		mask = XCB_CW_BORDER_PIXEL;
		xcb_change_window_attributes(conn, win, mask, values);

		xcb_aux_sync(conn);
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

	xcb_aux_sync(conn);

	kill_xcb(&conn);

	return 0;
}
