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

#include <string>
#include <cmath>

#include <QMouseEvent>
#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "insight_graphic_base.h"

namespace insight {
namespace graphic {


// TODO: move to base graphics descriptions (source file?)
vector<int> kDefaultInactiveColor {150, 150, 150, 180};
vector<vector<int>> kDefaultColorOrder{
  {0,    114, 189, 180},
  {217, 83, 250, 180},
  {237, 177, 32, 180},
  {126, 47, 142, 180},
  {119, 172, 48, 180},
  {77, 190, 238, 180},
  {162, 199, 47, 180}
};

WaveformGroup::WaveformGroup(QwtPlot * parent)
    : p_parent(parent),
      m_label(parent)
{
    m_zero_line.setPen(QColor(0, 0, 0, 100));
}

void WaveformGroup::set_dimensions(double normalised_height, double normalised_yoffset) {
    m_normalised_height  = normalised_height;
    m_normalised_yoffset = normalised_yoffset;
}

void WaveformGroup::add_channel(string channel_id) {
    m_channel_name = channel_id;
}

void WaveformGroup::attach(QwtPlot * plot_area)
{
    m_curve.attach(plot_area);
    m_zero_line.attach(plot_area);
}

void WaveformGroup::set_data_from_channel(data::Channel * channel) {
    size_t n = channel->length();
    
    // obtain an x channel from data?
    double * ydata = new double[n];
    
    double ymax = channel->max();
    double ymin = channel->min();
    double ymean = 0.5 * (ymin + ymax);
    
    for (size_t i = 0; i < n; ++i) {
        ydata[i] = m_normalised_yoffset + \
             m_normalised_height * ((channel->operator[](i) - ymean) / (ymax - ymin));
    }
    double * xdata = channel->get_time_data_ptr();
    
    m_curve.setSamples(xdata, ydata, n);
    
    double xdata_0line[2]{ xdata[0], xdata[n-1] };
    double ydata_0line[2]{
        m_normalised_yoffset,
        m_normalised_yoffset
    };
    m_zero_line.setSamples(xdata_0line, ydata_0line, 2);
    
    int label_xcoord = 5;
    int label_ycoord = (1. - m_normalised_yoffset) * p_parent->height();
    
    m_label.setText(QString(m_channel_name.c_str()));
    m_label.setGeometry(label_xcoord, label_ycoord, 300, 30);
    
    delete[] ydata;
}

void WaveformGroup::set_label_value(double value) {
    char * label_text = new char[m_channel_name.size()+13];
    sprintf(label_text, "%s: %.2f [-];", m_channel_name.c_str(), value);
    
    m_label.setText(QString(label_text));
    delete[] label_text;
}

void WaveformGroup::set_label_color(int r, int g, int b) {
    char label_stylesheet[59];
    sprintf(label_stylesheet, "QLabel { color : rgb(%d, %d, %d); font : 10pt 'Courier'; }", r, g, b);
    m_label.setStyleSheet(label_stylesheet);
}

WaveformDisplay::WaveformDisplay(data::Table * data) : Base(data) {
  p_ui->setupUi(this);

  setAutoFillBackground( true );
  QPalette p = palette();

  p.setColor(QPalette::Window, QColor(255, 255, 255, 255));
  setPalette(p);
    
  QPen dark_grey(QColor(0, 0, 0, 250));
  dark_grey.setWidth(2);
  m_cursor.setPen(dark_grey);

  enableAxis(xBottom, false);
  enableAxis(yLeft, false);
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
    for (auto& channel_name : json_config->operator[]("data")["channel"]) {
      WaveformGroup * waveform_group = new WaveformGroup(this);
      
      waveform_group->add_channel(channel_name);
      m_waveform_groups.push_back(waveform_group);
      
      ++i;
    }
  }
  m_nwaveform_groups = i;
  define_uniform_spacing();
}

void WaveformDisplay::update()
{
  int channels_to_plot = get_number_of_waveform_groups();

//  QPen default_pen(QColor(0, 0, 0, 255));
//  default_pen.setWidth(2);

  for (int i = 0; i < channels_to_plot; ++i) {
    // create curve object
    string id = m_waveform_groups[i]->get_channel_name();

    if (m_data->exists(id)) {
        vector<int> color = kDefaultColorOrder[0];
        QPen pen(QColor(color[0], color[1], color[2], color[3]));
        
        data::Channel * channel = m_data->get(id);
        m_waveform_groups[i]->set_data_from_channel(channel);
        m_waveform_groups[i]->set_label_color(color[0], color[1], color[2]);
        
        // draw curve on graphic
        QwtPlotCurve * curve = m_waveform_groups[i]->get_curve_ref();
        curve->setPen(pen);
        
        m_waveform_groups[i]->attach(this);
    }
  }
  update_cursor_position(0.);
  m_cursor.attach(this);
  replot();
}

void WaveformDisplay::update_cursor_position(double x) {
    double xcursor[2]{ x, x };
    double ycursor[2]{ 0., 1. };
    
    update_label_values_at(x);
    
    m_cursor.setSamples(xcursor, ycursor, 2);
    replot();
}

void WaveformDisplay::update_label_values_at(double x) {
    for (int i = 0; i < m_nwaveform_groups; ++i) {
        string channel_name = m_waveform_groups[i]->get_channel_name();
        data::Channel * channel = m_data->get(channel_name);
        
        double value = channel->value_at(x);
        m_waveform_groups[i]->set_label_value(value);
    }
}

void WaveformDisplay::mousePressEvent(QMouseEvent * event)
{
    QwtScaleMap map = canvasMap(xBottom);
    double x_coord = map.invTransform(event->x());
    update_cursor_position(x_coord);
}

void WaveformDisplay::reset()
{
    detachItems();
    replot();
}

}  // namespace graphic
}  // namespace insight
