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
*/

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/PropertyTypes/css_box_white_space.h"

CL_CSSBoxWhiteSpace::CL_CSSBoxWhiteSpace()
: type(type_inherit)
{
}

void CL_CSSBoxWhiteSpace::compute(const CL_CSSBoxWhiteSpace *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
			type = parent->type;
		else
			type = type_normal;
	}
}

CL_String CL_CSSBoxWhiteSpace::to_string() const
{
	switch (type)
	{
	default:
	case type_normal:
		return "normal";
	case type_pre:
		return "pre";
	case type_nowrap:
		return "nowrap";
	case type_pre_wrap:
		return "pre-wrap";
	case type_pre_line:
		return "pre-line";
	case type_inherit:
		return "inherit";
	}
}
