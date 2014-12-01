#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

#include "arg.h"

enum {
	MAP     = 1 << 0,
	UNMAP   = 1 << 1,
	TOGGLE  = 1 << 2
};

char *argv0;
static xcb_connection_t *conn;

static void usage(void);
static void xcbinit(void);
static void cleanup(void);
static int ismapped(xcb_window_t w);

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-h] [-mut <wid> [wid..]]\n", argv0);
	exit(1);
}

static void
xcbinit(void)
{
	conn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(conn))
		errx(1, "unable to connect to the X server");
}

static void
cleanup(void)
{
	if (conn != NULL)
		xcb_disconnect(conn);
}

static int
ismapped(xcb_window_t w)
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

int
main(int argc, char **argv)
{
	int mapflag = 0;
	xcb_window_t w = 0;

	ARGBEGIN {
		case 'm': mapflag = MAP;    break;
		case 'u': mapflag = UNMAP;  break;
		case 't': mapflag = TOGGLE; break;
		default : usage();
	} ARGEND;

	if (argc < 1)
		usage();

	atexit(cleanup);
	xcbinit();

	while (*argv) {
		w = strtoul(*argv++, NULL, 16);

		switch (mapflag) {
		case MAP:
			xcb_map_window(conn, w);
			break;
		case UNMAP:
			xcb_unmap_window(conn, w);
			break;
		case TOGGLE:
			if (ismapped(w)) {
				xcb_unmap_window(conn, w);
			} else {
				xcb_map_window(conn, w);
			}
			break;
		default:
			return ismapped(w) ? 0 : 1;
		}
	}
	xcb_flush(conn);

	return 0;
}
