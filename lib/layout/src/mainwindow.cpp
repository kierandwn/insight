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
// insight is distributed in the hope that it will be useful,
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


string k_USER_SPECIFIC_CONFIG_FILEPATH = QDir::homePath().toStdString() + "/.insight/.config";
string k_DEFAULT_DB_FILEPATH = QDir::homePath().toStdString() + "/.insight/insight.db";

ApplicationMainWindow::ApplicationMainWindow(string source_root_dir, QWidget * parent)
    : QMainWindow(parent),
      ui(new Ui::InsightMainWindow),
      m_source_root_dirpath(source_root_dir)
{
  ui->setupUi(this);
  load_config();
  
  data::establish_db(m_db_filepath);

  m_layout.import_from_config(m_layout_filepath, ui->PlotGrid, &m_data);
  
  setWindowTitle(QFileInfo(m_layout_filepath.c_str()).fileName());
}

ApplicationMainWindow::~ApplicationMainWindow()
{
  delete ui;
  data::shutdown_db();
}

void ApplicationMainWindow::update() {
  map<string, graphic::Base *>::iterator p;
  for (p = m_layout.first(); p != m_layout.last(); ++p) {
    p->second->update_after_data_load();
  }
}

void ApplicationMainWindow::load_config() {
  string config_filename;
  
  if (QFileInfo(k_USER_SPECIFIC_CONFIG_FILEPATH.c_str()).exists()) {
    config_filename = k_USER_SPECIFIC_CONFIG_FILEPATH;
    
  } else {
    config_filename = m_source_root_dirpath + "/config/default.config";
    
  }
  import_from_json(config_filename);
}

void ApplicationMainWindow::import_from_json(string filename) {
  ifstream ifs { filename };
  if ( !ifs.is_open() ) { cerr << "Could not open file for reading!\n"; throw; }

  nlohmann::json app_config;
  ifs >> app_config;
  
  string db_filepath = app_config["db"];
  if ( db_filepath == "" ) {
    m_db_filepath = k_DEFAULT_DB_FILEPATH;
  } else {
    m_db_filepath = db_filepath;
  }
  
  string layout_filename = app_config["layout_filename"];
  string layout_dirpath  = app_config["layout_dirpath"];
  
  string layout_filepath = layout_dirpath.append("/").append(layout_filename.c_str());
  
  if (!QFileInfo(layout_filepath.c_str()).exists()) {
    layout_filepath = QFileDialog::getOpenFileName(this,
      tr("Load Layout File"), m_layout_dirpath.c_str(), tr("Layout File (*.layout)")).toStdString();
  }

  m_layout_filepath = layout_filepath;
  
//  m_time_channel_name = app_config["time_channel"];
//  m_time_channel_unit = app_config["time_unit"];
  
  m_data.set_time_channel_name(app_config["time_channel"]);
  m_data.set_time_channel_unit(app_config["time_unit"]);
  
  m_mainwindow_size[0] = app_config["main_window_size"][0];
  m_mainwindow_size[1] = app_config["main_window_size"][1];
}


void ApplicationMainWindow::init()
{
  setAutoFillBackground(true);
  QPalette background_palette = palette();

  background_palette.setColor(QPalette::Window, QColor(255, 255, 255, 255));
  setPalette(background_palette);
  
  map<string, graphic::Base *>::iterator p;
  for (p = m_layout.first(); p != m_layout.last(); ++p) {
    p->second->init();
  }
  update();
  
  fit_plot_area_to_main_window_area();
}

vector<string> remove_dirpath(QStringList filepaths, string dirpath)
{
  vector<string> result;
  for (QString filepath : filepaths) {
    result.push_back(filepath.toStdString().substr(dirpath.size()+1, filepath.size()));
  }
  return result;
}

void ApplicationMainWindow::load_data_from_files(int layer)
{
  QStringList filepaths = QFileDialog::getOpenFileNames(this,
    tr("Load Data File"), tr((m_source_root_dirpath+"/demo").c_str()), tr("CSV Files (*.csv)"));
  
  if (filepaths.size() > 0)
  {
    string common_prefix = "";
    
    string fullpath = filepaths[0].toStdString();
    string dirpath = fullpath.substr(0, fullpath.rfind("/"));
    
    vector<string> filenames = remove_dirpath(filepaths, dirpath);
    
    if (filenames.size() > 1) {
      common_prefix = longest_common_string_prefix(filenames);
    }
    
    for (string filename : filenames) {
      import_from_csv(filename,
                      dirpath,
                      common_prefix,
                      m_data.get_time_channel_name()
                      );
    }
    data::compute_math_channels(layer, m_db_filepath, m_source_root_dirpath);
    update();
  }
}

void ApplicationMainWindow::resizeEvent(QResizeEvent * event)
{
  QMainWindow::resizeEvent(event);
  fit_plot_area_to_main_window_area();
}

void ApplicationMainWindow::fit_plot_area_to_main_window_area()
{
  QRect geom = geometry();
  
  ui->centralwidget->setGeometry(0, 0, geom.width(), geom.height());
  ui->PlotGrid->setGeometry(QRect(0, 0, geom.width(), geom.height()));
}

string longest_common_string_prefix(vector<string> string_list)
{
  string lcs = string_list[0];
  
  for (string st : string_list) {
    lcs = longest_common_string_prefix(lcs, st);
  }
  return lcs;
}

string longest_common_string_prefix(string X, string Y)
{
  size_t m = X.size();
  size_t n = Y.size();
  
  size_t i;
  for (i = 0; i < min(m, n); i++)
  {
    if (!(X[i] == Y[i])) {
      break;
    }
  }
  return X.substr(0, i);
}

}  // namespace insight

void insight::ApplicationMainWindow::on_actionImport_from_Files_triggered(int layer)
{
    load_data_from_files(layer);

}


