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
#include "waveformdisplay.h"

#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "insight_graphic_base.h"


WaveformDisplay::WaveformDisplay(table * data)
    : InsightBaseGraphic(data) {
  ui->setupUi(this);
}

// Apply configuation parameters held in json_config
void WaveformDisplay::apply_config(nlohmann::json * json_config) {
  if (json_config->contains("data")) {
    for (auto& channel_name : json_config->operator[]("data")["channel"]) {
      add_channel_name(channel_name);
    }
  }
}

void WaveformDisplay::update()
{
  int channels_to_plot = get_number_of_channels();

  for (int i = 0; i < channels_to_plot; ++i) {

    // create curve object
    string id = get_channel_name(i);

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
}

void WaveformDisplay::reset()
{
    detachItems();
    replot();
}
