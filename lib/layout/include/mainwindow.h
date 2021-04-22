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
#ifndef INSIGHTMAINWINDOW_H
#define INSIGHTMAINWINDOW_H
#pragma once

#include "ui_mainwindow.h"

#include <string.h>

#include <QMainWindow>
#include <QBoxLayout>
#include <QStringList>

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include "table.h"
#include "grid.h"

namespace insight {

using namespace std;


string longest_common_string_prefix(string, string);
string longest_common_string_prefix(vector<string>);

class ApplicationMainWindow : public QMainWindow
{
  Q_OBJECT
 private:
  Ui::InsightMainWindow * ui;
  string src_root_dir_;
  
  string m_layout_dirpath;
  string m_layout_filepath;
  
  string m_db_filepath;
  
  double m_mainwindow_size[2];
  
//  string m_time_channel_name; // TODO temp fix until queried at load
//  string m_time_channel_unit;
  
 public:
  ApplicationMainWindow(std::string source_main_directory, QWidget *parent = nullptr);
  ~ApplicationMainWindow();

  layout::Layout m_layout;
  data::Table m_data;
  
  void load_config();
  void import_from_json(string);
  
  void init();
  void update();
  
  void fit_plot_area_to_main_window_area();
    
 private slots:
  void on_actionLoad_File_triggered();
  void resizeEvent(QResizeEvent * event) override;
};


} // namespace insight
#endif // INSIGHTMAINWINDOW_H
