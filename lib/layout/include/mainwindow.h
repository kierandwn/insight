#ifndef INSIGHTMAINWINDOW_H
#define INSIGHTMAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include "ui_insightmainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InsightMainWindow; }
QT_END_NAMESPACE

class InsightMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    InsightMainWindow(QWidget *parent = nullptr);
    ~InsightMainWindow();

    double gain = 10.;
    static const int count = 100;

    // data arrays for the plot
    double xData[count];

    double y1Data[count];
    double y2Data[count];

private:
    Ui::InsightMainWindow *ui;
};
#endif // INSIGHTMAINWINDOW_H
