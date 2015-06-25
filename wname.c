/* See LICENSE file for copyright and license details. */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <err.h>

#include "util.h"

static xcb_connection_t *conn;

static void usage(char *);
static int get_title(xcb_window_t);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s <wid>\n", name);
	exit(1);
}

static
xcb_atom_t xcb_atom_get(xcb_connection_t *conn, char *name)
{
	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 0, strlen(name), name);
	xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);

	if (!reply)
		return XCB_ATOM_STRING;

	return reply->atom;
}

static int
get_title(xcb_window_t win)
{
	int len = 0;
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *r;

	cookie = xcb_get_property(conn, 0, win,
			xcb_atom_get(conn, "_NET_WM_NAME"), xcb_atom_get(conn, "UTF8_STRING"), 0L, 32L);
	r = xcb_get_property_reply(conn, cookie, NULL);

	if (r) {
		len = xcb_get_property_value_length(r);
		if (!len)
			return 1;

		printf("%.*s\n", len, (char *) xcb_get_property_value(r));

		free(r);
		return 0;
	}

	warnx("could not get window title");
	free(r);
	return 1;
}

int
main(int argc, char **argv)
{
	int i, r = 0;

	if (argc < 2)
		usage(argv[0]);

	init_xcb(&conn);

	for (i=1; i < argc; i++)
		r += get_title(strtoul(argv[i], NULL, 16));

	kill_xcb(&conn);

	return r;
}
