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
#include "grid.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>

#include "lib/json/single_include/nlohmann/json.hpp"

#include "ApplicationInterface.h"
#include "waveformdisplay.h"
#include "scatterdisplay.h"

#include "table.h"

namespace insight {
namespace layout {

using json = nlohmann::json;
using graphic_map = std::map<std::string, graphic::InsightGraphic *>;

inline void UpdateGroupReferences(std::string group_name,
                        graphic_map& first_added_to_group,
                        graphic_map& last_added_to_group,
                        graphic::InsightGraphic * current_graphic_ref)
{
    bool group_exists_already = last_added_to_group.find(group_name) == last_added_to_group.end();

    if ( !(group_name == "") )
    {
        if ( group_exists_already )
        {
            graphic::InsightGraphic * g = last_added_to_group[group_name];
            g->SetRefToNextInGroup(current_graphic_ref);
        }
        else
        {
            first_added_to_group[group_name] = current_graphic_ref;
        }
        current_graphic_ref->SetRefToFirstInGroup(first_added_to_group[group_name]);
        last_added_to_group[group_name] = current_graphic_ref;
    }
}

//map<string, graphic::InsightGraphic *>::iterator Grid::first() { return m_map.begin(); }
//map<string, graphic::InsightGraphic *>::iterator Grid::last()  { return m_map.end(); }

map<string, graphic::InsightGraphic *>& Layout::map() { return m_map; }

map<string, graphic::InsightGraphic *> Layout::import_from_config(json jsonConfig, QGridLayout * grid, data::Table * data)
{
  int i = 0;
  string id, graphic_type; // , first_id;
    
  std::map<string, graphic::InsightGraphic *> mp;

  grid->setSpacing(0);
  grid->setContentsMargins(0, 0, 0, 0);

  int rows = jsonConfig["size"][0];
//    int cols = jsonConfig["size"][1].GetInt();

  json children = jsonConfig["children"];

  std::map<std::string, graphic::InsightGraphic *> first_added_to_group;
  std::map<std::string, graphic::InsightGraphic *> previous_added_to_group;

  for ( json::iterator child = children.begin(); child != children.end(); ++child )
  {
    id = child.key();
    json child_config = child.value();

    graphic_type = child_config["type"];
//    if (i == 0) first_id = id;

    if ( graphic_type == "Waveform" )
    {
      graphic::WaveformDisplay * plot = new graphic::WaveformDisplay(data, this);
      plot->apply_config(&child_config);
      grid->addWidget(plot, i % rows, i / rows);

      mp[id] = plot;
      i++;

//      UpdateGroupReferences(child_config["group"],
//              first_added_to_group,
//              previous_added_to_group,
//              plot
//              );
    }
    if ( graphic_type == "Scatter" )
    {
      graphic::ScatterDisplay * plot = new graphic::ScatterDisplay(data, this);
      plot->apply_config(&child_config);

      grid->addWidget(plot, i % rows, i / rows);
      mp[id] = plot;
      i++;

//      UpdateGroupReferences(child_config["group"],
//              first_added_to_group,
//              previous_added_to_group,
//              plot
//              );
    }
    if ( graphic_type == "Line" )
    {
      graphic::LineDisplay * plot = new graphic::LineDisplay(data, this);
      plot->apply_config(&child_config);

      grid->addWidget(plot, i % rows, i / rows);
      mp[id] = plot;
      i++;

//      UpdateGroupReferences(child_config["group"],
//              first_added_to_group,
//              previous_added_to_group,
//              plot
//              );
    }
    else if ( graphic_type == "Grid" )
    {
      QGridLayout * childGrid = new QGridLayout;
      grid->addLayout(childGrid, i % rows, i / rows);
      i++;

      std::map<string, graphic::InsightGraphic *> sub_mp = import_from_config( child_config, childGrid, data );
      mp.insert( sub_mp.begin(), sub_mp.end() );
    }
  }
  return mp;
}

void Layout::import_from_config( std::string filename, QGridLayout * grid, data::Table * data )
{
  ifstream ifs { filename };
  if ( !ifs.is_open() ) { cerr << "Could not open file for reading!\n"; throw; }

  ifs >> GridJsonConfig_;

  std::map<string, graphic::InsightGraphic *> mp = import_from_config( GridJsonConfig_, grid, data );
  m_map.insert( mp.begin(), mp.end() );
}

void Layout::saveToFile(string filepath)
{
    ofstream ofs { filepath };
    if ( !ofs.is_open() ) { cerr << "Could not open file for writing!\n"; throw; }

    ofs << GridJsonConfig_;
//    ofs << std::setw(4) << AppJsonConfig_;
}

//void Grid::resize() {
//  const int n_rows = rowCount();
//  const int n_cols = columnCount();
//}


}  // namespace layout
}  // namespace insight
