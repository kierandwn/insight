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
#include "scatterdisplay.h"

#include <string>
#include <cmath>

#include <QMouseEvent>
#include <QWheelEvent>

#include "scattergroup.h"
#include "channel.h"
#include "painting_tools.h"

namespace insight {
namespace graphic {


double l2_norm(double dx, double dy) {
  return pow(pow(abs(dx), 2) + pow(abs(dy), 2), 0.5);
}

void DataXYDisplay::mousePressEvent(QMouseEvent * event)
{
  QwtScaleMap x_map = canvasMap(xBottom);
  QwtScaleMap y_map = canvasMap(yLeft);

  double mouse_xpos = x_map.invTransform(event->x());
  double mouse_ypos = y_map.invTransform(event->y());
  m_mouse_state.x(mouse_xpos);
  m_mouse_state.y(mouse_ypos);

  double dist_to_ch;
  double min_dist_to_ch;

  for (int i = 0; i < m_ncurves; ++i)
  {
    CrosshairGroup * ch = m_data_curves[i]->crosshairs();
    int i_ch = m_data_curves[i]->active_layer();

    dist_to_ch = l2_norm(event->x() - x_map.transform(ch->x(i_ch)),
                         event->y() - y_map.transform(ch->y(i_ch)) );

    if (dist_to_ch < 30)
    {
      m_mouse_state = CursorDrag;

      if (dist_to_ch < min_dist_to_ch)
        min_dist_to_ch = dist_to_ch;
    }
  }

  data::Channel * xchan = m_data->get(m_data_curves[0]->get_xchannel_name());

  int i_closest = m_data_curves[0]->get_shadow()->closestPoint(event->pos());
  double t_val = xchan->get_time_data_ptr()[i_closest];

  update_group_cursor_positions(t_val);
}

void DataXYDisplay::mouseReleaseEvent(QMouseEvent *)
{
  m_mouse_state = Ready;
}

void DataXYDisplay::mouseMoveEvent(QMouseEvent * event)
{
  if (m_mouse_state == CursorDrag)
    mousePressEvent(event); // update cursor position
    
  else
  if (m_mouse_state == Ready || m_mouse_state == Pan)
  {
    m_mouse_state = Pan;

    QwtScaleMap x_map = canvasMap(xBottom);
    QwtScaleMap y_map = canvasMap(yLeft);
    
    double delta_x = x_map.invTransform(event->x()) - m_mouse_state.x();
    double delta_y = y_map.invTransform(event->y()) - m_mouse_state.y();

    double x_lbound = axisScaleDiv(xBottom).lowerBound();
    double x_hbound = axisScaleDiv(xBottom).upperBound();
    double y_lbound = axisScaleDiv(yLeft).lowerBound();
    double y_hbound = axisScaleDiv(yLeft).upperBound();

    x_lbound -= delta_x * .85; // gets very jumpy at * 1.? not sure why?
    x_hbound -= delta_x * .85;
    y_lbound -= delta_y * .85;
    y_hbound -= delta_y * .85;
    
    double xbounds[2]{x_lbound, x_hbound};
    double ybounds[2]{y_lbound, y_hbound};
      
    update_plot_limits(xbounds, ybounds);
    
    for (DataXYGroup * curve : m_data_curves)
      curve->update_crosshair();
    
    replot();
      
    m_mouse_state.x(x_map.invTransform(event->x()));
    m_mouse_state.y(y_map.invTransform(event->y()));
  }
}

//void DataXYDisplay::mouseDoubleClickEvent(QMouseEvent * event) {
//    cout << "reached: double-click" << endl;
//}

void DataXYDisplay::wheelEvent(QWheelEvent * event)
{
  double scroll_speed_scalar = .001;
  double vertical_scroll_delta = event->angleDelta().y();
  
  double x_lbound = axisScaleDiv(xBottom).lowerBound();
  double x_hbound = axisScaleDiv(xBottom).upperBound();
  double y_lbound = axisScaleDiv(yLeft).lowerBound();
  double y_hbound = axisScaleDiv(yLeft).upperBound();
  double xrange = x_hbound - x_lbound;
  double yrange = y_hbound - y_lbound;

  double x_mouse = axis_coordx_from_painter_scale(this, event->position().x());
  double y_mouse = axis_coordy_from_painter_scale(this, event->position().y());
  
  double lhs_scaling = (x_mouse - x_lbound) / xrange;
  double bottom_scaling = (y_mouse - y_lbound) / yrange;
  
  x_lbound -= vertical_scroll_delta * scroll_speed_scalar * xrange * lhs_scaling;
  x_hbound += vertical_scroll_delta * scroll_speed_scalar * xrange * (1. - lhs_scaling);
  y_lbound -= vertical_scroll_delta * scroll_speed_scalar * yrange * bottom_scaling;
  y_hbound += vertical_scroll_delta * scroll_speed_scalar * yrange * (1. - bottom_scaling);
  
  double xbounds[2]{x_lbound, x_hbound};
  double ybounds[2]{y_lbound, y_hbound};
  
  update_plot_limits(xbounds, ybounds);
}

void DataXYDisplay::update_plot_limits()
{
  double max_xbounds_in_data[2];
  double max_ybounds_in_data[2];
  
  if (!get_total_data_ranges(max_xbounds_in_data, max_ybounds_in_data))
    return;
  
  double xrange_in_data = max_xbounds_in_data[1] - max_xbounds_in_data[0];
  double yrange_in_data = max_ybounds_in_data[1] - max_ybounds_in_data[0];
  
  double x_lbound = max_xbounds_in_data[0] - m_padding_scalar * xrange_in_data;
  double x_hbound = max_xbounds_in_data[1] + m_padding_scalar * xrange_in_data;
  double y_lbound = max_ybounds_in_data[0] - m_padding_scalar * yrange_in_data;
  double y_hbound = max_ybounds_in_data[1] + m_padding_scalar * yrange_in_data;
  
  double xbounds[2]{max_xbounds_in_data[0], max_xbounds_in_data[1]};
  double ybounds[2]{max_ybounds_in_data[0], max_ybounds_in_data[1]};
  
  setAxisScale(xBottom, x_lbound, x_hbound);
  setAxisScale(yLeft,   y_lbound, y_hbound);
  
  for (int i = 0; i < m_ncurves; ++i)
  {
    string xchannel_name = m_data_curves[i]->get_xchannel_name(); // TODO move this check inside update_crosshair?
    string ychannel_name = m_data_curves[i]->get_ychannel_name();

    if (m_data->exists_in_layer(xchannel_name) && m_data->exists_in_layer(ychannel_name))
      m_data_curves[i]->update_crosshair(xbounds, ybounds);

  }
  replot();
}

void DataXYDisplay::update_plot_limits(double * xbounds, double * ybounds)
{
  double x_lbound = xbounds[0];
  double x_hbound = xbounds[1];
  double y_lbound = ybounds[0];
  double y_hbound = ybounds[1];
  
  double max_xbounds_in_data[2];
  double max_ybounds_in_data[2];
  
  if (!get_total_data_ranges(max_xbounds_in_data, max_ybounds_in_data))
    return;
  
  double xrange_in_data = max_xbounds_in_data[1] - max_xbounds_in_data[0];
  double yrange_in_data = max_ybounds_in_data[1] - max_ybounds_in_data[0];
  
  x_lbound = max({x_lbound, max_xbounds_in_data[0] - m_padding_scalar * xrange_in_data});
  x_hbound = min({x_hbound, max_xbounds_in_data[1] + m_padding_scalar * xrange_in_data});
  y_lbound = max({y_lbound, max_ybounds_in_data[0] - m_padding_scalar * yrange_in_data});
  y_hbound = min({y_hbound, max_ybounds_in_data[1] + m_padding_scalar * yrange_in_data});
  
  double xbounds_for_crosshair[2]{max({x_lbound, max_xbounds_in_data[0]}),
                                  min({x_hbound, max_xbounds_in_data[1]})};
  double ybounds_for_crosshair[2]{max({y_lbound, max_ybounds_in_data[0]}),
                                  min({y_hbound, max_ybounds_in_data[1]})};
  
  setAxisScale(xBottom, x_lbound, x_hbound);
  setAxisScale(yLeft,   y_lbound, y_hbound);
  
  for (int i = 0; i < m_ncurves; ++i)
  {
    string xchannel_name = m_data_curves[i]->get_xchannel_name(); // TODO move this check inside update_crosshair?
    string ychannel_name = m_data_curves[i]->get_ychannel_name();

    if (m_data->exists_in_layer(xchannel_name) && m_data->exists_in_layer(ychannel_name))
      m_data_curves[i]->update_crosshair(xbounds_for_crosshair, ybounds_for_crosshair);

  }
  replot();
}

void DataXYDisplay::update_cursor_position(double tvalue)
{
  for (int i = 0; i < m_ncurves; ++i)
    if (m_data_curves[i]->channels_and_time_present_for_any_layer())
      m_data_curves[i]->update_crosshair_positions(tvalue);
  
  replot();
}

}  // namespace graphic
}  // namespace insight
