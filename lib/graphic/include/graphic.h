#ifndef INSIGHTGRAPHIC_H
#define INSIGHTGRAPHIC_H

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include <QWidget>

#include <map>
#include <array>

using namespace std;

const array<string, 2> INSIGHT_GRAPHICS{ "Waveform", "Grid" };


class insightGraphic : public QwtPlot
{

};

#endif // INSIGHTGRAPHIC_H
