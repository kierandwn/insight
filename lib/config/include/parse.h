#ifndef CONFIGPARSE_H
#define CONFIGPARSE_H

#include <iostream>
#include <fstream>
#include <map>

#include "lib/graphic/include/graphic.h"

static const char* kTypeNames[] =
    { "Null", "False", "True", "Object", "Array", "String", "Number" };

void parseConfig();


#endif // CONFIGPARSE_H