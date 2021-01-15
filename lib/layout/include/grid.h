#ifndef INSIGHTLAYOUT_H
#define INSIGHTLAYOUT_H

//#include "lib/graphic/include/graphic.h"
#include "lib/data/include/table.h"
#include "lib/graphic/waveform/include/waveformdisplay.h"

#include "lib/rapidjson/include/rapidjson/document.h"
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
#include <QGridLayout>

#include <string>
#include <map>

using namespace std;

class gridLayout {
protected:
    int m_gridSize = 1;
    map<string, InsightBaseGraphic *> m_map;

public:
    map<string, InsightBaseGraphic *>::iterator first();
    map<string, InsightBaseGraphic *>::iterator next();
    map<string, InsightBaseGraphic *>::iterator last();

    map<string, InsightBaseGraphic *> map() { return m_map; }
};

class insightLayout : public gridLayout {
public:
    insightLayout() {};

    void importFromConfig( string filename, QGridLayout * grid, table * data );
    ::map<string, InsightBaseGraphic *> importFromConfig( rapidjson::Value&, QGridLayout * grid, table * data );
};

#endif // INSIGHTLAYOUT_H
