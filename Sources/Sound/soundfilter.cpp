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
**    Mark Page
*/

#include "Sound/precomp.h"
#include "API/Sound/soundfilter.h"
#include "API/Sound/SoundProviders/soundfilter_provider.h"

class CL_SoundFilter_Impl
{
public:
	CL_SoundFilter_Impl()
	: provider(0)
	{
	}

	~CL_SoundFilter_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_SoundFilterProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_SoundFilter construction:

CL_SoundFilter::CL_SoundFilter(CL_SoundFilterProvider *provider)
: impl(new CL_SoundFilter_Impl)
{
	impl->provider = provider;
}

CL_SoundFilter::~CL_SoundFilter()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundFilter operations:

void CL_SoundFilter::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_SoundFilter is null");
}

CL_SoundFilterProvider *CL_SoundFilter::get_provider() const
{
	if (!impl)
		return 0;
	return impl->provider;
}

void CL_SoundFilter::filter(float **sample_data, int num_samples, int channels)
{
	impl->provider->filter(sample_data, num_samples, channels);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundFilter implementation:
