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
#include "waveformgroup.h"

#include <string>
#include <cmath>

#include <QLabel>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "defaults.h"


namespace insight {
namespace graphic {


WaveformGroup::WaveformGroup(graphic::Base * parent)
    : p_parent(parent),
      m_label(parent),
      m_metrics(parent)
{
    m_zero_line.setPen(QColor(0, 0, 0, 100));
}

void WaveformGroup::set_dimensions(double normalised_height, double normalised_yoffset) {
    m_normalised_height  = normalised_height;
    m_normalised_yoffset = normalised_yoffset;
}

void WaveformGroup::add_channel(string channel_id) {
  m_channel_names.push_back(channel_id);
  m_unit_strings.push_back("-");
  m_curves.push_back(new QwtPlotCurve);
}

void WaveformGroup::attach(QwtPlot * plot_area)
{
  for (size_t i = 0; i < m_curves.size(); ++i)
    m_curves[i]->attach(plot_area);
  
  m_zero_line.attach(plot_area);
}

void WaveformGroup::set_data_from_table(data::Table * table,
                                        double x_lbound, double x_hbound) {
  size_t n_waveforms = m_channel_names.size();
  
  double ymax, ymin, ymean;
  bool plotted = false;
    
  for (size_t i = 0; i < n_waveforms; ++i) {
    if (!channel_and_time_present_in(m_channel_names[i], table)) continue;
    
    plotted = true;
    string c_name = m_channel_names[i];
    data::Channel * channel = table->get(c_name);
    size_t n = channel->length();
    
    m_unit_strings[i] = channel->get_unit_string();
    
    double * ydata = new double[n];
    
    if (i == 0) {
      ymin = channel->min();
      ymax = channel->max();
      ymean = 0.5 * (ymin + ymax);
    }
    
    data::Channel * xchannel = channel->get_time_ref();
    if (!xchannel) continue; // TODO: is this necessary?
    
    x_lbound = max({x_lbound, xchannel->min()});
    x_hbound = min({x_hbound, xchannel->max()});
    
    double * xdata = channel->get_time_data_ptr();
    
    bool below_lbound = true; bool below_hbound = true;
    int i_lbound = n - 1; int i_hbound = n - 1;
    
    for (size_t i = 0; i < n; ++i) {
      if (xdata[i] < x_lbound) {
        continue;
      } else if (xdata[i] > x_hbound) {
        if (below_hbound) {
          i_hbound = i;
          below_hbound = false;
        }
        continue;
      } else {
        if (below_lbound) {
          i_lbound = i;
          below_lbound = false;
        }
      }
      
      ydata[i] = m_normalised_yoffset + \
        m_normalised_height * ((channel->operator[](i) - ymean) / (ymax - ymin));
    }
    int n_to_plot = i_hbound - i_lbound;
    if (n_to_plot < 2) { n_to_plot = 2; i_hbound = i_lbound + 1; }
    
    m_curves[i]->setSamples(&xdata[i_lbound], &ydata[i_lbound], n_to_plot);
    delete[] ydata;
  }
  
  if (plotted) {
    set_metric_values(ymin, ymax, ymean);
    
    m_xlim[0] = x_lbound; m_xlim[1] = x_hbound;
    m_ylim[0] = ymin; m_ylim[1] = ymax;
    
    set_zero_line_position(x_lbound, x_hbound);
  }
}

void WaveformGroup::init_curves()
{
  init_label();
  set_zero_line_position();
  
  // set color & attach curves to plot
  vector<int> color;
  for (size_t j = 0; j < m_curves.size(); ++j) {
    color = kDefaultColorOrder[j];
    QPen pen(QColor(color[0], color[1], color[2], color[3]));
    m_curves[j]->setPen(pen);
  }
  attach(p_parent);
}

void WaveformGroup::set_label_position() {
  int label_xcoord = 5;
//  int label_ycoord = (1. - m_normalised_yoffset - m_normalised_height / 2) * p_parent->height();
  int label_ycoord = p_parent->painter_coordy_from_axis_scale(m_normalised_yoffset + m_normalised_height / 2);
  
  int metrics_xcoord = p_parent->width() - (m_metrics.sizeHint().width() + 5);
  int metrics_ycoord = p_parent->painter_coordy_from_axis_scale(m_normalised_yoffset) - 12;
  
  m_label.setGeometry(label_xcoord, label_ycoord, m_label.sizeHint().width(),
                                                  m_label.sizeHint().height() );
  
  m_metrics.setGeometry(metrics_xcoord, metrics_ycoord, m_metrics.sizeHint().width(),
                                                        m_metrics.sizeHint().height() );
}

void WaveformGroup::init_label()
{
  m_label.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
  m_metrics.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
  m_metrics.setAlignment(Qt::AlignRight);
  
  init_metric_values();
  set_label_values_at();
}

void WaveformGroup::set_label_values_at(double xvalue, data::Table * table) {
  int channel_names_total_length = 0;
  int unit_strings_total_length = 0;

  size_t n_channels = m_channel_names.size();
  for (size_t i = 0; i < n_channels; ++i)
  {
    channel_names_total_length += m_channel_names[i].length();
    unit_strings_total_length += m_unit_strings[i].length();
  }
  size_t total_label_text_len = \
    channel_names_total_length + unit_strings_total_length + (n_channels * 66) + 1;
  
  char * label_text = new char[total_label_text_len];
  
  int string_cursor = 0;
  for (size_t i = 0; i < n_channels; ++i) {
    string channel_name = m_channel_names[i];
      
    double value;
    vector<int> color;
    
    if (table && channel_and_time_present_in(channel_name, table)) {
      value = table->get(channel_name)->value_at(xvalue);
      color = kDefaultColorOrder[i];
      
    } else {
      value = 0.;
      color = kDefaultInactiveColor;
    }
    
    sprintf(&label_text[string_cursor],
      "%s: <span style=\"color : rgb(%03d, %03d, %03d);\">%*.2f[%s];</span><br/>",
          channel_name.c_str(),
          color[0],
          color[1],
          color[2],
          7, value,
          m_unit_strings[i].c_str()
    );
    string_cursor += m_channel_names[i].length()+m_unit_strings[i].length()+67;
  }
  
  m_label.setText(QString(label_text));
  delete[] label_text;
  
  set_label_position();
}

void WaveformGroup::init_metric_values() {
  m_metrics.setText("mean \xE2\x88\x88 [min., max.[unit]];");
}

void WaveformGroup::set_metric_values(double min, double max, double mean) {
  char metric_text[37+m_unit_strings[0].length()];
  
  sprintf(metric_text,
    "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[%s]];",
      mean, min, max, m_unit_strings[0].c_str()
  );
  m_metrics.setText(QString::fromUtf8(metric_text));
}

void WaveformGroup::set_zero_line_position(double xmin, double xmax) {
  double xdata_0line[2];
  xdata_0line[0] = xmin;
  xdata_0line[1] = xmax;
  
  double ydata_0line[2]{
    m_normalised_yoffset,
    m_normalised_yoffset
  };
  m_zero_line.setSamples(xdata_0line, ydata_0line, 2);
}

void WaveformGroup::set_zero_line_position() {
  double xdata_0line[2];
  xdata_0line[0] = p_parent->axisScaleDiv(p_parent->xBottom).lowerBound();
  xdata_0line[1] = p_parent->axisScaleDiv(p_parent->xBottom).upperBound();
  
  double ydata_0line[2]{
    m_normalised_yoffset,
    m_normalised_yoffset
  };
  m_zero_line.setSamples(xdata_0line, ydata_0line, 2);
}

bool WaveformGroup::any_channel_present_in(data::Table * data) {
  for (string channel_name : m_channel_names) {
    if (data->exists(channel_name)) return true;
  }
  return false;
}

bool channel_and_time_present_in(string channel_name, data::Table * data) {
  return data->exists(channel_name) && data->get(channel_name)->get_time_ref();
}

}  // namespace graphic
}  // namespace insight
