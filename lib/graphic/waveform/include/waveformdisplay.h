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
    string channel_name_;
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

    void set_channel_name(string channel_name) { channel_name_ = channel_name; }
    string get_channel_name() { return channel_name_; }

    void reset () override;
    void update () override;

};

#endif // WAVEFORMDISPLAY_H
