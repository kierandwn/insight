// Copyright (c) 2020 Kieran Downie. All rights reserved.
//
// This file is part of insight.
//
// insight is free software : you can redistribute it and /
// or modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// insight is distributed in the hope that it will be useful,
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


DataXYDisplay::DataXYDisplay(data::Table * data, layout::Layout * layout)
    : InsightGraphic(data),
      m_mean_xlabel(this),
      m_mean_ylabel(this)
{
  p_ui->setupUi(this);

  enableAxis(xBottom, false);
  enableAxis(yLeft, false);
}

void DataXYDisplay::init_labels()
{
  set_label_positions();
  
  m_mean_xlabel.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
  m_mean_ylabel.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
  m_mean_xlabel.setAlignment(Qt::AlignRight);
  m_mean_ylabel.setAlignment(Qt::AlignLeft);
}

void DataXYDisplay::set_label_positions()
{
  int label_width = 200;
  int label_height = 15;

  m_mean_xlabel.setGeometry(
    0.5 * (width() - m_mean_xlabel.sizeHint().width()),
    5,
    m_mean_xlabel.sizeHint().width(),
    m_mean_xlabel.sizeHint().height()
  );
  m_mean_ylabel.setGeometry(
    width() - m_mean_ylabel.width() - 5,
    0.5 * (height() - m_mean_ylabel.sizeHint().height()),
    m_mean_ylabel.sizeHint().width(),
    m_mean_ylabel.sizeHint().height()
  );
}

void DataXYDisplay::init()
{
  init_labels();
  init_mean_lines();
  
  for (size_t i = 0; i < m_data_curves.size(); ++i)
    m_data_curves[i]->init();
  
  init_cursor_position();
}

void DataXYDisplay::init_cursor_position()
{
  double x_lbound = axisScaleDiv(xBottom).lowerBound();
  double x_hbound = axisScaleDiv(xBottom).upperBound();
  double y_lbound = axisScaleDiv(yLeft).lowerBound();
  double y_hbound = axisScaleDiv(yLeft).upperBound();

  double x_bounds[2]{ x_lbound, x_hbound };
  double y_bounds[2]{ y_lbound, y_hbound };
  
  double xrange = x_hbound - x_lbound;
  double yrange = y_hbound - y_lbound;
  
//  for (int i = 0; i < m_ncurves; ++i) {
//    m_data_curves[i]->crosshair()->set_xy(x_lbound + (i + 1) * .05 * xrange,
//                                            y_lbound + (i + 1) * .05 * yrange,
//                                            x_bounds,
//                                            y_bounds
//                                            );
//  }
  replot();
}

void DataXYDisplay::init_mean_lines()
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
  
  descriptive_mean_labels();
}

void DataXYDisplay::descriptive_mean_labels()
{
  m_mean_xlabel.setText("mean \xE2\x88\x88 [min., max.[unit]];");
  m_mean_ylabel.setText("mean \xE2\x88\x88 [min., max.[unit]];");
}

void DataXYDisplay::update_after_data_load()
{
  int n_layers = insight_data_ref_->get_number_of_layers();
  bool layer_added = n_layers > m_nlayers;
  
  m_nlayers = n_layers;
  
  if (layer_added)
    for (int i = 0; i < m_ncurves; ++i)
      m_data_curves[i]->add_layer();
  
  for (int curve = 0; curve < m_ncurves; ++curve)
    m_data_curves[curve]->set_data_from_table();

  update_plot_limits();
  
  update_axes_unit_strings();
  update_cursor_position();
  update_mean_lines();
  
//  update_axes_scales();
  replot();
}

void DataXYDisplay::update_view_limits(double tmin, double tmax)
{
  for (int curve = 0; curve < m_ncurves; ++curve)
    m_data_curves[curve]->set_data_from_table(tmin, tmax);
  
  update_axes_scales();
  replot();
}

void DataXYDisplay::update_axes_scales()
{
  double xlimits[2];
  double ylimits[2];
  if (!(xlim(xlimits) && ylim(ylimits)))
    return;
  
  double xpadding = m_padding_scalar * (xlimits[1] - xlimits[0]);
  double ypadding = m_padding_scalar * (ylimits[1] - ylimits[0]);
  
  setAxisScale(xBottom, xlimits[0] - xpadding, xlimits[1] + xpadding);
  setAxisScale(yLeft,   ylimits[0] - ypadding, ylimits[1] + ypadding);
  replot();
}

void DataXYDisplay::set_summary_stats_labels(double xmedian,
                                            double xmin,
                                            double xmax,
                                            double ymedian,
                                            double ymin,
                                            double ymax)
{
  char * xaxes_stats_text = new char[37+m_xaxis_unit_string.length()];
  char * yaxes_stats_text = new char[37+m_yaxis_unit_string.length()];
  
  sprintf(xaxes_stats_text,
      "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[%s]];",
        xmedian, xmin, xmax, m_xaxis_unit_string.c_str()
  );
  m_mean_xlabel.setText(QString::fromUtf8(xaxes_stats_text));

  sprintf(yaxes_stats_text,
      "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[%s]];",
        ymedian, ymin, ymax, m_yaxis_unit_string.c_str()
  );
  m_mean_ylabel.setText(QString::fromUtf8(yaxes_stats_text));

  set_label_positions();
}

void DataXYDisplay::update_axes_unit_strings()
{
  if (m_ncurves < 1) return;
  
  string xchannel_name;
  string ychannel_name;
  
  // if ncurves == 1, use unit strings stored in channel object
  if (m_data_curves[0]->channels_and_time_present_for_any_layer())
  {
    xchannel_name = m_data_curves[0]->get_xchannel_name();
    ychannel_name = m_data_curves[0]->get_ychannel_name();
    
    m_xaxis_unit_string = insight_data_ref_->get(xchannel_name)->get_unit_string();
    m_yaxis_unit_string = insight_data_ref_->get(ychannel_name)->get_unit_string();
  }
  
  // if n_curves > 1, check that unit strings are the same, otherwise, use '-'
  if (m_ncurves > 1)
  {
    string xaxis_unit_string_to_test;
    string yaxis_unit_string_to_test;
    
    for (int i = 0; i < m_ncurves; ++i) {
      if (m_data_curves[i]->channels_and_time_present_for_any_layer())
      {
        xchannel_name = m_data_curves[i]->get_xchannel_name();
        ychannel_name = m_data_curves[i]->get_ychannel_name();
        
        xaxis_unit_string_to_test = insight_data_ref_->get(xchannel_name)->get_unit_string();
        yaxis_unit_string_to_test = insight_data_ref_->get(ychannel_name)->get_unit_string();
        
        if (!(xaxis_unit_string_to_test == m_xaxis_unit_string))
          m_xaxis_unit_string = '-';
        
        if (!(yaxis_unit_string_to_test == m_yaxis_unit_string))
          m_yaxis_unit_string = '-';
      }
    }
  }
  
//  m_xaxis_unit_string
}

bool DataXYDisplay::xlim(double * xbounds)
{
  xbounds[0] = 10e12; xbounds[1] = -10e12;
  bool scatter_active = false;
  
  for (int i = 0; i < m_ncurves; ++i)
  {
    if (!m_data_curves[i]->channels_and_time_present_for_any_layer())
      continue;
    
    scatter_active = true;
    
    xbounds[0] = min({m_data_curves[i]->xlim()[0], xbounds[0]});
    xbounds[1] = max({m_data_curves[i]->xlim()[1], xbounds[1]});
  }
  return scatter_active;
}

bool DataXYDisplay::ylim(double * ybounds)
{
  ybounds[0] = 10e12; ybounds[1] = -10e12;
  bool scatter_active = false;
  
  for (int i = 0; i < m_ncurves; ++i)
  {
    if (!m_data_curves[i]->channels_and_time_present_for_any_layer())
      continue;
    
    scatter_active = true;
    
    ybounds[0] = min(m_data_curves[i]->ylim()[0], ybounds[0]);
    ybounds[1] = max(m_data_curves[i]->ylim()[1], ybounds[1]);
  }
  return scatter_active;
}

void DataXYDisplay::reset()
{
  detachItems();
  replot();
}

void DataXYDisplay::resizeEvent(QResizeEvent * event)
{
  QwtPlot::resizeEvent(event);
  
  set_label_positions();
  update_mean_lines();
  
  for (int i = 0; i < m_ncurves; ++i)
  {
    string xchannel_name = m_data_curves[0]->get_xchannel_name();
    string ychannel_name = m_data_curves[0]->get_ychannel_name();
    
    if (insight_data_ref_->exists_in_layer(xchannel_name) && insight_data_ref_->exists_in_layer(ychannel_name)) // TODO move this check inside update_crosshair?
      m_data_curves[i]->update_crosshair();
  }
  replot();
}

// Apply configuation parameters held in json_config
void DataXYDisplay::apply_config(nlohmann::json * json_config)
{
  int i = 0;

  if (json_config->contains("data"))
  {
    for (auto& channel_pair : json_config->operator[]("data")["xychannels"])
    {
      m_xchannel_names.push_back(channel_pair[0]);
      m_ychannel_names.push_back(channel_pair[1]);
      
//      ScatterGroup * scatter_pair = new ScatterGroup(this, channel_pair[0], channel_pair[1], i);
//      m_data_curves.push_back(scatter_pair);
      ++i;
    }
    
    if (json_config->contains("group"))
    {
      m_group_name = json_config->operator[]("group");
    }
    else
    {
      m_group_name = "";
    }
  }
  m_ncurves = i;
  init_curves();
}

void ScatterDisplay::init_curves()
{
  for (int i = 0; i < m_ncurves; ++i)
    m_data_curves.push_back(new ScatterGroup(get_data_table_ref(), this, m_xchannel_names[i], m_ychannel_names[i])); // TODO: free this memory?
  
}

void LineDisplay::init_curves()
{
  for (int i = 0; i < m_ncurves; ++i)
    m_data_curves.push_back(new LineGroup(get_data_table_ref(), this, m_xchannel_names[i], m_ychannel_names[i]));
  
}

void DataXYDisplay::update_mean_lines()
{
  double xlimits[2];
  double ylimits[2];
  
  if (!get_total_data_ranges(xlimits, ylimits))
  {
    descriptive_mean_labels();
    return;
  }
  
  double x_indicated_value;
  double y_indicated_value;
  
  if (m_indicator_lines_at_zero)
  {
    x_indicated_value = 0.;
    y_indicated_value = 0.;
  }
  else
  {
    x_indicated_value = (xlimits[0] + xlimits[1]) / 2.;
    y_indicated_value = (ylimits[0] + ylimits[1]) / 2.;
  }
  
  double xdata_xline[2]{x_indicated_value, x_indicated_value};
  double ydata_yline[2]{y_indicated_value, y_indicated_value};
  
  m_mean_xline.setSamples(xdata_xline, ylimits, 2);
  m_mean_yline.setSamples(xlimits, ydata_yline, 2);
  
  set_summary_stats_labels(x_indicated_value, xlimits[0], xlimits[1], y_indicated_value, ylimits[0], ylimits[1]);
}

bool DataXYDisplay::get_total_data_ranges(double * xrange, double * yrange)
{
  vector<string> x_channel_names;
  vector<string> y_channel_names;
  
  for (int i = 0; i < m_data_curves.size(); ++i)
  {
    x_channel_names.push_back(m_data_curves[i]->get_xchannel_name());
    y_channel_names.push_back(m_data_curves[i]->get_ychannel_name());
  }
  if (!(insight_data_ref_->get_max_bounds_in_data(x_channel_names, xrange) &&
        insight_data_ref_->get_max_bounds_in_data(y_channel_names, yrange)))
    return false;
  
  else
    return true;
}

}  // namespace graphic
}  // namespace insight
