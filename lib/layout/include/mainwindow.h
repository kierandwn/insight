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

#include "lib/layout/include/grid.h"
#include "lib/data/include/table.h"

using namespace std;

//QT_BEGIN_NAMESPACE
//namespace Ui { class InsightMainWindow; }
//QT_END_NAMESPACE

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

    void update();

private slots:
    void on_actionLoad_File_triggered();

private:
    Ui::InsightMainWindow *ui;
    string src_root_dir_;
};
#endif // INSIGHTMAINWINDOW_H
