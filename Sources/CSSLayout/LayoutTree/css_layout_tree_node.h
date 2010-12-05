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

#pragma once

#include "css_used_values.h"
#include "css_layout_hit_test_result.h"

class CL_CSSBoxElement;
class CL_CSSBlockFormattingContext;
class CL_CSSStackingContext;
class CL_CSSResourceCache;
class CL_CSSLayoutCursor;
class CL_CSSLayoutHitTestResult;
class CL_CSSBoxMarginWidth;
class CL_CSSBoxPaddingWidth;

class CL_CSSLayoutTreeNode
{
public:
	CL_CSSLayoutTreeNode(CL_CSSBoxElement *element_node);
	virtual ~CL_CSSLayoutTreeNode();

	enum LayoutStrategy
	{
		normal_strategy,
		preferred_strategy,
		minimum_strategy
	};

	void prepare(CL_CSSBlockFormattingContext *current_formatting_context, CL_CSSStackingContext *current_stacking_context);
	virtual void render(CL_GraphicContext &gc, CL_CSSResourceCache *resources) = 0;
	virtual void set_component_geometry() = 0;
	virtual int get_first_line_baseline() = 0;
	virtual int get_last_line_baseline() = 0;
	virtual bool find_content_box(CL_CSSBoxElement *element, CL_Rect &out_rect) { return false; }

	void layout_formatting_root(CL_GraphicContext &gc, CL_CSSResourceCache *resources, LayoutStrategy strategy = normal_strategy);
	void set_root_block_position(int x, int y);
	virtual void layout_absolute_and_fixed_content(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_Rect containing_block, const CL_Size &viewport_size) { }
	void layout_normal(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);

	void calc_preferred(CL_GraphicContext &gc, CL_CSSResourceCache *resources);
	void calc_minimum(CL_GraphicContext &gc, CL_CSSResourceCache *resources);
	void set_expanding_width(CL_CSSUsedValue expanding_width);
	void layout_formatting_root_helper(CL_GraphicContext &gc, CL_CSSResourceCache *resources, LayoutStrategy strategy);
	void layout_absolute_or_fixed(CL_GraphicContext &gc, CL_CSSResourceCache *resources, const CL_Rect &containing_block, const CL_Size &viewport_size);

	virtual void calculate_top_down_sizes();
	virtual void calculate_content_top_down_sizes() { }
	virtual void set_content_expanding_width() { }
	bool add_margin_top(CL_CSSLayoutCursor &cursor);
	int get_block_width() const;
	int get_block_height() const;
	virtual bool is_replaced() const { return false; }
	virtual CL_CSSLayoutHitTestResult hit_test(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache, const CL_Point &pos) const { return CL_CSSLayoutHitTestResult(); }

	CL_CSSBoxElement *get_element_node() { return element_node; }
	CL_CSSStackingContext *get_stacking_context() { return stacking_context; }

	struct LTRB
	{
		LTRB() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CL_CSSUsedValue left, right, top, bottom;
	};

	LTRB margin;
	LTRB border;
	LTRB padding;
	CL_CSSUsedWidth width;
	CL_CSSUsedHeight height;
	CL_CSSUsedWidth containing_width;
	CL_CSSUsedHeight containing_height;
	CL_CSSUsedValue preferred_width;
	CL_CSSUsedValue min_width;
	bool preferred_width_calculated;
	bool min_width_calculated;

	CL_Rect content_box;

protected:
	virtual void prepare_children() = 0;
	virtual void layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy) = 0;
	virtual bool margin_collapses();
	virtual bool add_content_margin_top(CL_CSSLayoutCursor &cursor) { return false; }

	CL_CSSUsedValue get_css_margin_width(const CL_CSSBoxMarginWidth &margin_width, CL_CSSUsedWidth containing_width);
	CL_CSSUsedValue get_css_margin_height(const CL_CSSBoxMarginWidth &margin_width, CL_CSSUsedHeight containing_height);
	CL_CSSUsedValue get_css_padding_width(const CL_CSSBoxPaddingWidth &padding_width, CL_CSSUsedWidth containing_width);
	CL_CSSUsedValue get_css_padding_height(const CL_CSSBoxPaddingWidth &padding_width, CL_CSSUsedHeight containing_height);

	void render_non_content(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache);
	void render_border(CL_GraphicContext &gc);
	void render_background(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache);
	CL_Rect get_border_box() const;
	CL_Rect get_padding_box() const;

	CL_CSSBoxElement *element_node;
	CL_CSSBlockFormattingContext *formatting_context;
	bool formatting_context_root;
	CL_CSSStackingContext *stacking_context;
	bool stacking_context_root;

private:
	void set_formatting_context(CL_CSSBlockFormattingContext *formatting_context, bool is_root);
	void establish_stacking_context_if_needed(CL_CSSStackingContext *current_stacking_context);

	void layout_shrink_to_fit(CL_GraphicContext &gc, CL_CSSResourceCache *resources);

	void calculate_absolute_sizes();
	void calculate_static_sizes();
};
