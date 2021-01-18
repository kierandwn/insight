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
#ifndef DATATABLE_H
#define DATATABLE_H

#include "channel.h"

#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;


class table {
private:
    std::map<std::string, channel *> m_channelMap;

public:
    table() {}
    ~table() { clear(); }

    channel * get(std::string id) {
        return m_channelMap[id];
    }

    void add_channel(string channel_name)
    {
        if (m_channelMap.find(channel_name) == m_channelMap.end()) {
            m_channelMap[channel_name] = new channel;
        }
    }

    bool exists(string id) { return (!(m_channelMap.find(id) == m_channelMap.end())); }

    void clear() {
        for (map<string, channel *>::iterator c = m_channelMap.begin(); c != m_channelMap.end(); ++c) {
            delete c->second;
        }
        m_channelMap.clear();
    }

    channel * operator[] (string id) { return m_channelMap[id]; }
};

#endif // DATATABLE_H
