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
#include "API/CSSLayout/PropertyTypes/css_box_font_family.h"
#include "../../css_resource_cache.h"

CL_CSSBoxFontFamily::CL_CSSBoxFontFamily()
: type(type_inherit)
{
}

void CL_CSSBoxFontFamily::compute(const CL_CSSBoxFontFamily *parent, CL_CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			names = parent->names;
		}
		else
		{
			type = type_names;
			names.clear();
			names.push_back(CL_CSSBoxFontFamilyName());
		}
	}
}

CL_String CL_CSSBoxFontFamily::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	CL_String s;
	for (size_t i = 0; i < names.size(); i++)
	{
		if (i > 0)
			s += ", ";
		switch (names[i].type)
		{
		default:
		case CL_CSSBoxFontFamilyName::type_family_name:
			s += cl_format("\"%1\"", names[i].name);
			break;
		case CL_CSSBoxFontFamilyName::type_serif:
			s += "serif";
			break;
		case CL_CSSBoxFontFamilyName::type_sans_serif:
			s += "sans-serif";
			break;
		case CL_CSSBoxFontFamilyName::type_cursive:
			s += "cursive";
			break;
		case CL_CSSBoxFontFamilyName::type_fantasy:
			s += "fantasy";
			break;
		case CL_CSSBoxFontFamilyName::type_monospace:
			s += "monospace";
			break;
		}
	}
	return s;
}
