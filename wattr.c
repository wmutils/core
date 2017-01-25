/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

#include "util.h"

static xcb_connection_t *conn;

static void usage(char *);
static int get_attribute(xcb_window_t, int);

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
usage(char *name)
{
	fprintf(stderr, "usage: %s [-h] [bmiowhxy] <wid>\n", name);
	exit(1);
}

static int
get_attribute(xcb_window_t w, int attr)
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
	int c, ret = 0;
	size_t i;
	xcb_window_t w = 0;

	if (argc < 2 || (strncmp(argv[1], "-h", 2) == 0)) {
		usage(argv[0]);
	}

	init_xcb(&conn);

	if (argc == 2) {
		w = strtoul(argv[1], NULL, 16);
		ret = exists(conn, w) ? 0 : 1;
		goto end;
	}

	for (c=2; argv[c]; c++) {
		w = strtoul(argv[c], NULL, 16);

		for (i=0; i<strlen(argv[1]); i++) {
			switch (argv[1][i]) {
				case 'i':
					printf("0x%08x", w);
					break;
				case 'b':
					printf("%d", get_attribute(w, ATTR_B));
					break;
				case 'h': 
					printf("%d", get_attribute(w, ATTR_H));
					break;
				case 'x':
					printf("%d", get_attribute(w, ATTR_X));
					break;
				case 'y':
					printf("%d", get_attribute(w, ATTR_Y));
					break;
				case 'w':
					printf("%d", get_attribute(w, ATTR_W));
					break;
				case 'o':
					ret = ignore(conn, w) ? 0 : 1;
					goto end;
				case 'm':
					ret = mapped(conn, w) ? 0 : 1;
					goto end;
				default: kill_xcb(&conn); usage(argv[0]);
			}
			/* add a space if more attribute come after */
			putc(i+1 < strlen(argv[1]) ? ' ' : '\n',stdout);
		}
	}

end:
	kill_xcb(&conn);

	return ret;
}
