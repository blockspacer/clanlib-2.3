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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Render/program_attribute.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramAttribute_Generic:

class CL_ProgramAttribute_Impl
{
//! Construction:
public:
	CL_ProgramAttribute_Impl()
	{
	}
	
	~CL_ProgramAttribute_Impl()
	{
	}

//! Attributes:
public:
	CL_String name;

	int type;

	int size;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramAttribute Construction:

CL_ProgramAttribute::CL_ProgramAttribute()
{
}

CL_ProgramAttribute::CL_ProgramAttribute(const CL_StringRef &name, int size, int type)
: impl(new CL_ProgramAttribute_Impl)
{
	impl->name = name;
	impl->size = size;
	impl->type = type;
}

CL_ProgramAttribute::~CL_ProgramAttribute()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramAttribute Attributes:

void CL_ProgramAttribute::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_ProgramAttribute is null");
}

CL_StringRef CL_ProgramAttribute::get_name() const
{
	return impl->name;
}

int CL_ProgramAttribute::get_size() const
{
	return impl->size;
}

int CL_ProgramAttribute::get_type() const
{
	return impl->type;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProgramAttribute Implementation:
