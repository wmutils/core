#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

#include "arg.h"

char *argv0;
static xcb_connection_t *conn;

static void xcbinit(void);
static void cleanup(void);
static void ignore(xcb_window_t);

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-sr] <wid> [wid..]\n", argv0);
	exit(1);
}

static void
xcbinit(void)
{
	conn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(conn))
		errx(1, "unable connect to the X server");
}

static void
cleanup(void)
{
	if (conn != NULL)
		xcb_disconnect(conn);
}

static void
setoverride(xcb_window_t w, int or)
{
	uint32_t mask = XCB_CW_OVERRIDE_REDIRECT;
	uint32_t val[] = { or };

	xcb_change_window_attributes(conn, w, mask, val);
}

int
main(int argc, char **argv)
{
	int setflag = 0;

	ARGBEGIN {
		case 's': setflag = 1; break;
		case 'r': setflag = 0; break;
		default: usage();
	} ARGEND;
	atexit(cleanup);
	xcbinit();

	while (*argv)
		setoverride(strtoul(*argv++, NULL, 16), setflag);

	xcb_flush(conn);

	return 0;
}
