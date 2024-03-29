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

//#include <qwt_plot.h>


#include <vector>

#include "table.h"
#include "lib/json/single_include/nlohmann/json.hpp"

namespace insight {
namespace graphic {

using namespace std;


class ApplicationInterface {
 protected:
  data::Table * m_data;
   
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
  ApplicationInterface(data::Table * data) : m_data(data) {}
  
  virtual void update_after_data_load () {}
  
  virtual void update_cursor_position(double) {}
  virtual void update_view_limits(double, double) {}
  
  virtual void init()   = 0;
  virtual void apply_config(nlohmann::json *) = 0;
  virtual void reset () = 0;
    
  data::Table * get_data_table_ref() { return m_data; }
  
  string group() { return m_group_name; }
  void update_data_ref(data::Table * data) { m_data = data; }
  
//  int painter_coordx_from_axis_scale(double x) { return painter_coord_from_axis_scale(x, xBottom); }
//  int painter_coordy_from_axis_scale(double y) { return painter_coord_from_axis_scale(y, yLeft); }
//
//  double axis_coordx_from_painter_scale(int x) { return axes_coord_from_painter_scale(x, xBottom); }
//  double axis_coordy_from_painter_scale(int y) { return axes_coord_from_painter_scale(y, yLeft); }
};

}  // namespace graphic
}  // namespace insight
#endif // INSIGHT_GRAPHIC_BASE_H
