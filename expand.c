/*
 * UNG's Not GNU
 *
 * Copyright (c) 2011-2017, Jakob Kaivo <jkk@ung.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#define _XOPEN_SOURCE 700
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int tabstop = 8;
//static int *tablist = NULL;

static int expand(const char *path)
{
	FILE *f = stdin;
	if (path) {
		f = fopen(path, "rb");
	}

	if (f == NULL) {
		fprintf(stderr, "expand: Couldn't open %s: %s\n", path ? path : "stdin", strerror(errno));
		return 1;
	}

	int col = 0;
	while (!feof(f)) {
		int c = fgetc(f);
		if (c != '\t') {
			if (c == '\n') {
				col = 0;
			} else if (c == '\b') {
				col--;
			} else {
				col++;
			}
			putchar(c);
			continue;
		}

		if (tabstop != 0) {
			do {
				putchar(' ');
				col++;
			} while (col % tabstop);
			continue;
		}

		/* TODO: next tabstop in tablist */
	}

	if (f != stdin) {
		fclose(f);
	}

	return 0;
}

static int set_tablist(const char *list)
{
	tabstop = atoi(list);
	return 0;
}

int main(int argc, char *argv[])
{
	int c;
	while ((c = getopt(argc, argv, "t:")) != -1) {
		switch (c) {
		case 't':	/** set tabs to [tablist] **/
			if (set_tablist(optarg) != 0) {
				return 1;
			}
			break;

		default:
			return 1;
		}
	}

	int r = 0;

	do {
		r |= expand(argv[optind++]);
	} while (optind < argc);

	return r;
}
