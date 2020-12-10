#ifndef INSIGHTLAYOUT_H
#define INSIGHTLAYOUT_H

#include "include/insightgraphic.h"

#include "lib/rapidjson/include/rapidjson/document.h"

#include <QGridLayout>

#include <map>

using namespace std;

class gridLayout {
private:
    int m_gridSize = 1;
    map<string, insightGraphic> m_map;
};

class insightLayout : public gridLayout {
public:
    insightLayout() {};

    void importFromConfig( string filename, QGridLayout * grid );
    void importFromConfig( rapidjson::Value&, QGridLayout * grid );
};

#endif // INSIGHTLAYOUT_H
