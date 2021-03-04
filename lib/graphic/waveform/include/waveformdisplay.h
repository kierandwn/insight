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

#include <string>

#include <QLabel>
#include <qwt_plot_curve.h>

#include "ui_waveform.h"
#include "insight_graphic_base.h"

#include "lib/json/single_include/nlohmann/json.hpp"
#include "table.h"


using namespace std;

namespace insight {
namespace graphic {


class WaveformGroup {
 private:
  QwtPlot * p_parent;
    
  QwtPlotCurve m_curve; // TODO: multiple curves later, overlays?
  QwtPlotCurve m_zero_line;
  
  QLabel m_label;
    
  string m_channel_name = "";

 public:
  double m_normalised_height;
  double m_normalised_yoffset;
    
  WaveformGroup(QwtPlot * parent);
    
  void add_channel(string);
  void set_dimensions(double, double);
    
  void set_label_color(int, int, int);
  void set_label_value(double);
    
  string get_channel_name() { return m_channel_name; }
  QwtPlotCurve * get_curve_ref() { return &m_curve; }
    
  void attach(QwtPlot *);
  void set_data_from_channel(data::Channel *);
};

class WaveformDisplay : public QwtPlot, virtual public Base
{
  Q_OBJECT

private:
  vector<string> m_channel_names;

  Ui::WaveformDisplay * p_ui = new Ui::WaveformDisplay;
  void define_uniform_spacing();
    
  vector<WaveformGroup *> m_waveform_groups;
  int m_nwaveform_groups;
    
  QwtPlotCurve m_cursor;

public:
  WaveformDisplay(data::Table * data);
  QWidget * p_plot_widget;
    
  void mousePressEvent(QMouseEvent * event) override;

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

  void add_channel_by_name(string channel_name)
  {
    m_channel_names.push_back(channel_name);
  }

  string get_channel_name(int i) { return m_channel_names[i]; }
  int get_number_of_waveform_groups() { return m_nwaveform_groups; }

  void apply_config(nlohmann::json *) override;
  void update_cursor_position(double);
    
  void update_label_values_at(double);
  
  void reset () override;
  void update () override;

};

}  // namespace graphic
}  // namespace insight
#endif // WAVEFORMDISPLAY_H
