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

double find_nearest_neighbour(double xval, double yval, data::Channel& xchan, data::Channel& ychan) {
  int n = xchan.length();
  
  double min_l2_dist = 10e12;
  double l2_dist;
  double tmin = 0;
  
  data::Channel& tchan = *(xchan.get_time_ref());
  
  for (int i = 0; i < n; ++i) {
    l2_dist = l2_norm(xval - xchan[i], yval - ychan[i]);
    
    if (l2_dist < min_l2_dist) {
      min_l2_dist = l2_dist;
      tmin = tchan[i];
    }
  }
  printf("hello world.\n");
  return tmin;
}

int find_nearest_neighbour(double xval, double yval, data::Channel& xchan, data::Channel& ychan, int) {
  int n = xchan.length();
  
  double min_l2_dist = 10e12;
  double l2_dist;
  int tmin = 0;
  
  data::Channel& tchan = *(xchan.get_time_ref());
  
  for (int i = 0; i < n; ++i) {
    l2_dist = l2_norm(xval - xchan[i], yval - ychan[i]);
    
    if (l2_dist < min_l2_dist) {
      min_l2_dist = l2_dist;
      tmin = i;
    }
  }
  return tmin;
}

void ScatterDisplay::mousePressEvent(QMouseEvent * event)
{
  QwtScaleMap x_map = canvasMap(xBottom);
  QwtScaleMap y_map = canvasMap(yLeft);
  
  double mouse_xpos = x_map.invTransform(event->x());
  double mouse_ypos = y_map.invTransform(event->y());
  
  // Get axes limits from axes objects (Qwt)
  double x_lbound = axisScaleDiv(xBottom).lowerBound();
  double x_hbound = axisScaleDiv(xBottom).upperBound();
  double y_lbound = axisScaleDiv(yLeft).lowerBound();
  double y_hbound = axisScaleDiv(yLeft).upperBound();

  double xrange = x_hbound - x_lbound;
  double yrange = y_lbound - y_hbound;
  
  double dist_to_ch = l2_norm(mouse_xpos - m_crosshair.x(), mouse_ypos - m_crosshair.y());
  
  if (dist_to_ch < (xrange * .05) && dist_to_ch < (yrange * .05))
    m_mouse_state = CursorDrag;
  
  m_mouse_state.x(mouse_xpos);
  m_mouse_state.y(mouse_ypos);
  
  data::Channel * xchan = m_data->get(m_scatter_pairs[0]->get_xchannel_name());
           
  int i_closest = m_scatter_pairs[0]->get_scatter_full()->closestPoint(event->pos());
  double t_val = xchan->get_time_data_ptr()[i_closest];
  
  update_group_cursor_positions(t_val);
}

void ScatterDisplay::mouseReleaseEvent(QMouseEvent *) {
  m_mouse_state = Ready;
}

void ScatterDisplay::mouseMoveEvent(QMouseEvent * event) {
  if (m_mouse_state == CursorDrag) {
    mousePressEvent(event); // update cursor positon
    
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
    
    x_lbound -= delta_x;
    x_hbound -= delta_x;
    y_lbound -= delta_y;
    y_hbound -= delta_y;
    
    setAxisScale(xBottom, x_lbound, x_hbound);
    setAxisScale(yLeft, y_lbound, y_hbound);
    
    m_mouse_state.x(x_map.invTransform(event->x()));
    m_mouse_state.y(y_map.invTransform(event->y()));
    replot();
  }
}

//void ScatterDisplay::mouseDoubleClickEvent(QMouseEvent * event) {
//    cout << "reached: double-click" << endl;
//}

void ScatterDisplay::wheelEvent(QWheelEvent * event) {
  double vertical_scroll_delta = event->angleDelta().y();

  double x_lbound = axisScaleDiv(xBottom).lowerBound();
  double x_hbound = axisScaleDiv(xBottom).upperBound();
  double y_lbound = axisScaleDiv(yLeft).lowerBound();
  double y_hbound = axisScaleDiv(yLeft).upperBound();

  double xrange = x_hbound - x_lbound;
  double yrange = y_hbound - y_lbound;

  double scroll_speed_scalar = .001;
  x_lbound -= vertical_scroll_delta * scroll_speed_scalar * xrange;
  x_hbound += vertical_scroll_delta * scroll_speed_scalar * xrange;
  y_lbound -= vertical_scroll_delta * scroll_speed_scalar * yrange;
  y_hbound += vertical_scroll_delta * scroll_speed_scalar * yrange;
  
  double xbounds[2]{ x_lbound, x_hbound };
  double ybounds[2]{ y_lbound, y_hbound };
  
  m_crosshair.set_xy(m_crosshair.x(), m_crosshair.y(), xbounds, ybounds);
  
  setAxisScale(xBottom, x_lbound, x_hbound);
  setAxisScale(yLeft, y_lbound, y_hbound);
  replot();
}

void ScatterDisplay::update_cursor_position(double tvalue) {
  string xchannel_name = m_scatter_pairs[0]->get_xchannel_name();
  string ychannel_name = m_scatter_pairs[0]->get_ychannel_name();
  
  if (m_data->exists(xchannel_name) && m_data->exists(ychannel_name)) {
    double xvalue = m_data->get(xchannel_name)->value_at(tvalue);
    double yvalue = m_data->get(ychannel_name)->value_at(tvalue);
    
    // Get axes limits from axes objects (Qwt)
    double x_lbound = axisScaleDiv(xBottom).lowerBound();
    double x_hbound = axisScaleDiv(xBottom).upperBound();
    double y_lbound = axisScaleDiv(yLeft).lowerBound();
    double y_hbound = axisScaleDiv(yLeft).upperBound();
    
    double xlimits[2]{ x_lbound, x_hbound };
    double ylimits[2]{ y_lbound, y_hbound };
    
//    cursor_in_xrange();
    m_crosshair.set_xy(xvalue, yvalue, xlimits, ylimits);
    set_label_values_at(tvalue);
    replot();
  }
}

}  // namespace graphic
}  // namespace insight
