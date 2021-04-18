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
#ifndef DATATABLE_H
#define DATATABLE_H
#pragma once

#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "channel.h"

namespace insight {
namespace data {

using namespace std;

void condition_for_sql(string& s);

void establish_db(string);
void add_table(string, vector<string>, string);
void add_to_table(string, string, int);

class Table {
private:
    map<string, Channel *> m_channels;

public:
    Table() {}
    ~Table() { clear(); }

    Channel * get(string id) { return m_channels[id]; }

    void add(string channel_name)
    {
        if (m_channels.find(channel_name) == m_channels.end()) {
            m_channels[channel_name] = new Channel;
        }
    }

    bool exists(string id) { return (!(m_channels.find(id) == m_channels.end())); }

    void clear() {
        for (map<string, Channel *>::iterator c = m_channels.begin(); c != m_channels.end(); ++c) {
            delete c->second;
        }
        m_channels.clear();
    }

    Channel * operator[] (string id) { return m_channels[id]; }
};

}  // namespace data
}  // namespace insight
#endif // DATATABLE_H
