/*
 * The authors of this software are Rob Pike and Ken Thompson,
 * with contributions from Mike Burrows and Sean Dorward.
 *
 *     Copyright (c) 2002-2006 by Lucent Technologies.
 *     Portions Copyright (c) 2004 Google Inc.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, provided that this entire notice
 * is included in all copies of any software which is or includes a copy
 * or modification of this software and in all copies of the supporting
 * documentation for such software.
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR LUCENT TECHNOLOGIES 
 * NOR GOOGLE INC MAKE ANY REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING 
 * THE MERCHANTABILITY OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */

#include <u.h>
#include <libc.h>
#include "fmtdef.h"

/* format the output into f->to and return the number of characters fmted  */

/* BUG: THIS FILE IS NOT UPDATED TO THE  NEW SPEC */
int
dorfmt(Fmt *f, const Rune *fmt)
{
	Rune *rt, *rs;
	Rune r;
	char *t, *s;
	int nfmt;

	nfmt = f->nfmt;
	for(;;){
		if(f->runes){
			rt = (Rune*)f->to;
			rs = (Rune*)f->stop;
			while((r = *fmt++) && r != '%'){
				FMTRCHAR(f, rt, rs, r);
			}
			f->nfmt += (int)(rt - (Rune *)f->to);
			f->to = rt;
			if(!r)
				return f->nfmt - nfmt;
			f->stop = rs;
		}else{
			t = (char*)f->to;
			s = (char*)f->stop;
			while((r = *fmt++) && r != '%'){
				FMTRUNE(f, t, f->stop, r);
			}
			f->nfmt += (int)(t - (char *)f->to);
			f->to = t;
			if(!r)
				return f->nfmt - nfmt;
			f->stop = s;
		}

		fmt = (Rune*)__fmtdispatch(f, (Rune*)fmt, 1);
		if(fmt == nil)
			return -1;
	}
	return 0;		/* not reached */
}
