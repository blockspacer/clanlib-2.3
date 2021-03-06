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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "font_metrics_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FontMetrics_Impl Construction:

CL_FontMetrics_Impl::CL_FontMetrics_Impl(
	float height,
	float ascent,
	float descent,
	float internal_leading,
	float external_leading,
	float average_character_width,
	float max_character_width,
	float weight,
	float overhang,
	float digitized_aspect_x,
	float digitized_aspect_y,		
	bool italic,
	bool underlined,
	bool struck_out,
	bool fixed_pitch
) :
	height(height),
	ascent(ascent),
	descent(descent),
	internal_leading(internal_leading),
	external_leading(external_leading),
	average_character_width(average_character_width),
	max_character_width(max_character_width),
	weight(weight),
	overhang(overhang),
	digitized_aspect_x(digitized_aspect_x),
	digitized_aspect_y(digitized_aspect_y),		
	italic(italic),
	underlined(underlined),
	struck_out(struck_out),
	fixed_pitch(fixed_pitch)
{
}

CL_FontMetrics_Impl::~CL_FontMetrics_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontMetrics_Impl Attributes:

CL_String::char_type CL_FontMetrics_Impl::get_first_char() const
{
	return CL_String::char_type('\0'); // todo
}

CL_String::char_type CL_FontMetrics_Impl::get_last_char() const
{
	return CL_String::char_type('\0'); // todo	
}

CL_String::char_type CL_FontMetrics_Impl::get_default_char() const
{
	return CL_String::char_type('?');
}

CL_String::char_type CL_FontMetrics_Impl::get_word_break_char() const
{
	return CL_String::char_type(' ');
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontMetrics_Impl Operations:


/////////////////////////////////////////////////////////////////////////////
// CL_FontMetrics_Impl Implementation:
