/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

#include "util.h"
#include "arg.h"

#define MAXLEN 512

static xcb_connection_t *conn;

void
usage(char *name)
{
	fprintf(stderr, "%s [-ds] wid atom [value]\n", name);
}

xcb_atom_t
add_atom(xcb_atom_t type, char *name, size_t len)
{
	xcb_atom_t atom;
	xcb_intern_atom_cookie_t c;
	xcb_intern_atom_reply_t *r;

	c = xcb_intern_atom(conn, 0, len, name);
	r = xcb_intern_atom_reply(conn, c, NULL);
	if (!r)
		return 0;

	atom = r->atom;
	free(r);

	return atom;
}

int
set_atom(xcb_window_t wid, xcb_atom_t atom, xcb_atom_t type, size_t len, void *data)
{
	int errcode;
	xcb_void_cookie_t c;
	xcb_generic_error_t *e;

	c = xcb_change_property_checked(conn, XCB_PROP_MODE_REPLACE,
		wid, atom, type, 8, len, data);
	e = xcb_request_check(conn, c);
	if (!e)
		return 0;

	errcode = e->error_code;
	free(e);

	return errcode;
}

int
get_atom(xcb_window_t wid, xcb_atom_t atom, xcb_atom_t type, char *data, size_t len)
{
	size_t n;
	xcb_get_property_cookie_t c;
	xcb_get_property_reply_t *r;

	c = xcb_get_property(conn, 0, wid, atom, type, 0, MAXLEN);
	r = xcb_get_property_reply(conn, c, NULL);
	if (!r)
		return -1;

	if (!(n = xcb_get_property_value_length(r))) {
		free(r);
		return -1;
	}

	strncpy(data, xcb_get_property_value(r), n);
	data[n] = 0;

	free(r);

	return 0;
}


int
main(int argc, char **argv)
{
	int dflag = 0, sflag = 0;
	char *key, *val, *argv0;
	char data[MAXLEN];
	xcb_window_t wid;
	xcb_atom_t atom;

	ARGBEGIN {
	case 'd':
		dflag = 1;
		break;
	case 's':
		sflag = 1;
		break;
	default:
		usage(argv0);
		return -1;
	} ARGEND;

	if (argc < 2 + sflag)
		return -1;

	wid = strtoul(argv[0], NULL, 16);
	key = argv[1];
	if (sflag)
		val = argv[2];

	init_xcb(&conn);

	/* retrieve atom ID from server */
	atom = add_atom(XCB_ATOM_STRING, key, strlen(key));
	if (!atom)
		return -1;

	/* remove property from window */
	if (dflag)
		xcb_delete_property(conn, wid, atom);

	/* set property on window (must be a string) */
	if (sflag)
		set_atom(wid, atom, XCB_ATOM_STRING, strlen(val), val);

	/* retrieve and print atom value to stdout */
	if (!get_atom(wid, atom, XCB_ATOM_STRING, data, MAXLEN))
		printf("%s\n", data);

	kill_xcb(&conn);

	return 0;
}
