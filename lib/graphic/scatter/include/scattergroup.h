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
#ifndef SCATTER_GROUP_H_
#define SCATTER_GROUP_H_
#pragma once

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include "table.h"

namespace insight {
namespace graphic {

using namespace std;

class ScatterGroup {
 private:
  QwtPlot * p_parent;

  QwtPlotCurve m_scatter;
  QwtPlotCurve m_shadow;
  
  QwtSymbol m_symbol;
  QwtSymbol m_shadow_symbol;

  double m_xlim[2]; double m_ylim[2];

  string m_xchannel_name;
  string m_ychannel_name;
  
  int m_color_index;

 public:
  ScatterGroup(QwtPlot *, string, string, int);
  ~ScatterGroup();

  void init_scatters();
  void init_labels(data::Table *);

  string get_xchannel_name() { return m_xchannel_name; }
  string get_ychannel_name() { return m_ychannel_name; }
    
  void set_label_values_at(double, data::Table *);
  void set_metric_values(double, double, double);

  double * xlim();
  double * ylim();

  void set_data_from_table(data::Table *, double=-10e12, double=10e12);
  
  QwtPlotCurve * get_scatter_full() { return &m_shadow; }
};

}  // namespace graphic
}  // namespace insight
#endif // SCATTER_GROUP_H_
