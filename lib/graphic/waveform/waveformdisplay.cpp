#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "include/waveformdisplay.h"

#include "lib/graphic/include/insight_graphic_base.h"


void WaveformDisplay::update()
{
    // create curve object
    string id = get_channel_name();

    if (data_->exists(id)) {
        QwtPlotCurve * curve = new QwtPlotCurve;
        channel * c = data_->get(id);
        size_t n = c->length();

        // obtain an x channel from data?
        double * xData = new double[n];
        for (size_t i = 0; i < n; ++i) { xData[i] = i; }

        // attach curve to graphic
        curve->setSamples(xData, c->begin(), n);
        curve->attach(this);

        replot();
    }
}

void WaveformDisplay::reset()
{
    detachItems();
    replot();
}
