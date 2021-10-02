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
// along with insight.  If not, see <https://www.gnu.org/licenses/>.
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

string determine_table_and_channel_ids(string row,
                                       vector<string> * channel_ids,
                                       string delim,
                                       string filename,
                                       string common_prefix
                                       )
{
  size_t pos = 0;
  string column_header;
  string channel_name;
  string channel_id;
  
  vector<string> channels;
  
  string table_id = filename.substr(common_prefix.size(), filename.size());
  table_id = table_id.substr(0, table_id.rfind("\."));
  
  while (row.size() > 0) {
    pos = row.find(delim);
    channel_id = row.substr(0, pos);
    trim(channel_id);
    data::condition_for_sql(channel_id);
    
    channel_ids->push_back(channel_id);

    if (pos != string::npos) { row.erase(0, pos + delim.length()); }
    else { row = ""; }
  }
  return table_id;
}

double convert_to_type(string var) {
    return stod(var.c_str());
}

void import_from_csv (string filename,
                      string dirpath,
                      int layer,
                      string common_prefix="",
                      string independent_var_channel_id="t",
                      string delim="," )
{
  string filepath = dirpath+"/"+filename;
  
  ifstream f;
  f.open(filepath);

  string row;
  getline(f, row);

  vector<string> channel_ids;
  string hreadable_id = determine_table_and_channel_ids(row,
                                                    &channel_ids,
                                                    ",",
                                                    filename,
                                                    common_prefix
                                                    );
  
  if (!data::has_been_imported_before(filepath))
  {
    data::add_file(filename, filepath, hreadable_id, channel_ids, independent_var_channel_id);
    
    size_t pos = 0;
    map<string, data::Channel> channels;
    
    for (string cid : channel_ids)
      channels[cid] = data::Channel();

    vector<string>::iterator cid;
    
    while (getline(f, row))
    {
      pos = 0;
      cid = channel_ids.begin();
      string element;
      
      while (row.size() > 0) {
        pos = row.find(delim);
        element = row.substr(0, pos);
        channels[*(cid)].push(convert_to_type(element));
        ++cid;
        
        if (pos != string::npos) { row.erase(0, pos + delim.length()); }
        else { row = ""; }
      }
    }
    
    data::add_channel_data(filepath,
                           channels,
                           channel_ids);
      
  }
  data::add_to_layer(filepath, layer);
  return;
}


}  // namespace insight
#endif // INSIGHT_CSV_H_
