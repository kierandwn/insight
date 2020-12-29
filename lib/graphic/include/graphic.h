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
#include <string>
#include <array>

using namespace std;

const array<string, 2> INSIGHT_GRAPHICS{ "Waveform", "Grid" };


class insightGraphic : virtual public QwtPlot
{
private:
    string m_channelName;
public:
    void setChannelName(string);
    string getChannelName();
};

#endif // INSIGHTGRAPHIC_H
