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

#ifndef VTX_FILESTREAM_H
#define VTX_FILESTREAM_H

#include <platform/vtx_iostream.h>
#ifdef WIN32
#include <Windows.h> // Why is this here?
#endif

namespace io
{

	class FileStream : public IOStream
	{
	public:
		void foo();
		virtual IOStream::ErrorCode openStream(std::wstring &path);
		virtual void closeStream();
	private:
	};

}

#endif
