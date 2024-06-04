/* SPDX-License-Identifier: LGPL-2.1-or-later */
/* From https://sourceware.org/git/?p=glibc.git;a=blob;f=inet/inet_ntoa.c;h=341d8591c30b88d4415898d93ae0dcbd7bf4e3d6;hb=46b5e98ef6f1b9f4b53851f152ecb8209064b26c */

/* Convert Inet number to ASCII representation.
   Copyright (C) 1997-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

/* The interface of this function is completely stupid, it requires a
   static buffer.  We relax this a bit in that we allow one buffer for
   each thread.  */
static __thread char buffer[18];

char *inet_ntoa(struct in_addr in)
{
  unsigned char *bytes = (unsigned char *) &in;
  snprintf (buffer, sizeof (buffer), "%d.%d.%d.%d",
	      bytes[0], bytes[1], bytes[2], bytes[3]);

  return buffer;
}