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

class DataXYDisplay : public LinkedPlot
{
  Q_OBJECT
 protected:
  QLabel m_mean_xlabel;
  VLabel m_mean_ylabel;
    
  QwtPlotCurve m_mean_xline;
  QwtPlotCurve m_mean_yline;
    
  Ui::ScatterDisplay * p_ui = new Ui::ScatterDisplay;
    
  vector<DataXYGroup *> m_data_curves;
  
  int m_ncurves;
  int m_cursor_track_curve = 0;
  
  MouseState m_mouse_state;
    
public:
  DataXYDisplay(data::Table *, layout::Layout *);
  virtual ~DataXYDisplay() {} // TODO: resolve data leak in m_data_curves

  int get_number_of_scatter_pairs() { return m_ncurves; }
  void update_cursor_position(double=0.) override;
  
  void set_label_positions();
  
  void update_after_data_load () override;
  void update_view_limits(double, double) override;
  virtual void update_mean_lines()=0;
    
  void init () override;
  
  void init_labels();
  void init_cursor_position();
  void init_mean_lines();
  
  void descriptive_mean_labels();
  
  void reset () override;
    
  bool xlim(double *);
  bool ylim(double *);
  
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  //  void mouseDoubleClickEvent(QMouseEvent * event) override;
  void wheelEvent(QWheelEvent * event) override;
  
  void resizeEvent(QResizeEvent * event) override;
};

class ScatterDisplay : public virtual DataXYDisplay { // TODO: difference between scatter/line implemented as a factory? seems unnecessary to have different plot classes?
 public:
  ScatterDisplay(data::Table * t, layout::Layout * l) : DataXYDisplay(t, l) {}
  ~ScatterDisplay() {}
  
  void apply_config(nlohmann::json *) override;
  
  void update_mean_lines() override;
};

class LineDisplay : public virtual DataXYDisplay {
 public:
  LineDisplay(data::Table * t, layout::Layout * l) : DataXYDisplay(t, l) {}
  ~LineDisplay() {}
  
  void apply_config(nlohmann::json *) override;
  
  void update_mean_lines() override;
};

}  // namespace graphic
}  // namespace insight
#endif // SCATTERDISPLAY_H_
