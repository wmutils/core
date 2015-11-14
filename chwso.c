/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <wm.h>

#include "arg.h"

xcb_connection_t *conn;
xcb_screen_t     *scrn;

static void usage(char *);

static void
usage(char *name)
{
	fprintf(stderr, "usage %s -rli <wid>\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	xcb_window_t win;
	uint32_t mode = 0;
	char *argv0 = NULL;
	
	if (argc != 3)
		usage(argv[0]);

	wm_init_xcb(&conn);

	win = strtoul(argv[2], NULL, 16);
	if (!win)
		return 1;

	ARGBEGIN {
	case 'r':
		mode = XCB_STACK_MODE_ABOVE;
		break;
	case 'l':
		mode = XCB_STACK_MODE_BELOW;
		break;
	case 'i':
		mode = XCB_STACK_MODE_OPPOSITE;
		break;
	default:
		usage(argv0);
		break;
	} ARGEND

	wm_restack(win, mode);

	wm_kill_xcb(&conn);
	return 0;
}
