#ifndef INSIGHT_CSV_H_
#define INSIGHT_CSV_H_

#include <stdlib.h>

#include <string>
#include <vector>
#include <fstream>

#include "lib/data/include/table.h"

using namespace std;

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

table * import_from_csv (string filename, string delim=",", table * t=new table)
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
        t->add_channel(*(h));

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
    return t;
}

#endif // INSIGHT_CSV_H_