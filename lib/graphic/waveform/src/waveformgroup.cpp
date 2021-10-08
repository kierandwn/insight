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
    m_curves.push_back(vector<QwtPlotCurve * >());
    m_zero_line.setPen(QColor(0, 0, 0, 100));
}

void WaveformGroup::set_dimensions(double normalised_height, double normalised_yoffset)
{
    m_normalised_height  = normalised_height;
    m_normalised_yoffset = normalised_yoffset;
}

void WaveformGroup::add_channel(string channel_id)
{
  m_channel_names.push_back(channel_id);
  m_unit_strings.push_back("-");
  
  for (size_t layer = 0; layer < m_curves.size(); ++layer)
    m_curves[layer].push_back(new QwtPlotCurve);
}

void WaveformGroup::add_layer()
{
  size_t n_curves = m_curves[0].size();
  size_t layer = m_curves.size();
  
  m_curves.push_back(vector<QwtPlotCurve *>());
  m_zero_points_x.push_back(0.);
  
  for (size_t curve = 0; curve < n_curves; ++curve)
    m_curves[layer].push_back(new QwtPlotCurve);
  
  reformat();
  attach(layer);
}

void WaveformGroup::attach(int layer)
{
  for (size_t curve = 0; curve < m_curves[layer].size(); ++curve)
    m_curves[layer][curve]->attach(p_parent);
  
  m_zero_line.attach(p_parent);
}

void WaveformGroup::set_data_from_table(data::Table * table,
                                        double x_lbound0, double x_hbound0)
{
  double x_lbound, x_hbound;
  
  int n_layers = table->get_number_of_layers();
  size_t n_waveforms = m_channel_names.size();
  
  double ymax, ymin, ymean;
  bool plotted = false;
  
  double x0, xf;
  
  for (size_t i_waveform = 0; i_waveform < n_waveforms; ++i_waveform)
  {
    string channel_name = m_channel_names[i_waveform];
    
    for (int i_layer = 0; i_layer < n_layers; ++i_layer)
    {
      if (!channel_and_time_present_in(m_channel_names[i_waveform], table, i_layer)) continue;
      plotted = true;
      
      data::Channel * channel = table->get(channel_name, i_layer);
      size_t n = channel->length();
      
      data::Channel * xchannel = channel->get_time_ref();
      if (!xchannel) continue; // TODO: is this necessary?
      
      m_unit_strings[i_waveform] = channel->get_unit_string(); // TODO: if match, use unit_string, otherwise use "-"
      
      double * xdata = new double[n];
      double * ydata = new double[n];
      
      if (i_waveform == 0)
      {
        ymin = channel->min();
        ymax = channel->max();
        ymean = 0.5 * (ymin + ymax);
      }
      
      x0 = table->get(m_channel_names[i_waveform], i_layer)->get_time_ref()->operator[](0);
      xf = table->get(m_channel_names[i_waveform], i_layer)->get_time_ref()->operator[](n - 1);
      
      if (m_zeroed_xdomain)
      {
        x_lbound = max({x_denormalised(x_lbound0, i_layer), x0});
        x_hbound = min({x_denormalised(x_hbound0, i_layer), xf});
      }
      else
      {
        x_lbound = max({x_lbound0, x0});
        x_hbound = min({x_hbound0, xf});
      }
      
      if (!m_zeroed_xdomain)
        xdata = channel->get_time_data_ptr();
      
      bool below_lbound = true; bool below_hbound = true;
      int i_lbound = n - 1; int i_hbound = n - 1;
      
      double xi;
      
      for (size_t i = 0; i < n; ++i)
      {
        xi = xchannel->operator[](i);
          
        if (xi < x_lbound)
          continue;

        else if (xi > x_hbound)
        {
          if (below_hbound)
          {
            i_hbound = i;
            below_hbound = false;
          }
          continue;
        }
        else
        {
          if (below_lbound)
          {
            i_lbound = i;
            below_lbound = false;
          }
        }
        
        if (m_zeroed_xdomain)
          xdata[i] = x_normalised(xi, i_layer);
        
        ydata[i] = m_normalised_yoffset + \
          m_normalised_height * ((channel->operator[](i) - ymean) / (ymax - ymin)); // TODO: fix ->operator[]() ick
      }
      int n_to_plot = i_hbound - i_lbound;
      if (n_to_plot < 2) { n_to_plot = 2; i_hbound = i_lbound + 1; }
      
      m_curves[i_layer][i_waveform]->setSamples(&xdata[i_lbound], &ydata[i_lbound], n_to_plot);
      
      delete[] ydata;
      if (m_zeroed_xdomain)
        delete[] xdata;
    }
  }
  if (plotted)
    set_metric_values(ymin, ymax, ymean);
  
  set_zero_line_position(x_lbound0, x_hbound0);
}

void WaveformGroup::init()
{
  init_label();
  set_zero_line_position(0., 1.);
  
  reformat();
  attach(0);
}

void WaveformGroup::reformat()
{
  // set color & attach curves to plot
  vector<int> color;
  for (size_t layer = 0; layer < m_curves.size(); ++layer)
  {
    for (size_t curve = 0; curve < m_curves[0].size(); ++curve)
    {
      color = determine_line_color(curve, layer);
      
      QPen pen(QColor(color[0], color[1], color[2], color[3]));
      m_curves[layer][curve]->setPen(pen);
    }
  }
}

vector<int> WaveformGroup::determine_line_color(int curve, int layer)
{
  size_t n_layers = m_curves.size();
  bool is_multi_layer = n_layers > 1;
  
  if (is_multi_layer)
    return kDefaultColorOrder[layer];
  else
    return kDefaultColorOrder[curve];
}

void WaveformGroup::set_label_position()
{
  int label_xcoord = 5;
  int label_ycoord = p_parent->painter_coordy_from_axis_scale(m_normalised_yoffset - m_normalised_height / 2) - m_label.sizeHint().height();
  
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

void WaveformGroup::set_label_values_at(double xpos, data::Table * table)
{
  // note the distiction between xpos and xvalue: xpos is the position on the normalised xdomain where the cursor
  // is positioned. xvalue is the corresponding denormalised value on the absolute domain.
  int channel_names_total_length = 0;
  int unit_strings_total_length = 0;

  size_t n_channels = m_channel_names.size();
  int n_layers = p_parent->get_data_table_ref()->get_number_of_layers();
  
  for (size_t i = 0; i < n_channels; ++i)
  {
    channel_names_total_length += m_channel_names[i].length();
    unit_strings_total_length += m_unit_strings[i].length();
  }
  size_t total_label_text_len = 1000; // \ // TODO: make this math work
  //  channel_names_total_length + unit_strings_total_length + (n_channels * 60) + (n_layers * 7) + 1;
  
  char * label_text = new char[total_label_text_len];
  
  int string_cursor = 0;
  
  for (size_t i = 0; i < n_channels; ++i)
  {
    string channel_name = m_channel_names[i];
    
    sprintf(&label_text[string_cursor], "%s: ", channel_name.c_str());
    string_cursor += channel_name.length() + 2;
    
    for (int layer = 0; layer < n_layers; ++layer)
    {
      double value;
      vector<int> color;
      
      if (table && channel_and_time_present_in(channel_name, table, layer))
      {
        value = table->get(channel_name, layer)->value_at(x_denormalised(xpos, layer));
        color = determine_line_color(i, layer);
        
      }
      else
      {
        continue;
//        value = 0.;
//        color = kDefaultInactiveColor;
      }
      
      sprintf(&label_text[string_cursor],
        "<span style=\"color : rgb(%03d, %03d, %03d);\">%*.2f[%s]; </span>",
            color[0],
            color[1],
            color[2],
            7, value,
            m_unit_strings[i].c_str()
      );
      string_cursor += m_unit_strings[i].length() + 60;
    }
    sprintf(&label_text[string_cursor], "<br/>");
    string_cursor += 5;
  }
  m_label.setText(QString(label_text));
  delete[] label_text;
  
  set_label_position();
}

void WaveformGroup::init_metric_values()
{
  m_metrics.setText("mean \xE2\x88\x88 [min., max.[unit]];");
}

void WaveformGroup::set_metric_values(double min, double max, double mean)
{
  char metric_text[37+m_unit_strings[0].length()];
  
  sprintf(metric_text,
    "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[%s]];",
      mean, min, max, m_unit_strings[0].c_str()
  );
  m_metrics.setText(QString::fromUtf8(metric_text));
}

void WaveformGroup::set_zero_line_position(double xmin, double xmax)
{
  double xdata_0line[2];
  xdata_0line[0] = xmin;
  xdata_0line[1] = xmax;

  double ydata_0line[2]{
    m_normalised_yoffset,
    m_normalised_yoffset
  };
  m_zero_line.setSamples(xdata_0line, ydata_0line, 2);
}

bool WaveformGroup::any_channel_present_in(data::Table * data)
{
  for (size_t layer = 0; layer < m_curves.size(); ++layer)
  {
    for (string channel_name : m_channel_names)
    {
      if (data->exists_in_layer(channel_name, layer))
        return true;
    }
  }
  return false;
}

void WaveformGroup::get_xlimits_in_data(double * xlimits)
{
  data::Table * data = p_parent->get_data_table_ref();
  
  int n_layers = data->get_number_of_layers();
  vector<string> channel_names = get_channel_names();
  
  double xmin =  10e13;
  double xmax = -10e13;
  
  for (string& channel_name : channel_names)
  {
    for (int layer = 0; layer < n_layers; ++layer)
    {
      data::Channel * xchannel = data->get(channel_name, layer)->get_time_ref();
      
      double current_xchannel_min = xchannel->min();
      double current_xchannel_max = xchannel->max();
      
      xmin = min({xmin, current_xchannel_min});
      xmax = max({xmax, current_xchannel_max});
    }
  }
  xlimits[0] = xmin;
  xlimits[1] = xmax;
}

bool channel_and_time_present_in(string channel_name, data::Table * data, int layer)
{
  return data->exists_in_layer(channel_name, layer) && data->get(channel_name, layer)->get_time_ref();
}



}  // namespace graphic
}  // namespace insight
