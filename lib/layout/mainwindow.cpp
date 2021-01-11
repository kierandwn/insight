#include "lib/layout/include/mainwindow.h"
#include "lib/layout/include/grid.h"
#include "lib/data/include/table.h"
#include "lib/csv/include/csv.h"
#include "lib/graphic/include/plotupdate.h"

#include <QFileDialog>

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include <iostream>

using namespace std;

InsightMainWindow::InsightMainWindow(string source_root_dir, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::InsightMainWindow)
{
    ui->setupUi(this);

    QString layout_filename = QFileDialog::getOpenFileName(this,
        tr("Load Data File"), source_root_dir.append("/config", 8).c_str(), tr("Layout File (*.layout)"));

    insightLayout l;
    l.importFromConfig(layout_filename.toStdString(), ui->PlotGrid);

    QString filename = QFileDialog::getOpenFileName(this,
        tr("Load Data File"), "C:/Users/kdwn/projects/insight/demo", tr("CSV Files (*.csv)"));

    table * t = import_from_csv(filename.toStdString());

    plotUpdater u(&l, t);
    u.init();
}

InsightMainWindow::~InsightMainWindow()
{
    delete ui;
}

