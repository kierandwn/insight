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
  cout << "press event" << endl;
  QwtScaleMap map = canvasMap(xBottom);
  double m_mouse_xpos = map.invTransform(event->x());
    
  double * widest_xlim = xlim();
  double xrange = widest_xlim[1] - widest_xlim[0];
    
  if (abs(m_mouse_xpos - m_xpos_cursor) < (xrange * .05))
  m_mouse_state = CursorDrag;
    
  update_group_cursor_positions(m_mouse_xpos);
  delete[] widest_xlim;
}

void WaveformDisplay::mouseMoveEvent(QMouseEvent * event) {
    if (m_mouse_state == CursorDrag) {
      cout << "cursor dragging.." << endl;
      mousePressEvent(event); // update cursor positon
    } else if (m_mouse_state == Ready || m_mouse_state == Pan) {
      m_mouse_state = Pan;
        
      double pan_speed_scalar = .001;
      double delta_x = event->x() - m_mouse_state.x();
    
      double * widest_xlim = xlim();
      double xrange = widest_xlim[1] - widest_xlim[0];
    
      widest_xlim[0] += delta_x * pan_speed_scalar * xrange;
      widest_xlim[1] += delta_x * pan_speed_scalar * xrange;
      update_group_view_limits(widest_xlim[0], widest_xlim[1]);
    
      m_mouse_state.x(event->x());
      delete[] widest_xlim;
    }
}

void WaveformDisplay::mouseDoubleClickEvent(QMouseEvent *) {
    cout << "reached: double-click" << endl;
}

void WaveformDisplay::mouseReleaseEvent(QMouseEvent *) {
  m_mouse_state = Ready;
}

void WaveformDisplay::wheelEvent(QWheelEvent * event) {
  double scroll_speed_scalar = .001;
  double vertical_scroll_delta = event->angleDelta().y();
  
  double * widest_xlim = xlim();
  double xrange = widest_xlim[1] - widest_xlim[0];
  
  widest_xlim[0] -= scroll_speed_scalar * vertical_scroll_delta * xrange;
  widest_xlim[1] += scroll_speed_scalar * vertical_scroll_delta * xrange;
  update_group_view_limits(widest_xlim[0], widest_xlim[1]);
  
  delete[] widest_xlim;
}

}  // namespace graphic
}  // namespace insight
