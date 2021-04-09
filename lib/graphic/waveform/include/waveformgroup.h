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
#ifndef WAVEFORM_GROUP_H_
#define WAVEFORM_GROUP_H_
#pragma once

#include <vector>
#include <string>

#include <QLabel>
#include <QWidget>

#include <qwt_plot_curve.h>

#include "table.h"


namespace insight {
namespace graphic {

using namespace std;

class WaveformGroup {
 private:
  QwtPlot * p_parent;
    
  vector<QwtPlotCurve *> m_curves;
  QwtPlotCurve m_zero_line;
  
  QLabel m_label;
  QLabel m_metrics;
    
  double m_xlim[2]; double m_ylim[2];
    
  vector<string> m_channel_names;

 public:
  double m_normalised_height;
  double m_normalised_yoffset;
    
  WaveformGroup(QwtPlot *);
    
  void init_curves();
  
  void init_label();
  void init_metric_values();
  
  void set_label_position();
    
  void add_channel(string);
  void set_dimensions(double, double);
    
  void set_label_values_at(double=0., data::Table * data=NULL);
  void set_metric_values(double, double, double);
  
  void set_zero_line_position();
  void set_zero_line_position(double, double);
  
  double * xlim() { return m_xlim; }
  double * ylim() { return m_ylim; }
    
  string get_channel_name(int i) { return m_channel_names[i]; }
  QwtPlotCurve * get_curve_ref(int i) { return m_curves[i]; }
    
  void attach(QwtPlot *);
  void set_data_from_table(data::Table *, double=-10e12, double=10e12);
};


}  // namespace graphic
}  // namespace insight
#endif // WAVEFORM_GROUP_H_
