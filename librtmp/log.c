/*
 *  Copyright (C) 2008-2009 Andrej Stepanchuk
 *  Copyright (C) 2009-2010 Howard Chu
 *
 *  This file is part of librtmp.
 *
 *  librtmp is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1,
 *  or (at your option) any later version.
 *
 *  librtmp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with librtmp see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/lgpl.html
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "log.h"

#define MAX_PRINT_LEN	2048

AMF_LogLevel debuglevel = LOGERROR;

static int neednl;

static FILE *fmsg;

static const char *levels[] = {
  "CRIT", "ERROR", "WARNING", "INFO",
  "DEBUG", "DEBUG2"
};

void LogSetOutput(FILE *file)
{
	fmsg = file;
}

void LogPrintf(const char *format, ...)
{
	char str[MAX_PRINT_LEN]="";
        int len;
	va_list args;
	va_start(args, format);
	len = vsnprintf(str, MAX_PRINT_LEN-1, format, args);
	va_end(args);

	if ( debuglevel==LOGCRIT )
		return;

	if ( !fmsg ) fmsg = stderr;

	if (neednl) {
		putc('\n', fmsg);
		neednl = 0;
	}

        if (len > MAX_PRINT_LEN-1)
          len = MAX_PRINT_LEN-1;
	fprintf(fmsg, "%s", str);
        if (str[len-1] == '\n')
	  fflush(fmsg);
}

void LogStatus(const char *format, ...)
{
	char str[MAX_PRINT_LEN]="";
	va_list args;
	va_start(args, format);
	vsnprintf(str, MAX_PRINT_LEN-1, format, args);
	va_end(args);

	if ( debuglevel==LOGCRIT )
		return;

	if ( !fmsg ) fmsg = stderr;

	fprintf(fmsg, "%s", str);
	fflush(fmsg);
	neednl = 1;
}

void Log(int level, const char *format, ...)
{
	char str[MAX_PRINT_LEN]="";
	va_list args;
	va_start(args, format);
	vsnprintf(str, MAX_PRINT_LEN-1, format, args);
	va_end(args);

	// Filter out 'no-name'
	if ( debuglevel<LOGALL && strstr(str, "no-name" ) != NULL )
		return;

	if ( !fmsg ) fmsg = stderr;

	if ( level <= debuglevel ) {
		if (neednl) {
			putc('\n', fmsg);
			neednl = 0;
		}
		fprintf(fmsg, "%s: %s\n", levels[level], str);
#ifdef _DEBUG
		fflush(fmsg);
#endif
	}
}

void LogHex(int level, const char *data, unsigned long len)
{
	unsigned long i;
	if ( level > debuglevel )
		return;
	for(i=0; i<len; i++) {
		LogPrintf("%02X ", (unsigned char)data[i]);
	}
	LogPrintf("\n");
}

void LogHexString(int level, const char *data, unsigned long len)
{
	static const char hexdig[] = "0123456789abcdef";
#define BP_OFFSET 9
#define BP_GRAPH 60
#define BP_LEN	80
	char	line[BP_LEN];
	unsigned long i;

	if ( !data || level > debuglevel )
		return;

	/* in case len is zero */
	line[0] = '\n';
	line[1] = '\0';

	for ( i = 0 ; i < len ; i++ ) {
		int n = i % 16;
		unsigned off;

		if( !n ) {
			if( i ) LogPrintf( "%s", line );
			memset( line, ' ', sizeof(line)-2 );
			line[sizeof(line)-2] = '\n';
			line[sizeof(line)-1] = '\0';

			off = i % 0x0ffffU;

			line[2] = hexdig[0x0f & (off >> 12)];
			line[3] = hexdig[0x0f & (off >>  8)];
			line[4] = hexdig[0x0f & (off >>  4)];
			line[5] = hexdig[0x0f & off];
			line[6] = ':';
		}

		off = BP_OFFSET + n*3 + ((n >= 8)?1:0);
		line[off] = hexdig[0x0f & ( data[i] >> 4 )];
		line[off+1] = hexdig[0x0f & data[i]];

		off = BP_GRAPH + n + ((n >= 8)?1:0);

		if ( isprint( (unsigned char) data[i] )) {
			line[BP_GRAPH + n] = data[i];
		} else {
			line[BP_GRAPH + n] = '.';
		}
	}

	LogPrintf( "%s", line );
}
