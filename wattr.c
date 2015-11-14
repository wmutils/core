/* See LICENSE file for copyright and license details. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <xcb/xcb.h>
#include <wm.h>


xcb_connection_t *conn;
xcb_screen_t     *scrn;

static void usage(char *);

static void
usage(char *name)
{
	fprintf(stderr, "usage: %s [-h] [bmiowhxy] <wid>\n", name);
	exit(1);
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
		ret = is_alive(w) ? 0 : 1;
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
				ret = is_ignored(w) ? 0 : 1;
				goto end;
			case 'm':
				ret = is_mapped(w) ? 0 : 1;
				goto end;
			default:
				kill_xcb(&conn); usage(argv[0]);
			}
			/* add a space if more attribute come after */
			putc(i+1 < strlen(argv[1]) ? ' ' : '\n',stdout);
		}
	}

end:
	kill_xcb(&conn);
	return ret;
}
