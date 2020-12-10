#include "include/insightgraphic.h"

#include "lib/rapidjson/include/rapidjson/document.h"

using namespace std;

//string type(auto& v) { return v.value.GetObject()["type"].GetString(); }

bool is_viable(string graphicClassName)
{
    for (size_t i = 0; i < INSIGHT_GRAPHICS.size(); i++) {
        if ( INSIGHT_GRAPHICS[i] == graphicClassName ) { return true; }
    }
    return false;
}

//insightGraphic::insightGraphic()
//{
//private:

//}
