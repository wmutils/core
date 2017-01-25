/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "arg.h"
#include "util.h"

static xcb_connection_t *conn;

static void usage(char *);
static void stack(xcb_window_t, uint32_t[1]);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s -rli <wid>\n", name);
	exit(1);
}

static void
stack(xcb_window_t win, uint32_t values[1])
{
	xcb_configure_window(conn, win, XCB_CONFIG_WINDOW_STACK_MODE, values);
}

int
main(int argc, char **argv)
{
	xcb_window_t win;
	uint32_t values[1];
	char *argv0 = NULL;

	if (argc != 3)
		usage(argv[0]);

	init_xcb(&conn);

	win = strtoul(argv[2], NULL, 16);
	if (!win)
		return 1;

	ARGBEGIN {
		case 'r':
			values[0] = XCB_STACK_MODE_ABOVE;
			break;
		case 'l':
			values[0] = XCB_STACK_MODE_BELOW;
			break;
		case 'i':
			values[0] = XCB_STACK_MODE_OPPOSITE;
			break;
		default:
			usage(argv0);
			break;
	} ARGEND

	stack(win, values);
	xcb_aux_sync(conn);

	kill_xcb(&conn);

	return 0;
}
