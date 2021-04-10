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

#include "vlabel.h"
#include "table.h"

namespace insight {
namespace graphic {

using namespace std;


class DisplayCrosshair {
 private:
  QwtPlot * p_parent;
  
  QwtPlotCurve m_horzbar;
  QwtPlotCurve m_vertbar;
  
  QwtPlotCurve m_centrepoint;
  QwtSymbol m_centresymbol;
  
  QLabel m_xlabel;
  VLabel m_ylabel;
  
  string m_xchannel_name;
  string m_ychannel_name;
  
  double m_xy[2];
    
 public:
  DisplayCrosshair(QwtPlot * parent, string xchannel_name, string ychannel_name)
      : p_parent(parent),
        m_xlabel(parent),
        m_ylabel(parent),
        m_xchannel_name(xchannel_name),
        m_ychannel_name(ychannel_name)
  {
    m_centresymbol.setStyle(QwtSymbol::Ellipse);
    m_centresymbol.setSize(QSize(4, 4));
    
    m_centrepoint.setSymbol(&m_centresymbol);
    
    m_xlabel.setAlignment(Qt::AlignRight);
    m_ylabel.setAlignment(Qt::AlignLeft);
    
    attach(parent);
  }
  
  void attach(QwtPlot * parent) {
    m_horzbar.attach(parent);
    m_vertbar.attach(parent);
    m_centrepoint.attach(parent);
  }
  
  void detach() {
    m_horzbar.detach();
    m_vertbar.detach();
    m_centrepoint.detach();
    
    m_xlabel.setText("");
    m_ylabel.setText("");
  }
  
  void set_color(int r=0, int g=0, int b=0) {
    int darken = -30;
    
//    QPen color(QColor(r+darken, g+darken, b+darken, 250));
    QPen color(QColor(r, g, b, 250));
    color.setWidth(1.5);
      
    m_horzbar.setPen(color);
    m_vertbar.setPen(color);
    
    m_centrepoint.setPen(color);
    m_centresymbol.setPen(color);
    
    char stylesheet[62];
    sprintf(stylesheet,
      "QLabel { color : rgb(%03d, %03d, %03d); font : 10pt 'Courier'; }", r, g, b);
    
    m_xlabel.setStyleSheet(stylesheet);
    m_ylabel.setStyleSheet(stylesheet);
  }
    
  void set_xy(double, double, double *, double *);
  void set_label_values(double, double);
  
  double x() { return m_xy[0]; }
  double y() { return m_xy[1]; }
};

class ScatterGroup {
 private:
  QwtPlot * p_parent;

  QwtPlotCurve m_scatter;
  QwtPlotCurve m_shadow;
  
  QwtSymbol m_symbol;
  QwtSymbol m_shadow_symbol;
  
  data::Channel * m_xchannel;
  data::Channel * m_ychannel;
  
  DisplayCrosshair m_crosshair;

  double m_xlim[2]{ -10e-12, 10e12 };
  double m_ylim[2]{ -10e-12, 10e12 };
  double m_tlim[2]{ -10e-12, 10e12 };

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
  double * tlim();

  void set_data_from_table(data::Table *, double=-10e12, double=10e12,
                                          double=-10e12, double=10e12,
                                          double=-10e12, double=10e12 );
  
  DisplayCrosshair * crosshair() { return &m_crosshair; }
  void update_crosshair(double);
  void update_crosshair();
  
  QwtPlotCurve * get_scatter_full() { return &m_shadow; }
  
  bool channels_present_in(data::Table *);
};

}  // namespace graphic
}  // namespace insight
#endif // SCATTER_GROUP_H_
