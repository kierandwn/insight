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
      m_xchannel_name(xchannel_id),
      m_ychannel_name(ychannel_id),
      m_color_index(color_index)
{}
ScatterGroup::~ScatterGroup() {}

double * ScatterGroup::xlim() { return m_xlim; }
double * ScatterGroup::ylim() { return m_ylim; }

void ScatterGroup::set_data_from_table(data::Table * table,
                                       double t_lbound, double t_hbound)
{
    double xmax, xmin, xmean;
    double ymax, ymin, ymean;
    
    data::Channel * xchannel = table->get(m_xchannel_name);
    data::Channel * ychannel = table->get(m_ychannel_name);
    size_t n = xchannel->length();
    
    xmin = xchannel->min();
    xmax = xchannel->max();
    xmean = 0.5 * (xmin + xmax);

    ymin = ychannel->min();
    ymax = ychannel->max();
    ymean = 0.5 * (ymin + ymax);
    
    data::Channel * tchannel = xchannel->get_time_ref();
  
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
    
    m_scatter.setSamples(&xchannel->get_data_ptr()[i_lbound], &ychannel->get_data_ptr()[i_lbound], n_to_plot);
    m_shadow.setSamples(xchannel->get_data_ptr(), ychannel->get_data_ptr(), n);
    
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
}


}  // namespace graphic
}  // namespace insight
