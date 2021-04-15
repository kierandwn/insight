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
#include "scatterdisplay.h"

#include <string>
#include <cmath>

#include <QMouseEvent>
#include <QWheelEvent>

#include "scattergroup.h"
#include "channel.h"

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
  
  for (int i = 0; i < m_ncurves; ++i) {
    DisplayCrosshair * ch = m_data_curves[i]->crosshair();
    
    dist_to_ch = l2_norm(event->x() - x_map.transform(ch->x()),
                         event->y() - y_map.transform(ch->y()) );
    
    if (dist_to_ch < 30) {
      m_mouse_state = CursorDrag;
      
      if (dist_to_ch < min_dist_to_ch) {
        min_dist_to_ch = dist_to_ch;
        m_cursor_track_curve = i;
      }
    }
  }
  
  data::Channel * xchan = m_data->get(m_data_curves[0]->get_xchannel_name());
           
  int i_closest = m_data_curves[m_cursor_track_curve]->get_shadow()->closestPoint(event->pos());
  double t_val = xchan->get_time_data_ptr()[i_closest];
  
  update_group_cursor_positions(t_val);
}

void DataXYDisplay::mouseReleaseEvent(QMouseEvent *) {
  m_mouse_state = Ready;
  m_cursor_track_curve = 0;
}

void DataXYDisplay::mouseMoveEvent(QMouseEvent * event) {
  if (m_mouse_state == CursorDrag) {
    mousePressEvent(event); // update cursor position
    
  } else if (m_mouse_state == Ready || m_mouse_state == Pan) {
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
      
    setAxisScale(xBottom, x_lbound, x_hbound);
    setAxisScale(yLeft, y_lbound, y_hbound);
    
    for (DataXYGroup * curve : m_data_curves) {
      curve->update_crosshair();
    }
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
  double x_lbound = axisScaleDiv(xBottom).lowerBound();
  double x_hbound = axisScaleDiv(xBottom).upperBound();
  double y_lbound = axisScaleDiv(yLeft).lowerBound();
  double y_hbound = axisScaleDiv(yLeft).upperBound();

  double xrange = x_hbound - x_lbound;
  double yrange = y_hbound - y_lbound;

  double scroll_speed_scalar = .001;
  double vertical_scroll_delta = event->angleDelta().y();
  x_lbound -= vertical_scroll_delta * scroll_speed_scalar * xrange;
  x_hbound += vertical_scroll_delta * scroll_speed_scalar * xrange;
  y_lbound -= vertical_scroll_delta * scroll_speed_scalar * yrange;
  y_hbound += vertical_scroll_delta * scroll_speed_scalar * yrange;
  
  setAxisScale(xBottom, x_lbound, x_hbound);
  setAxisScale(yLeft, y_lbound, y_hbound);
  
  for (int i = 0; i < m_ncurves; ++i) {
    string xchannel_name = m_data_curves[i]->get_xchannel_name(); // TODO move this check inside update_crosshair?
    string ychannel_name = m_data_curves[i]->get_ychannel_name();
    
    if (m_data->exists(xchannel_name) && m_data->exists(ychannel_name)) {
      m_data_curves[i]->update_crosshair();
    }
  }
  replot();
}

void DataXYDisplay::init_cursor_position()
{
  double x_lbound = axisScaleDiv(xBottom).lowerBound();
  double x_hbound = axisScaleDiv(xBottom).upperBound();
  double y_lbound = axisScaleDiv(yLeft).lowerBound();
  double y_hbound = axisScaleDiv(yLeft).upperBound();

  double x_bounds[2]{ x_lbound, x_hbound };
  double y_bounds[2]{ y_lbound, y_hbound };
  
  double xrange = x_hbound - x_lbound;
  double yrange = y_hbound - y_lbound;
  
  for (int i = 0; i < m_ncurves; ++i)
    m_data_curves[i]->crosshair()->set_xy(x_lbound + (i + 1) * .05 * xrange,
                                            y_lbound + (i + 1) * .05 * yrange,
                                            x_bounds,
                                            y_bounds
                                            );
  
  replot();
}

void DataXYDisplay::update_cursor_position(double tvalue)
{
  for (int i = 0; i < m_ncurves; ++i) {
    if (m_data_curves[i]->channels_present_in(m_data))
      m_data_curves[i]->update_crosshair(tvalue);
    
  }
  replot();
}

}  // namespace graphic
}  // namespace insight
