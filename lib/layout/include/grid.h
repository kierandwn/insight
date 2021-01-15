#ifndef INSIGHTLAYOUT_H
#define INSIGHTLAYOUT_H

//#include "lib/graphic/include/graphic.h"
#include "lib/data/include/table.h"
#include "lib/graphic/waveform/include/waveformdisplay.h"

#include "lib/rapidjson/include/rapidjson/document.h"

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
