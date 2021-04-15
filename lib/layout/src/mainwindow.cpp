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
#include "mainwindow.h"

#include <iostream>

#include <QStyle>
#include <QFileDialog>
#include <QFileInfo>

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
  load_config();

  m_layout.import_from_config(m_layout_filepath, ui->PlotGrid, &m_data);
  
  setWindowTitle(QFileInfo(m_layout_filepath.c_str()).fileName());
}

ApplicationMainWindow::~ApplicationMainWindow() { delete ui; }

void ApplicationMainWindow::update() {
  map<string, graphic::Base *>::iterator p;
  for (p = m_layout.first(); p != m_layout.last(); ++p) {
    p->second->update_after_data_load();
  }
}

void ApplicationMainWindow::load_config() {
  string config_filename;
  string user_specific_config = QDir::homePath().toStdString() + "/.insight";
  
  if (QFileInfo(user_specific_config.c_str()).exists()) {
    config_filename = QDir::homePath().toStdString() + "/.insight";
    
  } else {
    config_filename = src_root_dir_ + "/config/default.config";
    
  }
  import_from_json(config_filename);
}

void ApplicationMainWindow::import_from_json(string filename) {
  ifstream ifs { filename };
  if ( !ifs.is_open() ) { cerr << "Could not open file for reading!\n"; throw; }

  nlohmann::json app_config;
  ifs >> app_config;
  
  string layout_filename = app_config["layout_filename"];
  string layout_dirpath  = app_config["layout_dirpath"];
  
  string layout_filepath = layout_dirpath.append("/").append(layout_filename.c_str());
  
  if (!QFileInfo(layout_filepath.c_str()).exists()) {
    layout_filepath = QFileDialog::getOpenFileName(this,
      tr("Load Layout File"), m_layout_dirpath.c_str(), tr("Layout File (*.layout)")).toStdString();
  }

  m_layout_filepath = layout_filepath;
  
  m_time_channel_name = app_config["time_channel"];
  m_time_channel_unit = app_config["time_unit"];
  
  m_mainwindow_size[0] = app_config["main_window_size"][0];
  m_mainwindow_size[1] = app_config["main_window_size"][1];
}


void ApplicationMainWindow::init()
{
  setAutoFillBackground(true);
  QPalette background_palette = palette();

  background_palette.setColor(QPalette::Window, QColor(255, 255, 255, 255));
  setPalette(background_palette);
  
  fit_plot_area_to_main_window_area();
  
  map<string, graphic::Base *>::iterator p;
  for (p = m_layout.first(); p != m_layout.last(); ++p) {
    p->second->init();
  }
}

void ApplicationMainWindow::on_actionLoad_File_triggered()
{
  QStringList filenames = QFileDialog::getOpenFileNames(this,
    tr("Load Data File"), tr(src_root_dir_.append("/demo", 5).c_str()), tr("CSV Files (*.csv)"));
  
  string common_prefix = longest_common_string_prefix(filenames);
  printf("longest common string prefix: %s.\n", common_prefix.c_str());
  
  for (QString filename : filenames) {
    import_from_csv(filename.toStdString(),
                    &m_data,
                    common_prefix,
                    m_time_channel_name,
                    m_time_channel_unit
                    );
  }
  update();
}

void ApplicationMainWindow::resizeEvent(QResizeEvent * event)
{
  QMainWindow::resizeEvent(event);
  fit_plot_area_to_main_window_area();
}

void ApplicationMainWindow::fit_plot_area_to_main_window_area() {
  QRect geom = geometry();
  
  ui->centralwidget->setGeometry(0, 0, geom.width(), geom.height());
  ui->PlotGrid->setGeometry(QRect(0, 0, geom.width(), geom.height()));
}

string longest_common_string_prefix(QStringList string_list) {
  string lcs = string_list[0].toStdString();
  
  for (QString st : string_list) {
    lcs = longest_common_string_prefix(lcs, st.toStdString());
  }
  return lcs;
}

string longest_common_string_prefix(string X, string Y)
{
  int m = X.size();
  int n = Y.size();
  
  int i;
  for (i = 0; i < min(m, n); i++)
  {
    if (!(X[i] == Y[i])) {
      break;
    }
  }
  return X.substr(0, i);
}

}  // namespace insight
