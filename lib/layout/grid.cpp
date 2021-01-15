#include "lib/layout/include/grid.h"
#include "lib/data/include/table.h"
//#include "lib/graphic/include/graphic.h"
#include "lib/graphic/waveform/include/waveformdisplay.h"

#include "lib/rapidjson/include/rapidjson/document.h"
#include <lib/rapidjson/include/rapidjson/istreamwrapper.h>

#include <iostream>
#include <fstream>
#include <cmath>

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
            // QwtPlot * plot = new QwtPlot();

            string channelName;

            if (m.value.GetObject().HasMember("data")) {
                channelName = m.value.GetObject()["data"]["channel"].GetString();
                plot->set_channel_name(channelName);
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
