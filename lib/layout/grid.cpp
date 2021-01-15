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
#include "lib/layout/include/grid.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <map>

#include "lib/rapidjson/include/rapidjson/document.h"
#include <lib/rapidjson/include/rapidjson/istreamwrapper.h>

#include "lib/data/include/table.h"
#include "lib/graphic/waveform/include/waveformdisplay.h"


using namespace std;
using namespace rapidjson;

map<string, InsightBaseGraphic *>::iterator gridLayout::first() { return m_map.begin(); }
map<string, InsightBaseGraphic *>::iterator gridLayout::last()  { return m_map.end(); }

map<string, InsightBaseGraphic *> insightLayout::importFromConfig( Value& jsonConfig, QGridLayout * grid, table * data)
{
  int i = 0;
  string id, typ;

  ::map<string, InsightBaseGraphic *> mp;

  grid->setSpacing(0);
  grid->setContentsMargins(0, 0, 0, 0);


  int rows = jsonConfig["size"][0].GetInt();
//    int cols = jsonConfig["size"][1].GetInt();

  for ( auto& m : jsonConfig["children"].GetObject() )
  {
    id = m.name.GetString();
    typ = m.value.GetObject()["type"].GetString();

    if ( typ == "Waveform" )
    {
      WaveformDisplay * plot = new WaveformDisplay(data);

      vector<string> channel_names;

      if (m.value.GetObject().HasMember("data")) {
        const Value& channel_list = m.value.GetObject()["data"]["channel"].GetArray();
        for (SizeType i = 0; i < channel_list.Size(); ++i) {
          plot->add_channel_name(channel_list[i].GetString());
        }
      }

      grid->addWidget(plot, i % rows, i / rows);
      mp[id] = plot;

      i++;
    }
    else if ( typ == "Grid" )
    {
      QGridLayout * childGrid = new QGridLayout;
      grid->addLayout(childGrid, i % rows, i / rows);
      i++;

      ::map<string, InsightBaseGraphic *> sub_mp = importFromConfig( m.value, childGrid, data );
      mp.insert( sub_mp.begin(), sub_mp.end() );
    }
  }
  return mp;
}

void insightLayout::importFromConfig( string filename, QGridLayout * grid, table * data )
{
    ifstream ifs { filename };
    if ( !ifs.is_open() ) { cerr << "Could not open file for reading!\n"; throw; }

    IStreamWrapper isw{ ifs };

    Document d;
    d.ParseStream( isw );

    ::map<string, InsightBaseGraphic *> mp = importFromConfig( d, grid, data );
    m_map.insert( mp.begin(), mp.end() );
}
