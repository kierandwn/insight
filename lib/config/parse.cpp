#include "lib/config/include/parse.h"

#include <iostream>
#include <fstream>
#include <map>

#include "lib/rapidjson/include/rapidjson/document.h"
#include <lib/rapidjson/include/rapidjson/istreamwrapper.h>

using namespace std;
using namespace rapidjson;

static const char* kTypeNames[] =
    { "Null", "False", "True", "Object", "Array", "String", "Number" };

void parseConfig()
{
    std::ifstream ifs { R"(C:\Users\kdwn\projects\insight\config\layout.config)" };
    if ( !ifs.is_open() )
    {
        std::cerr << "Could not open file for reading!\n";
        return;
    }

    IStreamWrapper isw{ ifs };

    Document d;
    d.ParseStream(isw);

    string type;

    for (auto& m : d.GetObject())
    {
        type = m.value.GetObject()["type"].GetString();

        printf("Type of member %s is %s.\n",
            m.name.GetString(), type.c_str());
    }

//    for (Value::ConstValueIterator itr = d.Begin(); itr != d.End(); ++itr) {
//        printf("hihihi\n");
//        printf("Type of member %s is %s\n", \
//            itr->name.GetString(), kTypeNames[itr->value.GetType()]);
//    }

    // 2. Modify it by DOM.
//    Value& s = d["stars"];
}
