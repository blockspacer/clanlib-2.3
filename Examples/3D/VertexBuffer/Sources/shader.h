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
**    Mark Page
*/

#pragma once

class Shader
{
public:
	Shader(CL_GraphicContext &gc);

	void Set(CL_GraphicContext &gc);
	void Use(CL_GraphicContext &gc);
public:
	float material_shininess;
	CL_Vec4f material_emission;
	//CL_Vec4f material_ambient;
	CL_Vec4f material_specular;

	CL_Vec4f light_position;
	CL_Vec4f light_specular;
	CL_Vec4f light_diffuse;

private:
	static char vertex[];
	static char fragment[];
	CL_ProgramObject program_object;
};
