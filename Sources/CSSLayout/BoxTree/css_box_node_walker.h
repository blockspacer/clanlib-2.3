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

#pragma once

class CL_CSSBoxNode;
class CL_CSSBoxElement;
class CL_CSSBoxText;
class CL_CSSBoxObject;

class CL_CSSBoxNodeWalker
{
public:
	CL_CSSBoxNodeWalker(CL_CSSBoxNode *node, bool allow_upwards_walking = false);

	bool is_node() const;
	bool is_element() const;
	bool is_text() const;
	bool is_object() const;

	CL_CSSBoxNode *get() const;
	CL_CSSBoxElement *get_element() const;
	CL_CSSBoxText *get_text() const;
	CL_CSSBoxObject *get_object() const;

	bool next(bool traverse_children = true);

private:
	CL_CSSBoxNode *cur;
	int level;
};
