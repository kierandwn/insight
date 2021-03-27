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
#include "mouse_state.h"

namespace insight {
namespace graphic {


MouseState::MouseState() : m_id(MouseDragMode::Init) {}
  
double * MouseState::pos() { return m_pos_xy; }
double MouseState::x() { return m_pos_xy[0]; }
double MouseState::y() { return m_pos_xy[1]; }
  
void MouseState::x(double xval) { m_pos_xy[0] = xval; }
void MouseState::y(double yval) { m_pos_xy[1] = yval; }
void MouseState::pos(double xval, double yval) {
  m_pos_xy[0] = xval;
  m_pos_xy[1] = yval;
}
  
bool MouseState::operator==(MouseDragMode sid) { return m_id == sid; }
MouseState& MouseState::operator= (MouseDragMode sid) { m_id = sid; return *(this); }

}  // namespace graphic
}  // namespace insight
