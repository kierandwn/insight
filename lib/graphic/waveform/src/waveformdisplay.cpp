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

#include <cmath>

#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "insight_graphic_base.h"

namespace insight {

WaveformDisplay::WaveformDisplay(data::Table * data)
    : InsightBaseGraphic(data) {
  ui->setupUi(this);

  setAutoFillBackground( true );
  QPalette p = palette();

  p.setColor(QPalette::Window, QColor(255, 255, 255, 255));
  setPalette(p);

  enableAxis(xBottom, false);
  enableAxis(yLeft, false);
}

// Apply configuation parameters held in json_config
void WaveformDisplay::apply_config(nlohmann::json * json_config) {
  string label = "";
  int i = 0;

  ui->channel_table->horizontalHeader()->setVisible(false);
  ui->channel_table->verticalHeader()->setVisible(false);

  ui->channel_table->insertColumn(0); // channel names
  ui->channel_table->insertColumn(1); // value at cursor
  ui->channel_table->insertColumn(2); // unit

  if (json_config->contains("data")) {
    for (auto& channel_name : json_config->operator[]("data")["channel"]) {
      add_channel_name(channel_name);
      label += channel_names_[channel_names_.size() - 1] + "; ";

      QTableWidgetItem * channel_element = new QTableWidgetItem;
      QTableWidgetItem * value_element = new QTableWidgetItem;
      QTableWidgetItem * unit_element = new QTableWidgetItem;

      ui->channel_table->insertRow(i);
      ui->channel_table->setItem(i, 0, channel_element);

      channel_element->setForeground(QBrush(QColor(
        default_inactive_color[0], default_inactive_color[1], default_inactive_color[2])
      ));
      channel_element->setText(
        QString(channel_names_[channel_names_.size() - 1].c_str())
      );

      value_element->setForeground(QBrush(QColor(
        default_inactive_color[0], default_inactive_color[1], default_inactive_color[2])
      ));

      unit_element->setForeground(QBrush(QColor(
        default_inactive_color[0], default_inactive_color[1], default_inactive_color[2])
      ));
      unit_element->setText("[-]");

      ++i;
    }
  }
  ui->channel_table->insertColumn(1);
  ui->data_label->setText(QString(label.c_str()));
}

void WaveformDisplay::update()
{
  int channels_to_plot = get_number_of_channels();

  QPen default_pen(QColor(0, 0, 0, 255));
  default_pen.setWidth(2);

  // plot cursor
  double min_value;
  double max_value;
  double max_channel_value;
  double min_channel_value;

  for (int i = 0; i < channels_to_plot; ++i) {
    // create curve object
    string id = get_channel_name(i);

    if (data_->exists(id)) {
        QwtPlotCurve * curve = new QwtPlotCurve;
        data::Channel * c = data_->get(id);
        size_t n = c->length();

        QPen pen = default_pen;
        vector<int> color = color_order[i];
        pen.setColor(QColor(color[0], color[1], color[2], color[3]));
        curve->setPen(pen);

        // obtain an x channel from data?
        double * xData = new double[n];
        for (size_t i = 0; i < n; ++i) { xData[i] = i; }

        // attach curve to graphic
        curve->setRawSamples(xData, c->begin(), n);
        curve->attach(this);

        // update data table
        for (int j = 0; j < 3; ++j) {
          QTableWidgetItem * element = ui->channel_table->itemAt(i, j);
          element->setForeground(QBrush(QColor(color[0], color[1], color[2])));
        }
        replot();

        max_channel_value = c->max();
        if (i == 0) { max_value = max_channel_value; }
        else { if ( max_channel_value > max_value ) { max_value = max_channel_value; } }

        min_channel_value = c->min();
        if (i == 0) { min_value = min_channel_value; }
        else { if ( min_channel_value > min_value ) { min_value = min_channel_value; } }
    }
  }

  QwtPlotCurve * cursor = new QwtPlotCurve;
  double x_cursor_data[2]{0, 0};
  double y_cursor_data[2]{min_value, max_value};

  cursor->setSamples(x_cursor_data, y_cursor_data, 2);
  replot();
}

void WaveformDisplay::reset()
{
    detachItems();
    replot();
}


}  // namespace insight
