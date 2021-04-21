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
#include "table.h"

#include <QFileInfo>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include <string>
#include <algorithm>
#include <fstream>

namespace insight {
namespace data {

using namespace std;

int k_FILE_TABLE_LEN = 0;
QSqlDatabase k_DB;

string stringify(double d) {
  string s;
  if (isnan(d) || isinf(d))
    s = "NULL";
  else
    s = to_string(d);
  
  return s;
}

QSqlQuery sql_query(string s) {
  QSqlQuery q(k_DB);
  q.prepare(s.c_str());
  
  if(!q.exec()) {
    qWarning() << "ERROR: " << q.lastError().text();
    qWarning() << "\t query: " << q.executedQuery();
  }
  return q;
}

void sql_query(QSqlQuery& q) {
  if(!q.exec()) {
    qWarning() << "ERROR: " << q.lastError().text();
    qWarning() << "\t query: " << q.executedQuery();
  }
}

void condition_for_sql(string& s) {
  replace(s.begin(), s.end(), '[', '_');
  replace(s.begin(), s.end(), ']', '_');
  replace(s.begin(), s.end(), '-', '_');
  replace(s.begin(), s.end(), '/', '_');
  replace(s.begin(), s.end(), '\\', '_');
  replace(s.begin(), s.end(), '.', '_');
  return;
}

void open_db(string filepath) {
  const QString DRIVER("QSQLITE");
   
  if ( QSqlDatabase::isDriverAvailable(DRIVER) ) {
    QSqlDatabase k_DB = QSqlDatabase::addDatabase(DRIVER);
    
    k_DB.setDatabaseName(filepath.c_str());
  
    // Check that database is open
    if(!k_DB.open()) {
      qWarning() << "ERROR: " << k_DB.lastError();
    }
  }
  return;
}
void establish_db(string filepath)
{
  if (QFileInfo(filepath.c_str()).exists()) {
    open_db(filepath);
  } else {
    open_db(filepath);
    
    // Create file table
    sql_query("CREATE TABLE files ( table_id INT PRIMARY KEY, filename TEXT, dirpath TEXT )");
  }
  k_FILE_TABLE_LEN = get_row_count("files");
  return;
}

void add_file(string filename, string filepath, vector<string> channel_names)
{
  condition_for_sql(filepath);
  
  if (!get_table_id(filepath))
  {
    QSqlQuery q(k_DB);
    q.prepare("INSERT INTO files VALUES(?, ?, ?)");
    q.bindValue(0, to_string(++k_FILE_TABLE_LEN).c_str());
    q.bindValue(1, filename.c_str());
    q.bindValue(2, filepath.c_str());
    sql_query(q);
    
    add_table(filename, channel_names);
  }
}

void add_table(string filename, vector<string> channel_names)
{
  string query_string = "CREATE TABLE " + filename + " (idx INT PRIMARY KEY";
  string channel_name;
  for (size_t i = 0; i < channel_names.size(); ++i)
  {
    channel_name = channel_names[i];
    
    condition_for_sql(channel_name);
    query_string += ", "+channel_name+" DOUBLE";
  }
  query_string += ");";
  sql_query(query_string);
}

int get_table_id(string filepath) {
//  QSqlQuery q = sql_query("SELECT table_id FROM files WHERE files.dirpath=\'" + filepath + "\');");
  QSqlQuery q(k_DB);
  q.prepare("SELECT table_id FROM files WHERE files.dirpath=?");
  q.bindValue(0, filepath.c_str());
  sql_query(q);
  
  int tid;
  if (q.first())
    tid = q.value(0).toInt();
  else
    tid = NULL;
      
  return tid;
}

bool does_table_exist(string table_name) {
  return get_table_count(table_name) > 0;
}

int get_table_count(string table_name) {
  QSqlQuery q = sql_query("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='" + table_name + "';");
  
  q.first();
  return q.value(0).toInt();
}

int get_row_count(string table_name) {
  QSqlQuery q = sql_query("SELECT COUNT(*) FROM " + table_name);
  q.first();
  return q.value(0).toInt();
}

void add_index_channel(string table_name, int n)
{
  string query_text = "INSERT INTO " + table_name + "(idx) VALUES (0)";
  for (int i = 1; i < n; ++i) {
    query_text += ",(" + to_string(i) + ")";
  }
  query_text += ";";
  sql_query(query_text);
}

void add_channel_data(string table_name, string channel_name, double * data, int n)
{
  string datapoint = stringify(data[0]);
  
  string query_text = "INSERT INTO " + table_name + "(" + channel_name + ") VALUES (" + datapoint + ")";
  for (int i = 1; i < n; ++i) {
    datapoint = stringify(data[i]);
    query_text += ",(" + datapoint + ")";
  }
  query_text += ";";
  sql_query(query_text);
}

string row_values_string(map<string, Channel> channels, vector<string> channel_ids, int i) {
  string datapoint = to_string(i);
  string result = datapoint;
  
  for (string cid : channel_ids) {
    datapoint = stringify(channels[cid][i]);
    result += ", " + datapoint;
  }
  return result;
}

void add_channel_data(string table_name, map<string, Channel> channels, vector<string> channel_ids)
{
  int n = channels[channel_ids[0]].length();
  string query_text = "INSERT INTO " + table_name + " VALUES (" + row_values_string(channels, channel_ids, 0) + ")";
  
  for (int i = 1; i < n; ++i)
  {
    query_text += ",(" + row_values_string(channels, channel_ids, i) + ")";
  }
  query_text += ";";
  sql_query(query_text);
}

void get_channel_data(string table_name, string channel_name, Channel * channel)
{
  QSqlQuery q(k_DB);
  q.prepare(QString("SELECT %1 FROM %2").arg(channel_name.c_str(),
                                             table_name.c_str()));
  sql_query(q);
  
  if (q.isActive()) {
    q.first();
    channel->push(q.value(channel_name.c_str()).toDouble());

    while (q.next())
      channel->push(q.value(channel_name.c_str()).toDouble());
  }
}

Channel * Table::get(string id)
{
  bool channel_in_memory =
    m_channels_in_memory.find(id) != m_channels_in_memory.end();
  
  if (channel_in_memory)
    return m_channels_in_memory[id];
    
  else {
    string table_id = id.substr(0, id.find("::"));
    string channel_id = id.substr(id.find("::")+2, id.size());
    condition_for_sql(channel_id);
    
    Channel * data_channel = new Channel;
    get_channel_data(table_id, channel_id, data_channel);
    
    string time_channel_id = m_time_channel_name;
    condition_for_sql(time_channel_id);
    
    Channel * time_channel = new Channel;
    get_channel_data(table_id, time_channel_id, time_channel);
    
    time_channel->set_unit_string(m_time_channel_unit);
    data_channel->update_time_channel_ptr(time_channel);
    
    m_channels_in_memory[id] = data_channel;
    m_channels_in_memory[table_id+"::"+m_time_channel_name] = time_channel;
    return data_channel;
  }
}

bool Table::exists(string id) {
  bool channel_in_memory =
    m_channels_in_memory.find(id) != m_channels_in_memory.end();
  
  if (channel_in_memory)
    return true;
  
  else {
    string table_id = id.substr(0, id.find("::"));
    string channel_id = id.substr(id.find("::")+2, id.size());
    condition_for_sql(channel_id);
    
    return does_table_exist(table_id);
  }
}


}  // namespace data
}  // namespace insight
