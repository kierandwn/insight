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
#include "waveformdisplay.h"

#include <string>
#include <cmath>

#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "defaults.h"
#include "insight_graphic_base.h"
#include "grid.h"

namespace insight {
namespace graphic {


WaveformDisplay::WaveformDisplay(data::Table * data, layout::Layout * layout)
    : LinkedPlot(data, layout),
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
  char * label_text = new char[17];
  sprintf(label_text, "time: %.3f[s]", value);
  
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
    
    for (int i = 0; i < n_groups; ++i) {
        noffset = ((i * (1.0 + npadding)) + (0.5 + npadding)) * nheight;
        m_waveform_groups[i]->set_dimensions(nheight, noffset);
    }
}

// Apply configuation parameters held in json_config
void WaveformDisplay::apply_config(nlohmann::json * json_config) {
  int i = 0;

  if (json_config->contains("data")) {
    for (auto& channel_names : json_config->operator[]("data")["channel"]) {
      WaveformGroup * waveform_group = new WaveformGroup(this);
      
      for (auto& channel_name : channel_names) {
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

void WaveformDisplay::init() {
    init_xlabel();
    for (size_t i = 0; i < m_waveform_groups.size(); ++i) {
        m_waveform_groups[i]->init_curves();
    }
    m_cursor.attach(this);
}

void WaveformDisplay::update_after_data_load()
{
  int channels_to_plot = get_number_of_waveform_groups();
    
  for (int i = 0; i < channels_to_plot; ++i) {
    m_waveform_groups[i]->set_data_from_table(m_data);
    m_waveform_groups[i]->init_label();
  }
  update_view_limits();
  update_cursor_position(xlim()[0]);
  
  replot();
  m_mouse_state = Ready;
}

void WaveformDisplay::update_view_limits()
{
  cursor_in_xrange();
  double * xlimits = xlim();
  
  setAxisScale(xBottom, xlimits[0], xlimits[1]);
  replot();
  
  delete[] xlimits;
}

void WaveformDisplay::update_view_limits(double xmin, double xmax)
{
  int channels_to_plot = get_number_of_waveform_groups();
    
  for (int i = 0; i < channels_to_plot; ++i) {
    m_waveform_groups[i]->set_data_from_table(m_data, xmin, xmax);
  }
  cursor_in_xrange();
    
  setAxisScale(xBottom, xmin, xmax);
  replot();
}

void WaveformDisplay::update_cursor_position(double x) {
  double xcursor[2]{ x, x };
  double ycursor[2]{ 0., 1. };
  
  cursor_in_xrange();
  update_label_values_at(x);
  
  m_cursor.setSamples(xcursor, ycursor, 2);
  m_xpos_cursor = x;
  replot();
}

void WaveformDisplay::cursor_in_xrange() {
  double * xrange = xlim();
  if (m_xpos_cursor > xrange[0] && m_xpos_cursor < xrange[1]) { m_cursor.attach(this); }
  else { m_cursor.detach(); }
}

double * WaveformDisplay::xlim() {
  double * widest_xbound = new double;
  widest_xbound[0] = 10e12; widest_xbound[1] = -10e12;
//  bool display_active = false;
  
  for (int i = 0; i < m_nwaveform_groups; ++i) {
    if (m_waveform_groups[i]->any_channel_present_in(m_data)) {
//      display_active = true;
      widest_xbound[0] = min({m_waveform_groups[i]->xlim()[0], widest_xbound[0]});
      widest_xbound[1] = max({m_waveform_groups[i]->xlim()[1], widest_xbound[1]});
    }
  }
  return widest_xbound; // KNOWN potential memory leak, returned array needs to be deleted by calling fcn.
}

void WaveformDisplay::update_label_values_at(double x) {
  for (int i = 0; i < m_nwaveform_groups; ++i) {
      m_waveform_groups[i]->set_label_values_at(x, m_data);
  }
  set_xlabel_value(x);
}

void WaveformDisplay::reset()
{
  detachItems();
  replot();
}

void WaveformDisplay::resizeEvent(QResizeEvent * event) {
  Base::resizeEvent(event);

  double * xlimits = xlim();
  
  set_xlabel_position();
  for (int i = 0; i < m_nwaveform_groups; ++i) {
    m_waveform_groups[i]->set_data_from_table(m_data, xlimits[0], xlimits[1]);
    m_waveform_groups[i]->set_label_position();
  }
  
  replot();
}

}  // namespace graphic
}  // namespace insight
