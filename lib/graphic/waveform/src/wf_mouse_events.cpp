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
#include "waveformdisplay.h"

#include <QMouseEvent>
#include <QWheelEvent>

namespace insight {
namespace graphic {

using namespace std;


void WaveformDisplay::mousePressEvent(QMouseEvent * event)
{
  double x_mouse = axis_coordx_from_painter_scale(event->x());
  m_mouse_state.x(x_mouse);
  
  double widest_xlim[2];
  xlim(widest_xlim);
  
  double xrange = widest_xlim[1] - widest_xlim[0];
    
  if (abs(m_mouse_state.x() - m_xpos_cursor) < (xrange * .05))
    m_mouse_state = CursorDrag;
    
  update_group_cursor_positions(m_mouse_state.x());
}

void WaveformDisplay::mouseMoveEvent(QMouseEvent * event)
{
    if (m_mouse_state == CursorDrag) {
      mousePressEvent(event); // update cursor positon
      
    } else if (m_mouse_state == Ready || m_mouse_state == Pan) {
      m_mouse_state = Pan;
      
      double x_mouse = axis_coordx_from_painter_scale(event->x());
      double delta_x = x_mouse - m_mouse_state.x();
      
      double x_lbound = axisScaleDiv(xBottom).lowerBound();
      double x_hbound = axisScaleDiv(xBottom).upperBound();
      
      x_lbound -= delta_x * .80;
      x_hbound -= delta_x * .80;
      
      update_group_view_limits(x_lbound, x_hbound);
      m_mouse_state.x(x_mouse);
    }
}

void WaveformDisplay::mouseDoubleClickEvent(QMouseEvent *) {
    cout << "reached: double-click" << endl;
}

void WaveformDisplay::mouseReleaseEvent(QMouseEvent *) {
  m_mouse_state = Ready;
}

void WaveformDisplay::wheelEvent(QWheelEvent * event)
{
  double scroll_speed_scalar = .001;
  double vertical_scroll_delta = event->angleDelta().y();
  
  double x_lbound = axisScaleDiv(xBottom).lowerBound();
  double x_hbound = axisScaleDiv(xBottom).upperBound();
  double xrange = x_hbound - x_lbound;
  
  double x_mouse = axis_coordx_from_painter_scale(event->position().x());
  double lhs_scaling = (x_mouse - x_lbound) / xrange;
  
  x_lbound -= scroll_speed_scalar * vertical_scroll_delta * xrange * lhs_scaling;
  x_hbound += scroll_speed_scalar * vertical_scroll_delta * xrange * (1. - lhs_scaling);
  update_group_view_limits(x_lbound, x_hbound);
}

}  // namespace graphic
}  // namespace insight
