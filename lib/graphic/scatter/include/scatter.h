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
#include <tuple>
#include <string>

#include <QLabel>
#include <QStaticText>
#include <QStylePainter>
#include <qwt_plot_curve.h>

#include "ui_scatter.h"

#include "insight_graphic_base.h"
#include "linked_graphic.h"

#include "table.h"
#include "grid.h"
#include "mainwindow.h"

#include "lib/json/single_include/nlohmann/json.hpp"


namespace insight {
namespace graphic {

using namespace std;


class VLabel : public QLabel
{
  Q_OBJECT
  
 public:
  VLabel(QWidget *parent) : QLabel(parent) {}
  VLabel(const QString& text, QWidget * parent) : QLabel(text, parent) {}
  
 protected:
  void paintEvent(QPaintEvent *)
  {
    QStylePainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::Dense1Pattern);

//    painter.translate(sizeHint().width(), sizeHint().height());
    painter.translate(sizeHint().width(), 0);
    painter.rotate(90);
    
//    painter.drawStaticText(0, 0, height(), width(), alignment(), text());
    painter.drawStaticText(0, 0, QStaticText(text()));
  }
  
  QSize minimumSizeHint() const
  {
    QSize s = QLabel::minimumSizeHint();
    return QSize(s.height(), s.width());
  }

  QSize sizeHint() const
  {
    QSize s = QLabel::sizeHint();
    return QSize(s.height(), s.width());
  }
};

class ScatterGroup {
 private:
  QwtPlot * p_parent;

  QwtPlotCurve m_scatter;
  QwtPlotCurve m_shadow;
  
  QwtSymbol m_symbol;
  QwtSymbol m_shadow_symbol;

  double m_xlim[2]; double m_ylim[2];

  string m_xchannel_name;
  string m_ychannel_name;
  
  int m_color_index;

 public:
  ScatterGroup(QwtPlot *, string, string, int);
  ~ScatterGroup();

  void init_scatters();
  void init_labels(data::Table *);

  string get_xchannel_name() { return m_xchannel_name; }
  string get_ychannel_name() { return m_ychannel_name; }
    
  void set_label_values_at(double, data::Table *);
  void set_metric_values(double, double, double);

  double * xlim() { return m_xlim; }
  double * ylim() { return m_ylim; }

  void set_data_from_table(data::Table *, double, double);
};

class DisplayCrosshair {
 private:
  QwtPlotCurve m_horzbar;
  QwtPlotCurve m_vertbar;
    
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
  }
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
  
//  double m_xlim[2];
//  double m_mouse_xpos;
  
//  bool m_drag_cursor = false;
//  bool m_panning = false;
    
//  void cursor_in_xrange();
    void update_mean_lines();
    
public:
  ScatterDisplay(data::Table *, layout::Layout *);
    
//  void mousePressEvent(QMouseEvent * event) override;
//  void mouseMoveEvent(QMouseEvent * event) override;
//  void mouseReleaseEvent(QMouseEvent * event) override {
//      m_drag_cursor = false;
//      m_panning = false;
//  }
//  void mouseDoubleClickEvent(QMouseEvent * event) override;
//  void wheelEvent(QWheelEvent * event) override;


  int get_number_of_scatter_pairs() { return m_nscatter_pairs; }
    
  void apply_config(nlohmann::json *) override;
  void update_cursor_position(double) override;
  
  void init_labels();
  void set_label_values_at(double);
  
  void update_after_data_load () override;
  void update_view_limits(double, double) override;
    
  void init () override;
  void reset () override;
    
  void xlim(double *);
  void ylim(double *);
};

}  // namespace graphic
}  // namespace insight
#endif // SCATTERDISPLAY_H_
