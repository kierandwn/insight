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
#ifndef INSIGHT_CSV_H_
#define INSIGHT_CSV_H_

#include <stdlib.h>

#include <string>
#include <vector>
#include <fstream>

#include "table.h"

using namespace std;

namespace insight {

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

void findHeaderNames (string row, vector<string> * headers, string delim)
{
    size_t pos = 0;
    string column_header;
    string channel_name;

    while (row.size() > 0) {
        pos = row.find(delim);
        channel_name = row.substr(0, pos);
        trim(channel_name);

        headers->push_back(channel_name);

        if (pos != string::npos) { row.erase(0, pos + delim.length()); }
        else { row = ""; }
    }
}

double convert_to_type(string var) {
    return stod(var.c_str());
}

data::Table * import_from_csv (string filename, data::Table * t=new data::Table, string delim=",")
{
    ifstream f;
    f.open(filename);

    string row;
    getline(f, row);

    vector<string> headers;
    findHeaderNames(row, &headers, ",");

    // size_t n_columns = headers.size();
    size_t pos = 0;

    for (vector<string>::iterator h = headers.begin(); h != headers.end(); ++h)
        t->add(*(h));

    size_t i;
    vector<string>::iterator h;

    while (getline(f, row)) {
        pos = 0;
        i = 0;

        h = headers.begin();

        string element;

        while (row.size() > 0) {
            pos = row.find(delim);
            element = row.substr(0, pos);
            t->get(*(h))->push(convert_to_type(element));
            ++h;

            if (pos != string::npos) { row.erase(0, pos + delim.length()); }
            else { row = ""; }
        }
    }
    
    data::Channel * time_channel = t->get("time");
    
    for (size_t i = 0; i < headers.size(); ++i) {
        string channel_name = headers[i];
        t->get(channel_name)->update_time_channel_ptr(time_channel);
    }
    return t;
}


}  // namespace insight
#endif // INSIGHT_CSV_H_
