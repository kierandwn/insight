#ifndef DATATABLE_H
#define DATATABLE_H

#define CSV_IO_NO_THREAD

#include "lib/data/include/channel.h"
#include "lib/csv/csv.h"

#include <string>
#include <map>

class table {
private:
    std::map<std::string, channel *> m_channelMap;

public:
    table() {}

    channel * get(std::string id) { return m_channelMap[id]; }

    void importFromCSV(std::string filename)
    {
        const int nRows = 9;

        // TODO: check that filename exists in filesystem.
        io::CSVReader<nRows> in(filename);
        in.read_header(io::ignore_extra_column, "a", "b", "c", "d", "e", "f", "g", "h", "i");

        m_channelMap["a"] = new channel;
        m_channelMap["b"] = new channel;
        m_channelMap["c"] = new channel;
        m_channelMap["d"] = new channel;
        m_channelMap["e"] = new channel;
        m_channelMap["f"] = new channel;
        m_channelMap["g"] = new channel;
        m_channelMap["h"] = new channel;
        m_channelMap["i"] = new channel;

        double valA, valB, valC, valD, valE, valF, valG, valH, valI;

        while(in.read_row(valA, valB, valC, valD, valE, valF, valG, valH, valI))
        {
            m_channelMap["a"]->push(valA);
            m_channelMap["b"]->push(valB);
            m_channelMap["c"]->push(valC);
            m_channelMap["d"]->push(valD);
            m_channelMap["e"]->push(valE);
            m_channelMap["f"]->push(valF);
            m_channelMap["g"]->push(valG);
            m_channelMap["h"]->push(valH);
            m_channelMap["i"]->push(valI);
        }
    }
};

#endif // DATATABLE_H
