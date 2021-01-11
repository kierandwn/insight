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

    void init() {
        std::map<std::string, insightGraphic *>::iterator p;
        for (p = m_plotGrid->first(); p != m_plotGrid->last(); ++p)
        {
            insightGraphic * g(p->second);

            string id = p->second->getChannelName();

            if (m_dataTable->exists(id)) {
                QwtPlotCurve * curve = new QwtPlotCurve;

                channel * c = m_dataTable->get(id);
                size_t n = c->length();

                double * xData = new double[n];
                for (size_t i = 0; i < n; ++i) { xData[i] = i; }

                channel yData = *(m_dataTable->get(id));

                // make a plot curve from the data and attach it to the plot
                curve->setSamples(xData, c->begin(), n);
                curve->attach(g);

                delete[] xData;
            }

            g->replot();
            g->show();
        }
    }

    void update() {
        clear();

        map<string, insightGraphic *>::iterator p;
        for (p = m_plotGrid->first(); p != m_plotGrid->last(); ++p) {
            // obtain graphic object
            insightGraphic * g = p->second;

            // create curve object
            string id = p->second->getChannelName();

            if (m_dataTable->exists(id)) {
                QwtPlotCurve * curve = new QwtPlotCurve;
                channel * c = m_dataTable->get(id);
                size_t n = c->length();

                // obtain an x channel from data?
                double * xData = new double[n];
                for (size_t i = 0; i < n; ++i) { xData[i] = i; }

                // attach curve to graphic
                curve->setSamples(xData, c->begin(), n);
                curve->attach(g);

                g->replot();
            }
        }
    }

    void clear() {
        map<string, insightGraphic *>::iterator p;
        for (p = m_plotGrid->first(); p != m_plotGrid->last(); ++p) {
            // obtain graphic object
            insightGraphic * g = p->second;
            g->detachItems();
            g->replot();
        }
    }
};

#endif // PLOTUPDATE_H
