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
// along with attitude.  If not, see <https://www.gnu.org/licenses/>.
//
#ifndef WAVEFORMDISPLAY_H
#define WAVEFORMDISPLAY_H

#include "ui_waveform.h"

#include "lib/data/include/table.h"
#include "lib/graphic/include/insight_graphic_base.h"


using namespace std;

//QT_BEGIN_NAMESPACE
//namespace Ui { class  WaveformDisplay; }
//QT_END_NAMESPACE

class WaveformDisplay : public QwtPlot, virtual public InsightBaseGraphic
{
private:
    vector<string> channel_names_;
    string test_;

    Ui::WaveformDisplay * ui = new Ui::WaveformDisplay;

public:
    QWidget * PlotArea;

    WaveformDisplay() {
        ui->setupUi(this);
    }
    WaveformDisplay(table * data) : InsightBaseGraphic(data) {
        ui->setupUi(this);
    }
    WaveformDisplay(QWidget * parent, table * data)
        : QwtPlot(parent),
          InsightBaseGraphic(data)
    {
        ui->setupUi(this);
    }


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

    void add_channel_name(string channel_name) { channel_names_.push_back(channel_name); }

    string get_channel_name(int i) { return channel_names_[i]; }
    int get_number_of_channels() { return channel_names_.size(); }

    void reset () override;
    void update () override;

};

#endif // WAVEFORMDISPLAY_H
