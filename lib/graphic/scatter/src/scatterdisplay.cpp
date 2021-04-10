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
// along with insight. If not, see <https://www.gnu.org/licenses/>.
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
      m_mean_xlabel(this),
      m_mean_ylabel(this)
{
  p_ui->setupUi(this);

  enableAxis(xBottom, false);
  enableAxis(yLeft, false);
}

void ScatterDisplay::init_labels() {
  set_label_positions();
  
  m_mean_xlabel.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
  m_mean_ylabel.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
  m_mean_xlabel.setAlignment(Qt::AlignRight);
  m_mean_ylabel.setAlignment(Qt::AlignLeft);
}

void ScatterDisplay::set_label_positions() {
  int label_width = 200;
  int label_height = 15;

  m_mean_xlabel.setGeometry(
    width() - (label_width + 5),
    height() - (label_height + 5),
    label_width,
    label_height
  );
  m_mean_ylabel.setGeometry(
    5,
    5,
    label_height,
    label_width
  );
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
  init_mean_lines();
  
  descriptive_mean_labels();
  
  for (size_t i = 0; i < m_scatter_pairs.size(); ++i) {
    m_scatter_pairs[i]->init_scatters();
  }
//  update_cursor_position(0.);
  init_cursor_position();
}

void ScatterDisplay::init_mean_lines()
{
  m_mean_xline.setPen(QColor(0, 0, 0, 100));
  m_mean_xline.attach(this);
  
  m_mean_yline.setPen(QColor(0, 0, 0, 100));
  m_mean_yline.attach(this);
  
  // Get axes limits from axes objects (Qwt)
  double x_lbound = axisScaleDiv(xBottom).lowerBound();
  double x_hbound = axisScaleDiv(xBottom).upperBound();
  double y_lbound = axisScaleDiv(yLeft).lowerBound();
  double y_hbound = axisScaleDiv(yLeft).upperBound();
  
  double xaxes_bounds[2]{ x_lbound, x_hbound };
  double yaxes_bounds[2]{ y_lbound, y_hbound };
  
  double xmid = (x_lbound + x_hbound) / 2.;
  double ymid = (y_lbound + y_hbound) / 2.;

  double xdata_xline[2]{xmid, xmid};
  double ydata_yline[2]{ymid, ymid};
  
  m_mean_xline.setSamples(xdata_xline, yaxes_bounds, 2);
  m_mean_yline.setSamples(xaxes_bounds, ydata_yline, 2);
}

void ScatterDisplay::descriptive_mean_labels()
{
  m_mean_xlabel.setText("mean \xE2\x88\x88 [min., max.[unit]];");
  m_mean_ylabel.setText("mean \xE2\x88\x88 [min., max.[unit]];");
}

void ScatterDisplay::update_mean_lines()
{
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

  if (!(xlim(xlimits) && ylim(ylimits))) {
    descriptive_mean_labels();
    return;
  }
  
  double xmean = (xlimits[0] + xlimits[1]) / 2.;
  double ymean = (ylimits[0] + ylimits[1]) / 2.;
  
  double xdata_xline[2]{xmean, xmean};
  double ydata_yline[2]{ymean, ymean};
  
  m_mean_xline.setSamples(xdata_xline, yaxes_bounds, 2);
  m_mean_yline.setSamples(xaxes_bounds, ydata_yline, 2);
  
  char metrics_label_text[38];
  sprintf(metrics_label_text,
      "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[-]];",
        xmean, xlimits[0], xlimits[1]
  );
  m_mean_xlabel.setText(QString::fromUtf8(metrics_label_text));

  sprintf(&metrics_label_text[0],
      "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[-]];",
        ymean, ylimits[0], ylimits[1]
  );
  m_mean_ylabel.setText(QString::fromUtf8(metrics_label_text));
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
    
  update_cursor_position();
  replot();
}

void ScatterDisplay::update_view_limits(double tmin, double tmax)
{
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    m_scatter_pairs[i]->set_data_from_table(m_data, tmin, tmax);
  }
  replot();
}

bool ScatterDisplay::xlim(double * xbounds) {
  xbounds[0] = 10e12; xbounds[1] = -10e12;
  bool scatter_active = false;
  
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    if (!m_scatter_pairs[i]->channels_present_in(m_data)) continue;
    scatter_active = true;
    
    xbounds[0] = min({m_scatter_pairs[i]->xlim()[0], xbounds[0]});
    xbounds[1] = max({m_scatter_pairs[i]->xlim()[1], xbounds[1]});
  }
  return scatter_active;
}

bool ScatterDisplay::ylim(double * ybounds) {
  ybounds[0] = 10e12; ybounds[1] = -10e12;
  bool scatter_active = false;
  
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    if (!m_scatter_pairs[i]->channels_present_in(m_data)) continue;
    scatter_active = true;
    
    ybounds[0] = min(m_scatter_pairs[i]->ylim()[0], ybounds[0]);
    ybounds[1] = max(m_scatter_pairs[i]->ylim()[1], ybounds[1]);
  }
  return scatter_active;
}

void ScatterDisplay::reset()
{
  detachItems();
  replot();
}

void ScatterDisplay::resizeEvent(QResizeEvent * event) {
  Base::resizeEvent(event);
  
  set_label_positions();
  update_mean_lines();
  
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    string xchannel_name = m_scatter_pairs[0]->get_xchannel_name();
    string ychannel_name = m_scatter_pairs[0]->get_ychannel_name();
    
    if (m_data->exists(xchannel_name) && m_data->exists(ychannel_name)) { // TODO move this check inside update_crosshair?
      m_scatter_pairs[i]->update_crosshair();
    }
  }
  replot();
}

}  // namespace graphic
}  // namespace insight
