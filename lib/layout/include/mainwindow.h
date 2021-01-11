#ifndef INSIGHTMAINWINDOW_H
#define INSIGHTMAINWINDOW_H

#include "ui_mainwindow.h"

#include <string.h>

#include <QMainWindow>
#include <QBoxLayout>

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include "lib/layout/include/grid.h"
#include "lib/data/include/table.h"
#include "lib/graphic/include/plotupdate.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class InsightMainWindow; }
QT_END_NAMESPACE

class InsightMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    InsightMainWindow(std::string source_main_directory, QWidget *parent = nullptr);
    ~InsightMainWindow();

    double gain = 10.;
    static const int count = 100;

    // data arrays for the plot
    double xData[count];

    double y1Data[count];
    double y2Data[count];

    insightLayout layout;
    table data;

    plotUpdater updater = plotUpdater(&layout, &data);

private slots:
    void on_actionLoad_File_triggered();

private:
    Ui::InsightMainWindow *ui;
};
#endif // INSIGHTMAINWINDOW_H
