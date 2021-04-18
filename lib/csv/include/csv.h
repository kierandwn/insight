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
#pragma once

#include <stdlib.h>

#include <string>
#include <vector>
#include <fstream>

#include "table.h"

namespace insight {

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

string findHeaderNames (string row,
                      vector<string> * headers,
                      string delim,
                      string time_channel_name,
                      string filename,
                      string common_prefix
                      )
{
  size_t pos = 0;
  string column_header;
  string channel_name;
  string channel_id;
  
  vector<string> channels;
  
  string id_prefix = filename.substr(
     common_prefix.size(), filename.rfind(".")-common_prefix.size()).append("::");
  
  while (row.size() > 0) {
    pos = row.find(delim);
    channel_name = row.substr(0, pos);
    trim(channel_name);
    
    channel_id = id_prefix+channel_name;
    headers->push_back(channel_id);
    
    channels.push_back(channel_name);

    if (pos != string::npos) { row.erase(0, pos + delim.length()); }
    else { row = ""; }
  }
  
  data::add_table(id_prefix.substr(0, id_prefix.size()-2), channels, time_channel_name);
  return id_prefix+time_channel_name;
}

double convert_to_type(string var) {
    return stod(var.c_str());
}

data::Table * import_from_csv (string filename,
                               data::Table * t=new data::Table,
                               string common_prefix="",
                               string time_channel_name="time",
                               string time_channel_unit="s",
                               string delim=","
                               )
{
  ifstream f;
  f.open(filename);

  string row;
  getline(f, row);

  vector<string> headers;
  string time_channel_id = findHeaderNames(row,
                                           &headers,
                                           ",",
                                           time_channel_name,
                                           filename,
                                           common_prefix
                                           );

  // size_t n_columns = headers.size();
  size_t pos = 0;

  for (string h : headers)
    t->add(h);

  size_t k = 0;
  vector<string>::iterator h;

  // for new DB soln
  string id_prefix = filename.substr(
     common_prefix.size(), filename.rfind(".")-common_prefix.size()).append("::");
  
  while (getline(f, row)) {
    pos = 0;

    h = headers.begin();

    string element;

//    data::add_to_table(id_prefix.substr(0, id_prefix.size()-2));
//    string db_channel_id;
    
    data::add_to_table(id_prefix.substr(0, id_prefix.size()-2), row, k);
    
    while (row.size() > 0) {
      pos = row.find(delim);
      element = row.substr(0, pos);
      t->get(*(h))->push(convert_to_type(element));
      
//      db_channel_id = h->substr(id_prefix.size(), h->size());
      ++h;
      
      if (pos != string::npos) { row.erase(0, pos + delim.length()); }
      else { row = ""; }
    }
    ++k;
  }
    
  if (t->exists(time_channel_id)) {
    data::Channel * time_channel = t->get(time_channel_id.c_str());
    time_channel->set_unit_string(time_channel_unit);
    
    for (size_t i = 0; i < headers.size(); ++i) {
      string channel_name = headers[i];
      t->get(channel_name)->update_time_channel_ptr(time_channel);
    }
  }
  return t;
}


}  // namespace insight
#endif // INSIGHT_CSV_H_
