#include "lib/layout/include/mainwindow.h"
#include "lib/layout/include/grid.h"

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

InsightMainWindow::InsightMainWindow(QWidget *parent) : QMainWindow(parent),
                                                        ui(new Ui::InsightMainWindow)
{
    ui->setupUi(this);

    insightLayout l;
    l.importFromConfig("C:/Users/kdwn/projects/insight/config/layout.config", ui->PlotGrid);

}

InsightMainWindow::~InsightMainWindow()
{
    delete ui;
}

