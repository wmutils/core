/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <wm.h>


xcb_connection_t *conn;
xcb_screen_t     *scrn;

static void usage(char *name);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <x> <y> <w> <h> <wid>\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	xcb_window_t win;

	if (argc != 6)
		usage(argv[0]);

	wm_init_xcb(&conn);

	win = strtoul(argv[5], NULL, 16);
	if (!win)
		errx(1, "cannot get window");

	wm_teleport(win, atoi(argv[1]), atoi(argv[2]),
	              atoi(argv[3]), atoi(argv[4]));

	wm_kill_xcb(&conn);
	return 0;
}
