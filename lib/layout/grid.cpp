#include "lib/layout/include/grid.h"
#include "lib/graphic/include/graphic.h"

#include "lib/rapidjson/include/rapidjson/document.h"
#include <lib/rapidjson/include/rapidjson/istreamwrapper.h>

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace rapidjson;

map<string, insightGraphic *>::iterator gridLayout::first() { return m_map.begin(); }
map<string, insightGraphic *>::iterator gridLayout::last()  { return m_map.end(); }

map<string, insightGraphic *> insightLayout::importFromConfig( Value& jsonConfig, QGridLayout * grid )
{
    int i = 0;
    string id, typ;

    ::map<string, insightGraphic *> mp;

    int rows = jsonConfig["size"][0].GetInt();
//    int cols = jsonConfig["size"][1].GetInt();

    for ( auto& m : jsonConfig["children"].GetObject() )
    {
        id = m.name.GetString();
        typ = m.value.GetObject()["type"].GetString();

        if ( typ == "Waveform" )
        {
            insightGraphic * plot = new insightGraphic;

            string channelName;

            if (m.value.GetObject().HasMember("data")) {
                channelName = m.value.GetObject()["data"]["channel"].GetString();
                plot->setChannelName(channelName);
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

            ::map<string, insightGraphic *> sub_mp = importFromConfig( m.value, childGrid );
            mp.insert( sub_mp.begin(), sub_mp.end() );
        }
    }
    return mp;
}

void insightLayout::importFromConfig( string filename, QGridLayout * grid )
{
    ifstream ifs { filename };
    if ( !ifs.is_open() ) { cerr << "Could not open file for reading!\n"; throw; }

    IStreamWrapper isw{ ifs };

    Document d;
    d.ParseStream( isw );

    ::map<string, insightGraphic *> mp = importFromConfig( d, grid );
    m_map.insert( mp.begin(), mp.end() );
}
