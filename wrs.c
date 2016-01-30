/* See LICENSE file for copyright and license details. */

#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <wm.h>


xcb_connection_t *conn;
xcb_screen_t     *scrn;

static void usage(char *);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s [-a] <x> <y> <wid> [wid..]\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	int x, y, mode = RELATIVE;
	if (argc < 4)
		usage(argv[0]);

	wm_init_xcb(&conn);
	wm_get_screen(conn, &scrn);

	if (argv[1][0] == '-' && argv[1][1] == 'a') {
		mode = ABSOLUTE;
		argv++;
	}

	x = atoi(*(++argv));
	y = atoi(*(++argv));

	while (*argv)
		wm_resize(strtoul(*argv++, NULL, 16), mode, x, y);

	xcb_flush(conn);

	wm_kill_xcb(&conn);

	return 0;
}
