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
#ifndef SCATTERDISPLAY_H_
#define SCATTERDISPLAY_H_
#pragma once

#include <vector>
#include <string>

#include "ui_scatter.h"
#include "linked_graphic.h"

#include <QLabel>

#include "scattergroup.h"
#include "mouse_state.h"
#include "vlabel.h"
#include "table.h"
#include "grid.h"
#include "mainwindow.h"

#include "lib/json/single_include/nlohmann/json.hpp"

namespace insight {
namespace graphic {

using namespace std;


class DisplayCrosshair {
 private:
  QwtPlotCurve m_horzbar;
  QwtPlotCurve m_vertbar;
  
  double m_xy[2];
    
 public:
  DisplayCrosshair() {
    QPen dark_grey(QColor(0, 0, 0, 250));
    dark_grey.setWidth(2);
      
    m_horzbar.setPen(dark_grey);
    m_vertbar.setPen(dark_grey);
  }
    
  void attach(QwtPlot * plot_area) {
    m_horzbar.attach(plot_area);
    m_vertbar.attach(plot_area);
  }
    
  void set_xy(double x, double y, double * xlim, double * ylim) {
    double horzbar_ydata[2]{y, y};
    double vertbar_xdata[2]{x, x};
      
    m_horzbar.setSamples(xlim, horzbar_ydata, 2);
    m_vertbar.setSamples(vertbar_xdata, ylim, 2);
    
    m_xy[0] = x;
    m_xy[1] = y;
  }
  
  double x() { return m_xy[0]; }
  double y() { return m_xy[1]; }
};

class ScatterDisplay : public LinkedPlot
{
  Q_OBJECT
 private:
  data::Table * m_data;
    
  QLabel m_xlabel;
  VLabel m_ylabel;
    
  VLabel m_mean_xlabel;
  QLabel m_mean_ylabel;
    
  QwtPlotCurve m_mean_xline;
  QwtPlotCurve m_mean_yline;
    
  Ui::ScatterDisplay * p_ui = new Ui::ScatterDisplay;
    
  vector<ScatterGroup *> m_scatter_pairs;
  int m_nscatter_pairs;
    
  DisplayCrosshair m_crosshair;
  
  MouseState m_mouse_state;
  
//  double m_xlim[2];
//  double m_mouse_xpos;
    
//  void cursor_in_xrange();
    void update_mean_lines();
    
public:
  ScatterDisplay(data::Table *, layout::Layout *);

  int get_number_of_scatter_pairs() { return m_nscatter_pairs; }
    
  void apply_config(nlohmann::json *) override;
  void update_cursor_position(double) override;
  void update_cursor_position(double xvalue, double yvalue);
  
  void init_labels();
  void set_label_values_at(double);
  
  void update_after_data_load () override;
  void update_view_limits(double, double) override;
    
  void init () override;
  void reset () override;
    
  void xlim(double *);
  void ylim(double *);
  
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  //  void mouseDoubleClickEvent(QMouseEvent * event) override;
    void wheelEvent(QWheelEvent * event) override;
};

}  // namespace graphic
}  // namespace insight
#endif // SCATTERDISPLAY_H_
