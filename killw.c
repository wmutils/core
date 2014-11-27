#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <xcb/xcb.h>

static xcb_connection_t *conn;

static void usage(void);
static void xcbinit(void);
static void cleanup(void);

static void
xcbinit(void)
{
	conn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(conn))
		errx(1, "xcb_connect");
}

static void
cleanup(void)
{
	if (conn != NULL)
		xcb_disconnect(conn);
}

int
main(int argc, char **argv)
{
	int i;

	atexit(cleanup);
	xcbinit();

	for (i=1; i<argc; i++)
		xcb_kill_client(conn, strtoul(argv[i], NULL, 16));

	xcb_flush(conn);

	return 0;
}