#ifndef INSIGHTLAYOUT_H
#define INSIGHTLAYOUT_H

#include "lib/graphic/include/graphic.h"

#include "lib/rapidjson/include/rapidjson/document.h"

#include <QGridLayout>

#include <string>
#include <map>

using namespace std;

class gridLayout {
protected:
    int m_gridSize = 1;
    map<string, insightGraphic *> m_map;

public:
    map<string, insightGraphic *>::iterator first();
    map<string, insightGraphic *>::iterator next();
    map<string, insightGraphic *>::iterator last();

    map<string, insightGraphic *> map() { return m_map; }
};

class insightLayout : public gridLayout {
public:
    insightLayout() {};

    void importFromConfig( string filename, QGridLayout * grid );
    ::map<string, insightGraphic *> importFromConfig( rapidjson::Value&, QGridLayout * grid );
};

#endif // INSIGHTLAYOUT_H
