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
#ifndef SCATTERDISPLAY_H_
#define SCATTERDISPLAY_H_
#pragma once

#include <vector>
#include <string>

#include "ui_scatter.h"
//#include "linked_graphic.h"

#include <qwt_plot.h>

#include <QLabel>

#include "ApplicationInterface.h"
#include "mouse_state.h"
#include "vlabel.h"
#include "table.h"
#include "grid.h"
#include "mainwindow.h"
#include "scattergroup.h"

#include "lib/json/single_include/nlohmann/json.hpp"

namespace insight {
namespace graphic {

using namespace std;

class DataXYDisplay : public QwtPlot, public InsightGraphic
{
  Q_OBJECT
 protected:
  QLabel m_mean_xlabel;
  VLabel m_mean_ylabel;
    
  QwtPlotCurve m_mean_xline;
  QwtPlotCurve m_mean_yline;
    
  Ui::ScatterDisplay * p_ui = new Ui::ScatterDisplay;
    
  vector<DataXYGroup *> m_data_curves;
  
  int m_ncurves = 0;
  int m_nlayers = 0;
  
  int m_panning_crosshair = -1;
  
  vector<string> m_xchannel_names;
  vector<string> m_ychannel_names;
  
  string m_xaxis_unit_string = "-";
  string m_yaxis_unit_string = "-";
  
  MouseState m_mouse_state;
  
  void set_summary_stats_labels(double xmedian,
                                double xmin,
                                double xmax,
                                double ymedian,
                                double ymin,
                                double ymax );
  void update_axes_unit_strings();
  
  bool m_indicator_lines_at_zero = true;
  
  double m_padding_scalar = .04;
  
//  double inline axis_coordx_from_painter_scale(int i) {
//    QwtScaleMap map = canvasMap(xBottom);
//    return map.transform(i);
//  }
  
//  int inline axis_coordx_from_painter_scale(double d) {
//    QwtScaleMap map = canvasMap(xBottom);
//    return map.invTransform(d);
//  }
  
    
public:
  DataXYDisplay(data::Table *, layout::Layout *);
  virtual ~DataXYDisplay() {} // TODO: resolve data leak in m_data_curves
  
  virtual void apply_config(nlohmann::json *) override;
  void init () override;
  virtual void init_curves()=0;
  
  void init_labels();
  void init_cursor_position();
  void init_mean_lines();

  int get_number_of_scatter_pairs() { return m_ncurves; }
  void update_cursor_position(double=0.) override;
  
  void set_label_positions();
  
  void update_after_data_load () override;
  void update_mean_lines();
  
  void update_view_limits(double, double) override;
  void update_axes_scales();
  
//  virtual void add_layer()=0;
  
  void descriptive_mean_labels();
  
  void reset () override;
    
  bool xlim(double *);
  bool ylim(double *);
  
  void update_plot_limits();
  void update_plot_limits(double *, double *);
  
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  //  void mouseDoubleClickEvent(QMouseEvent * event) override;
  void wheelEvent(QWheelEvent * event) override;
  
  void resizeEvent(QResizeEvent * event) override;
  
  bool get_total_data_ranges(double *, double *);
};

class ScatterDisplay : public virtual DataXYDisplay // TODO: difference between scatter/line implemented as a factory? seems unnecessary to have different plot classes?
{
 protected:
  bool m_indicator_lines_at_zero = false;
  
 public:
  ScatterDisplay(data::Table * t, layout::Layout * l) : DataXYDisplay(t, l) {}
  ~ScatterDisplay() {}
  
  void init_curves() override;
  
//  void apply_config(nlohmann::json *) override;
//  void update_mean_lines() override;
};

class LineDisplay : public virtual DataXYDisplay
{
 public:
  LineDisplay(data::Table * t, layout::Layout * l) : DataXYDisplay(t, l) {}
  ~LineDisplay() {}
  
  void init_curves() override;
  
//  void apply_config(nlohmann::json *) override;
//  void update_mean_lines() override;
};

}  // namespace graphic
}  // namespace insight
#endif // SCATTERDISPLAY_H_
