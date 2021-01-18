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

#include "grid.h"
#include "table.h"


using namespace std;

class InsightMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    InsightMainWindow(std::string source_main_directory, QWidget *parent = nullptr);
    ~InsightMainWindow();

    insightLayout layout;
    table data;

    void update();

private slots:
    void on_actionLoad_File_triggered();

private:
    Ui::InsightMainWindow *ui;
    string src_root_dir_;
};


#endif // INSIGHTMAINWINDOW_H
