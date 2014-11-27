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

enum {
	ATTR_W = 1 << 0,
	ATTR_H = 1 << 1,
	ATTR_X = 1 << 2,
	ATTR_Y = 1 << 3,
	ATTR_MAX
};

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-h] [whxy] <wid>\n", argv0);
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
	int ms;
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
getattribute(xcb_window_t w, int attr)
{
	xcb_get_geometry_cookie_t c;
	xcb_get_geometry_reply_t *r;

	c = xcb_get_geometry(conn, w);
	r = xcb_get_geometry_reply(conn, c, NULL);

	if (r == NULL)
		errx(1, "0x%08x: no such window", w);

	switch (attr) {
		case ATTR_W: attr = r->width  + 2 * r->border_width; break;
		case ATTR_H: attr = r->height + 2 * r->border_width; break;
		case ATTR_X: attr = r->x - r->border_width; break;
		case ATTR_Y: attr = r->y - r->border_width; break;
	}

	free(r);
	return attr;
}

int
main(int argc, char **argv)
{
	int i;
	xcb_window_t w = 0;

	if (argc < 2 || (strncmp(argv[1], "-h", 2) == 0)) {
		argv0 = argv[0];
		usage();
	}

	atexit(cleanup);
	xcbinit();

	w = strtoul(argv[argc-1], NULL, 16);

	for (i=0; i<strnlen(argv[1], ATTR_MAX); i++) {
		switch (argv[1][i]) {
			case 'w': printf("%d", getattribute(w, ATTR_W)); break;
			case 'h': printf("%d", getattribute(w, ATTR_H)); break;
			case 'x': printf("%d", getattribute(w, ATTR_X)); break;
			case 'y': printf("%d", getattribute(w, ATTR_Y)); break;
			default : exists(w) ? exit(0) : exit(1);
		}

		/* add a space if more attribute come after */
		putc(i+1 < strnlen(argv[1], ATTR_MAX) ? ' ' : '\n',stdout);
	}

	return 0;
}
