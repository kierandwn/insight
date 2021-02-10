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

#include <string>
#include <map>

#include <QGridLayout>

#include "lib/json/single_include/nlohmann/json.hpp"

#include "insight_graphic_base.h"
#include "waveformdisplay.h"
#include "table.h"

class gridLayout {
protected:
  int m_size[2];
  std::map<string, InsightBaseGraphic *> m_map;

public:
  gridLayout() : m_size{0, 0} {}
  gridLayout(int rows, int cols) : m_size{rows, cols} {}

  map<string, InsightBaseGraphic *>::iterator first();
  map<string, InsightBaseGraphic *>::iterator next();
  map<string, InsightBaseGraphic *>::iterator last();

  std::map<string, InsightBaseGraphic *>& map();
};

class insightLayout : public gridLayout {
private:
//  gridLayout m_plotgrid;

public:
  insightLayout() {}

  void importFromConfig( string filename, QGridLayout * grid, table * data );
  std::map<string, InsightBaseGraphic *> importFromConfig( nlohmann::json, QGridLayout *, table * );
};


#endif // INSIGHTLAYOUT_H
