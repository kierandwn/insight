#include "lib/layout/include/grid.h"
#include "lib/graphic/include/graphic.h"

#include "lib/rapidjson/include/rapidjson/document.h"
#include <lib/rapidjson/include/rapidjson/istreamwrapper.h>

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace rapidjson;


void insightLayout::importFromConfig( Value& jsonConfig, QGridLayout * grid )
{
    const int count = 100;
    double gain = 10.;

    // data arrays for the plot
    double xData[count];
    double yData[count];

    // set up the initial plot data
    for (int index = 0; index < count; ++index)
    {
        xData[index] = 2 * M_PI * index / count;
        yData[index] = gain * std::rand() / RAND_MAX;
    }

    int i = 0;
    string t;

    int rows = jsonConfig["size"][0].GetInt();
//    int cols = jsonConfig["size"][1].GetInt();

    for ( auto& m : jsonConfig["children"].GetObject() )
    {
        t = m.value.GetObject()["type"].GetString();

        if ( t == "Waveform" )
        {
            QwtPlotCurve * curve = new QwtPlotCurve;
            insightGraphic * plot = new insightGraphic;

//            if (m.value.GetObject().HasMember("data")) {
//                string col = m.value.GetObject()["data"]["lineColor"].GetString();
//                int w = m.value.GetObject()["data"]["lineWidth"].GetInt();

//                curve->setPen(new QColor(col));
//            }

            // make a plot curve from the data and attach it to the plot
            curve->setSamples(xData, yData, count);
            curve->attach(plot);

            grid->addWidget(plot, i % rows, i / rows);
            i++;

            plot->replot();
            plot->show();
        }

        else if ( t == "Grid" )
        {
            QGridLayout * childGrid = new QGridLayout;
            grid->addLayout(childGrid, i % rows, i / rows);
            i++;

            importFromConfig( m.value, childGrid );
        }
    }
}

void insightLayout::importFromConfig( string filename, QGridLayout * grid )
{
    printf("from file.\n");
    ifstream ifs { filename };
    if ( !ifs.is_open() ) { cerr << "Could not open file for reading!\n"; return; }

    IStreamWrapper isw{ ifs };

    Document d;
    d.ParseStream( isw );

    insightLayout::importFromConfig( d, grid );
}
