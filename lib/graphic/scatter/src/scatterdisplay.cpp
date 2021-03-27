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
// along with insight.  If not, see <https://www.gnu.org/licenses/>.
//
#include "scatterdisplay.h"

#include <string>
#include <cmath>

#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>

#include <qwt_plot.h>
#include <qwt_plot_item.h>

#include "scattergroup.h"
#include "defaults.h"
#include "vlabel.h"

#include "grid.h"

namespace insight {
namespace graphic {


ScatterDisplay::ScatterDisplay(data::Table * data, layout::Layout * layout)
    : LinkedPlot(data, layout),
      m_data(data),
      m_xlabel(this),
      m_ylabel(this),
      m_mean_xlabel(this),
      m_mean_ylabel(this)
{
  p_ui->setupUi(this);

  setAutoFillBackground( true );
  QPalette p = palette();

  p.setColor(QPalette::Window, QColor(255, 255, 255, 255));
  setPalette(p);

  enableAxis(xBottom, false);
  enableAxis(yLeft, false);
}

void ScatterDisplay::init_labels() {
    int label_width = 500;
    int label_height = 15;

    m_xlabel.setStyleSheet(
        "QLabel { color : rgb(50, 50, 50); font : 10pt 'Courier'; }"
    );
    m_ylabel.setStyleSheet(
        "QLabel { color : rgb(50, 50, 50); font : 10pt 'Courier'; }"
    );
    
    m_xlabel.setAlignment(Qt::AlignRight);
    m_ylabel.setAlignment(Qt::AlignLeft);
    
    m_xlabel.setGeometry(
        width() - (label_width + 5),
        height() - (label_height + 5),
        label_width,
        label_height
    );
    m_ylabel.setGeometry(
        5,
        5,
        label_height,
        label_width
    );
    set_label_values_at(0.);
    
    m_mean_xlabel.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
    m_mean_ylabel.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
    m_mean_xlabel.setAlignment(Qt::AlignRight);
}

void ScatterDisplay::set_label_values_at(double tvalue)
{
  // determine channel names
  string xchannel_name, ychannel_name;
  int xchannel_names_total_length = 0;
  int ychannel_names_total_length = 0;
  
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    xchannel_name = m_scatter_pairs[i]->get_xchannel_name();
    ychannel_name = m_scatter_pairs[i]->get_ychannel_name();
      
    xchannel_names_total_length += xchannel_name.length();
    ychannel_names_total_length += ychannel_name.length();
  }
    
  char * xlabel_text = new char[xchannel_names_total_length+(m_nscatter_pairs*62)+1];
  char * ylabel_text = new char[ychannel_names_total_length+(m_nscatter_pairs*62)+1];
  int xstring_cursor = 0;
  int ystring_cursor = 0;
    
  for (int i = 0; i < m_nscatter_pairs; ++i) {
      xchannel_name = m_scatter_pairs[i]->get_xchannel_name();
      ychannel_name = m_scatter_pairs[i]->get_ychannel_name();
      vector<int> color = kDefaultColorOrder[i];
      
      double xvalue, yvalue;
      if (m_data->exists(xchannel_name) && m_data->exists(ychannel_name)) {
        xvalue = m_data->get(xchannel_name)->value_at(tvalue);
        yvalue = m_data->get(ychannel_name)->value_at(tvalue);
      } else {
        xvalue = 0.;
        yvalue = 0.;
      }
      
      sprintf(&xlabel_text[xstring_cursor],
          "%s: <span style=\"color : rgb(%03d, %03d, %03d);\">%*.2f[-];</span>",
              xchannel_name.c_str(),
              color[0],
              color[1],
              color[2],
              7, xvalue
      );
      sprintf(&ylabel_text[ystring_cursor],
          "%s: <span style=\"color : rgb(%03d, %03d, %03d);\">%*.2f[-];</span>",
              ychannel_name.c_str(),
              color[0],
              color[1],
              color[2],
              7, yvalue
      );
    
      xstring_cursor += xchannel_name.length()+62;
      ystring_cursor += ychannel_name.length()+62;
  }
    
  m_xlabel.setText(QString(xlabel_text));
  m_ylabel.setText(QString(ylabel_text));

  delete[] xlabel_text;
  delete[] ylabel_text;
}

// Apply configuation parameters held in json_config
void ScatterDisplay::apply_config(nlohmann::json * json_config) {
  int i = 0;

  if (json_config->contains("data")) {
    for (auto& channel_pair : json_config->operator[]("data")["xychannels"]) {
      ScatterGroup * scatter_pair = new ScatterGroup(this, channel_pair[0], channel_pair[1], i);
      m_scatter_pairs.push_back(scatter_pair);
      ++i;
    }
    
    if (json_config->contains("group")) {
      m_group_name = json_config->operator[]("group");
    } else {
      m_group_name = "";
    }
  }
  m_nscatter_pairs = i;
}

void ScatterDisplay::init() {
    init_labels();
    for (size_t i = 0; i < m_scatter_pairs.size(); ++i) {
        m_scatter_pairs[i]->init_scatters();
    }
    
    m_mean_xline.setPen(QColor(0, 0, 0, 100));
    m_mean_xline.attach(this);
    
    m_mean_yline.setPen(QColor(0, 0, 0, 100));
    m_mean_yline.attach(this);
    
    update_cursor_position(0.);
    m_crosshair.attach(this);
}

void ScatterDisplay::update_mean_lines() {
    replot();
    
    // Get axes limits from axes objects (Qwt)
    double x_lbound = axisScaleDiv(xBottom).lowerBound();
    double x_hbound = axisScaleDiv(xBottom).upperBound();
    double y_lbound = axisScaleDiv(yLeft).lowerBound();
    double y_hbound = axisScaleDiv(yLeft).upperBound();
    
    double xaxes_bounds[2]{ x_lbound, x_hbound };
    double yaxes_bounds[2]{ y_lbound, y_hbound };
  
    double xlimits[2];
    double ylimits[2];
  
    xlim(xlimits);
    ylim(ylimits);
    
    double xmean = (xlimits[0] + xlimits[1]) / 2.;
    double ymean = (ylimits[0] + ylimits[1]) / 2.;
    double xrange = (abs(xlimits[0]) + abs(xlimits[1]));
    double yrange = (abs(ylimits[0]) + abs(ylimits[1]));
    
    double xdata_xline[2]{xmean, xmean};
    double ydata_yline[2]{ymean, ymean};
    
    m_mean_xline.setSamples(xdata_xline, yaxes_bounds, 2);
    m_mean_yline.setSamples(xaxes_bounds, ydata_yline, 2);
    
    // metrics labels
    int label_width = 200;
    int label_height = 15;
    
    char metrics_label_text[38];
    sprintf(metrics_label_text,
        "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[-]];",
          xmean, xlimits[0], xlimits[1]
    );
    
    m_mean_xlabel.setText(QString::fromUtf8(metrics_label_text));
    m_mean_xlabel.setGeometry(
        ((xmean - xlimits[0]) / xrange) * width() + label_height,
        5,
        label_height,
        label_width
    );
  
    sprintf(&metrics_label_text[0],
        "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[-]];",
          ymean, ylimits[0], ylimits[1]
    );
    
    m_mean_ylabel.setText(QString::fromUtf8(metrics_label_text));
    m_mean_ylabel.setGeometry(
        width() - (label_width + 5),
        ((ylimits[1] - ymean) / yrange) * height() - label_height,
        label_width,
        label_height
    );
}

void ScatterDisplay::update_after_data_load()
{
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    m_scatter_pairs[i]->set_data_from_table(m_data);
  }
  update_mean_lines();
  
  double xlimits[2];
  double ylimits[2];
  
  xlim(xlimits);
  ylim(ylimits);
  
  setAxisScale(xBottom, xlimits[0], xlimits[1]);
  setAxisScale(yLeft, ylimits[0], ylimits[1]);
    
  update_cursor_position(xlimits[0]);
  replot();
}

void ScatterDisplay::update_view_limits(double xmin, double xmax)
{
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    m_scatter_pairs[i]->set_data_from_table(m_data, xmin, xmax);
  }
  replot();
}

void ScatterDisplay::xlim(double * xbounds) {
    xbounds[0] = 10e12; xbounds[1] = -10e12;
    
    for (int i = 0; i < m_nscatter_pairs; ++i) {
        xbounds[0] = min({m_scatter_pairs[i]->xlim()[0], xbounds[0]});
        xbounds[1] = max({m_scatter_pairs[i]->xlim()[1], xbounds[1]});
    }
}

void ScatterDisplay::ylim(double * ybounds) {
    ybounds[0] = 10e12; ybounds[1] = -10e12;
    
    for (int i = 0; i < m_nscatter_pairs; ++i) {
        ybounds[0] = min(m_scatter_pairs[i]->ylim()[0], ybounds[0]);
        ybounds[1] = max(m_scatter_pairs[i]->ylim()[1], ybounds[1]);
    }
}

void ScatterDisplay::reset()
{
    detachItems();
    replot();
}

}  // namespace graphic
}  // namespace insight
