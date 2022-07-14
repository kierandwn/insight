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
#ifndef INSIGHT_SCATTER_CROSSHAIR_H_
#define INSIGHT_SCATTER_CROSSHAIR_H_
#pragma once

//#include <scatterdisplay.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include "qlabel.h"
#include "vlabel.h"
//#include "table.h"

#include "defaults.h"

namespace insight {
namespace graphic {

using namespace std;


class Crosshair
{
 private:
  QwtPlotCurve m_horzbar;
  QwtPlotCurve m_vertbar;
  
  QwtPlotCurve m_centrepoint;
  QwtSymbol m_centresymbol;
  
  double m_xy[2];
 
 public:
  Crosshair()
  {
    m_centresymbol.setStyle(QwtSymbol::Ellipse);
    m_centresymbol.setSize(QSize(4, 4));
    
    m_centrepoint.setSymbol(&m_centresymbol);
  }
  
  void attach(QwtPlot * parent)
  {
    m_horzbar.attach(parent);
    m_vertbar.attach(parent);
    m_centrepoint.attach(parent);
  }
  
  void detach() {
    m_horzbar.detach();
    m_vertbar.detach();
    m_centrepoint.detach();
  }
  
  void set_color(int r=0, int g=0, int b=0)
  {
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
  }
  
  void set_xy(double, double, double *, double *);
  
  double x() { return m_xy[0]; }
  double y() { return m_xy[1]; }
};

class CrosshairGroup
{
 private:
  QwtPlot * p_parent;
  
  vector<Crosshair *> m_crosshairs;
  
  QLabel m_xlabel;
  VLabel m_ylabel;
  
  string m_xchannel_name;
  string m_ychannel_name;
  
  string m_xchannel_unit_string = "-";
  string m_ychannel_unit_string = "-";
  
  vector<vector<double>> m_xy;
  double m_xbounds[2];
  double m_ybounds[2];
  
  vector<vector<int>> m_colors;
  
  int m_nlayers = 0;
    
 public:
  CrosshairGroup(QwtPlot * parent, string xchannel_name, string ychannel_name)
      : p_parent(parent),
        m_xlabel(parent),
        m_ylabel(parent),
        m_xchannel_name(xchannel_name),
        m_ychannel_name(ychannel_name)
  {
    m_xlabel.setStyleSheet("QLabel { color : rgb(050, 050, 050); font : 10pt 'Courier'; }");
    m_ylabel.setStyleSheet("QLabel { color : rgb(050, 050, 050); font : 10pt 'Courier'; }");
    
    render_labels();
  }
  
  void attach(int layer)
  {
    m_crosshairs[layer]->attach(p_parent);
  }
  
  void detach(int layer)
  {
    m_crosshairs[layer]->detach();
  }
  
  void add_layer()
  {
    m_crosshairs.push_back(new Crosshair());
    m_crosshairs[m_nlayers]->attach(p_parent);
    
    m_xy.push_back(vector<double>{0., 0.});
    m_colors.push_back(kDefaultInactiveColor);
    ++m_nlayers;
  }
  
  void set_color(int layer, int r=0, int g=0, int b=0)
  {
    m_colors[layer][0] = r;
    m_colors[layer][1] = g;
    m_colors[layer][2] = b;
    
    m_crosshairs[layer]->set_color(r, g, b);
  }
  
//  void position_labels();
  void render_labels();
  
  void set_xy(double, double, int);
  void set_xy(double, double, int, double *, double *);
  void set_label_values(int, double, double);
  
  double x(int layer) { return m_xy[layer][0]; }
  double y(int layer) { return m_xy[layer][1]; }
  double& xy(int layer) { return m_xy[layer][0]; }
  
  int len() { return m_nlayers; }
  
  void set_xchannel_unit_string(string s) { m_xchannel_unit_string = s; }
  void set_ychannel_unit_string(string s) { m_ychannel_unit_string = s; }
  
  void update_plot_limits(double *, double *);
};


}  // namespace graphic
}  // namespace insight
#endif // INSIGHT_SCATTER_CROSSHAIR_H_
