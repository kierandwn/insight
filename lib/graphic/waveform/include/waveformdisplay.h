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
#ifndef WAVEFORMDISPLAY_H
#define WAVEFORMDISPLAY_H
#pragma once

#include "ui_waveform.h"
#include "linked_graphic.h"

#include <vector>
#include <string>

#include <QLabel>
#include <QWidget>

#include <qwt_plot_curve.h>

#include "waveformgroup.h"
#include "mouse_state.h"
#include "table.h"
#include "grid.h"
#include "mainwindow.h"

#include "lib/json/single_include/nlohmann/json.hpp"

namespace insight {
namespace graphic {

using namespace std;


class WaveformDisplay : public LinkedPlot
{
  Q_OBJECT
private:
  QLabel m_xlabel;
    
  Ui::WaveformDisplay * p_ui = new Ui::WaveformDisplay;
  void define_uniform_spacing();
    
  vector<WaveformGroup *> m_waveform_groups;
  int m_nwaveform_groups;
    
  QwtPlotCurve m_cursor;
  double m_xpos_cursor;
  
//  double m_xlim[2];
  MouseState m_mouse_state;
    
  void cursor_in_xrange();

public:
  WaveformDisplay(data::Table *, layout::Layout *);
    
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void mouseDoubleClickEvent(QMouseEvent * event) override;
  void wheelEvent(QWheelEvent * event) override;

//    QString includeFile() const override { return QStringLiteral("waveformdisplay.h"); }
//    QString name() const override { return QStringLiteral("InsightWaveformDisplay"); }

//    QWidget * createWidget(QWidget * parent) override { return new WaveformDisplay(parent, data_); }

//    QString domXml() const override
//    {
//        return "<ui language=\"c++\">\n"
//               " <widget class=\"AnalogClock\" name=\"analogClock\">\n"
//               "  <property name=\"geometry\">\n"
//               "   <rect>\n"
//               "    <x>0</x>\n"
//               "    <y>0</y>\n"
//               "    <width>100</width>\n"
//               "    <height>100</height>\n"
//               "   </rect>\n"
//               "  </property>\n"
//               "  <property name=\"toolTip\" >\n"
//               "   <string>The current time</string>\n"
//               "  </property>\n"
//               "  <property name=\"whatsThis\" >\n"
//               "   <string>The analog clock widget displays the current time.</string>\n"
//               "  </property>\n"
//               " </widget>\n"
//               "</ui>\n";
//    }


  int get_number_of_waveform_groups() { return m_nwaveform_groups; }
    
  void apply_config(nlohmann::json *) override;
  void update_cursor_position(double) override;
  
//  void update_group_cursor_positions(double);
//  void update_group_view_limits(double, double);
//  double process_mouse_event(QMouseEvent *) override;
  
  void init_xlabel();
  
  void set_xlabel_position();
  void set_xlabel_value(double);
    
  void update_label_values_at(double);
  
  void update_after_data_load () override;
  void update_view_limits(double, double) override;
    
  void init () override;
  void reset () override;
  
  void resizeEvent(QResizeEvent *) override;
    
  double * xlim();
};

}  // namespace graphic
}  // namespace insight
#endif // WAVEFORMDISPLAY_H
