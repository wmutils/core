#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <err.h>

static xcb_connection_t *conn;

static void usage (char *);
static void cleanup (void);
static int  get_title (xcb_window_t);

static void cleanup (void)
{
	if (conn)
		xcb_disconnect(conn);
}

static void
usage (char *name)
{
	fprintf(stderr, "usage: %s <wid>\n", name);
	exit(1);
}

static int
get_title (xcb_window_t win)
{
	int len = 0;
	xcb_get_property_cookie_t cookie;
	xcb_get_property_reply_t *r;

	cookie = xcb_get_property(conn, 0, win,
			XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0L, 32L);
	r = xcb_get_property_reply(conn, cookie, NULL);

	if (r) {
		len = xcb_get_property_value_length(r);
		printf("%.*s\n", len, (char *) xcb_get_property_value(r));

		return 0;
	}

	warnx("could not get window title");
	free(r);
	return 1;
}

int main (int argc, char **argv)
{
	int i, r = 0;

	if (argc < 2)
		usage(argv[0]);

	atexit(cleanup);
	if (xcb_connection_has_error(conn = xcb_connect(NULL, NULL)))
		errx(1, "error connecting to X");

	for (i=1; i < argc; i++)
		r += get_title(strtoul(argv[i], NULL, 16));

	return r;
}
