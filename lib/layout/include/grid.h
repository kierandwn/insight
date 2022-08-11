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
#ifndef INSIGHTLAYOUT_H
#define INSIGHTLAYOUT_H
#pragma once

#include <string>
#include <map>

#include <QGridLayout>

#include "ApplicationInterface.h"
#include "table.h"

#include "lib/json/single_include/nlohmann/json.hpp"

namespace insight {
namespace layout {

using namespace std;

class Grid
{
protected:
    nlohmann::json GridJsonConfig_;
    int m_size[2];

public:
    Grid() : m_size{0, 0} {}
    Grid(int rows, int cols) : m_size{rows, cols} {}

//    map<string, graphic::InsightGraphic *>::iterator first();
//    map<string, graphic::InsightGraphic *>::iterator next();
//    map<string, graphic::InsightGraphic *>::iterator last();

//    std::map<string, graphic::InsightGraphic *>& map();

    void resize();
};

class Layout : public Grid
{
private:
//  QGridLayout * m_plotgrid;
    std::map<string, graphic::InsightGraphic *> m_map;

public:
  Layout() {}

  std::map<string, graphic::InsightGraphic *>& map();

  void import_from_config(string, QGridLayout *, data::Table *);
  std::map<string, graphic::InsightGraphic *> import_from_config(nlohmann::json, QGridLayout *, data::Table *);

  void saveToFile(string);
};

}  // namespace layout
}  // namespace insight
#endif // INSIGHTLAYOUT_H
