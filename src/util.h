
#ifdef __cplusplus
extern "C" {
#endif

/*
 * util.h -- Utility functions
 *
 * Copyright (C)1999-2006 Mark Simpson <damned@theworld.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can either send email to this
 * program's maintainer or write to: The Free Software Foundation,
 * Inc.; 59 Temple Place, Suite 330; Boston, MA 02111-1307, USA.
 *
 */
#ifndef UTIL_H
#define UTIL_H

#if HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef WIN64
#define IS_WINDOWS
#elif defined WIN32
#define IS_WINDOWS
#endif

#ifdef __APPLE__
typedef signed char             int8_t;
typedef unsigned char           uint8_t;
typedef signed short int        int16_t;
typedef unsigned short int      uint16_t;
typedef signed int              int32_t;
typedef unsigned int            uint32_t;
typedef unsigned long long      uint64_t;
typedef long long               int64_t;
#endif

#ifdef IS_WINDOWS
typedef signed char             int8_t;
typedef unsigned char           uint8_t;
typedef signed short int        int16_t;
typedef unsigned short int      uint16_t;
typedef signed int              int32_t;
typedef unsigned int            uint32_t;
typedef unsigned long long      uint64_t;
typedef long long               int64_t;
#endif

#include "common.h"

extern uint32_t GETINT32(unsigned char*p);
extern uint16_t GETINT16(unsigned char*p);
extern uint8_t GETINT8(unsigned char*p);

extern unsigned char* getbuf (FILE *fp, unsigned char buf[], size_t n);
extern uint32_t geti32(FILE *fp);
extern uint16_t geti16(FILE *fp);
extern uint8_t geti8(FILE *fp);

extern unsigned char* unicode_to_utf8 (size_t len, unsigned char*buf);

#endif /* UTIL_H */

#ifdef __cplusplus
}
#endif