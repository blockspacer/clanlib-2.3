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
*/

#include "CSSLayout/precomp.h"
#include "css_layout_tree_node.h"
#include "css_block_formatting_context.h"
#include "css_stacking_context.h"
#include "css_layout_cursor.h"
#include "../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"
#include "clan_image_stretch.h"

CL_CSSLayoutTreeNode::CL_CSSLayoutTreeNode(CL_CSSBoxElement *element_node)
: preferred_width(0.0f), min_width(0.0f), preferred_width_calculated(false), min_width_calculated(false),
  element_node(element_node), formatting_context(0), formatting_context_root(false), stacking_context(0), stacking_context_root(false)
{
}

CL_CSSLayoutTreeNode::~CL_CSSLayoutTreeNode()
{
	if (formatting_context_root)
		delete formatting_context;
//	if (stacking_context_root)
//		delete stacking_context;
}

void CL_CSSLayoutTreeNode::prepare(CL_CSSBlockFormattingContext *current_formatting_context, CL_CSSStackingContext *current_stacking_context)
{
	if (current_formatting_context == 0 || element_node->is_inline_block_level() || element_node->is_float() || element_node->is_table() || element_node->is_table_cell() || is_replaced() || element_node->is_absolute())
		set_formatting_context(new CL_CSSBlockFormattingContext(current_formatting_context), true);
	else if (element_node->is_fixed())
		set_formatting_context(new CL_CSSBlockFormattingContext(0), true);
	else
		set_formatting_context(current_formatting_context, false);
	establish_stacking_context_if_needed(current_stacking_context);
	prepare_children();
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_css_margin_width(const CL_CSSBoxMarginWidth &margin_width, CL_CSSUsedWidth containing_width)
{
	switch (margin_width.type)
	{
	case CL_CSSBoxMarginWidth::type_auto: return 0.0f;
	case CL_CSSBoxMarginWidth::type_length: return margin_width.length.value;
	case CL_CSSBoxMarginWidth::type_percentage: return margin_width.percentage * containing_width.value / 100.0f;
	default: return 0.0f;
	}
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_css_padding_width(const CL_CSSBoxPaddingWidth &padding_width, CL_CSSUsedWidth containing_width)
{
	switch (padding_width.type)
	{
	case CL_CSSBoxPaddingWidth::type_length: return padding_width.length.value;
	case CL_CSSBoxPaddingWidth::type_percentage: return padding_width.percentage * containing_width.value / 100.0f;
	default: return 0.0f;
	}
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_css_margin_height(const CL_CSSBoxMarginWidth &margin_width, CL_CSSUsedHeight containing_height)
{
	switch (margin_width.type)
	{
	case CL_CSSBoxMarginWidth::type_auto: return 0.0f;
	case CL_CSSBoxMarginWidth::type_length: return margin_width.length.value;
	case CL_CSSBoxMarginWidth::type_percentage: return margin_width.percentage * containing_height.value / 100.0f;
	default: return 0.0f;
	}
}

CL_CSSUsedValue CL_CSSLayoutTreeNode::get_css_padding_height(const CL_CSSBoxPaddingWidth &padding_width, CL_CSSUsedHeight containing_height)
{
	switch (padding_width.type)
	{
	case CL_CSSBoxPaddingWidth::type_length: return padding_width.length.value;
	case CL_CSSBoxPaddingWidth::type_percentage: return padding_width.percentage * containing_height.value / 100.0f;
	default: return 0.0f;
	}
}

void CL_CSSLayoutTreeNode::calculate_top_down_sizes()
{
	bool is_table_cell = element_node->computed_properties.display.type == CL_CSSBoxDisplay::type_table_cell;
	bool is_float = element_node->computed_properties.float_box.type != CL_CSSBoxFloat::type_none;

	margin.left = get_css_margin_width(element_node->computed_properties.margin_width_left, containing_width);
	margin.right = get_css_margin_width(element_node->computed_properties.margin_width_right, containing_width);
	border.left = element_node->computed_properties.border_width_left.length.value;
	border.right = element_node->computed_properties.border_width_right.length.value;
	padding.left = get_css_padding_width(element_node->computed_properties.padding_width_left, containing_width);
	padding.right = get_css_padding_width(element_node->computed_properties.padding_width_right, containing_width);

	if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_length)
	{
		width.value = element_node->computed_properties.width.length.value;
		width.expanding = false;
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_percentage)
	{
		if (containing_width.expanding)
		{
			width.value = 0.0f;
			width.expanding = true;
		}
		else
		{
			width.value = element_node->computed_properties.width.percentage * containing_width.value / 100.0f;
			width.expanding = false;
		}
	}
	else if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
	{
		if (containing_width.expanding || is_table_cell || is_float)
		{
			width.value = 0.0f;
			width.expanding = true;
		}
		else
		{
			width.value = cl_max(0.0f, containing_width.value - margin.left - margin.right - border.left - border.right - padding.left - padding.right);
			width.expanding = false;
		}
	}
	else
	{
		// Unreachable code (unless something is seriously wrong elsewhere).
		width.value = 0.0f;
		width.expanding = false;
	}

	if (!width.expanding)
	{
		if (element_node->computed_properties.max_width.type == CL_CSSBoxMaxWidth::type_length)
			width.value = cl_min(width.value, element_node->computed_properties.max_width.length.value);
		else if (element_node->computed_properties.max_width.type == CL_CSSBoxMaxWidth::type_percentage && !containing_width.expanding)
			width.value = cl_min(width.value, element_node->computed_properties.max_width.percentage * containing_width.value / 100.0f);

		if (element_node->computed_properties.min_width.type == CL_CSSBoxMinWidth::type_length)
			width.value = cl_max(width.value, element_node->computed_properties.min_width.length.value);
		else if (element_node->computed_properties.min_width.type == CL_CSSBoxMinWidth::type_percentage && !containing_width.expanding)
			width.value = cl_max(width.value, element_node->computed_properties.min_width.percentage * containing_width.value / 100.0f);

		if (!containing_width.expanding && !is_float)
		{
			if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto && element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.left = cl_max(0.0f, (containing_width.value-border.left-border.right-padding.left-padding.right-width.value)/2.0f);
				margin.right = cl_max(0.0f, containing_width.value-border.left-border.right-padding.left-padding.right-width.value-margin.left);
			}
			else if (element_node->computed_properties.margin_width_left.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.left = cl_max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width.value);
			}
			else if (element_node->computed_properties.margin_width_right.type == CL_CSSBoxMarginWidth::type_auto)
			{
				margin.right = cl_max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width.value);
			}

			if (margin.left + border.left + width.value + border.right + padding.right + margin.right > containing_width.value)
			{
				if (element_node->computed_properties.direction.type == CL_CSSBoxDirection::type_ltr)
					margin.right = cl_max(0.0f, containing_width.value-margin.left-border.left-border.right-padding.left-padding.right-width.value);
				else
					margin.left = cl_max(0.0f, containing_width.value-margin.right-border.left-border.right-padding.left-padding.right-width.value);
			}
		}
	}

	margin.top = get_css_margin_height(element_node->computed_properties.margin_width_top, containing_height);
	margin.bottom = get_css_margin_height(element_node->computed_properties.margin_width_bottom, containing_height);
	border.top = element_node->computed_properties.border_width_top.length.value;
	border.bottom = element_node->computed_properties.border_width_bottom.length.value;
	padding.top = get_css_padding_height(element_node->computed_properties.padding_width_top, containing_height);
	padding.bottom = get_css_padding_height(element_node->computed_properties.padding_width_bottom, containing_height);

	if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_length)
	{
		height.value = element_node->computed_properties.height.length.value;
		height.use_content = false;
	}
	else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_percentage)
	{
		if (containing_height.use_content)
		{
			height.value = 0.0f;
			height.use_content = true;
		}
		else
		{
			height.value = containing_height.value * element_node->computed_properties.height.percentage / 100.0f;
			height.use_content = false;
		}
	}
	else if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto)
	{
		height.value = 0.0f;
		height.use_content = true;
	}
	else
	{
		// Unreachable code (unless something is seriously wrong elsewhere).
		height.value = 0.0f;
		height.use_content = false;
	}

	if (!height.use_content)
	{
		if (element_node->computed_properties.max_height.type == CL_CSSBoxMaxHeight::type_length)
		{
			height.value = cl_min(height.value, element_node->computed_properties.max_height.length.value);
		}
		else if (element_node->computed_properties.max_height.type == CL_CSSBoxMaxHeight::type_percentage && !containing_height.use_content)
		{
			height.value = cl_min(height.value, element_node->computed_properties.max_height.percentage * containing_height.value / 100.0f);
		}

		if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_length)
		{
			height.value = cl_max(height.value, element_node->computed_properties.min_height.length.value);
		}
		else if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_percentage && !containing_height.use_content)
		{
			height.value = cl_max(height.value, element_node->computed_properties.min_height.percentage * containing_height.value / 100.0f);
		}
	}

	calculate_content_top_down_sizes();
}

void CL_CSSLayoutTreeNode::set_root_block_position(int x, int y)
{
	if (!formatting_context_root)
		throw CL_Exception("CL_CSSLayoutTreeNode::set_root_block_position misuse");
	content_box = CL_Rect(CL_Point(x+margin.left+border.left+padding.left, y+margin.top+border.top+padding.top), content_box.get_size());
	formatting_context->set_position(content_box.left, content_box.top);
}

void CL_CSSLayoutTreeNode::calc_preferred(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	if (!preferred_width_calculated)
	{
		calculate_top_down_sizes();
		layout_formatting_root_helper(gc, resources, preferred_strategy);
	}
}

void CL_CSSLayoutTreeNode::calc_minimum(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	if (!min_width_calculated)
	{
		calculate_top_down_sizes();
		layout_formatting_root_helper(gc, resources, minimum_strategy);
	}
}

void CL_CSSLayoutTreeNode::layout_minimum(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	calc_minimum(gc, resources);
	calculate_top_down_sizes();
	set_expanding_width(min_width);
	layout_formatting_root_helper(gc, resources, normal_strategy);
}

void CL_CSSLayoutTreeNode::layout_preferred(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	calc_preferred(gc, resources);
	calculate_top_down_sizes();
	set_expanding_width(preferred_width);
	layout_formatting_root_helper(gc, resources, normal_strategy);
}

void CL_CSSLayoutTreeNode::layout_shrink_to_fit(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	calc_preferred(gc, resources);
	CL_CSSUsedValue available_width = containing_width.value;
	CL_CSSUsedValue shrink_to_fit_width;
	if (preferred_width > available_width + 0.1f)
	{
		calc_minimum(gc, resources);
		shrink_to_fit_width = cl_max(min_width, available_width);
	}
	else
	{
		shrink_to_fit_width = preferred_width;
	}
	calculate_top_down_sizes();
	set_expanding_width(shrink_to_fit_width);
	layout_formatting_root_helper(gc, resources, normal_strategy);
}

void CL_CSSLayoutTreeNode::layout_formatting_root(CL_GraphicContext &gc, CL_CSSResourceCache *resources, LayoutStrategy strategy)
{
	if (strategy == normal_strategy && element_node->is_shrink_to_fit())
	{
		layout_shrink_to_fit(gc, resources);
	}
	else
	{
		calculate_top_down_sizes();
		layout_formatting_root_helper(gc, resources, strategy);
	}

/*	if (element_node->computed_properties.background_color.type == CL_CSSBoxBackgroundColor::type_color &&
		element_node->computed_properties.background_color.color == CL_Colorf::green)
	{
		cl_debug_breakpoint = 0xdeadbabe;
	}*/
}

void CL_CSSLayoutTreeNode::set_expanding_width(CL_CSSUsedValue expanding_width)
{
	if (width.expanding)
	{
		width.value = expanding_width;
		set_content_expanding_width();
	}
}

void CL_CSSLayoutTreeNode::layout_formatting_root_helper(CL_GraphicContext &gc, CL_CSSResourceCache *resources, LayoutStrategy strategy)
{
	formatting_context->clear();

	CL_CSSLayoutCursor cursor;
	cursor.x = 0;
	cursor.y = 0;
	cursor.resources = resources;
	add_content_margin_top(cursor);
	layout_content(gc, cursor, strategy);
	cursor.apply_margin();

	if (strategy == preferred_strategy)
	{
		if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
			preferred_width = cursor.max_written_width;
		else
			preferred_width = width.value;
		preferred_width_calculated = true;
	}
	else if (strategy == minimum_strategy)
	{
		if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
			min_width = cursor.max_written_width;
		else
			min_width = width.value;
		min_width_calculated = true;
	}

	if (element_node->computed_properties.height.type == CL_CSSBoxHeight::type_auto)
	{
		height.value = cursor.y;
		CL_CSSUsedValue left_float_height = formatting_context->find_left_clearance();
		CL_CSSUsedValue right_float_height = formatting_context->find_right_clearance();
		height.value = cl_max(height.value, cl_max(left_float_height, right_float_height));
	}

	content_box = CL_Size(width.value, height.value);
}

void CL_CSSLayoutTreeNode::layout_normal(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	float old_x = cursor.x;

	cursor.x += margin.left + border.left + padding.left;
	add_margin_top(cursor);

	// Note: this may have to be added to add_margin_top
	if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_length)
		cursor.apply_margin();
	else if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_percentage && !containing_height.use_content)
		cursor.apply_margin();

	if (border.top > 0 || padding.top > 0)
		cursor.apply_margin();

	cursor.y += border.top+padding.top;

	if (element_node->computed_properties.clear.type == CL_CSSBoxClear::type_left || element_node->computed_properties.clear.type == CL_CSSBoxClear::type_both)
	{
		int clear_left = formatting_context->find_left_clearance();
		if (cursor.y+cursor.margin_y < clear_left)
			cursor.y = clear_left-cursor.margin_y;
	}
	if (element_node->computed_properties.clear.type == CL_CSSBoxClear::type_right || element_node->computed_properties.clear.type == CL_CSSBoxClear::type_both)
	{
		int clear_right = formatting_context->find_right_clearance();
		if (cursor.y+cursor.margin_y < clear_right)
			cursor.y = clear_right-cursor.margin_y;
	}

	content_box.left = cursor.x;
	content_box.top = cursor.y+cursor.margin_y;
	content_box.right = content_box.left+width.value;
	content_box.bottom = content_box.top+height.value;
	cursor.apply_written_width(content_box.right);

	layout_content(gc, cursor, strategy);

	if (border.bottom > 0 || padding.bottom > 0)
		cursor.apply_margin();

	if (height.use_content)
	{
		height.value = cl_max(0.0f, cursor.y - content_box.top);

		if (element_node->computed_properties.max_height.type == CL_CSSBoxMaxHeight::type_length)
		{
			height.value = cl_min(height.value, element_node->computed_properties.max_height.length.value);
		}
		else if (element_node->computed_properties.max_height.type == CL_CSSBoxMaxHeight::type_percentage && !containing_height.use_content)
		{
			height.value = cl_min(height.value, element_node->computed_properties.max_height.percentage * containing_height.value / 100.0f);
		}

		if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_length)
		{
			height.value = cl_max(height.value, element_node->computed_properties.min_height.length.value);
		}
		else if (element_node->computed_properties.min_height.type == CL_CSSBoxMinHeight::type_percentage && !containing_height.use_content)
		{
			height.value = cl_max(height.value, element_node->computed_properties.min_height.percentage * containing_height.value / 100.0f);
		}

		content_box.bottom = content_box.top+height.value; // content_box.bottom = cursor.y;
		cursor.y = content_box.top + height.value;
	}
	else
	{
		float inner_margin_y = cursor.y+cursor.margin_y;
		cursor.y = content_box.top + height.value;
		cursor.margin_y = cl_max(0.0f, inner_margin_y-cursor.y);
	}

	cursor.y += border.bottom+padding.bottom;
	cursor.add_margin(margin.bottom);

	cursor.x = old_x;
}

bool CL_CSSLayoutTreeNode::add_margin_top(CL_CSSLayoutCursor &cursor)
{
	if (element_node->computed_properties.float_box.type != CL_CSSBoxFloat::type_none ||
		element_node->computed_properties.position.type == CL_CSSBoxPosition::type_absolute ||
		element_node->computed_properties.position.type == CL_CSSBoxPosition::type_fixed)
	{
		return false;
	}
	else
	{
		cursor.add_margin(margin.top);
		if (margin_collapses())
		{
			if (add_content_margin_top(cursor))
				return true;

			if (border.bottom != 0 || padding.bottom != 0)
				return true;

			cursor.add_margin(margin.bottom);
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool CL_CSSLayoutTreeNode::margin_collapses()
{
	return border.top == 0 && padding.top == 0 && height.value == 0 && !height.use_content;
}

void CL_CSSLayoutTreeNode::set_formatting_context(CL_CSSBlockFormattingContext *new_formatting_context, bool is_root)
{
	if (formatting_context_root)
		delete formatting_context;
	formatting_context = new_formatting_context;
	formatting_context_root = is_root;
}

void CL_CSSLayoutTreeNode::establish_stacking_context_if_needed(CL_CSSStackingContext *current_stacking_context)
{
	if (element_node->computed_properties.position.type != CL_CSSBoxPosition::type_static && element_node->computed_properties.z_index.type != CL_CSSBoxZIndex::type_auto)
	{
		stacking_context = new CL_CSSStackingContext(this);
		stacking_context_root = true;
		stacking_context->push_back(current_stacking_context);
	}
	else
	{
		stacking_context = current_stacking_context;
		stacking_context_root = false;
	}
}

int CL_CSSLayoutTreeNode::get_block_width() const
{
	return cl_used_to_actual(margin.left + border.left + padding.left + width.value + padding.right + border.right + margin.right);
}

int CL_CSSLayoutTreeNode::get_block_height() const
{
	return cl_used_to_actual(margin.top + border.top + padding.top + height.value + padding.bottom + border.bottom + margin.bottom);
}

void CL_CSSLayoutTreeNode::render_non_content(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache)
{
	if (element_node->computed_properties.visibility.type == CL_CSSBoxVisibility::type_visible &&
		element_node->computed_properties.display.type != CL_CSSBoxDisplay::type_table_cell)
	{
		render_background(gc, resource_cache);
		render_border(gc);

		if (element_node->computed_properties.display.type == CL_CSSBoxDisplay::type_list_item &&
			element_node->computed_properties.list_style_type.type != CL_CSSBoxListStyleType::type_none)
		{
			CL_String bullet = CL_StringHelp::wchar_to_utf8(8226);
			CL_Font &font = resource_cache->get_font(gc, element_node->computed_properties);
			CL_Size offset = font.get_text_size(gc, bullet);
			offset.width += 8;
			// to do: find baseline of first item
			font.draw_text(gc, formatting_context->get_x() + content_box.left-offset.width, formatting_context->get_y() + content_box.top + (int)(font.get_font_metrics(gc).get_ascent()), bullet, element_node->computed_properties.color.color);
		}
	}
}

CL_Rect CL_CSSLayoutTreeNode::get_border_box() const
{
	CL_Rect border_rect = get_padding_box();
	border_rect.expand(border.left, border.top, border.right, border.bottom);
	return border_rect;
}

CL_Rect CL_CSSLayoutTreeNode::get_padding_box() const
{
	CL_Rect padding_rect = content_box;
	if (!formatting_context_root)
		padding_rect.translate(formatting_context->get_x(), formatting_context->get_y());
	else if (formatting_context->get_parent())
		padding_rect.translate(formatting_context->get_parent()->get_x(), formatting_context->get_parent()->get_y());
	padding_rect.expand(padding.left, padding.top, padding.right, padding.bottom);
	return padding_rect;
}

void CL_CSSLayoutTreeNode::render_background(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache)
{
	CL_Rect padding_box = get_padding_box();
	if (element_node->computed_properties.background_color.type == CL_CSSBoxBackgroundColor::type_color)
		CL_Draw::fill(gc, padding_box, element_node->computed_properties.background_color.color);

	if (element_node->computed_properties.background_image.type == CL_CSSBoxBackgroundImage::type_uri)
	{
		CL_Image &image = resource_cache->get_image(gc, element_node->computed_properties.background_image.url);
		if (!image.is_null())
		{
			int x = padding_box.left;
			switch (element_node->computed_properties.background_position.type_x)
			{
			case CL_CSSBoxBackgroundPosition::type1_left:
				x = padding_box.left;
				break;
			case CL_CSSBoxBackgroundPosition::type1_center:
				x = padding_box.left + (padding_box.get_width() - image.get_width()) / 2;
				break;
			case CL_CSSBoxBackgroundPosition::type1_right:
				x = padding_box.right - image.get_width();
				break;
			case CL_CSSBoxBackgroundPosition::type1_percentage:
				x = cl_used_to_actual(padding_box.left + (padding_box.get_width()-image.get_width()) * element_node->computed_properties.background_position.percentage_x / 100.0f);
				break;
			case CL_CSSBoxBackgroundPosition::type1_length:
				x = cl_used_to_actual(padding_box.left + element_node->computed_properties.background_position.length_x.value);
				break;
			}

			int y = padding_box.top;
			switch (element_node->computed_properties.background_position.type_y)
			{
			case CL_CSSBoxBackgroundPosition::type2_top:
				y = padding_box.top;
				break;
			case CL_CSSBoxBackgroundPosition::type2_center:
				y = padding_box.top + (padding_box.get_height() - image.get_height()) / 2;
				break;
			case CL_CSSBoxBackgroundPosition::type2_bottom:
				y = padding_box.bottom - image.get_height();
				break;
			case CL_CSSBoxBackgroundPosition::type2_percentage:
				y = cl_used_to_actual(padding_box.top + (padding_box.get_height()-image.get_height()) * element_node->computed_properties.background_position.percentage_y / 100.0f);
				break;
			case CL_CSSBoxBackgroundPosition::type1_length:
				y = cl_used_to_actual(padding_box.top + element_node->computed_properties.background_position.length_y.value);
				break;
			}

			if (element_node->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_no_repeat)
			{
				gc.push_cliprect(padding_box);
				image.draw(gc, x, y);
				gc.pop_cliprect();
			}
			else if (element_node->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_repeat_x)
			{
				gc.push_cliprect(padding_box);
				int start_x = (x-padding_box.left)%image.get_width();
				if (start_x > 0)
					start_x -= image.get_width();
				for (x = start_x; x < padding_box.right; x += image.get_width())
					image.draw(gc, x, y);
				gc.pop_cliprect();
			}
			else if (element_node->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_repeat_y)
			{
				gc.push_cliprect(padding_box);
				int start_y = (y-padding_box.top)%image.get_height();
				if (start_y > 0)
					start_y -= image.get_height();
				for (y = start_y; y < padding_box.bottom; y += image.get_height())
					image.draw(gc, x, y);
				gc.pop_cliprect();
			}
			else if (element_node->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_repeat)
			{
				gc.push_cliprect(padding_box);
				int start_x = (x-padding_box.left)%image.get_width();
				if (start_x > 0)
					start_x -= image.get_width();
				int start_y = (y-padding_box.top)%image.get_height();
				if (start_y > 0)
					start_y -= image.get_height();

				for (y = start_y; y < padding_box.bottom; y += image.get_height())
					for (x = start_x; x < padding_box.right; x += image.get_width())
						image.draw(gc, x, y);
				gc.pop_cliprect();
			}
		}
	}
}

void CL_CSSLayoutTreeNode::render_border(CL_GraphicContext &gc)
{
	CL_Rect border_box = get_border_box();
	if (element_node->computed_properties.border_style_top.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)border_box.top,
			(float)border_box.right,
			(float)(border_box.top+border.top),
			element_node->computed_properties.border_color_top.color);
	}
	if (element_node->computed_properties.border_style_bottom.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)(border_box.bottom-border.bottom),
			(float)border_box.right,
			(float)border_box.bottom,
			element_node->computed_properties.border_color_bottom.color);
	}
	if (element_node->computed_properties.border_style_left.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)border_box.left,
			(float)(border_box.top+border.top),
			(float)(border_box.left+border.left),
			(float)(border_box.bottom-border.bottom),
			element_node->computed_properties.border_color_left.color);
	}
	if (element_node->computed_properties.border_style_right.type == CL_CSSBoxBorderStyle::type_solid)
	{
		CL_Draw::fill(
			gc,
			(float)(border_box.right-border.right),
			(float)(border_box.top+border.top),
			(float)border_box.right,
			(float)(border_box.bottom-border.bottom),
			element_node->computed_properties.border_color_right.color);
	}
}
