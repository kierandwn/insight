// Copyright (c) 2020 Kieran Downie. All rights reserved.
//
// This file is part of insight.
//
// insight is free software : you can redistribute it and /
// or modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// insight is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with insight.  If not, see <https://www.gnu.org/licenses/>.
//
#ifndef INSIGHT_GRAPHIC_BASE_H
#define INSIGHT_GRAPHIC_BASE_H
#pragma once

#include <qwt_plot.h>
#include <QWidget>

#include <vector>

//#include "grid.h"
#include "table.h"
#include "lib/json/single_include/nlohmann/json.hpp"

namespace insight {
namespace graphic {

using namespace std;


class InsightGraphic
{
protected:
  data::Table * insight_data_ref_;
//  layout::Layout * insight_layout_ref_;
//  QWidget * p_plot_widget;

  InsightGraphic * first_in_group_ = nullptr;
  InsightGraphic * next_in_group_ = nullptr;
   
  string m_group_name = "";
  
// private:
//  int painter_coord_from_axis_scale(double d, int axes_id)
//  {
//    QwtScaleMap map = canvasMap(axes_id);
//    return map.transform(d);
//  }
//
//  double axes_coord_from_painter_scale(int d, int axes_id)
//  {
//    QwtScaleMap map = canvasMap(axes_id);
//    return map.invTransform(d);
//  }

public:
  InsightGraphic(data::Table * data) : insight_data_ref_(data) {}
  
  virtual void update_after_data_load () {}
  
  virtual void update_cursor_position(double) {}
  virtual void update_view_limits(double, double) {}
  
  virtual void init()   = 0;
  virtual void apply_config(nlohmann::json *) = 0;
  virtual void reset () = 0;

  void SetRefToNextInGroup(InsightGraphic * n)
  {
      next_in_group_ = n;
  }

  void SetRefToFirstInGroup(InsightGraphic * f)
  {
      first_in_group_ = f;
  }

  InsightGraphic * next_in_group()
  {
      return next_in_group_;
  }

  InsightGraphic * first_in_group()
  {
      return first_in_group_;
  }
    
  data::Table * get_data_table_ref() { return insight_data_ref_; }
  
  string group() { return m_group_name; }
  void update_data_ref(data::Table * data) { insight_data_ref_ = data; }
  
//  int painter_coordx_from_axis_scale(double x) { return painter_coord_from_axis_scale(x, xBottom); }
//  int painter_coordy_from_axis_scale(double y) { return painter_coord_from_axis_scale(y, yLeft); }
//
//  double axis_coordx_from_painter_scale(int x) { return axes_coord_from_painter_scale(x, xBottom); }
//  double axis_coordy_from_painter_scale(int y) { return axes_coord_from_painter_scale(y, yLeft); }

//  void update_group_cursor_positions(double tval)
//  {
//      if ("" == m_group_name) return;

//      map<string, InsightGraphic *>::iterator graphic_itr = insight_layout_ref_->first();
//      InsightGraphic * graphic_ptr;

//      graphic::InsightGraphic * current_graphic_in_group =

//      while (graphic_itr != insight_layout_ref_->last())
//      {
//        graphic_ptr = graphic_itr->second;
//        if (graphic_ptr->group() == m_group_name) graphic_ptr->update_cursor_position(tval);
//        ++graphic_itr;
//      }
//  }

//  void update_group_view_limits(double xmin, double xmax)
//  {
//      if ("" == m_group_name) return;

//      map<string, InsightGraphic *>::iterator graphic_itr = insight_layout_ref_->first();
//      InsightGraphic * graphic_ptr;

//      while (graphic_itr != insight_layout_ref_->last())
//      {
//        graphic_ptr = graphic_itr->second;

//        if (graphic_ptr->group() == m_group_name)
//          graphic_ptr->update_view_limits(xmin, xmax);

//        ++graphic_itr;
//      }
//  }

    void update_group_cursor_positions(double selected_value_indepvar)
    {
        graphic::InsightGraphic * current_graphic_in_group = first_in_group_;

        while (current_graphic_in_group)
        {
            current_graphic_in_group->update_cursor_position(selected_value_indepvar);
            current_graphic_in_group = current_graphic_in_group->next_in_group();
        }
    }

    void update_group_view_limits(double min_value_indepvar, double max_value_indepvar)
    {
        graphic::InsightGraphic * current_graphic_in_group = first_in_group_;

        while (current_graphic_in_group)
        {
            current_graphic_in_group->update_view_limits(min_value_indepvar, max_value_indepvar);
            current_graphic_in_group = current_graphic_in_group->next_in_group();
        }
    }
};

}  // namespace graphic
}  // namespace insight
#endif // INSIGHT_GRAPHIC_BASE_H
