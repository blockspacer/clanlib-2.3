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
#include "API/CSSLayout/PropertyTypes/css_box_background_origin.h"

CL_CSSBoxBackgroundOrigin::CL_CSSBoxBackgroundOrigin()
: type(type_value)
{
	values.push_back(origin_padding_box);
}

void CL_CSSBoxBackgroundOrigin::compute(const CL_CSSBoxBackgroundOrigin *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			values = parent->values;
		}
		else
		{
			type = type_value;
			values.clear();
			values.push_back(origin_padding_box);
		}
	}
}

CL_String CL_CSSBoxBackgroundOrigin::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	CL_String s;
	for (size_t i = 0; i < values.size(); i++)
	{
		if (i > 0)
			s += ", ";
		switch (values[i])
		{
		case origin_border_box:
			s += "border-box";
			break;
		case origin_padding_box:
			s += "padding-box";
			break;
		case origin_content_box:
			s += "content-box";
			break;
		}
	}
	return s;
}
