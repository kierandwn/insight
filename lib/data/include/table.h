#ifndef DATATABLE_H
#define DATATABLE_H

#define CSV_IO_NO_THREAD

#include "lib/data/include/channel.h"
// #include "lib/csv/include/csv.h"

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
