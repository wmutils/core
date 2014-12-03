#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

#include "arg.h"

char *argv0;
static xcb_connection_t *conn;
static xcb_screen_t *scrn;

static void usage(void);
static void xcbinit(void);
static void cleanup(void);
static int mapped(xcb_window_t);
static int ignored(xcb_window_t);
static int shouldlist(xcb_window_t, int);

enum {
	LIST_HIDDEN = 1 << 0,
	LIST_IGNORE = 1 << 1
};

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-har] [wid...]\n", argv0);
	exit(1);
}

static void
xcbinit(void)
{
	conn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(conn))
		errx(1, "unable connect to the X server");

	scrn = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

	if (scrn == NULL)
		errx(1, "unable to retrieve screen informations");
}

static void
cleanup(void)
{
	if (conn != NULL)
		xcb_disconnect(conn);
}

static int
mapped(xcb_window_t w)
{
	int ms;
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(conn, w);
	r = xcb_get_window_attributes_reply(conn, c, NULL);

	if (r == NULL)
		return 0;

	ms = r->map_state;

	free(r);
	return ms == XCB_MAP_STATE_VIEWABLE;
}

static int
ignored(xcb_window_t w)
{
	int or;
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(conn, w);
	r = xcb_get_window_attributes_reply(conn, c, NULL);

	if (r == NULL)
		return 0;

	or = r->override_redirect;

	free(r);
	return or;
}

static int
shouldlist(xcb_window_t w, int mask)
{
	if (ignored(w) && !(mask & LIST_IGNORE))
		return 0;

	if (!mapped(w) && !(mask & LIST_HIDDEN))
		 return 0;

	return 1;
}

static void
listwindows(xcb_window_t w, int listmask)
{
	int i;
	xcb_window_t *wc;
	xcb_query_tree_cookie_t c;
	xcb_query_tree_reply_t *r;

	c = xcb_query_tree(conn, w);
	r = xcb_query_tree_reply(conn, c, NULL);
	if (r == NULL)
		errx(1, "0x%08x: no such window", w);

	wc = xcb_query_tree_children(r);
	if (wc == NULL)
		errx(1, "0x%08x: unable to retrieve children", w);

	for (i=0; i<r->children_len; i++) {
		if (shouldlist(wc[i], listmask))
			printf("0x%08x\n", wc[i]);
	}

	free(r);
}

int
main(int argc, char **argv)
{
	int listmask = 0, rootflag = 0;

	ARGBEGIN {
		case 'a': listmask |= LIST_HIDDEN; break;
		case 'i': listmask |= LIST_IGNORE; break;
		case 'r': rootflag = 1; break;
		default : usage();
	} ARGEND;

	atexit(cleanup);
	xcbinit();

	if (rootflag == 1) {
		printf("0x%08x\n", scrn->root);
		return 0;
	}

	if (argc == 0)
		listwindows(scrn->root, listmask);

	while (*argv)
		listwindows(strtoul(*argv++, NULL, 16), listmask);

	return 0;
}
