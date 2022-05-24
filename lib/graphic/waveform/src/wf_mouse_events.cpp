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
#include "waveformdisplay.h"

#include <QMouseEvent>
#include <QWheelEvent>

#include <algorithm>
#include <iterator>

namespace insight {
namespace graphic {

using namespace std;


void WaveformDisplay::mousePressEvent(QMouseEvent * event)
{
  if (event->buttons() == Qt::LeftButton)
  {
    if (m_mouse_state == Ready || m_mouse_state == CursorDrag)
    {
      double x_mouse = axis_coordx_from_painter_scale(event->x());
      m_mouse_state.x(x_mouse);
      
      double widest_xlim[2];
      xlim(widest_xlim);
      
      double xrange = widest_xlim[1] - widest_xlim[0];
        
      if (abs(x_mouse - m_xpos_cursor) < (xrange * .05))
        m_mouse_state = CursorDrag;
        
      update_group_cursor_positions(x_denormalised(m_mouse_state.x()));
    }
    else
    if (m_mouse_state == DragAndDropReconfigureReady)
    {
      double y_mouse = axis_coordy_from_painter_scale(event->y());
      m_mouse_state.y(y_mouse); // only drag plot if it's within mouse is close within specified threshold.
      
      m_mouse_state = DragAndDropReconfigure;
    }
//    else
//    if (m_mouse_state == DragAndDropReconfigure)
//    {
//      double y_mouse = axis_coordy_from_painter_scale(event->y());
//      m_mouse_state.y(y_mouse);
//
//      m_mouse_state == DragAndDropReconfigure;
//    }
  }
  else
  if (event->buttons() == Qt::MidButton)
  {
    if (m_mouse_state == Ready)
      m_mouse_state = DragAndDropReconfigure;
    
    else
    if (m_mouse_state == DragAndDropReconfigure)
      m_mouse_state = Ready;
  }
}

void WaveformDisplay::mouseMoveEvent(QMouseEvent * event)
{
  if (m_mouse_state == CursorDrag)
  {
    mousePressEvent(event); // update cursor positon
  }
  else
  if (m_mouse_state == Ready || m_mouse_state == Pan)
  {
    m_mouse_state = Pan;
    
    double x_mouse = axis_coordx_from_painter_scale(event->x());
    double delta_x = x_mouse - m_mouse_state.x();
    
    double x_lbound = axisScaleDiv(xBottom).lowerBound();
    double x_hbound = axisScaleDiv(xBottom).upperBound();
    
    x_lbound -= delta_x * .80;
    x_hbound -= delta_x * .80;
    
    update_group_view_limits(x_denormalised(x_lbound), x_denormalised(x_hbound));
    m_mouse_state.x(x_mouse);
  }
}

void WaveformDisplay::mouseDoubleClickEvent(QMouseEvent *)
{
    cout << "reached: double-click" << endl;
}

void WaveformDisplay::mouseReleaseEvent(QMouseEvent *)
{
  if (m_mouse_state == DragAndDropReconfigure)
    m_mouse_state = DragAndDropReconfigureReady;
  
  else
    m_mouse_state = Ready;
}

void WaveformDisplay::wheelEvent(QWheelEvent * event)
{
  if (m_mouse_state == Ready)
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
    
//    double max_xbounds_in_data[2];
//    calculate_maximum_xdomain(max_xbounds_in_data);
    
//    double new_lower_limit = max({x_lbound, max_xbounds_in_data[0]});
//    double new_upper_limit = max({x_hbound, max_xbounds_in_data[1]});
    
    double new_lower_limit = max({x_lbound, m_max_xbounds[0]});
    double new_upper_limit = min({x_hbound, m_max_xbounds[1]});
    
    update_group_view_limits(new_lower_limit, new_upper_limit);
  }
  else
  if (m_mouse_state == DragAndDropReconfigureReady)
  {
    double scroll_speed_scalar = .001;
    double vertical_scroll_delta = event->angleDelta().y();
    
    double y_mouse = axis_coordy_from_painter_scale(event->y());
//    m_mouse_state.y(y_mouse);
    
    double * delta_ys = new double[m_waveform_groups.size()];
    
    for (int i = 0; i < m_waveform_groups.size(); ++i)
      delta_ys[i] = abs(m_waveform_groups[i]->get_normalised_plot_position() - y_mouse);
    
    
    int closest_waveform_index = 0;
    double min_delta = delta_ys[0];
    
    for (int i = 1; i < m_waveform_groups.size(); ++i)
    {
      if (delta_ys[i] < min_delta)
      {
        min_delta = delta_ys[i];
        closest_waveform_index = i;
      }
    }
    
    double current_height = m_waveform_groups[closest_waveform_index]->get_normalised_plot_height();
    double new_height     = max(0., current_height + scroll_speed_scalar * vertical_scroll_delta);
    
    m_waveform_groups[closest_waveform_index]->set_normalised_plot_height(new_height);
    m_waveform_groups[closest_waveform_index]->set_data_from_table(m_data, m_max_xbounds[0], m_max_xbounds[1]);
    
    replot();
  }
}

}  // namespace graphic
}  // namespace insight
