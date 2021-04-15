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
#include "grid.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>

#include "lib/json/single_include/nlohmann/json.hpp"

#include "insight_graphic_base.h"
#include "waveformdisplay.h"
#include "scatterdisplay.h"

#include "table.h"

namespace insight {
namespace layout {

using json = nlohmann::json;

map<string, graphic::Base *>::iterator Grid::first() { return m_map.begin(); }
map<string, graphic::Base *>::iterator Grid::last()  { return m_map.end(); }

map<string, graphic::Base *>& Grid::map() { return m_map; }

map<string, graphic::Base *> Layout::import_from_config(json jsonConfig, QGridLayout * grid, data::Table * data)
{
  int i = 0;
  string id, typ, first_id;

//  graphic::WaveformDisplay * plot;
    
  std::map<string, graphic::Base *> mp;

  grid->setSpacing(0);
  grid->setContentsMargins(0, 0, 0, 0);

  int rows = jsonConfig["size"][0];
//    int cols = jsonConfig["size"][1].GetInt();

  json children = jsonConfig["children"];

  for ( json::iterator child = children.begin(); child != children.end(); ++child )
  {
    id = child.key();
    json child_config = child.value();

    typ = child_config["type"];
    if (i == 0) first_id = id;
      
    if ( typ == "Waveform" )
    {
      graphic::WaveformDisplay * plot = new graphic::WaveformDisplay(data, this);
      plot->apply_config(&child_config);
      grid->addWidget(plot, i % rows, i / rows);
      
      mp[id] = plot;
      i++;
    }
    if ( typ == "Scatter" )
    {
      graphic::ScatterDisplay * plot = new graphic::ScatterDisplay(data, this);
      plot->apply_config(&child_config);

      grid->addWidget(plot, i % rows, i / rows);
      mp[id] = plot;
      i++;
    }
    if ( typ == "Line" )
    {
      graphic::LineDisplay * plot = new graphic::LineDisplay(data, this);
      plot->apply_config(&child_config);

      grid->addWidget(plot, i % rows, i / rows);
      mp[id] = plot;
      i++;
    }
    else if ( typ == "Grid" )
    {
      QGridLayout * childGrid = new QGridLayout;
      grid->addLayout(childGrid, i % rows, i / rows);
      i++;

      std::map<string, graphic::Base *> sub_mp = import_from_config( child_config, childGrid, data );
      mp.insert( sub_mp.begin(), sub_mp.end() );
    }
  }
  return mp;
}

void Layout::import_from_config( std::string filename, QGridLayout * grid, data::Table * data )
{
  ifstream ifs { filename };
  if ( !ifs.is_open() ) { cerr << "Could not open file for reading!\n"; throw; }

  json config;
  ifs >> config;

  std::map<string, graphic::Base *> mp = import_from_config( config, grid, data );
  m_map.insert( mp.begin(), mp.end() );
}

//void Grid::resize() {
//  const int n_rows = rowCount();
//  const int n_cols = columnCount();
//}


}  // namespace layout
}  // namespace insight
