/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

#include "util.h"

static xcb_connection_t *conn;
static xcb_screen_t *scrn;

static void usage(char *);
static xcb_window_t focus_window(void);
static xcb_window_t parent(xcb_window_t);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s [wid]\n", name);
	exit(1);
}

static xcb_window_t
focus_window(void)
{
	xcb_window_t w = 0;
	xcb_get_input_focus_cookie_t c;
	xcb_get_input_focus_reply_t *r;

	c = xcb_get_input_focus(conn);
	r = xcb_get_input_focus_reply(conn, c, NULL);
	if (r == NULL)
		errx(1, "xcb_get_input_focus");

	w = r->focus;
	free(r);

	if (w == XCB_NONE || w == XCB_INPUT_FOCUS_POINTER_ROOT)
		errx(1, "focus not set");

	return w;
}

static xcb_window_t
parent(xcb_window_t w)
{
	xcb_query_tree_cookie_t c;
	xcb_query_tree_reply_t *r;

	c = xcb_query_tree(conn, w);
	r = xcb_query_tree_reply(conn, c, NULL);
	if (r == NULL)
		errx(1, "failed to get parent");

	w = (!r->parent || r->parent == scrn->root) ? w : parent(r->parent);
	free(r);

	return w;
}

int
main(int argc, char **argv)
{
	xcb_window_t w;

	if (argc > 2 || (argc > 1 && (!strncmp(argv[1], "-h", 2))))
		usage(argv[0]);

	init_xcb(&conn);
	get_screen(conn, &scrn);

	w = (argc > 1) ? strtoul(argv[1], NULL, 16) : focus_window();

	printf("0x%08x\n", parent(w));

	kill_xcb(&conn);
	return 0;
}
