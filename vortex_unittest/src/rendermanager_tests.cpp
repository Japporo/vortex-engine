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

#include "vtx_render.h"
#include "gtest/gtest.h"

TEST(RenderManager, CreateWindow)
{
	RenderManager manager;
	RenderCreationParams renderParams;
	WindowCreationParams windowParams;

	renderParams.rapi = E_RAPI_DX10;
	manager.init(renderParams, windowParams);
	ASSERT_NE(manager.getWindowHandle(), (platform::WINDOW)0);
}