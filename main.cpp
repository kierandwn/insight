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
#include <string>
#include <QApplication>


using namespace std;

int main(int argc, char *argv[])
{
    string file_path = __FILE__;
    string dir_path = file_path.substr(0, file_path.rfind("\\"));

    QApplication a(argc, argv);
    InsightMainWindow w(dir_path);
    w.show();

    return a.exec();
}
