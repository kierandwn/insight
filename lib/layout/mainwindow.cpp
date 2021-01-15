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
#include "lib/layout/include/mainwindow.h"

#include <iostream>

#include <QFileDialog>

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include "lib/layout/include/grid.h"
#include "lib/data/include/table.h"
#include "lib/csv/include/csv.h"


using namespace std;

InsightMainWindow::InsightMainWindow(string source_root_dir, QWidget * parent)
    : QMainWindow(parent),
      ui(new Ui::InsightMainWindow),
      src_root_dir_(source_root_dir)
{
    ui->setupUi(this);

//    QGridLayout * grid = ui->PlotGrid;
//    grid->addWidget(new WaveformDisplay(), 0, 0);

    QString layout_filename = QFileDialog::getOpenFileName(this,
        tr("Load Data File"), src_root_dir_.append("/config", 8).c_str(), tr("Layout File (*.layout)"));

    layout.importFromConfig(layout_filename.toStdString(), ui->PlotGrid, &data);
}

InsightMainWindow::~InsightMainWindow() { delete ui; }

void InsightMainWindow::update() {
    map<string, InsightBaseGraphic *>::iterator p;
    for (p = layout.first(); p != layout.last(); ++p) {
        p->second->update();
    }
}


void InsightMainWindow::on_actionLoad_File_triggered()
{
    data.clear();

    QString filename = QFileDialog::getOpenFileName(this,
        tr("Load Data File"), tr(src_root_dir_.append("/demo", 5).c_str()), tr("CSV Files (*.csv)"));

    import_from_csv(filename.toStdString(), &data);
    update();
}
