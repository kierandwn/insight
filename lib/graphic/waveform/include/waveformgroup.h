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
#ifndef WAVEFORM_GROUP_H_
#define WAVEFORM_GROUP_H_
#pragma once

#include <vector>
#include <string>

#include <QLabel>
#include <QWidget>

#include <qwt_plot_curve.h>

#include "insight_graphic_base.h"
#include "table.h"


namespace insight {
namespace graphic {

using namespace std;

class WaveformGroup
{
 private:
  graphic::Base * p_parent;
    
  vector<vector<QwtPlotCurve *>> m_curves;
  QwtPlotCurve m_zero_line;
  
  QLabel m_label;
  QLabel m_metrics;
    
  bool m_zeroed_xdomain = false;
  vector<double> m_zero_points_x{0.};
    
  vector<string> m_channel_names;
  vector<string> m_unit_strings;
  
  double m_normalised_height;
  double m_normalised_yoffset;

 public:
  WaveformGroup(graphic::Base *);
    
  void init();
  void reformat();
  
  void init_label();
  void init_metric_values();
  
  vector<int> determine_line_color(int, int);
  
  void set_label_position();
  void set_number_of_layers(size_t);
    
  void add_channel(string);
  void add_layer();
  
  void set_dimensions(double, double);
    
  void set_label_values_at(double=0., data::Table * data=NULL);
  void set_metric_values(double, double, double);
  
  void set_zero_line_position(double, double);
  
  bool get_xlimits_in_data(double *, bool=false);
  
  double x_normalised(double x, int layer) { return x - m_zero_points_x[layer]; }
  double x_denormalised(double x, int layer) { return x + m_zero_points_x[layer]; }
    
  string get_channel_name(int i) { return m_channel_names[i]; }
  vector<string> get_channel_names() { return m_channel_names; }
  
  QwtPlotCurve * get_curve_ref(int channel, int layer) { return m_curves[layer][channel]; }
    
  void attach(int);
  void set_data_from_table(data::Table *, double=-10e12, double=10e12);
  
  void set_x_zero_value(double zero_value, int layer) { m_zero_points_x[layer] = zero_value; }
  
  bool any_channel_present_in(data::Table *);
  
  double get_normalised_plot_position() { return m_normalised_yoffset; }
  double get_normalised_plot_height()   { return m_normalised_height; }
  
  void set_normalised_plot_height(double height) { m_normalised_height = height; }
};

bool channel_and_time_present_in(string, data::Table *, int);


}  // namespace graphic
}  // namespace insight
#endif // WAVEFORM_GROUP_H_
