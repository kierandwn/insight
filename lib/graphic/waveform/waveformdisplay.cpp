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
