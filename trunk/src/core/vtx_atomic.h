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

#ifndef VTX_ATOMIC_H
#define VTX_ATOMIC_H
#include <core/vtx_buildconfig.h>
#include <string>
#if defined(VTX_PLATFORM_WIN32)
#include <Windows.h>
#endif
#if defined(VTX_PLATFORM_LINUX)
#include <X11/X.h>
#endif

#include <stdint.h>

namespace core
{

#if defined(VTX_PLATFORM_WIN32)
	typedef HWND WINDOW;
	typedef HANDLE SYSTEM_HANDLE;
	//TODO: Figure out why this #define can not be used anywhere without compile errors...
	#define INVALID_SYSTEM_HANDLE INVALID_HANDLE_VALUE;
#endif
#if defined(VTX_PLATFORM_LINUX)
	//TODO: get this fixed.
	typedef XID WINDOW;
	typedef void *SYSTEM_HANDLE;
#endif

#define U32_SIGNIFICANT_BIT (1 << 31)
#if defined (VTX_PLATFORM_LINUX)
	typedef unsigned char		U8_t;
	typedef signed char			I8_t;
	typedef unsigned short		U16_t;
	typedef signed short		I16_t;
	typedef unsigned int		U32_t;
	typedef signed int			I32_t;
	typedef unsigned long long	U64_t;
	typedef float				F32_t;
	typedef double				F64_t;
#endif
#if defined(VTX_PLATFORM_WIN32)
	typedef unsigned __int8		U8_t;
	typedef signed __int8		I8_t;
	typedef unsigned __int16	U16_t;
	typedef signed __int16		I16_t;
	typedef unsigned __int32	U32_t;
	typedef signed __int32		I32_t;
	typedef unsigned __int64	U64_t;
	typedef float				F32_t;
	typedef double				F64_t;
#endif

#if defined(VTX_PLATFORM_LINUX)
	// Workaround until I find out how to get 64 bit integers on unix with g++.
	
#endif
	

}

#endif