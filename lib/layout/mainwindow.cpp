#include "lib/layout/include/mainwindow.h"
#include "lib/layout/include/grid.h"
#include "lib/data/include/table.h"
#include "lib/graphic/include/plotupdate.h"

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include <iostream>

InsightMainWindow::InsightMainWindow(QWidget *parent) : QMainWindow(parent),
                                                        ui(new Ui::InsightMainWindow)
{
    ui->setupUi(this);

    insightLayout l;
    l.importFromConfig("C:/Users/kdwn/projects/insight/config/layout.config", ui->PlotGrid);

    table t;
    t.importFromCSV("C:/Users/kdwn/projects/insight/demo/table.csv");

    plotUpdater u(&l, &t);
    u.init();

    std::cout << "hello." << std::endl;
}

InsightMainWindow::~InsightMainWindow()
{
    delete ui;
}

