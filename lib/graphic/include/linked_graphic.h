// Copyright (c) 2020 Kieran Downie. All rights reserved.
//
// This file is part of insight.
//
// attitude is free software : you can redistribute it and /
// or modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// attitude is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with insight.  If not, see <https://www.gnu.org/licenses/>.
//
#ifndef INSIGHT_LINKED_GRAPHIC_H_
#define INSIGHT_LINKED_GRAPHIC_H_
#pragma once

#include "insight_graphic_base.h"

#include "grid.h"
#include "table.h"

namespace insight {
namespace graphic {


class LinkedPlot : public Base
{
//  Q_OBJECT
 protected:
    layout::Layout * p_layout;
    QWidget * p_plot_widget;
 
 public:
    LinkedPlot(data::Table * data, layout::Layout * layout)
      : Base(data),
        p_layout(layout) {}

    void update_group_cursor_positions(double tval)
    {
        if ("" == m_group_name) return;

        map<string, graphic::Base *>::iterator graphic_itr = p_layout->first();
        graphic::Base * graphic_ptr;

        while (graphic_itr != p_layout->last())
        {
          graphic_ptr = graphic_itr->second;
          if (graphic_ptr->group() == m_group_name) graphic_ptr->update_cursor_position(tval);
          ++graphic_itr;
        }
    }

    void update_group_view_limits(double xmin, double xmax)
    {
        if ("" == m_group_name) return;

        map<string, graphic::Base *>::iterator graphic_itr = p_layout->first();
        graphic::Base * graphic_ptr;

        while (graphic_itr != p_layout->last())
        {
          graphic_ptr = graphic_itr->second;
          
          if (graphic_ptr->group() == m_group_name)
            graphic_ptr->update_view_limits(xmin, xmax);
          
          ++graphic_itr;
        }
    }
};

}  // namespace graphic
}  // namespace insight
#endif  // INSIGHT_LINKED_GRAPHIC_H_
