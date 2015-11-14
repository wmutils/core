/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>
#include <wm.h>

#include "arg.h"

xcb_connection_t *conn;
xcb_screen_t *scrn;

static void usage(char *);
static void list_windows(xcb_window_t w, int mask);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s [-houra] [wid...]\n", name);
	exit(1);
}

static void
list_windows(xcb_window_t w, int listmask)
{
	int i, wn;
	xcb_window_t *wc;

	wn = get_windows(w, &wc);

	if (wc == NULL)
		errx(1, "0x%08x: unable to retrieve children", w);

	for (i=0; i<wn; i++) {
		if (is_listable(wc[i], listmask))
			printf("0x%08x\n", wc[i]);
	}

	free(wc);
}

int
main(int argc, char **argv)
{
	int listmask = 0, rootflag = 0;
	char *argv0;

	ARGBEGIN {
	case 'a':
		listmask |= LIST_ALL;
		break;
	case 'u':
		listmask |= LIST_HIDDEN;
		break;
	case 'o':
		listmask |= LIST_IGNORE;
		break;
	case 'r':
		rootflag = 1;
		break;
	default:
		usage(argv0);
		/* NOTREACHED */
	} ARGEND

	init_xcb(&conn);
	get_screen(conn, &scrn);

	if (rootflag == 1) {
		printf("0x%08x\n", scrn->root);
		return 0;
	}

	if (argc == 0)
		list_windows(scrn->root, listmask);

	while (*argv)
		list_windows(strtoul(*argv++, NULL, 16), listmask);

	kill_xcb(&conn);

	return 0;
}
