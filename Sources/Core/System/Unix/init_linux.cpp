/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"

#include <libgen.h>
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "init_linux.h"
// note: this cannot be replaced by <ctime>! (timeval needs to be defined)
#include <sys/time.h>
#include <sys/stat.h>
#include "API/Core/System/setup_core.h"
#include "API/Core/System/system.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

static int init_ref_count = 0;

void init_system()
{
	init_ref_count++;
	if (init_ref_count > 1) return;

	#ifdef NDEBUG
	signal(SIGSEGV, deinit);
	#endif
}

void deinit_system()
{
	init_ref_count--;
	if (init_ref_count > 0) return;

	#ifdef NDEBUG
	signal(SIGSEGV,SIG_DFL); //restore default behavior
	#endif
}

unsigned int CL_System::get_time()
{
	return (unsigned int) (get_microseconds() / 1000);
}

cl_uint64 CL_System::get_microseconds()
{
	timeval tv;
	gettimeofday(&tv, NULL);
	return (cl_uint64) tv.tv_sec*(cl_uint64) 1000000 + (cl_uint64) tv.tv_usec;
}

CL_String CL_System::get_exe_path()
{
	char exe_file[PATH_MAX];
#ifdef __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	if (mainBundle) 
	{
		CFURLRef mainURL = CFBundleCopyBundleURL(mainBundle);
		
		if (mainURL) 
		{
			int ok = CFURLGetFileSystemRepresentation (
				mainURL, (Boolean) true, (UInt8*)exe_file, PATH_MAX 
			);
			
			if (ok)
			{
				return CL_StringRef(exe_file) + "/";
			}
		}
	}
	
	throw CL_Exception("get_exe_path failed");

#else
#ifndef PROC_EXE_PATH
#define PROC_EXE_PATH "/proc/self/exe"
#endif
	int size;
	struct stat sb;
	if (lstat(PROC_EXE_PATH, &sb) < 0)
	{
#ifdef EXTERN___PROGNAME
		extern const char *__progname;
		char *pathenv, *name, *end;
		char fname[PATH_MAX];
		char cwd[PATH_MAX];
		struct stat sba;

		exe_file[0] = '\0';
		if ((pathenv = getenv("PATH")) != NULL)
		{
			for (name = pathenv; name; name = end)
			{
				if ((end = strchr(name, ':')))
					*end++ = '\0';
				snprintf(fname, sizeof(fname),
					"%s/%s", name, (char *)__progname);
				if (stat(fname, &sba) == 0) {
					snprintf(exe_file, sizeof(exe_file),
						"%s/", name);
					break;
				}
			}
		}
		// if getenv failed or path still not found
		// try current directory as last resort
		if (!exe_file[0])
		{
			if (getcwd(cwd, sizeof(cwd)) != NULL)
			{
				snprintf(fname, sizeof(fname),
					"%s/%s", cwd, (char *)__progname);
				if (stat(fname, &sba) == 0)
					snprintf(exe_file, sizeof(exe_file),
						"%s/", cwd);
			}
		}
		if (!exe_file[0])
			throw CL_Exception("get_exe_path: could not find path");
		else
			return CL_StringRef(exe_file);
#else
		throw CL_Exception("get_exe_path: proc file system not accesible");
#endif
	}
	else
	{
		size = readlink(PROC_EXE_PATH, exe_file, PATH_MAX);
		if (size < 0)
		{
			throw CL_Exception(strerror(errno));
		}
		else
		{
			exe_file[size] = '\0';
			return CL_StringRef(dirname(exe_file)) + "/";
		}
	}
#endif
	
}

/* EOF */
