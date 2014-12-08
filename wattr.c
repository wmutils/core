#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <xcb/xcb.h>

char *argv0;
static xcb_connection_t *conn;

static void usage(void);
static void xcbinit(void);
static void cleanup(void);
static int getattribute(xcb_window_t, int);
static int mapped(xcb_window_t);
static int exists(xcb_window_t);

enum {
	ATTR_W = 1 << 0,
	ATTR_H = 1 << 1,
	ATTR_X = 1 << 2,
	ATTR_Y = 1 << 3,
	ATTR_B = 1 << 4,
	ATTR_M = 1 << 5,
	ATTR_I = 1 << 6,
	ATTR_MAX
};

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-h] [bmiwhxy] <wid>\n", argv0);
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
exists(xcb_window_t w)
{
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t  *r;

	c = xcb_get_window_attributes(conn, w);
	r = xcb_get_window_attributes_reply(conn, c, NULL);

	if (r == NULL)
		return 0;

	free(r);
	return 1;
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
getattribute(xcb_window_t w, int attr)
{
	xcb_get_geometry_cookie_t c;
	xcb_get_geometry_reply_t *r;

	c = xcb_get_geometry(conn, w);
	r = xcb_get_geometry_reply(conn, c, NULL);

	if (r == NULL)
		errx(1, "0x%08x: no such window", w);

	switch (attr) {
		case ATTR_X: attr = r->x; break;
		case ATTR_Y: attr = r->y; break;
		case ATTR_W: attr = r->width; break;
		case ATTR_H: attr = r->height; break;
		case ATTR_B: attr = r->border_width; break;
	}

	free(r);
	return attr;
}

int
main(int argc, char **argv)
{
	int i,c;
	xcb_window_t w = 0;

	if (argc < 2 || (strncmp(argv[1], "-h", 2) == 0)) {
		argv0 = argv[0];
		usage();
	}

	atexit(cleanup);
	xcbinit();

	for (c=2; argv[c]; c++) {
		w = strtoul(argv[c], NULL, 16);

		for (i=0; i<strlen(argv[1]); i++) {
			switch (argv[1][i]) {
				case 'i': printf("0x%08x", w);
					  break;
				case 'b': printf("%d", getattribute(w, ATTR_B));
					  break;
				case 'h': printf("%d", getattribute(w, ATTR_H));
					  break;
				case 'x': printf("%d", getattribute(w, ATTR_X));
					  break;
				case 'y': printf("%d", getattribute(w, ATTR_Y));
					  break;
				case 'w': printf("%d", getattribute(w, ATTR_W));
					  break;
				case 'o': ignored(w)? exit(0) : exit(1);
				case 'm': mapped(w) ? exit(0) : exit(1);
				default : exists(w) ? exit(0) : exit(1);
			}
			/* add a space if more attribute come after */
			putc(i+1 < strlen(argv[1]) ? ' ' : '\n',stdout);
		}
	}

	return 0;
}
