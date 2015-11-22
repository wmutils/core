/* See LICENSE file for copyright and license details. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <wm.h>

xcb_connection_t *conn;
xcb_screen_t     *scrn;

int
main(int argc, char **argv)
{
	wm_init_xcb(&conn);

	printf("0x%08x\n", wm_get_focus());

	wm_kill_xcb(&conn);
	return 0;
}
