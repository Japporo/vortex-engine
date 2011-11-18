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

#define ASSERTIONS_ENABLED

#include <core/vtx_nativewindow.h>
#include <core/vtx_assertions.h>
#include <core/vtx_vortex.h>

#include <core/vtx_eventoutput.h>

#include <core/vtx_windowcreationparams.h>

#if defined(VTX_PLATFORM_WIN32)
#include <Windows.h>
#include <text\vtx_stringutil.h>
#include <vector>
#endif

#if defined(VTX_PLATFORM_LINUX)
#include <stdlib.h>
std::map<platform::WINDOW, XErrorEvent*> *NativeWindow::lastErrorMap;
bool NativeWindow::isHandlingXErrors = false;
#endif

core::NativeWindow::NativeWindow(core::Root &parent) : core::VortexBase(parent)
{
#if defined(VTX_PLATFORM_LINUX)
	if(!core::NativeWindow::isHandlingXErrors)
	{
		core::NativeWindow::isHandlingXErrors = true;
		core::NativeWindow::lastErrorMap = new std::map<core::WINDOW, XErrorEvent*>;
		XSetErrorHandler(core::NativeWindow::xErrorHandler);
	}
#endif
}

core::NativeWindow *core::NativeWindow::create(Root &parent, core::WindowCreationParams &params)
{
		// Window does not exist and should be created.
	NativeWindow *window = new NativeWindow(parent);
	if(params.windowHandle == 0)
	{
		parent.output->reportEvent(core::EventOutput::E_LEVEL_VERBOSE, "RenderManager: No handle supplied, creating new window.");
		window->manageWindow = true;
#if defined(VTX_PLATFORM_WIN32)
		HINSTANCE instance = GetModuleHandle(NULL);
		LPCWSTR className = __TEXT("VortexWin32");

		WNDCLASSEX wndclass;
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = CS_OWNDC; // CS_HREDRAW | CS_VREDRAW; //Redraw on horizontal or vertical movement or size changes.
		wndclass.lpfnWndProc = params.wndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = instance;
		wndclass.hIcon = NULL; // TODO: Fix properly?
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = className;
		wndclass.hIconSm = NULL; // TODO: Fix properly?

		DWORD style;
		if(params.isFullscreen)
			style = WS_POPUP;
		else
		{
			style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			if(params.styleFlags & WindowCreationParams::STYLE_MENU)
				style |= WS_SYSMENU;
			if((params.styleFlags & WindowCreationParams::STYLE_NOCAPTION))
				style |= WS_CAPTION;
			if(params.styleFlags & WindowCreationParams::STYLE_MAXIMIZEBUTTON)
				style |= WS_MAXIMIZEBOX;
			if(params.styleFlags & WindowCreationParams::STYLE_MAXIMIZED)
				style |= WS_MAXIMIZE;
			if(params.styleFlags & WindowCreationParams::STYLE_SIZEABLE)
				style |= WS_SIZEBOX;
		}

		ATOM atom = RegisterClassEx(&wndclass);
		if(atom == 0)
		{
			int err = GetLastError();
			if(err != ERROR_CLASS_ALREADY_EXISTS)
				return 0;
		}
		
		std::vector<wchar_t> wideTitle;
		text::StringUtil::utf8to16Vector(params.windowTitle, wideTitle);

		window->handle = CreateWindow(
			className,
			&wideTitle.front(),
			style,
			params.windowPosition.x,
			params.windowPosition.y,
			params.windowSize.x,
			params.windowSize.y,
			NULL,
			NULL,
			instance,
			NULL);

		if(window->handle == NULL)
		{
			int err = GetLastError();
		}
		ShowWindow(window->handle, SW_SHOW);
#elif defined(VTX_PLATFORM_LINUX) && defined(VTX_COMPILE_WITH_OPENGL) 
	// TODO: Need to create window without glX methods for software rendering.
	// When done, investigate if its usable for when compiling with OpenGL aswell. Unity is king.
	int result;
	if(params.displayX11 == NULL)
	{
		window->display = XOpenDisplay(getenv("DISPLAY"));
	}
	else
	{
		window->display = params.displayX11;
	}
	ASSERT(window->display != NULL);
	if(window->display == NULL)
	{
		std::string message("XOpenDisplay failed");
		parent.output->reportEvent(core::EventOutput::E_LEVEL_FATAL, message);
	}

	int visualAttribs[12] = { GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 1};
	XVisualInfo *visualInfo = glXChooseVisual(window->display, DefaultScreen(window->display), visualAttribs);	//TODO: Fix attributes!
	ASSERT(visualInfo != NULL);
	if(visualInfo == NULL)
	{
		std::string message("glXChooseVisual failed");
		parent.output->reportEvent(core::EventOutput::E_LEVEL_FATAL, message);
	}
	XSetWindowAttributes attribs;
	attribs.event_mask = ExposureMask | VisibilityChangeMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
	attribs.border_pixel = 0;
	attribs.bit_gravity = StaticGravity;
	attribs.colormap = XCreateColormap(
				window->display,
				RootWindow(window->display, visualInfo->screen),
				visualInfo->visual,
				AllocNone);
	ASSERT(attribs.colormap != BadAlloc);
	ASSERT(attribs.colormap != BadMatch);
	ASSERT(attribs.colormap != BadValue);
	ASSERT(attribs.colormap != BadWindow);
	if(	BadAlloc == attribs.colormap ||
		BadMatch == attribs.colormap ||
		BadValue == attribs.colormap ||
		BadWindow == attribs.colormap)
	{
		std::string message("XCreateColormap");
		parent.output->reportMethodFailedEvent(core::EventOutput::E_LEVEL_FATAL, message, attribs.colormap);
	}


	unsigned long mask = CWBorderPixel | CWBitGravity | CWEventMask | CWColormap;

	window->win = XCreateWindow(
				window->display,
				DefaultRootWindow(window->display),
				params.windowPosition.x,
				params.windowPosition.y,
				params.windowSize.x,
				params.windowSize.y,
				0,					// Border width
				0,					// Depth (?)
				InputOutput,				// Class
				visualInfo->visual,
				mask,
				&attribs);
	ASSERT(window->win != BadAlloc);
	ASSERT(window->win != BadColor);
	ASSERT(window->win != BadCursor);
	ASSERT(window->win != BadMatch);
	ASSERT(window->win != BadPixmap);
	ASSERT(window->win != BadValue);
	ASSERT(window->win != BadWindow);
	if(	window->win == BadAlloc  ||
		window->win == BadColor  ||	
		window->win == BadCursor ||
		window->win == BadMatch  ||
		window->win == BadPixmap ||
		window->win == BadValue  ||
		window->win == BadWindow)
	{
		std::string message("XCreateWindow");
		parent.output->reportMethodFailedEvent(core::EventOutput::E_LEVEL_FATAL, message, window->win);
	}


	// TODO: Find out a better way to get a char* from a wstring.
	std::string title(params.windowTitle.begin(), params.windowTitle.end());
	title.assign(params.windowTitle.begin(), params.windowTitle.end());
	result = XStoreName(window->display, window->win, title.c_str());
	ASSERT(result != BadAlloc);
	ASSERT(result != BadWindow);
	if(BadAlloc == result || BadWindow == result)
	{
		std::string message("XStoreName");
		parent.output->reportMethodFailedEvent(core::EventOutput::E_LEVEL_FATAL, message, result);
	}
		
	result = XMapWindow(window->display, window->win);
	ASSERT(result != BadWindow);
	if(BadWindow == result)
	{
		std::string message("XMapWindow failed with BadWindow");
		parent.output->reportEvent(core::EventOutput::E_LEVEL_FATAL, message);
	}

	window->context = glXCreateContext(
				window->display,
				visualInfo,
				NULL,
				true);
	ASSERT(window->context != NULL);
	if(window->context == NULL)
	{
		XErrorEvent *event = NativeWindow::getLastXError(window->win);
		std::string message("glXCreateContext");
		parent.output->reportMethodFailedEvent(core::EventOutput::E_LEVEL_FATAL, message, event->error_code);
	}
#endif
	}
	// Window already exists.
	else
	{
		parent.output->reportEvent(core::EventOutput::E_LEVEL_VERBOSE, "RenderManager: Will not create new window, using supplied windowhandle.");
		window->handle = params.windowHandle;
		window->manageWindow = false;
#if defined(VTX_PLATFORM_LINUX)
        window->win = window->handle;

		if(params.displayX11 == NULL)
		{
			window->display = XOpenDisplay(getenv("DISPLAY"));
		}
		else
		{
			window->display = params.displayX11;
		}
		ASSERT(window->display != NULL);
		if(window->display == NULL)
		{
			std::string message("XOpenDisplay failed");
			parent.output->reportEvent(core::EventOutput::E_LEVEL_FATAL, message);
		}

		int visualAttribs[12] = { GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 1};
		XVisualInfo *visualInfo = glXChooseVisual(window->display, DefaultScreen(window->display), visualAttribs);	//TODO: Fix attributes!
		ASSERT(visualInfo != NULL);
		if(visualInfo == NULL)
		{
			std::string message("glXChooseVisual failed");
			parent.output->reportEvent(Ecore::ventOutput::E_LEVEL_FATAL, message);
		}


		window->context = glXCreateContext(
				window->display,
				visualInfo,
				NULL,
				true);
		ASSERT(window->context != NULL);
		if(window->context == NULL)
		{
			XErrorEvent *event = NativeWindow::getLastXError(window->win);
			std::string message("glXCreateContext");
			parent.output->reportMethodFailedEvent(core::EventOutput::E_LEVEL_FATAL, message, event->error_code);
		}
#endif
	}
	ASSERT(window->handle != NULL);

return window;

}

void core::NativeWindow::destroy(void)
{
#if defined(VTX_PLATFORM_WIN32)
	HINSTANCE instance = GetModuleHandle(NULL);
	CloseWindow(this->handle);
	DestroyWindow(this->handle);
	UnregisterClass(__TEXT("VortexWin32"), instance);
#endif
#if defined(VTX_PLATFORM_LINUX)
	// TODO: Do it!
#endif
}

core::WINDOW core::NativeWindow::getHandle(void)
{
	return this->handle;
}

#if defined(VTX_PLATFORM_LINUX)
XErrorEvent *NativeWindow::getLastXError(platform::WINDOW handle)
{
	std::map<platform::WINDOW, XErrorEvent*>::iterator it;
	XErrorEvent *lastError = NULL;
	
	it = core::NativeWindow::lastErrorMap->find(handle);
	if(it != core::NativeWindow::lastErrorMap->end())
	{
		lastError = it->second;
	}
	return lastError;
}

int core::NativeWindow::xErrorHandler(Display *display, XErrorEvent *event)
{
	core::NativeWindow::lastErrorMap[0][event->resourceid] = event;
	return 0;
}
#endif