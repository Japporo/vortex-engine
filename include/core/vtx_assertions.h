//Copyright (C) 2011 Emil Nord�n
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef VTX_ASSERTIONS_H
#define VTX_ASSERTIONS_H

void reportAssertionFailure(const char *expr, const char *file, long line);
#if defined(ASSERTIONS_ENABLED) && ASSERTIONS_ENABLED > 0
	#define debugBreak() __asm { int 3 }

	#define ASSERT(expr) \
		if(expr) { } \
		else \
		{ \
			reportAssertionFailure(#expr, \
			__FILE__, \
			__LINE__); \
			debugBreak(); \
		}
#else
	#define ASSERT(expr)
#endif

#endif