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
// along with attitude.  If not, see <https://www.gnu.org/licenses/>.
//
#ifndef INSIGHTLAYOUT_H
#define INSIGHTLAYOUT_H
#pragma once

#include <string>
#include <map>

#include <QGridLayout>

#include "insight_graphic_base.h"
//#include "table.h"

#include "lib/json/single_include/nlohmann/json.hpp"

namespace insight {
namespace layout {

using namespace std;

class Grid {
 protected:
  int m_size[2];
  std::map<string, graphic::Base *> m_map;

 public:
  Grid() : m_size{0, 0} {}
  Grid(int rows, int cols) : m_size{rows, cols} {}

  map<string, graphic::Base *>::iterator first();
  map<string, graphic::Base *>::iterator next();
  map<string, graphic::Base *>::iterator last();

  std::map<string, graphic::Base *>& map();
  
  void resize();
};

class Layout : public Grid {
 private:
//  gridLayout m_plotgrid;

 public:
  Layout() {}

  void import_from_config( string filename, QGridLayout * grid, data::Table * data );
  std::map<string, graphic::Base *> import_from_config( nlohmann::json, QGridLayout *, data::Table * );
};

}  // namespace layout
}  // namespace insight
#endif // INSIGHTLAYOUT_H
