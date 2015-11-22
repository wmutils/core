/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>
#include <wm.h>

#include "arg.h"

xcb_connection_t *conn;
xcb_screen_t     *scrn;

static void usage(char *);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s [-sr] <wid> [wid..]\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	int setflag = 0;
	char *argv0;

	ARGBEGIN {
	case 's': setflag = 1; break;
	case 'r': setflag = 0; break;
	default: usage(argv0);
	} ARGEND;

	wm_init_xcb(&conn);

	while (*argv)
		wm_set_override(strtoul(*argv++, NULL, 16), setflag);

	xcb_flush(conn);

	wm_kill_xcb(&conn);

	return 0;
}
