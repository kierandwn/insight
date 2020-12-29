#ifndef PLOTUPDATE_H
#define PLOTUPDATE_H

#include "lib/data/include/table.h"
#include "lib/layout/include/grid.h"
#include "lib/graphic/include/graphic.h"

#include <qwt_plot_item.h>

#include <string>
#include <map>
#include <typeinfo>

using namespace std;

class plotUpdater
{
private:
    table * m_dataTable;
    gridLayout * m_plotGrid;

public:
    plotUpdater(gridLayout * plotCollection, table * t) : m_dataTable(t),
                                                          m_plotGrid(plotCollection) {}

    void init()
    {
        double * xData = new double[101];
        for (int i = 0; i < 101; ++i) { xData[i] = i; }

        std::map<std::string, insightGraphic *>::iterator p;
        for (p = m_plotGrid->first(); p != m_plotGrid->last(); ++p)
        {
            insightGraphic * g(p->second);

            string id = p->second->getChannelName();

            QwtPlotCurve * curve = new QwtPlotCurve;

            double * yData = *(m_dataTable->get(id));

            // make a plot curve from the data and attach it to the plot
            curve->setSamples(xData, yData, 101);
            curve->attach(g);

            g->replot();
            g->show();
        }
    }
};

#endif // PLOTUPDATE_H
