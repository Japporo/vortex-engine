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

#ifndef VTX_JOBMANAGER_H
#define VTX_JOBMANAGER_H

#include <core/vtx_atomic.h>

namespace core
{
	class Allocator;
}

namespace concurrency
{
	class JobProcessor;

	void InitJobMgr(core::Allocator &allocator);

	class JobManager
	{
	private:
		core::Allocator &alloc;
		core::U32_t promotionIncrement;
		core::U32_t noJobProcessors;
		core::U32_t getCurrentProcessAffinityMask(void);
		core::U32_t getNumberOfProcessorsAvailable(void);
		void initJobProcessors(core::U32_t processAffinity);
		JobProcessor **processors;
	public:
		JobManager(core::Allocator &allocator);
		~JobManager();
		void setPromotionIncrement(core::U32_t value);
		core::U32_t getPromotionIncrement(void);
	};

	extern JobManager *jobMgr;
}

#endif