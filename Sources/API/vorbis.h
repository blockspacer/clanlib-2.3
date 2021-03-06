/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

/// \brief <p>Ogg Vorbis playback. With this library, you can play music with the same
/// \brief quality as todays mp3, but with a completely open sound format (mp3 is
/// \brief patented by some greedy companies that want money for each mp3 you use in a
/// \brief commercial product).</p>
//! Global=Vorbis

#pragma once

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "Vorbis/soundprovider_vorbis.h"
#include "Vorbis/setupvorbis.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(CL_DLL)
			#pragma comment(lib, "clanVorbis-dll.lib")
			#pragma comment(lib, "libvorbis-dll.lib")
			#pragma comment(lib, "libogg-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanVorbis-static-mtdll.lib")
			#pragma comment(lib, "libvorbis-static-mtdll.lib")
			#pragma comment(lib, "libogg-static-mtdll.lib")
		#else
			#pragma comment(lib, "clanVorbis-static-mt.lib")
			#pragma comment(lib, "libvorbis-static-mt.lib")
			#pragma comment(lib, "libogg-static-mt.lib")
		#endif
	#else
		#if defined(CL_DLL)
			#pragma comment(lib, "clanVorbis-dll-debug.lib")
			#pragma comment(lib, "libvorbis-dll-debug.lib")
			#pragma comment(lib, "libogg-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanVorbis-static-mtdll-debug.lib")
			#pragma comment(lib, "libvorbis-static-mtdll-debug.lib")
			#pragma comment(lib, "libogg-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "clanVorbis-static-mt-debug.lib")
			#pragma comment(lib, "libvorbis-static-mt-debug.lib")
			#pragma comment(lib, "libogg-static-mt-debug.lib")
		#endif
	#endif
#endif
