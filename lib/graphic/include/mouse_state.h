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
#ifndef MOUSE_STATE_H_
#define MOUSE_STATE_H_
#pragma once

namespace insight {
namespace graphic {


enum MouseDragMode {
  Init, Ready, Pan, CursorDrag
};

class MouseState {
 private:
  MouseDragMode m_id; // = StateId::init;
  double m_pos_xy[2]{ 0., 0. };
  
 public:
  MouseState();
  
  double * pos();
  double x();
  double y();
  
  void x(double);
  void y(double);
  void pos(double, double);
  
  bool operator==(MouseDragMode);
  MouseState& operator= (MouseDragMode);
};

}  // namespace graphic
}  // namespace insight
#endif  // MOUSE_STATE_H_
