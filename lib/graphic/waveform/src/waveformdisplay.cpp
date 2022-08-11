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
#include "waveformdisplay.h"

#include <string>
#include <cmath>

#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "defaults.h"
#include "ApplicationInterface.h"
#include "grid.h"

namespace insight {
namespace graphic {


WaveformDisplay::WaveformDisplay(data::Table * data, layout::Layout * layout)
    : InsightGraphic(data),
      m_xlabel(this)
{
  p_ui->setupUi(this);
    
  QPen dark_grey(QColor(0, 0, 0, 250));
  dark_grey.setWidth(1.5);
  m_cursor.setPen(dark_grey);

  enableAxis(xBottom, false);
  enableAxis(yLeft, false);
  
  setAxisScale(yLeft, 0., 1.);
}

void WaveformDisplay::init_xlabel()
{
  m_xlabel.setStyleSheet(
    "QLabel { color : rgb(50, 50, 50); font : 10pt 'Courier'; }"
  );
  m_xlabel.setAlignment(Qt::AlignRight);
  set_xlabel_position();
  
  set_xlabel_value(0.);
}

void WaveformDisplay::set_xlabel_position()
{
  int label_width = 300;
  int label_height = 15;
  
  m_xlabel.setGeometry(
    width() - (label_width + 5),
    height() - 1.02 * label_height,
    label_width,
    label_height
  );
}
                   
void WaveformDisplay::set_xlabel_value(double value)
{
  size_t label_len = 12 + m_xchannel_name.length() + m_xchannel_unit_string.length();
  
  char * label_text = new char[label_len];
  sprintf(label_text, "%s: %.3f[%s]", m_xchannel_name.c_str(), value, m_xchannel_unit_string.c_str());
  
  m_xlabel.setText(QString(label_text));
  delete[] label_text;
}

void WaveformDisplay::define_uniform_spacing()
{
  reset();
  double n_groups = get_number_of_waveform_groups();
  
  double nheight  = 1. / (1.1 * n_groups + 0.1);
  double npadding = 0.1; // space betwen plots descibed as factor of plot height above
  double noffset;
  
  for (int i = 0; i < n_groups; ++i)
  {
    noffset = ((i * (1.0 + npadding)) + (0.5 + npadding)) * nheight;
    m_waveform_groups[i]->set_dimensions(nheight, noffset);
  }
}

// Apply configuation parameters held in json_config
void WaveformDisplay::apply_config(nlohmann::json * json_config)
{
  int i = 0;

  if (json_config->contains("data"))
  {
    for (auto& channel_names : json_config->operator[]("data")["channel"])
    {
      WaveformGroup * waveform_group = new WaveformGroup(insight_data_ref_, this);
      
      for (auto& channel_name : channel_names)
      {
        waveform_group->add_channel(channel_name);
      }
      m_waveform_groups.push_back(waveform_group);
      ++i;
    }
    
    if (json_config->contains("group")) {
      m_group_name = json_config->operator[]("group");
    } else {
      m_group_name = "";
    }
  }
  m_nwaveform_groups = i;
  define_uniform_spacing();
}

void WaveformDisplay::init()
{
  init_xlabel();
  for (size_t i = 0; i < m_waveform_groups.size(); ++i)
    m_waveform_groups[i]->init();
  
//  m_cursor.attach(this);
}

void WaveformDisplay::determine_zero_xvalues()
{
  int n_layers = insight_data_ref_->get_number_of_layers();
  
  for (int layer = 0; layer < n_layers; ++layer)
  {
    double x_zero = 10e13;
    
    for (WaveformGroup * group : m_waveform_groups)
    {
      vector<string> channel_names = group->get_channel_names();
      
      for (string& channel_name : channel_names)
      {
        if (insight_data_ref_->exists_in_layer(channel_name, layer))
        {
          data::Channel * xchannel = insight_data_ref_->get(channel_name, layer)->get_time_ref();
          
          double current_xchannel_zero = xchannel->operator[](0);
          x_zero = min({x_zero, current_xchannel_zero});
        }
      }
    }
    
    for (WaveformGroup * group : m_waveform_groups)
      group->set_x_zero_value(x_zero, layer);
    
    m_xzeros[layer] = x_zero;
  }
}

void WaveformDisplay::update_xchannel_data()
{
  int channels_to_plot = get_number_of_waveform_groups();
  if (channels_to_plot > 0) {
    string first_channel_name = m_waveform_groups[0]->get_channel_name(0);
    
    if (insight_data_ref_->exists_in_layer(first_channel_name))
    {
      data::Channel * xchannel = insight_data_ref_->get(first_channel_name)->get_time_ref();
      m_xchannel_unit_string = xchannel->get_unit_string();
    }
  }
}

void WaveformDisplay::update_after_data_load()
{
  int n_layers = insight_data_ref_->get_number_of_layers();
  bool layer_added = n_layers > m_number_of_layers;
  
  m_number_of_layers = n_layers;
  int n_waveform_groups = get_number_of_waveform_groups();
  
  if (layer_added)
    for (int i = 0; i < n_waveform_groups; ++i)
      m_waveform_groups[i]->add_layer();
  
  determine_zero_xvalues();
    
  for (int i = 0; i < n_waveform_groups; ++i)
    m_waveform_groups[i]->set_data_from_table(insight_data_ref_);
  
  get_max_xrange(m_max_xbounds);
  
  update_view_limits(m_max_xbounds[0], m_max_xbounds[1]);

  update_xchannel_data();
  update_cursor_position(x_denormalised(m_max_xbounds[0])); // TODO: ???
  
  replot();
  m_mouse_state = Ready;
}

void WaveformDisplay::update_view_limits(double xmin, double xmax)
{
  xmin = max(xmin, m_max_xbounds[0]);
  xmax = min(xmax, m_max_xbounds[1]);
  
  double xmin_normd = x_normalised(xmin);
  double xmax_normd = x_normalised(xmax);
  
  int channels_to_plot = get_number_of_waveform_groups();
    
  for (int i = 0; i < channels_to_plot; ++i)
    m_waveform_groups[i]->set_data_from_table(insight_data_ref_, xmin, xmax);

  cursor_in_xrange();
    
  setAxisScale(xBottom, xmin_normd, xmax_normd);
  replot();
}

void WaveformDisplay::update_cursor_position(double x)
{
  double xnorm = x_normalised(x);
  
  double xcursor[2]{ xnorm, xnorm };
  double ycursor[2]{ 0., 1. };
  
  cursor_in_xrange();
  update_label_values_at(xnorm);
  
  m_cursor.setSamples(xcursor, ycursor, 2);
  m_xpos_cursor = xnorm;
  replot();
}

void WaveformDisplay::cursor_in_xrange()
{
  double lbound = axisScaleDiv(xBottom).lowerBound();
  double hbound = axisScaleDiv(xBottom).upperBound();
  
  if (m_xpos_cursor > lbound && m_xpos_cursor < hbound)
  {
//    m_cursor.attach(this);
  }
  else
    m_cursor.detach();
}

bool WaveformDisplay::xlim(double * limits)
{
  limits[0] = axisScaleDiv(xBottom).lowerBound();
  limits[1] = axisScaleDiv(xBottom).upperBound();
  
//  limits[0] = 10e12; limits[1] = -10e12;
  bool display_active = false;
  
  for (int i = 0; i < m_nwaveform_groups; ++i)
  {
    if (m_waveform_groups[i]->any_channel_present_in(insight_data_ref_))
    {
      display_active = true;
      break;
    }
  }
  return display_active;
}

void WaveformDisplay::get_max_xrange(double * xlimits)
{
  xlimits[0] =  10e13;
  xlimits[1] = -10e13;
  
  bool plot_active = false;
  
  for (WaveformGroup * group : m_waveform_groups)
  {
    if (group->any_channel_present_in(insight_data_ref_))
    {
      plot_active = true;
      
      double current_group_xlimits[2];
      group->get_xlimits_in_data(current_group_xlimits);
      
      xlimits[0] = min({xlimits[0], current_group_xlimits[0]});
      xlimits[1] = max({xlimits[1], current_group_xlimits[1]});
    }
  }
  if (!plot_active)
  {
    xlimits[0] = 0.;
    xlimits[1] = 1.;
  }
}

void WaveformDisplay::update_label_values_at(double x)
{
  for (int i = 0; i < m_nwaveform_groups; ++i)
      m_waveform_groups[i]->set_label_values_at(x, insight_data_ref_);
  
  set_xlabel_value(x);
}

void WaveformDisplay::reset()
{
  detachItems();
  replot();
}

void WaveformDisplay::resizeEvent(QResizeEvent * event)
{
  QwtPlot::resizeEvent(event);

  double xlimits[2];
  xlim(xlimits);
  
  set_xlabel_position();
  for (int i = 0; i < m_nwaveform_groups; ++i)
  {
    m_waveform_groups[i]->set_data_from_table(insight_data_ref_);
    m_waveform_groups[i]->set_label_position();
  }
  
  replot();
}

}  // namespace graphic
}  // namespace insight
