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

#include "table.h"
#include "channel.h"

namespace insight {
namespace graphic {


ScatterGroup::ScatterGroup(QwtPlot * parent, string xchannel_id, string ychannel_id, int color_index=0)
    : p_parent(parent),
      m_crosshair(parent, xchannel_id, ychannel_id),
      m_xchannel_name(xchannel_id),
      m_ychannel_name(ychannel_id),
      m_color_index(color_index)
{}
ScatterGroup::~ScatterGroup() {}

double * ScatterGroup::xlim() { return m_xlim; }
double * ScatterGroup::ylim() { return m_ylim; }

void ScatterGroup::update_crosshair(double tvalue) {
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

void ScatterGroup::update_crosshair() {
  // Obtain plot limits from axes objects
  double x_lbound = p_parent->axisScaleDiv(p_parent->xBottom).lowerBound();
  double x_hbound = p_parent->axisScaleDiv(p_parent->xBottom).upperBound();
  double y_lbound = p_parent->axisScaleDiv(p_parent->yLeft).lowerBound();
  double y_hbound = p_parent->axisScaleDiv(p_parent->yLeft).upperBound();
  
  double xbounds[2]{ x_lbound, x_hbound };
  double ybounds[2]{ y_lbound, y_hbound };
  
  m_crosshair.set_xy(m_crosshair.x(), m_crosshair.y(), xbounds, ybounds);
}

void ScatterGroup::set_data_from_table(data::Table * table,
                                       double t_lbound, double t_hbound)
{
    double xmax, xmin, xmean;
    double ymax, ymin, ymean;
    
    m_xchannel = table->get(m_xchannel_name);
    m_ychannel = table->get(m_ychannel_name);
    size_t n = m_xchannel->length();
    
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

    for (size_t i = 0; i < n; ++i) {
        if (tchannel->operator[](i) < t_lbound) {
            continue;
        } else if (tchannel->operator[](i) > t_hbound) {
            if (below_hbound) {
                i_hbound = i - 1;
                below_hbound = false;
            }
            continue;
        } else {
            if (below_lbound) {
                i_lbound = i;
                below_lbound = false;
            }
        }
    }
    int n_to_plot = i_hbound - i_lbound;
    if (n_to_plot < 2) { n_to_plot = 2; i_hbound = i_lbound + 1; }
    
    m_scatter.setSamples(&m_xchannel->get_data_ptr()[i_lbound], &m_ychannel->get_data_ptr()[i_lbound], n_to_plot);
    m_shadow.setSamples(m_xchannel->get_data_ptr(), m_ychannel->get_data_ptr(), n);
    
    m_xlim[0] = xmin; m_xlim[1] = xmax;
    m_ylim[0] = ymin; m_ylim[1] = ymax;
}

void ScatterGroup::init_scatters() {
  // draw curve on graphic
  vector<int> color = kDefaultColorOrder[m_color_index];
      
  m_symbol.setStyle(QwtSymbol::Cross);
  m_symbol.setSize(QSize(6, 6));
  m_symbol.setColor(QColor(color[0], color[1], color[2], 255));
  
  m_shadow_symbol.setStyle(QwtSymbol::Cross);
  m_shadow_symbol.setSize(QSize(6, 6));
  m_shadow_symbol.setColor(QColor(color[0], color[1], color[2], 50));
    
  m_scatter.setStyle(QwtPlotCurve::NoCurve);
  m_scatter.setSymbol(&m_symbol);
  m_scatter.attach(p_parent);
  
  m_shadow.setStyle(QwtPlotCurve::NoCurve);
  m_shadow.setSymbol(&m_shadow_symbol);
  m_shadow.attach(p_parent);
  
//  m_crosshair.attach(p_parent);
  m_crosshair.set_color(color[0], color[1], color[2]);
}

void DisplayCrosshair::set_label_values(double xvalue, double yvalue)
{
  char * xlabel_text = new char[m_xchannel_name.size()+62];
  char * ylabel_text = new char[m_ychannel_name.size()+62];
  
  sprintf(xlabel_text,
      "<span style=\"color : rgb(50, 50, 50);\">%s:</span> %*.2f[-];",
          m_xchannel_name.c_str(), 7, xvalue
  );
  sprintf(ylabel_text,
      "<span style=\"color : rgb(50, 50, 50);\">%s:</span> %*.2f[-];",
          m_ychannel_name.c_str(), 7, yvalue
  );
    
  m_xlabel.setText(QString(xlabel_text));
  m_ylabel.setText(QString(ylabel_text));

  delete[] xlabel_text;
  delete[] ylabel_text;
}

void DisplayCrosshair::set_xy(double x, double y, double * xlim, double * ylim)
{
  int label_width = 200;
  int label_height = 15;
  
  double horzbar_ydata[2]{y, y};
  double vertbar_xdata[2]{x, x};
    
  m_horzbar.setSamples(xlim, horzbar_ydata, 2);
  m_vertbar.setSamples(vertbar_xdata, ylim, 2);
  
  m_xy[0] = x;
  m_xy[1] = y;
  
  // update crosshair label positions
  m_xlabel.setGeometry(
    5,
    ((ylim[1] - y) / (ylim[1] - ylim[0])) * p_parent->height() - label_height,
    label_width,
    label_height
  );
    
  m_ylabel.setGeometry(
    ((x - xlim[0]) / (xlim[1] - xlim[0])) * p_parent->width(),
    5,
    label_height,
    label_width
  );
  set_label_values(x, y);
}

}  // namespace graphic
}  // namespace insight
