/**
*      Copyright (c) 2014, Broseph <dcat (at) iotek (dot) org>
*
*      Permission to use, copy, modify, and/or distribute this software for any
*      purpose with or without fee is hereby granted, provided that the above
*      copyright notice and this permission notice appear in all copies.
*
*      THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
*      WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
*      MERCHANTABILITY AND FITNESS IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
*      ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
*      WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
*      ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
*      OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**/

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include "util.h"

static xcb_connection_t *conn;

static void usage (char *name);

static void
usage (char *name)
{
	fprintf(stderr, "usage: %s <wid>\n", name);
	exit(1);
}

int
main (int argc, char **argv)
{
	xcb_window_t win;


	if (argc != 2)
		usage(argv[0]);

	init_xcb(&conn);

	win = strtoul(argv[1], NULL, 16);
	if (!win)
		errx(1, "could not get focused window");

	xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, win,
			XCB_CURRENT_TIME);

	xcb_flush(conn);

	kill_xcb(&conn);
	return 0;
}
