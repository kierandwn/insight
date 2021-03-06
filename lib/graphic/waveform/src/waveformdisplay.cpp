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
#include "grid.h"

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
    m_curves.push_back(new QwtPlotCurve);
}

void WaveformGroup::attach(QwtPlot * plot_area)
{
    for (size_t i = 0; i < m_curves.size(); ++i) {
      m_curves[i]->attach(plot_area);
    }
    m_zero_line.attach(plot_area);
}

void WaveformGroup::set_data_from_table(data::Table * table) {
    size_t n_waveforms = m_channel_names.size();
    
    double ymax, ymin, ymean;
    double xmax, xmin;
    
    for (size_t i = 0; i < n_waveforms; ++i) {
        data::Channel * channel = table->get(m_channel_names[i]);
        size_t n = channel->length();
        
        double * ydata = new double[n];
        
        if (i == 0) {
            ymin = channel->min();
            ymax = channel->max();
            ymean = 0.5 * (ymin + ymax);
        }
        
        data::Channel * xchannel = channel->get_time_ref();
        if (xchannel->max() > xmax) { xmax = xchannel->max(); }
        else if (xchannel->min() < xmin) { xmin = xchannel->min(); }
        
        for (size_t i = 0; i < n; ++i) {
            ydata[i] = m_normalised_yoffset + \
                 m_normalised_height * ((channel->operator[](i) - ymean) / (ymax - ymin));
        }
        double * xdata = channel->get_time_data_ptr();
        
        m_curves[i]->setSamples(xdata, ydata, n);
        delete[] ydata;
    }
    
    double xdata_0line[2]{ xmin, xmax };
    double ydata_0line[2]{
        m_normalised_yoffset,
        m_normalised_yoffset
    };
    m_zero_line.setSamples(xdata_0line, ydata_0line, 2);
    
    init_label(table);
    set_metric_values(ymin, ymax, ymean);
}

void WaveformGroup::init_curves() {
  // draw curve on graphic
  vector<int> color;
  for (size_t j = 0; j < m_curves.size(); ++j) {
    color = kDefaultColorOrder[j];
    QPen pen(QColor(color[0], color[1], color[2], color[3]));
    m_curves[j]->setPen(pen);
  }
  attach(p_parent);
}

void WaveformGroup::init_label(data::Table * table) {
    int label_xcoord = 5;
    int label_ycoord = (1. - m_normalised_yoffset) * p_parent->height();
    
    int metrics_xcoord = p_parent->width() - (300 + 5);
    int metrics_ycoord = ((1. - m_normalised_yoffset) * p_parent->height()) - 16;
    
    m_label.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
    m_label.setGeometry(label_xcoord, label_ycoord, 300, 30);
    
    m_metrics.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
    m_metrics.setGeometry(metrics_xcoord, metrics_ycoord, 300, 30);
    m_metrics.setAlignment(Qt::AlignRight);

    set_label_values_at(0., table);
}

void WaveformGroup::set_label_values_at(double xvalue, data::Table * table) {
    int channel_names_total_length = 0;
    size_t n_channels = m_channel_names.size();
    for (size_t i = 0; i < n_channels; ++i) {
        channel_names_total_length += m_channel_names[i].length();
    }
    char * label_text = new char[channel_names_total_length+(n_channels*67)+1];
    
    int string_cursor = 0;
    for (size_t i = 0; i < n_channels; ++i) {
        string channel_name = m_channel_names[i];
        
        double value;
        if (table->exists(channel_name)) {
          value = table->get(channel_name)->value_at(xvalue);
        } else {
          value = 0.;
        }
        
        sprintf(&label_text[string_cursor],
            "%s: <span style=\"color : rgb(%03d, %03d, %03d);\">%*.2f[-];</span><br/>",
                channel_name.c_str(),
                kDefaultColorOrder[i][0],
                kDefaultColorOrder[i][1],
                kDefaultColorOrder[i][2],
                7, value
        );
        string_cursor += m_channel_names[i].length()+67;
    }
    
    m_label.setText(QString(label_text));
    delete[] label_text;
}

void WaveformGroup::set_metric_values(double min, double max, double mean) {
  char metric_text[38];
  
  sprintf(metric_text,
      "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[-]];",
        mean, min, max
  );
  m_metrics.setText(QString::fromUtf8(metric_text));
}

WaveformDisplay::WaveformDisplay(data::Table * data, layout::Layout * layout)
    : Base(data),
      p_layout(layout),
      m_xlabel(this)
{
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

void WaveformDisplay::init_xlabel() {
    int label_width = 300;
    int label_height = 15;
    
    m_xlabel.setStyleSheet(
        "QLabel { color : rgb(50, 50, 50); font : 10pt 'Courier'; }"
    );
    m_xlabel.setAlignment(Qt::AlignRight);
    set_xlabel_value(0.);
    
    m_xlabel.setGeometry(
        width() - (label_width + 5),
        height() - 1.02 * label_height,
        label_width,
        label_height
    );
}
                   
void WaveformDisplay::set_xlabel_value(double value) {
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
}

void WaveformDisplay::update()
{
  int channels_to_plot = get_number_of_waveform_groups();

  for (int i = 0; i < channels_to_plot; ++i) {
    m_waveform_groups[i]->set_data_from_table(m_data);
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
        m_waveform_groups[i]->set_label_values_at(x, m_data);
    }
    set_xlabel_value(x);
}

void WaveformDisplay::mousePressEvent(QMouseEvent * event)
{
    QwtScaleMap map = canvasMap(xBottom);
    double xval = map.invTransform(event->x());
    
    update_group_cursor_positions(xval);
}

void WaveformDisplay::update_group_cursor_positions(double xval) { // TODO: create intermediate subclass of Base: LinkedGraphic
    if ("" == m_group_name) return;
    
    map<string, graphic::Base *>::iterator graphic_itr = p_layout->first();
    graphic::Base * graphic_ptr;
    
    while (graphic_itr != p_layout->last()) {
      graphic_ptr = graphic_itr->second;
      if (graphic_ptr->group() == m_group_name) graphic_ptr->update_cursor_position(xval);
      ++graphic_itr;
    }
}

void WaveformDisplay::reset()
{
    detachItems();
    replot();
}

}  // namespace graphic
}  // namespace insight
