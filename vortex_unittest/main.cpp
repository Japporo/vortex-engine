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

#include "gtest\gtest.h"
#include <stdio.h>
#include <core\vtx_buildconfig.h>

#if defined(VTX_PLATFORM_WIN32)
#include <Windows.h>
#elif(VTX_PLATFORM_LINUX)
#include <unistd.h>
#endif

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

#if defined(VTX_PLATFORM_WIN32)
	if(SetCurrentDirectory(L"testground") == FALSE)
	{
		std::cout << "SetCurrentDirectory failed with: " << GetLastError() << std::endl;
		return -1;
	}
#elif(VTX_PLATFORM_LINUX)
	// handle returnvalue of chdir.
	chdir("./testground");
#endif

	if(RUN_ALL_TESTS() > 0)
	{
		getchar();
	}

	return 0;
}
