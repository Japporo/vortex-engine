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

#ifndef VTX_JOB_H
#define VTX_JOB_H

#include <core/vtx_atomic.h>

namespace concurrency
{
	class JobProcessor;

	class Job
	{
	public:
		typedef void (*complete_callback)(Job &job);
		Job(core::U32_t priority, complete_callback callback);
		virtual void performJob() = 0;
	private:
		core::U32_t priority;
		complete_callback callback;
	};
}

#endif
