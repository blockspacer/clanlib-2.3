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
#include "css_parser_font.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserFont::get_names()
{
	std::vector<CL_String> names;
	names.push_back("font");
	return names;
}

void CL_CSSParserFont::parse(CL_CSSBoxProperties &properties, const CL_String &propname, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set)
{
	CL_CSSBoxFontStyle style;
	CL_CSSBoxFontVariant variant;
	CL_CSSBoxFontWeight weight;
	CL_CSSBoxFontSize size;
	CL_CSSBoxLineHeight line_height;
	CL_CSSBoxFontFamily family;
	style.type = CL_CSSBoxFontStyle::type_normal;
	variant.type = CL_CSSBoxFontVariant::type_normal;
	weight.type = CL_CSSBoxFontWeight::type_normal;
	size.type = CL_CSSBoxFontSize::type_medium;
	line_height.type = CL_CSSBoxLineHeight::type_normal;
	family.type = CL_CSSBoxFontFamily::type_names;
	family.names.push_back(CL_CSSBoxFontFamilyName());

	bool font_style_set = false;
	bool font_variant_set = false;
	bool font_weight_set = false;
	int normal_count = 0;
	size_t pos = 0;
	CL_CSSToken token;
	while (pos < tokens.size())
	{
		token = next_token(pos, tokens);
		if (token.type == CL_CSSToken::type_ident)
		{
			if (tokens.size() == 1 &&
				(equals(token.value, "caption") ||
				equals(token.value, "icon") ||
				equals(token.value, "menu") ||
				equals(token.value, "message-box") ||
				equals(token.value, "small-caption") ||
				equals(token.value, "status-bar")))
			{
				properties.font_style = style;
				properties.font_variant = variant;
				properties.font_weight = weight;
				properties.font_size = size;
				properties.line_height = line_height;
				properties.font_family = family;
				return;
			}
			else if (equals(token.value, "inherit") && tokens.size() == 1)
			{
				properties.font_style.type = CL_CSSBoxFontStyle::type_inherit;
				properties.font_variant.type = CL_CSSBoxFontVariant::type_inherit;
				properties.font_weight.type = CL_CSSBoxFontWeight::type_inherit;
				properties.font_size.type = CL_CSSBoxFontSize::type_inherit;
				properties.line_height.type = CL_CSSBoxLineHeight::type_inherit;
				properties.font_family.type = CL_CSSBoxFontFamily::type_inherit;
				return;
			}
			else if (equals(token.value, "normal")) // font-style or font-weight or font-variant
			{
				int allowed = 3;
				if (font_style_set)
					allowed--;
				if (font_weight_set)
					allowed--;
				if (font_variant_set)
					allowed--;
				if (normal_count < allowed)
					normal_count++;
			}
			else if (equals(token.value, "italic") && !font_style_set) // font-style
			{
				font_style_set = true;
				style.type = CL_CSSBoxFontStyle::type_italic;
			}
			else if (equals(token.value, "oblique") && !font_style_set) // font-style
			{
				font_style_set = true;
				style.type = CL_CSSBoxFontStyle::type_oblique;
			}
			else if (equals(token.value, "small-caps") && !font_variant_set) // font-variant
			{
				font_style_set = true;
				variant.type = CL_CSSBoxFontVariant::type_small_caps;
			}
			else if (equals(token.value, "bold") && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_bold;
			}
			else if (equals(token.value, "bolder") && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_bolder;
			}
			else if (equals(token.value, "lighter") && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_lighter;
			}
			else if (token.value == "100" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_100;
			}
			else if (token.value == "200" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_200;
			}
			else if (token.value == "300" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_300;
			}
			else if (token.value == "400" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_400;
			}
			else if (token.value == "500" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_500;
			}
			else if (token.value == "600" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_600;
			}
			else if (token.value == "700" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_700;
			}
			else if (token.value == "800" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_800;
			}
			else if (token.value == "900" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight.type = CL_CSSBoxFontWeight::type_900;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (pos == tokens.size())
	{
		debug_parse_error(propname, tokens);
		return;
	}

	if (token.type == CL_CSSToken::type_ident)
	{
		if (equals(token.value, "xx-small"))
			size.type = CL_CSSBoxFontSize::type_xx_small;
		else if (equals(token.value, "x-small"))
			size.type = CL_CSSBoxFontSize::type_x_small;
		else if (equals(token.value, "small"))
			size.type = CL_CSSBoxFontSize::type_small;
		else if (equals(token.value, "medium"))
			size.type = CL_CSSBoxFontSize::type_medium;
		else if (equals(token.value, "large"))
			size.type = CL_CSSBoxFontSize::type_large;
		else if (equals(token.value, "x-large"))
			size.type = CL_CSSBoxFontSize::type_x_large;
		else if (equals(token.value, "xx-large"))
			size.type = CL_CSSBoxFontSize::type_xx_large;
		else if (equals(token.value, "smaller"))
			size.type = CL_CSSBoxFontSize::type_smaller;
		else if (equals(token.value, "larger"))
			size.type = CL_CSSBoxFontSize::type_larger;
		else if (equals(token.value, "inherit"))
			size.type = CL_CSSBoxFontSize::type_inherit;
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}
	}
	else if (is_length(token))
	{
		CL_CSSBoxLength length;
		if (parse_length(token, length))
		{
			size.type = CL_CSSBoxFontSize::type_length;
			size.length = length;
		}
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}
	}
	else if (token.type == CL_CSSToken::type_percentage)
	{
		size.type = CL_CSSBoxFontSize::type_percentage;
		size.percentage = CL_StringHelp::text_to_float(token.value);
	}
	else
	{
		debug_parse_error(propname, tokens);
		return;
	}

	token = next_token(pos, tokens);
	if (token.type == CL_CSSToken::type_delim && token.value == "/")
	{
		token = next_token(pos, tokens);

		if (token.type == CL_CSSToken::type_ident)
		{
			if (equals(token.value, "normal"))
				line_height.type = CL_CSSBoxLineHeight::type_normal;
			else if (equals(token.value, "inherit"))
				line_height.type = CL_CSSBoxLineHeight::type_inherit;
			else
			{
				debug_parse_error(propname, tokens);
				return;
			}
		}
		else if (token.type == CL_CSSToken::type_number)
		{
			line_height.type = CL_CSSBoxLineHeight::type_number;
			line_height.number = CL_StringHelp::text_to_float(token.value);
		}
		else if (is_length(token))
		{
			CL_CSSBoxLength length;
			if (parse_length(token, length))
			{
				line_height.type = CL_CSSBoxLineHeight::type_length;
				line_height.length = length;
			}
			else
			{
				debug_parse_error(propname, tokens);
				return;
			}
		}
		else if (token.type == CL_CSSToken::type_percentage)
		{
			line_height.type = CL_CSSBoxLineHeight::type_percentage;
			line_height.percentage = CL_StringHelp::text_to_float(token.value);
		}
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}

		token = next_token(pos, tokens);
	}

	family.names.clear();
	while (true)
	{
		if (token.type == CL_CSSToken::type_ident)
		{
			CL_CSSBoxFontFamilyName name;
			if (equals(token.value, "serif"))
			{
				name.type = CL_CSSBoxFontFamilyName::type_serif;
			}
			else if (equals(token.value, "sans-serif"))
			{
				name.type = CL_CSSBoxFontFamilyName::type_sans_serif;
			}
			else if (equals(token.value, "cursive"))
			{
				name.type = CL_CSSBoxFontFamilyName::type_cursive;
			}
			else if (equals(token.value, "fantasy"))
			{
				name.type = CL_CSSBoxFontFamilyName::type_fantasy;
			}
			else if (equals(token.value, "monospace"))
			{
				name.type = CL_CSSBoxFontFamilyName::type_monospace;
			}
			else if (equals(token.value, "default"))
			{
				// reserved for future use
				return;
			}
			else if (equals(token.value, "initial"))
			{
				// reserved for future use
				return;
			}
			else
			{
				name.type = CL_CSSBoxFontFamilyName::type_family_name;
			}

			if (name.type == CL_CSSBoxFontFamilyName::type_family_name)
			{
				name.name = token.value;
				while (pos != tokens.size())
				{
					token = tokens[pos++];
					if (token.type == CL_CSSToken::type_whitespace)
					{
						name.name += " ";
					}
					else if (token.type == CL_CSSToken::type_ident)
					{
						name.name += token.value;
					}
					else if (token.type == CL_CSSToken::type_delim && token.value == ",")
					{
						break;
					}
				}
				family.names.push_back(name);
				if (pos == tokens.size())
					break;
				token = next_token(pos, tokens);
			}
			else
			{
				family.names.push_back(name);

				if (pos == tokens.size())
					break;
				token = next_token(pos, tokens);
				if (token.type != CL_CSSToken::type_delim || token.value != ",")
				{
					debug_parse_error(propname, tokens);
					return;
				}
				token = next_token(pos, tokens);
			}
		}
		else if (token.type == CL_CSSToken::type_string)
		{
			CL_CSSBoxFontFamilyName name;
			name.type = CL_CSSBoxFontFamilyName::type_family_name;
			name.name = token.value;
			family.names.push_back(name);

			if (pos == tokens.size())
				break;
			token = next_token(pos, tokens);
			if (token.type != CL_CSSToken::type_delim || token.value != ",")
			{
				debug_parse_error(propname, tokens);
				return;
			}
			token = next_token(pos, tokens);
		}
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}
	}

	properties.font_style = style;
	properties.font_variant = variant;
	properties.font_weight = weight;
	properties.font_size = size;
	properties.line_height = line_height;
	properties.font_family = family;

	if (out_change_set)
	{
		(*out_change_set)["font-style"] = &properties.font_style;
		(*out_change_set)["font-variant"] = &properties.font_variant;
		(*out_change_set)["font-weight"] = &properties.font_weight;
		(*out_change_set)["font-size"] = &properties.font_size;
		(*out_change_set)["line-height"] = &properties.line_height;
		(*out_change_set)["font-family"] = &properties.font_family;
	}
}
