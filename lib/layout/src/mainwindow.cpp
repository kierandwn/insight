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
#include "mainwindow.h"

#include <iostream>

#include <QStyle>
#include <QFileDialog>

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include "grid.h"
#include "table.h"
#include "csv.h"


namespace insight {


ApplicationMainWindow::ApplicationMainWindow(string source_root_dir, QWidget * parent)
    : QMainWindow(parent),
      ui(new Ui::InsightMainWindow),
      src_root_dir_(source_root_dir)
{
    ui->setupUi(this);

    QString layout_filename = QFileDialog::getOpenFileName(this,
        tr("Load Data File"), src_root_dir_.append("/config", 8).c_str(), tr("Layout File (*.layout)"));

    m_layout.import_from_config(layout_filename.toStdString(), ui->PlotGrid, &m_data);
}

ApplicationMainWindow::~ApplicationMainWindow() { delete ui; }

void ApplicationMainWindow::update() {
    map<string, graphic::Base *>::iterator p;
    for (p = m_layout.first(); p != m_layout.last(); ++p) {
        p->second->update_after_data_load();
    }
}

void ApplicationMainWindow::init() {
    
    map<string, graphic::Base *>::iterator p;
    for (p = m_layout.first(); p != m_layout.last(); ++p) {
        p->second->init();
    }
//    statusBar()->setFont(QFont("Courier", 10));
//    statusBar()->showMessage("initialised.");
}

void ApplicationMainWindow::update_cursors_in_group(string group_name, double xval) {
    if (group_name == "") return;
    
    map<string, graphic::Base *>::iterator p;
    for (p = m_layout.first(); p != m_layout.last(); ++p) {
        if (p->second->group() == group_name) {
            p->second->update_cursor_position(xval);
        }
    }
}

void ApplicationMainWindow::on_actionLoad_File_triggered()
{
    m_data.clear();

    QString filename = QFileDialog::getOpenFileName(this,
        tr("Load Data File"), tr(src_root_dir_.append("/demo", 5).c_str()), tr("CSV Files (*.csv)"));

    import_from_csv(filename.toStdString(), &m_data);
    update();
}

void ApplicationMainWindow::resizeEvent(QResizeEvent * event)
{
  QMainWindow::resizeEvent(event);
  
  QSize size = event->size();
  ui->PlotGrid->setGeometry(QRect(0, 0, size.width(), size.height()));
  
//  const int n_rows = m_layout.rowCount();
//  const int n_cols = m_layout.columnCount();
  
//  QObject * parent;
//  int width;
//  int height;
//
//  map<string, graphic::Base *>::iterator p;
//  for (p = m_layout.first(); p != m_layout.last(); ++p) {
//    parent = p->second->parent();
//
//    width = parent->width();
//    height = parent->height();
//
//    p->second->resize(width / 2, height / 2);
//  }
}

}  // namespace insight
