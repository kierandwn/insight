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
#include "scattergroup.h"
#include "defaults.h"

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>

#include "insight_graphic_base.h"
#include "table.h"
#include "channel.h"

namespace insight {
namespace graphic {

void DisplayCrosshair::set_label_values(double xvalue, double yvalue)
{
  char * xlabel_text = new char[m_xchannel_name.size()+m_xchannel_unit_string.size()+61];
  char * ylabel_text = new char[m_ychannel_name.size()+m_ychannel_unit_string.size()+61];
  
  sprintf(xlabel_text,
      "<span style=\"color : rgb(50, 50, 50);\">%s:</span> %*.2f[%s];",
          m_xchannel_name.c_str(), 7, xvalue, m_xchannel_unit_string.c_str()
  );
  sprintf(ylabel_text,
      "<span style=\"color : rgb(50, 50, 50);\">%s:</span> %*.2f[%s];",
          m_ychannel_name.c_str(), 7, yvalue, m_ychannel_unit_string.c_str()
  );

  m_xlabel.setText(QString(xlabel_text));
  m_ylabel.setText(QString(ylabel_text));

  delete[] xlabel_text;
  delete[] ylabel_text;
}

void DisplayCrosshair::set_xy(double x, double y, double * xlim, double * ylim)
{
  set_label_values(x, y);
  
  int paint_coord_x = p_parent->painter_coordx_from_axis_scale(x);
  int paint_coord_y = p_parent->painter_coordy_from_axis_scale(y);
  
  double horzbar_ydata[2]{y, y};
  double vertbar_xdata[2]{x, x};
    
  m_horzbar.setSamples(xlim, horzbar_ydata, 2);
  m_vertbar.setSamples(vertbar_xdata, ylim, 2);
  m_centrepoint.setSamples(&x, &y, 1);
  
  m_xy[0] = x;
  m_xy[1] = y;
  
  // update crosshair label positions
  m_xlabel.setGeometry(
    p_parent->width() - (m_xlabel.sizeHint().width() + 5),
    paint_coord_y - 10,
    m_xlabel.sizeHint().width(),
    m_xlabel.sizeHint().height()
  );
    
  m_ylabel.setGeometry(
    paint_coord_x,
    5,
    m_ylabel.sizeHint().width(),
    m_ylabel.sizeHint().height()
  );
}

DataXYGroup::DataXYGroup(Base * parent, string xchannel_id, string ychannel_id, int color_index=0)
    : p_parent(parent),
      m_crosshair(parent, xchannel_id, ychannel_id),
      m_xchannel_name(xchannel_id),
      m_ychannel_name(ychannel_id),
      m_color_index(color_index)
{}
DataXYGroup::~DataXYGroup() {}

double * DataXYGroup::xlim() { return m_xlim; }
double * DataXYGroup::ylim() { return m_ylim; }
double * DataXYGroup::tlim() { return m_tlim; }

bool DataXYGroup::channels_present_in(data::Table * data) {
  return data->exists_in_layer(m_xchannel_name) && data->exists_in_layer(m_ychannel_name);
}

void DataXYGroup::update_crosshair(double tvalue)
{
  // Obtain plot limits from axes objects
  double x_lbound = p_parent->axisScaleDiv(p_parent->xBottom).lowerBound();
  double x_hbound = p_parent->axisScaleDiv(p_parent->xBottom).upperBound();
  double y_lbound = p_parent->axisScaleDiv(p_parent->yLeft).lowerBound();
  double y_hbound = p_parent->axisScaleDiv(p_parent->yLeft).upperBound();
  
  double xbounds[2]{ x_lbound, x_hbound };
  double ybounds[2]{ y_lbound, y_hbound };
  
  double xvalue = m_xchannel->value_at(tvalue);
  double yvalue = m_ychannel->value_at(tvalue);
  
  m_crosshair.set_xy(xvalue, yvalue, xbounds, ybounds);
}

void DataXYGroup::update_crosshair() {
  // Obtain plot limits from axes objects
  double x_lbound = p_parent->axisScaleDiv(p_parent->xBottom).lowerBound();
  double x_hbound = p_parent->axisScaleDiv(p_parent->xBottom).upperBound();
  double y_lbound = p_parent->axisScaleDiv(p_parent->yLeft).lowerBound();
  double y_hbound = p_parent->axisScaleDiv(p_parent->yLeft).upperBound();
  
  double xbounds[2]{ x_lbound, x_hbound };
  double ybounds[2]{ y_lbound, y_hbound };
  
  m_crosshair.set_xy(m_crosshair.x(), m_crosshair.y(), xbounds, ybounds);
}

void ScatterGroup::init_curves() {
  // draw curve on graphic
  vector<int> color = kDefaultColorOrder[m_color_index];

  m_symbol.setStyle(QwtSymbol::Cross);
  m_symbol.setSize(QSize(6, 6));
  m_symbol.setColor(QColor(color[0], color[1], color[2], 255));
  
  m_shadow_symbol.setStyle(QwtSymbol::Cross);
  m_shadow_symbol.setSize(QSize(2, 2));
  m_shadow_symbol.setColor(QColor(color[0], color[1], color[2], 50));
    
  m_scatter.setStyle(QwtPlotCurve::NoCurve);
  m_scatter.setSymbol(&m_symbol);
  m_scatter.attach(p_parent);
  
  m_shadow.setStyle(QwtPlotCurve::NoCurve);
  m_shadow.setSymbol(&m_shadow_symbol);
  m_shadow.attach(p_parent);
  
  m_crosshair.set_color(kDefaultInactiveColor[0],
                        kDefaultInactiveColor[1],
                        kDefaultInactiveColor[2]
                        );
}

void ScatterGroup::set_data_from_table(data::Table * table,
                                       double t_lbound, double t_hbound,
                                       double x_lbound, double x_hbound,
                                       double y_lbound, double y_hbound )
{
  // data is present
  if (!channels_present_in(table)) {
//    m_crosshair.detach();
    return;
  }
  
  data::Channel * ychannel = table->get(m_ychannel_name);
  m_xchannel = table->get(m_xchannel_name);
  
  if (m_xchannel->get_time_ref() == ychannel) {
    m_ychannel = ychannel;
  } else {
    m_ychannel = ychannel->resample_on(m_xchannel->get_time_ref(), m_xchannel->length());
  }
  m_crosshair.set_xchannel_unit_string(m_xchannel->get_unit_string());
  m_crosshair.set_ychannel_unit_string(m_ychannel->get_unit_string());
  
  size_t n = m_xchannel->length();
  
  double xmax, xmin, xmean;
  double ymax, ymin, ymean;
  
  xmin = m_xchannel->min();
  xmax = m_xchannel->max();
  xmean = 0.5 * (xmin + xmax);

  ymin = m_ychannel->min();
  ymax = m_ychannel->max();
  ymean = 0.5 * (ymin + ymax);
  
  data::Channel * tchannel = m_xchannel->get_time_ref();

  t_lbound = max({t_lbound, tchannel->min()});
  t_hbound = min({t_hbound, tchannel->max()});
  
  bool below_lbound = true; bool below_hbound = true;
  int i_lbound = n - 1; int i_hbound = n - 1;
  
  double * xdata = new double[n];
  double * ydata = new double[n];
  int i_data = 0;
  
  double xi, yi, ti;
  bool x_in_bounds;
  bool y_in_bounds;
  
  for (size_t i = 0; i < n; ++i) {
    xi = m_xchannel->operator[](i);
    yi = m_ychannel->operator[](i);
    ti = tchannel->operator[](i);
    
    x_in_bounds = xi >= x_lbound and xi <= x_hbound;
    y_in_bounds = yi >= y_lbound and yi <= y_hbound;
//      t_in_bounds = ti >= t_lbound and ti <= t_hbound;
    
    if (x_in_bounds && y_in_bounds) {
      xdata[i_data] = xi;
      ydata[i_data] = yi;
      ++i_data;
    
      if (tchannel->operator[](i) < t_lbound) {
        continue;
        
      } else if (tchannel->operator[](i) > t_hbound) {
        if (below_hbound) {
          i_hbound = i_data - 1;
          below_hbound = false;
        }
        continue;
      } else {
        if (below_lbound) {
          i_lbound = i_data - 1;
          below_lbound = false;
        }
      }
    }
  }
  int n_to_plot = i_hbound - i_lbound;
  if (n_to_plot < 2) { n_to_plot = 2; i_hbound = i_lbound + 1; }
    
  m_scatter.setSamples(&xdata[i_lbound], &ydata[i_lbound], n_to_plot);
  m_shadow.setSamples(xdata, ydata, i_data);
    
  m_xlim[0] = xmin; m_xlim[1] = xmax;
  m_ylim[0] = ymin; m_ylim[1] = ymax;
  m_tlim[0] = t_lbound; m_tlim[1] = t_hbound;
  
  m_crosshair.set_color(kDefaultColorOrder[m_color_index][0],
                        kDefaultColorOrder[m_color_index][1],
                        kDefaultColorOrder[m_color_index][2]
                        );
  delete[] xdata;
  delete[] ydata;
}

void LineGroup::init_curves() {
  // draw curve on graphic
  vector<int> color = kDefaultColorOrder[m_color_index];
  
  QColor c(color[0], color[1], color[2]);
  c.setAlpha(180);
  QPen pen(c);
  pen.setWidth(3.5);
  
  m_line.setPen(pen);
  m_line.attach(p_parent);
  
  c.setAlpha(50);
  pen.setColor(c);
  pen.setWidth(1.5);
  
  m_shadow.setPen(pen);
  m_shadow.attach(p_parent);
  
  m_crosshair.set_color(kDefaultInactiveColor[0],
                        kDefaultInactiveColor[1],
                        kDefaultInactiveColor[2]
                        );
}

void LineGroup::set_data_from_table(data::Table * table,
                                    double t_lbound, double t_hbound,
                                    double x_lbound, double x_hbound,
                                    double y_lbound, double y_hbound )
{
  // data is present
  if (!channels_present_in(table)) {
//    m_crosshair.detach();
    return;
  }
  
  data::Channel * ychannel = table->get(m_ychannel_name);
  m_xchannel = table->get(m_xchannel_name);
  
  if (m_xchannel->get_time_ref() == ychannel) {
    m_ychannel = ychannel;
  } else {
    m_ychannel = ychannel->resample_on(m_xchannel->get_time_ref(), m_xchannel->length());
  }
  
  size_t n = m_xchannel->length();
  
  double xmax, xmin, xmean;
  double ymax, ymin, ymean;
  
  xmin = m_xchannel->min();
  xmax = m_xchannel->max();
  xmean = 0.5 * (xmin + xmax);

  ymin = m_ychannel->min();
  ymax = m_ychannel->max();
  ymean = 0.5 * (ymin + ymax);
  
  data::Channel * tchannel = m_xchannel->get_time_ref();

  t_lbound = max({t_lbound, tchannel->min()});
  t_hbound = min({t_hbound, tchannel->max()});
  
  bool below_lbound = true; bool below_hbound = true;
  int i_lbound = n - 1; int i_hbound = n - 1;
  
  double * xdata = new double[n];
  double * ydata = new double[n];
  int i_data = 0;
  
  double xi, yi, ti;
  bool x_in_bounds;
  bool y_in_bounds;
  
  for (size_t i = 0; i < n; ++i) {
    xi = m_xchannel->operator[](i);
    yi = m_ychannel->operator[](i);
    ti = tchannel->operator[](i);
    
    x_in_bounds = xi >= x_lbound and xi <= x_hbound;
    y_in_bounds = yi >= y_lbound and yi <= y_hbound;
//      t_in_bounds = ti >= t_lbound and ti <= t_hbound;
    
    if (x_in_bounds && y_in_bounds) {
      xdata[i_data] = xi;
      ydata[i_data] = yi;
      ++i_data;
    
      if (tchannel->operator[](i) < t_lbound) {
        continue;
        
      } else if (tchannel->operator[](i) > t_hbound) {
        if (below_hbound) {
          i_hbound = i_data - 1;
          below_hbound = false;
        }
        continue;
      } else {
        if (below_lbound) {
          i_lbound = i_data - 1;
          below_lbound = false;
        }
      }
    }
  }
  int n_to_plot = i_hbound - i_lbound;
  if (n_to_plot < 2) { n_to_plot = 2; i_hbound = i_lbound + 1; }
    
  m_line.setSamples(&xdata[i_lbound], &ydata[i_lbound], n_to_plot);
  m_shadow.setSamples(xdata, ydata, i_data);
    
  m_xlim[0] = xmin; m_xlim[1] = xmax;
  m_ylim[0] = ymin; m_ylim[1] = ymax;
  m_tlim[0] = t_lbound; m_tlim[1] = t_hbound;
  
  m_crosshair.set_color(kDefaultColorOrder[m_color_index][0],
                        kDefaultColorOrder[m_color_index][1],
                        kDefaultColorOrder[m_color_index][2]
                        );
  delete[] xdata;
  delete[] ydata;
}

}  // namespace graphic
}  // namespace insight
