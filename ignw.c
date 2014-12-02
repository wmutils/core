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
ignore(xcb_window_t w)
{
	uint32_t mask = XCB_CW_OVERRIDE_REDIRECT;
	uint32_t val[] = { 1 };

	xcb_change_window_attributes(conn, w, mask, val);
}

int
main(int argc, char **argv)
{
	atexit(cleanup);
	xcbinit();

	while (*argv)
		ignore(strtoul(*argv++, NULL, 16));

	xcb_flush(conn);

	return 0;
}
