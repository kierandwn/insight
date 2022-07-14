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
#include "scattergroup.h"
#include "defaults.h"

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>

#include "ApplicationInterface.h"
#include "table.h"
#include "channel.h"

namespace insight {
namespace graphic {

//void Crosshair::set_xy(double x, double y, double * xlim, double * ylim)
//{
//  double horzbar_ydata[2]{y, y};
//  double vertbar_xdata[2]{x, x};

//  m_horzbar.setSamples(xlim, horzbar_ydata, 2);
//  m_vertbar.setSamples(vertbar_xdata, ylim, 2);
//  m_centrepoint.setSamples(&x, &y, 1);

//  m_xy[0] = x;
//  m_xy[1] = y;
//}


//void CrosshairGroup::set_xy(double x, double y, int layer, double * xlim, double * ylim)
//{
//  set_label_values(layer, x, y);
//  m_crosshairs[layer]->set_xy(x, y, xlim, ylim);

//  m_xy[layer][0] = x;
//  m_xy[layer][1] = y;

//  m_xbounds[0] = xlim[0];
//  m_xbounds[1] = xlim[1];
//  m_ybounds[0] = ylim[0];
//  m_ybounds[1] = ylim[1];
//}

//void CrosshairGroup::set_xy(double x, double y, int layer)
//{
//  set_label_values(layer, x, y);
//  m_crosshairs[layer]->set_xy(x, y, m_xbounds, m_ybounds);

//  m_xy[layer][0] = x;
//  m_xy[layer][1] = y;
//}

//void CrosshairGroup::update_plot_limits(double * xlimits, double * ylimits)
//{
//  m_xbounds[0] = xlimits[0];
//  m_xbounds[1] = xlimits[1];
//  m_ybounds[0] = ylimits[0];
//  m_ybounds[1] = ylimits[1];

//  for (Crosshair * c: m_crosshairs)
//    c->set_xy(c->x(), c->y(), xlimits, ylimits);
//}

//void CrosshairGroup::set_label_values(int layer, double xvalue, double yvalue)
//{
//  m_xy[layer][0] = xvalue;
//  m_xy[layer][1] = yvalue;

//  render_labels();
//}



DataXYGroup::DataXYGroup(data::Table * data_table, QwtPlot * parent, string xchannel_id, string ychannel_id)
    : p_data(data_table),
      p_plot(parent),
      m_crosshair_group(parent, xchannel_id, ychannel_id),
      m_plot_data_info(xchannel_id, ychannel_id)
{}
DataXYGroup::~DataXYGroup() {}

void DataXYGroup::init()
{
}

double * DataXYGroup::xlim() { return m_xlim; }
double * DataXYGroup::ylim() { return m_ylim; }
double * DataXYGroup::tlim() { return m_tlim; }

bool DataXYGroup::channels_and_time_present_for_any_layer()
{
  bool any_data_present = false;
  
  for (int layer = 0; layer < m_nlayers; ++layer)
  {
    any_data_present = channels_and_time_present_in_layer(layer);
    if (any_data_present)
    {
      m_active_layer = layer;
      return true;
    }
  }
  return false;
}

bool DataXYGroup::channels_and_time_present_in_layer(int layer)
{
//  data::Table * data = p_parent->get_data_table_ref();
  
  string xchannel_name = m_plot_data_info.xchannel_name;
  string ychannel_name = m_plot_data_info.ychannel_name;
  
  bool is_all_data_present_in_layer = (p_data->exists_in_layer(xchannel_name, layer) && p_data->get(xchannel_name, layer)->get_time_ref()) &&
                                      (p_data->exists_in_layer(ychannel_name, layer) && p_data->get(ychannel_name, layer)->get_time_ref());
    
  return is_all_data_present_in_layer;
}

void DataXYGroup::update_crosshair_positions(double tvalue)
{
//  data::Table * data_table = p_parent->get_data_table_ref();
  
  for (int layer = 0; layer < m_nlayers; ++layer)
  {
    if (p_data->exists_in_layer(m_plot_data_info.xchannel_name, layer) && p_data->exists_in_layer(m_plot_data_info.ychannel_name, layer))
    {
      data::Channel * xchannel = p_data->get(m_plot_data_info.xchannel_name, layer);
      data::Channel * ychannel = p_data->get(m_plot_data_info.ychannel_name, layer);
      
      double xvalue = xchannel->value_at(tvalue);
      double yvalue = ychannel->value_at(tvalue);
      
      m_crosshair_group.set_xy(xvalue, yvalue, layer); // , xbounds, ybounds);
    }
  }
}

void DataXYGroup::update_crosshair(double * xbounds, double * ybounds)
{
  m_crosshair_group.render_labels();
  m_crosshair_group.update_plot_limits(xbounds, ybounds);
}

void DataXYGroup::update_crosshair()
{
  m_crosshair_group.render_labels();
//  m_crosshair_group.set_xy(m_crosshair.x(), m_crosshair.y(), layer);
}

void DataXYGroup::format_curves(int layer)
{
  m_graphics_info[layer]->attach(p_plot);
  
//  // draw curve on graphic
//  m_symbol_inrange[layer]->setStyle(QwtSymbol::Cross);
//  m_symbol_inrange[layer]->setSize(QSize(6, 6));
////  m_symbol.setColor(QColor(m_color[0], m_color[1], m_color[2], 255));
//
//  m_symbol_outrange[layer]->setStyle(QwtSymbol::Cross);
//  m_symbol_outrange[layer]->setSize(QSize(2, 2));
////  m_outrange_symbol.setColor(QColor(m_color[0], m_color[1], m_color[2], 50));
//
//  m_curve_inrange[layer]->setStyle(QwtPlotCurve::NoCurve);
//  m_curve_inrange[layer]->setSymbol(m_symbol_inrange[layer]);
//  m_curve_inrange[layer]->attach(p_parent);
//
//  m_curve_outrange[layer]->setStyle(QwtPlotCurve::NoCurve);
//  m_curve_outrange[layer]->setSymbol(m_symbol_outrange[layer]);
//  m_curve_outrange[layer]->attach(p_parent);
}

//void LineGroup::format_curves(int layer)
//{
//  vector<int> m_color = kDefaultInactiveColor; // TODO: update color assignment
//
//  // draw curve on graphic
////  QColor c(m_color[0], m_color[1], m_color[2]);
////  c.setAlpha(180);
//  QPen pen;
//  pen.setWidth(3.5);
//
//  m_curve_inrange[layer]->setPen(pen);
//  m_curve_inrange[layer]->attach(p_parent);
//
////  c.setAlpha(50);
////  pen.setColor(c);
//  pen.setWidth(3.5);
//
//  m_curve_outrange[layer]->setPen(pen);
//  m_curve_outrange[layer]->attach(p_parent);
//
//  m_crosshair_group.set_color(layer,
//                        kDefaultInactiveColor[0],
//                        kDefaultInactiveColor[1],
//                        kDefaultInactiveColor[2]
//                        );
//}

void DataXYGroup::set_data_from_table(double t_lbound, double t_hbound,
                                      double x_lbound, double x_hbound,
                                      double y_lbound, double y_hbound)
{
//  data::Table * table = p_parent->get_data_table_ref();
  
  // data is present
  if (!channels_and_time_present_for_any_layer())
    return;
  
  double xmax, xmin; // , xmean;
  double ymax, ymin; // , ymean;
  double tmax, tmin;
  
  for (int layer = 0; layer < m_nlayers; ++layer)
  {
    if (!channels_and_time_present_in_layer(layer))
      continue;
    
    data::Channel * xchannel = p_data->get(m_plot_data_info.xchannel_name, layer);
    data::Channel * ychannel = p_data->get(m_plot_data_info.ychannel_name, layer);
    
    if (xchannel->get_time_ref() != ychannel->get_time_ref())
      ychannel = ychannel->resample_on(xchannel->get_time_ref(), xchannel->length()); // TODO: single argument, tchannel?
    
    size_t n = xchannel->length();
    
    data::Channel * tchannel = xchannel->get_time_ref();
    
    t_lbound = max({t_lbound, tchannel->min()});
    t_hbound = min({t_hbound, tchannel->max()});
    
    if (layer == 0)
    {
      xmin = xchannel->min();
      xmax = xchannel->max();

      ymin = ychannel->min();
      ymax = ychannel->max();

      tmin = tchannel->min();
      tmax = tchannel->max();
    }
    else
    {
      xmin = min({xmin, xchannel->min()});
      xmax = max({xmax, xchannel->max()});

      ymin = min({ymin, ychannel->min()});
      ymax = max({ymax, ychannel->max()});
      
      tmin = min({tmin, tchannel->min()});
      tmax = max({tmax, tchannel->max()});
    }
    
    bool below_lbound = true; bool below_hbound = true;
    int i_lbound = n - 1; int i_hbound = n - 1;
    
    double * xdata = new double[n];
    double * ydata = new double[n];
    int i_data = 0;
    
    double xi, yi, ti;
    bool x_in_bounds;
    bool y_in_bounds;
    
    for (size_t i = 0; i < n; ++i)
    {
      xi = xchannel->operator[](i);
      yi = ychannel->operator[](i);
      ti = tchannel->operator[](i);
      
      x_in_bounds = xi >= x_lbound and xi <= x_hbound;
      y_in_bounds = yi >= y_lbound and yi <= y_hbound;
  //      t_in_bounds = ti >= t_lbound and ti <= t_hbound;
      
      if (x_in_bounds && y_in_bounds)
      {
        xdata[i_data] = xi;
        ydata[i_data] = yi;
        ++i_data;
      
        if (tchannel->operator[](i) < t_lbound)
          continue;
          
        else
        if (tchannel->operator[](i) > t_hbound)
        {
          if (below_hbound)
          {
            i_hbound = i_data - 1;
            below_hbound = false;
          }
          continue;
        }
        else
        {
          if (below_lbound)
          {
            i_lbound = i_data - 1;
            below_lbound = false;
          }
        }
      }
    }
    int n_to_plot_inrange  = i_hbound - i_lbound;
    int n_to_plot_outrange = i_data;
    
    if (n_to_plot_inrange < 2)
    {
      n_to_plot_inrange = 2;
      i_hbound = i_lbound + 1;
    }
      
//    m_curve_inrange[layer]->setSamples(&xdata[i_lbound], &ydata[i_lbound], n_to_plot_inrange);
//    m_curve_outrange[layer]->setSamples(xdata, ydata, n_to_plot_outrange);
    
    m_graphics_info[layer]->set_data(&xdata[i_lbound], &ydata[i_lbound], n_to_plot_inrange);
    m_graphics_info[layer]->set_shadow_data(xdata, ydata, n_to_plot_outrange);
    
    delete[] xdata;
    delete[] ydata;
  }
  
  m_xlim[0] = xmin; m_xlim[1] = xmax;
  m_ylim[0] = ymin; m_ylim[1] = ymax;
  m_tlim[0] = tmin; m_tlim[1] = tmax;
}

}  // namespace graphic
}  // namespace insight
