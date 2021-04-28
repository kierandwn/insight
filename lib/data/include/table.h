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
#ifndef DATATABLE_H
#define DATATABLE_H
#pragma once

#include <fstream>
#include <string>
#include <map>
#include <vector>

#include <QSqlQuery>

#include "channel.h"

namespace insight {
namespace data {

using namespace std;

void condition_for_sql(string& s);

void open_db(string);
void close_db();

void establish_db(string);

void add_files_table();
void add_units_table();

void add_maths_table();
void add_layer_table(int=0);

void delete_layer_tables();
void delete_maths_tables();

void add_to_layer(string, int i=0);

void add_file(string, string, string, vector<string>, string);
void add_table(int, string, vector<string>);

void init_unit_row(string, string);

QSqlQuery get_file_record(string);
int get_table_id(string);

int get_table_count(string);
int get_row_count(string);

bool does_file_exist(string);
bool does_table_exist(string);

bool hid_in_layer(string, int=0);
bool mid_in_layer(string, int=0);

void add_index_channel(string, int);
void add_channel_data(string, string, double *, int);
void add_channel_data(string, map<string, Channel>, vector<string>);

void get_channel_data(string, string, Channel *, Channel *);

void compute_math_channels(int, string, string);

class Table {
private:
  map<string, Channel *> m_channels_in_memory;
  string m_time_channel_name;
  string m_time_channel_unit;

public:
  Table() {}
  ~Table() { clear(); }

  Channel * get(string id);
  
  void set_time_channel_name(string channel_name) { m_time_channel_name = channel_name; }
  void set_time_channel_unit(string channel_unit) { m_time_channel_unit = channel_unit; }
  
  string get_time_channel_name() { return m_time_channel_name; }
  string get_time_channel_unit() { return m_time_channel_unit; }

  void add(string channel_name)
  {
    if (m_channels_in_memory.find(channel_name) == m_channels_in_memory.end()) {
      m_channels_in_memory[channel_name] = new Channel;
    }
  }

//  bool exists(string id) { return (!(m_channels_in_memory.find(id) == m_channels_in_memory.end())); }
  bool exists(string id);

  void clear() {
    for (map<string, Channel *>::iterator c = m_channels_in_memory.begin(); c != m_channels_in_memory.end(); ++c) {
      delete c->second;
    }
    m_channels_in_memory.clear();
  }

  Channel * operator[] (string id) { return m_channels_in_memory[id]; }
};

}  // namespace data
}  // namespace insight
#endif // DATATABLE_H
