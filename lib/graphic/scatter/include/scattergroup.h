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
#ifndef SCATTER_GROUP_H_
#define SCATTER_GROUP_H_
#pragma once

//#include "scatterdisplay.h"
#include "crosshair.h"

#include <qwt_plot.h>

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>

//#include "ApplicationInterface.h"
#include "vlabel.h"
#include "table.h"

#include "defaults.h"

namespace insight {
namespace graphic {

using namespace std;

struct DataInfo
{
 public:
  DataInfo(string xchannel_name, string ychannel_name)
    : xchannel_name(xchannel_name),
      ychannel_name(ychannel_name)
  {}
                    
  string xchannel_name;
  string ychannel_name;
};

class GraphicInfo
{
 protected:
  QwtSymbol m_symbol_inrange;
  QwtSymbol m_symbol_outrange;
  
  QwtPlotCurve m_curve_inrange;
  QwtPlotCurve m_curve_outrange;
  
  QPen m_pen_inrange;
  QPen m_pen_outrange;
  
  int m_inrange_size   = 1;
  int m_outrange_size  = 1;
  
  int m_inrange_opacity = 180;
  int m_outrange_opacity = 50;
  
  GraphicInfo(QwtSymbol::Style si,
              QwtSymbol::Style so,
              QwtPlotCurve::CurveStyle ci,
              QwtPlotCurve::CurveStyle co,
              QPen pi=QPen(),
              QPen po=QPen()
              )
    : m_symbol_inrange(si),
      m_symbol_outrange(so),
      m_pen_inrange(pi),
      m_pen_outrange(po)
  {
    m_curve_inrange.setStyle(ci);
    m_curve_outrange.setStyle(co);
  }
  
  void apply_to_curve()
  {
    m_curve_inrange.setSymbol(&m_symbol_inrange);
    m_curve_inrange.setPen(m_pen_inrange);
    
    m_curve_outrange.setSymbol(&m_symbol_outrange);
    m_curve_outrange.setPen(m_pen_outrange);
  }
  
 public:
  void attach(QwtPlot * plot_area)
  {
    m_curve_inrange.attach(plot_area);
    m_curve_outrange.attach(plot_area);
  }
  
  void set_data(double * xdata, double * ydata, int n)
  {
    m_curve_inrange.setSamples(xdata, ydata, n);
  }
  
  void set_shadow_data(double * xdata, double * ydata, int n)
  {
    m_curve_outrange.setSamples(xdata, ydata, n);
  }
  
//  QPen& pen()
//  {
//    return m_pen_inrange;
//  }
//
//  QPen& pen()
//  {
//    return m_pen_inrange;
//  }
//
//  QwtSymbol& inrange_symbol()
//  {
//    return m_symbol_inrange;
//  }
//
//  QwtSymbol& outrange_symbol()
//  {
//    return m_symbol_outrange;
//  }
//
//  int inrange_opacity()
//  {
//    return m_inrange_opacity;
//  }
//
//  int outrange_opacity()
//  {
//    return m_outrange_opacity;
//  }
  
  void set_symbol_size(int sz)
  {
    m_symbol_inrange.setSize(QSize(sz, sz));
    apply_to_curve();
  }
  
  void set_shadow_symbol_size(int sz)
  {
    m_symbol_outrange.setSize(QSize(sz, sz));
    apply_to_curve();
  }
  
  void set_pen_width(int wd)
  {
    m_pen_inrange.setWidth(wd);
    apply_to_curve();
  }
  
  void set_shadow_pen_width(int wd)
  {
    m_pen_outrange.setWidth(wd);
    apply_to_curve();
  }
  
  void set_color(int r, int g, int b)
  {
    QColor c(r, g, b);
    c.setAlpha(m_inrange_opacity);
    m_symbol_inrange.setColor(c);
    m_pen_inrange.setColor(c);
    
    c.setAlpha(m_outrange_opacity);
    m_symbol_outrange.setColor(c);
    m_pen_outrange.setColor(c);
    
    apply_to_curve();
  }
  
  QwtPlotCurve * get_shadow() { return &m_curve_outrange; }
};


class ScatterGraphics : public virtual GraphicInfo
{
 public:
  ScatterGraphics() : GraphicInfo(QwtSymbol::Cross,
                                  QwtSymbol::Cross,
                                  QwtPlotCurve::NoCurve,
                                  QwtPlotCurve::NoCurve,
                                  Qt::NoPen)
  {
    set_symbol_size(6);
    set_shadow_symbol_size(2);
  }
};

class LineGraphics : public virtual GraphicInfo
{
 public:
  LineGraphics() : GraphicInfo(QwtSymbol::NoSymbol,
                               QwtSymbol::NoSymbol,
                               QwtPlotCurve::Lines,
                               QwtPlotCurve::Lines
                               )
  {
    set_pen_width(4.5);
    set_shadow_pen_width(4.5);
    
    m_inrange_opacity  = 180;
    m_outrange_opacity =  90;
  }
};


class DataXYGroup
{
 protected:
  data::Table * p_data;
  QwtPlot * p_plot;
  
  vector<GraphicInfo *> m_graphics_info;
  DataInfo m_plot_data_info;
  
//  vector<QwtPlotCurve *> m_curve_inrange;
//  vector<QwtPlotCurve *> m_curve_outrange;
//
//  vector<QwtSymbol *> m_symbol_inrange;
//  vector<QwtSymbol *> m_symbol_outrange;
  
  CrosshairGroup m_crosshair_group;

  double m_xlim[2]{ -10e-12, 10e12 };
  double m_ylim[2]{ -10e-12, 10e12 };
  double m_tlim[2]{ -10e-12, 10e12 };

  string m_xchannel_name;
  string m_ychannel_name;

  int m_color_index = 0;
//  vector<vector<int>> m_colors = kDefaultInactiveColor;
  
  int m_nlayers = 0;
  int m_active_layer = 0;

 public:
  DataXYGroup(data::Table *, QwtPlot *, string, string);
  virtual ~DataXYGroup();

  void format_curves(int);
  void init();
  void init_labels(data::Table *);
  
  void add_curve(string, string);

  string get_xchannel_name() { return m_plot_data_info.xchannel_name; }
  string get_ychannel_name() { return m_plot_data_info.ychannel_name; }
//  int get_layer() { return m_plot_data_info.layer; }
  
  virtual void add_layer()
  {
//    m_symbol_inrange.push_back(new QwtSymbol());
//    m_symbol_outrange.push_back(new QwtSymbol());
//
//    m_curve_inrange.push_back(new QwtPlotCurve());
//    m_curve_outrange.push_back(new QwtPlotCurve());
    
    m_crosshair_group.add_layer();
    
    ++m_nlayers;
    format_curves(m_nlayers - 1);
    update_colors();
  }
   
  void set_label_values_at(double, data::Table *);
  void set_metric_values(double, double, double);
  
  void update_colors()
  {
    vector<int> color;
    
    if (m_nlayers > 1)
    {
      for (int i = 0; i < m_nlayers; ++i)
      {
        color = kDefaultColorOrder[i];
        
        int r = color[0];
        int g = color[1];
        int b = color[2];
        
//        m_symbol_inrange[i]->setColor(QColor(r, g, b, 255));
//        m_symbol_outrange[i]->setColor(QColor(r, g, b, 50));
        
        m_graphics_info[i]->set_color(r, g, b);
        m_crosshair_group.set_color(i, r, g, b);
      }
    }
    else
    {
      color = kDefaultColorOrder[m_color_index];
      
      int r = color[0];
      int g = color[1];
      int b = color[2];
      
//      m_symbol_inrange[0]->setColor(QColor(r, g, b, 255));
//      m_symbol_outrange[0]->setColor(QColor(r, g, b, 50));
      
      m_graphics_info[0]->set_color(r, g, b);
      m_crosshair_group.set_color(0, r, g, b);
    }
  }

  double * xlim();
  double * ylim();
  double * tlim();
  
  int active_layer() { return m_active_layer; }

  virtual void set_data_from_table(double=-10e12, double=10e12,
                                   double=-10e12, double=10e12,
                                   double=-10e12, double=10e12 );
  
  void update_crosshair();
  void update_crosshair_positions(double);
  void update_crosshair(double *, double *);
  
  void set_xchannel_unit_string(string s) { m_crosshair_group.set_xchannel_unit_string(s); }
  void set_ychannel_unit_string(string s) { m_crosshair_group.set_ychannel_unit_string(s); }

  QwtPlotCurve * get_shadow() { return m_graphics_info[m_active_layer]->get_shadow(); }
  QwtPlotCurve * get_shadow(int layer) { return m_graphics_info[layer]->get_shadow(); }
  
  bool channels_and_time_present_for_any_layer();
  bool channels_and_time_present_in_layer(int);
  
  CrosshairGroup * crosshairs() { return &m_crosshair_group; }

};

class ScatterGroup : public DataXYGroup
{
 public:
  ScatterGroup(data::Table * data_table, QwtPlot * plot, string xid, string yid)
    : DataXYGroup(data_table, plot, xid, yid)
  {
  }
  ~ScatterGroup()
  {
  }
  
//  void format_curves(int) override;
  void add_layer() override
  {
    m_graphics_info.push_back(new ScatterGraphics()); // TODO: fix memory leak
    
    DataXYGroup::add_layer();
  }
};

class LineGroup : public DataXYGroup
{
 public:
  LineGroup(data::Table * data_table, QwtPlot * plot, string xid, string yid)
    : DataXYGroup(data_table, plot, xid, yid)
  {
  }
  ~LineGroup() {}
  
//  void format_curves(int) override;
  void add_layer() override
  {
    m_graphics_info.push_back(new LineGraphics()); // TODO: fix memory leak
    
    DataXYGroup::add_layer();
  }
};

}  // namespace graphic
}  // namespace insight
#endif // SCATTER_GROUP_H_
