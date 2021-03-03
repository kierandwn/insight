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
    : m_parent(parent),
      m_label(parent)
{
    m_zero_line.setPen(QColor(0, 0, 0, 100));
    m_label.setText(QString("Hello World."));
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
    double * xdata = new double[n];
    double * ydata = new double[n];
    
    double ymax = channel->max();
    double ymin = channel->min();
    double ymean = 0.5 * (ymin + ymax);
    
    for (size_t i = 0; i < n; ++i) {
        xdata[i] = i;
        ydata[i] = m_normalised_yoffset + \
             m_normalised_height * ((channel->operator[](i) - ymean) / (ymax - ymin));
    }
    m_curve.setSamples(xdata, ydata, n);
    
    double xdata_0line[2]{ xdata[0], xdata[n-1] };
    double ydata_0line[2]{
        m_normalised_yoffset + m_normalised_height * (ymean / (ymax - ymin)),
        m_normalised_yoffset + m_normalised_height * (ymean / (ymax - ymin))
    };
    m_zero_line.setSamples(xdata_0line, ydata_0line, 2);
    
    QwtScaleMap plot_to_widget_xcoords = m_parent->canvasMap(m_parent->xBottom);
    QwtScaleMap plot_to_widget_ycoords = m_parent->canvasMap(m_parent->yLeft);
    
    int label_xcoord = m_parent->x() + 5;
    int label_ycoord = m_parent->y() + m_normalised_yoffset * m_parent->height();
    
    m_label.setText(QString(m_channel_name.c_str()));
    m_label.setGeometry(label_xcoord, label_ycoord, 100, 20);
    
    delete[] xdata;
    delete[] ydata;
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
//    cout << "height: " << m_waveform_groups[i]->m_normalised_height << endl;
//    cout << "offset: " << m_waveform_groups[i]->m_normalised_yoffset << endl;

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
  replot();
}

void WaveformDisplay::reset()
{
    detachItems();
    replot();
}

}  // namespace graphic
}  // namespace insight
