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

#include "insight_graphic_base.h"
#include "vlabel.h"
#include "table.h"

namespace insight {
namespace graphic {

using namespace std;


class DisplayCrosshair {
 private:
  Base * p_parent;
  
  QwtPlotCurve m_horzbar;
  QwtPlotCurve m_vertbar;
  
  QwtPlotCurve m_centrepoint;
  QwtSymbol m_centresymbol;
  
  QLabel m_xlabel;
  VLabel m_ylabel;
  
  string m_xchannel_name;
  string m_ychannel_name;
  
  string m_xchannel_unit_string = "-";
  string m_ychannel_unit_string = "-";
  
  double m_xy[2];
    
 public:
  DisplayCrosshair(Base * parent, string xchannel_name, string ychannel_name)
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
  
  void set_xchannel_unit_string(string s) { m_xchannel_unit_string = s; }
  void set_ychannel_unit_string(string s) { m_ychannel_unit_string = s; }
  
  double x() { return m_xy[0]; }
  double y() { return m_xy[1]; }
};

class DataXYGroup {
 protected:
  Base * p_parent;

  QwtPlotCurve m_shadow;
  
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
  DataXYGroup(Base *, string, string, int);
  virtual ~DataXYGroup();

  virtual void init_curves()=0;
  void init_labels(data::Table *);

  string get_xchannel_name() { return m_xchannel_name; }
  string get_ychannel_name() { return m_ychannel_name; }
   
  void set_label_values_at(double, data::Table *);
  void set_metric_values(double, double, double);

  double * xlim();
  double * ylim();
  double * tlim();

  virtual void set_data_from_table(data::Table *, double=-10e12, double=10e12,
                                                  double=-10e12, double=10e12,
                                                  double=-10e12, double=10e12 )=0;

  DisplayCrosshair * crosshair() { return &m_crosshair; }
  void update_crosshair(double);
  void update_crosshair();
  
  void set_xchannel_unit_string(string s) { m_crosshair.set_xchannel_unit_string(s); }
  void set_ychannel_unit_string(string s) { m_crosshair.set_ychannel_unit_string(s); }

  QwtPlotCurve * get_shadow() { return &m_shadow; }
  
  bool channels_present_in(data::Table *);
};

class ScatterGroup : public DataXYGroup {
 protected:
  QwtPlotCurve m_scatter;
  
  QwtSymbol m_symbol;
  QwtSymbol m_shadow_symbol;

 public:
  ScatterGroup(Base * p, string xid, string yid, int i) : DataXYGroup(p, xid, yid, i) {};
  ~ScatterGroup() {};
  
  void init_curves() override;
  void set_data_from_table(data::Table *, double=-10e12, double=10e12,
                                          double=-10e12, double=10e12,
                                          double=-10e12, double=10e12 ) override;
};

class LineGroup : public DataXYGroup {
  QwtPlotCurve m_line;
  
 public:
  LineGroup(Base * p, string xid, string yid, int i) : DataXYGroup(p, xid, yid, i) {};
  ~LineGroup() {};
  
  void init_curves() override;
  void set_data_from_table(data::Table *, double=-10e12, double=10e12,
                                          double=-10e12, double=10e12,
                                          double=-10e12, double=10e12 ) override;
};

}  // namespace graphic
}  // namespace insight
#endif // SCATTER_GROUP_H_
