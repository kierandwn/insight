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

QSqlDatabase k_DATABASE;
const QString k_DATABASE_DRIVER("QSQLITE");

int k_FILE_TABLE_LEN = 0;
string k_DATABASE_FILEPATH;

string stringify(double d)
{
  string s;
  if (isnan(d) || isinf(d))
    s = "NULL";
  else
    s = to_string(d);
  
  return s;
}

QSqlQuery sql_query(QSqlQuery& q)
{
  if(!q.exec())
  {
    qWarning() << "ERROR: " << q.lastError().text();
    qWarning() << "\t query: " << q.executedQuery();
  }
  k_DATABASE.commit();
  return q;
}

QSqlQuery sql_query(string s)
{
  QSqlQuery q(k_DATABASE);
  q.prepare(s.c_str());
  
  sql_query(q);
  return q;
}

void condition_for_sql(string& s)
{
  replace(s.begin(), s.end(), '[', '_');
  replace(s.begin(), s.end(), ']', '_');
  replace(s.begin(), s.end(), '-', '_');
  replace(s.begin(), s.end(), '/', '_');
  replace(s.begin(), s.end(), '\\', '_');
  replace(s.begin(), s.end(), '.', '_');
  return;
}

void set_db_filepath(string filepath)
{
    k_DATABASE_FILEPATH = filepath;
}

void open_db()
{
  if ( QSqlDatabase::isDriverAvailable(k_DATABASE_DRIVER) )
  {
    QSqlDatabase k_DATABASE = QSqlDatabase::addDatabase(k_DATABASE_DRIVER);
    k_DATABASE.setDatabaseName(k_DATABASE_FILEPATH.c_str());
  
    // Check that database is open
    if(!k_DATABASE.open())
      qWarning() << "ERROR: " << k_DATABASE.lastError();
  }
  return;
}

void close_db()
{
  k_DATABASE.close();
}

void shutdown_db()
{
  delete_layer_tables();
  delete_maths_tables();
  
  close_db();
}

void establish_db(string filepath)
{
  if (QFileInfo(filepath.c_str()).exists())
  {
    set_db_filepath(filepath);
    open_db();
    
    delete_layer_tables(); // remove tracking from previous session if present
    delete_maths_tables();
    
  } else {
    set_db_filepath(filepath);
    open_db();
    
    add_files_table();
    add_units_table();
  }
  add_layer_table();
  add_maths_table();
  k_FILE_TABLE_LEN = get_row_count("files");
  return;
}

void add_files_table()
{
  sql_query("CREATE TABLE files (table_id INT, string_id TEXT, filename TEXT, filepath TEXT PRIMARY KEY, independent_variable TEXT)");
  return;
}

void add_units_table()
{
  sql_query("CREATE TABLE unit_table (table_sid TEXT, channel_id TEXT, unit_string TEXT)");
  return;
}

void add_maths_table()
{
  sql_query("CREATE TABLE math_tables (layer INT, table_id INT PRIMARY KEY, string_id TEXT, independent_variable TEXT)");
  return;
}

void add_layer_table()
{
  delete_layer_tables();
  sql_query("CREATE TABLE layers_table (layer INT, table_id INT, filename TEXT, string_id TEXT)");
  return;
}

void delete_layer_tables()
{
  sql_query("DROP TABLE layers_table;");
}

void delete_maths_tables()
{
  QSqlQuery q = sql_query("SELECT table_id FROM math_tables");
  vector<int> math_table_ids;
  
  while (q.next())
    math_table_ids.push_back(q.value("table_id").toInt());
  
  for (auto const& mid: math_table_ids)
    sql_query("DROP TABLE math_"+to_string(mid)+";");
  
  sql_query("DROP TABLE math_tables;");
}

int get_no_of_layers()
{
  QSqlQuery q = sql_query("SELECT MAX(layer) FROM layers_table;");
  q.first();
  
  return q.value(0).toInt() + 1;
}

bool is_in_layer(string string_id, int layer)
{
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT count(*) FROM layers_table WHERE layer=(:layer) AND string_id=(:sid);");
  q.bindValue(":layer", to_string(layer).c_str());
  q.bindValue(":sid", string_id.c_str());
  sql_query(q);
  
  q.first();
  return q.value(0).toInt() >= 1;
}

void add_to_layer(string filepath, int i_layer)
{
  condition_for_sql(filepath);
  
  QSqlQuery q = get_file_record(filepath);
  if (q.first()) {
    int tid = q.value("table_id").toInt();
    QString filename = q.value("filename").toString();
    QString sid = q.value("string_id").toString();
    
    if (!is_in_layer(sid.toStdString(), i_layer))
    {
      QSqlQuery q(k_DATABASE);
      q.prepare("INSERT INTO layers_table VALUES((:layer), (:tid), (:fn), (:sid))");
      q.bindValue(":layer", i_layer);
      q.bindValue(":tid", tid);
      q.bindValue(":fn", filename);
      q.bindValue(":sid", sid);
      sql_query(q);
    }
  }
}

void add_file(string filename, string filepath, string hreadable_id, vector<string> channel_names, string independent_var_channel_id)
{
  condition_for_sql(filepath);
  
  if (!get_table_id(filepath))
  {
    k_FILE_TABLE_LEN++;
    
    QSqlQuery q(k_DATABASE);
    q.prepare("INSERT INTO files VALUES(?, ?, ?, ?, ?)");
    q.bindValue(0, to_string(k_FILE_TABLE_LEN).c_str());
    q.bindValue(1, hreadable_id.c_str());
    q.bindValue(2, filename.c_str());
    q.bindValue(3, filepath.c_str());
    q.bindValue(4, independent_var_channel_id.c_str());
    sql_query(q);
    
    add_table(k_FILE_TABLE_LEN, hreadable_id, channel_names);
  }
}

void add_table(int table_id, string table_sid, vector<string> channel_names)
{
  string query_string = "CREATE TABLE data_" + to_string(table_id) + " (idx INT PRIMARY KEY";
  string channel_name;
  for (size_t i = 0; i < channel_names.size(); ++i)
  {
    channel_name = channel_names[i];
    
    condition_for_sql(channel_name);
    query_string += ", "+channel_name+" DOUBLE";
    
    init_unit_row(table_sid, channel_name);
  }
  query_string += ");";
  sql_query(query_string);
}

void init_unit_row(string table_sid, string channel_name)
{
  QSqlQuery q;
  q.prepare("SELECT count(*) FROM unit_table WHERE table_sid=(:sid) AND channel_id=(:cid);");
  q.bindValue(":sid", table_sid.c_str());
  q.bindValue(":cid", channel_name.c_str());
  sql_query(q);
  q.first();
  
  bool unit_row_exists = q.value(0).toInt() == 1;
  
  if (!unit_row_exists)
  {
    QSqlQuery qr;
    qr.prepare("INSERT INTO unit_table VALUES((:sid), (:cid), (:unit))");
    qr.bindValue(":sid", table_sid.c_str());
    qr.bindValue(":cid", channel_name.c_str());
    qr.bindValue(":unit", "-");
    sql_query(qr);
  }
}

QSqlQuery get_file_record(string filepath)
{
  condition_for_sql(filepath);
  
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT * FROM files WHERE files.filepath=? LIMIT 1;");
  q.bindValue(0, filepath.c_str());
  sql_query(q);
  return q;
}

int get_table_id(string filepath)
{
  QSqlQuery q = get_file_record(filepath);
  if (q.first())
    return q.value("table_id").toInt();
  
  return 0;
}

bool has_been_imported_before(string filepath)
{
  condition_for_sql(filepath);
  
  QSqlQuery q = sql_query("SELECT count(*) FROM files WHERE filepath='" + filepath + "';");
  q.first();
  
  return q.value(0).toInt() > 0;
}

bool does_table_exist(string table_name)
{
  return get_table_count(table_name) > 0;
}

int get_table_count(string table_name)
{
  QSqlQuery q = sql_query("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='" + table_name + "';");
  q.first();
  
  return q.value(0).toInt();
}

int get_row_count(string table_name)
{
  QSqlQuery q = sql_query("SELECT COUNT(*) FROM " + table_name);
  q.first();
  
  return q.value(0).toInt();
}

int file_count_in_layer(int layer)
{
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT COUNT(*) FROM layers_table WHERE layer=(:layer)");
  q.bindValue(":layer", layer);
  sql_query(q);
  q.first();
  
  return q.value(0).toInt();
}

bool hid_in_layer(string hid, int layer)
{
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT count(*) FROM layers_table WHERE string_id=(:hid) AND layer=(:layer)");
  q.bindValue(":hid", hid.c_str());
  q.bindValue(":layer", layer);
  sql_query(q);
  q.first();
  
  return q.value(0).toInt() > 0;
}

bool mid_in_layer(string mid, int layer)
{
  QSqlQuery q(k_DATABASE);
  q.prepare(QString("SELECT count(*) FROM math_tables WHERE string_id=(:mid) AND layer=(:layer)"));
  q.bindValue(":mid", mid.c_str());
  q.bindValue(":layer", layer);
  sql_query(q);
  q.first();
  
  return q.value(0).toInt() > 0;
}

bool is_channel_in_table(string channel_id, string table_id)
{
  QSqlQuery q = sql_query("PRAGMA table_info("+table_id+");");
  while (q.next())
  {
    if (q.value(1).toString().toStdString() == channel_id)
    {
      q.finish();
      return true;
    }
  }
  return false;
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
  for (int i = 1; i < n; ++i)
  {
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

void add_channel_data(string filepath, map<string, Channel> channels, vector<string> channel_ids)
{
  int tid = get_table_id(filepath);
  int n = channels[channel_ids[0]].length();
  
  string query_text = "INSERT INTO data_" + to_string(tid) + " VALUES (" + row_values_string(channels, channel_ids, 0) + ")";
  
  for (int i = 1; i < n; ++i)
  {
    query_text += ",(" + row_values_string(channels, channel_ids, i) + ")";
  }
  query_text += ";";
  sql_query(query_text);
}

string get_time_channel_id(int tid) {
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT independent_variable FROM files WHERE table_id=? LIMIT 1");
  q.bindValue(0, to_string(tid).c_str());
  sql_query(q);
  q.first();
  
  return q.value("independent_variable").toString().toStdString();
}

string get_math_time_channel_id(int mid) {
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT independent_variable FROM math_tables WHERE table_id=? LIMIT 1");
  q.bindValue(0, to_string(mid).c_str());
  sql_query(q);
  q.first();
  
  return q.value("independent_variable").toString().toStdString();
}

// obtain integer table id from human readable id
int get_tid_from_hid(string hid, int layer)
{
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT table_id FROM layers_table WHERE string_id=(:sid) AND layer=(:layer) LIMIT 1");
  q.bindValue(":sid", hid.c_str());
  q.bindValue(":layer", layer);
  sql_query(q);
  q.first();
  
  return q.value("table_id").toInt();
}

// obtain integer (math) table id from channel id
int get_mid_from_hid(string hid, int layer)
{
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT table_id FROM math_tables WHERE string_id=(:hid) AND layer=(:layer) LIMIT 1");
  q.bindValue(":hid", hid.c_str());
  q.bindValue(":layer", layer);
  sql_query(q);
  q.first();
  
  return q.value("table_id").toInt();
}

string get_channel_data(string table_hid, string channel_name, int layer, Channel * channel)
{
  int table_id = get_tid_from_hid(table_hid, layer);
  
  QSqlQuery q(k_DATABASE);
  q.prepare(QString("SELECT %1 FROM data_%2").arg(channel_name.c_str(),
                                             to_string(table_id).c_str()));
  sql_query(q);
  
  if (q.isActive())
  {
    while (q.next())
      channel->push(q.value(channel_name.c_str()).toDouble());
  }
  return get_time_channel_id(table_id);
}

string get_math_channel_data(string math_table_sid, string math_channel_name, int layer, Channel * channel)
{
  int math_table_id = get_mid_from_hid(math_table_sid, layer);
  
  QSqlQuery q(k_DATABASE);
  q.prepare(QString("SELECT %1 FROM math_%2").arg(math_channel_name.c_str(),
                                                  to_string(math_table_id).c_str()));
  sql_query(q);
  
  if (q.isActive())
  {
    while (q.next())
      channel->push(q.value(math_channel_name.c_str()).toDouble());
  } else
      q.finish();

  return get_math_time_channel_id(math_table_id);
}

string get_unit_string(string table_sid, string channel_id)
{
  QSqlQuery q(k_DATABASE);
  q.prepare("SELECT unit_string FROM unit_table where table_sid=(:sid) AND channel_id=(:cid) LIMIT 1;");
  q.bindValue(":sid", table_sid.c_str());
  q.bindValue(":cid", channel_id.c_str());
  sql_query(q);
  
  if (q.isActive())
  {
      q.first();
      return q.value("unit_string").toString().toStdString();
  } else
      q.finish();

  return "-";
}

Channel * Table::get(string id, int layer)
{
  map<string, Channel *>& channels_in_layer = m_channels_in_memory[layer];
  
  bool channel_in_memory =
  channels_in_layer.find(id) != channels_in_layer.end();
  
  if (channel_in_memory)
    return channels_in_layer[id];
    
  else {
    string table_id = id.substr(0, id.rfind("::"));
    string channel_id = id.substr(id.rfind("::")+2, id.size());
    condition_for_sql(channel_id);
    
    Channel * data_channel = new Channel;
    Channel * time_channel;
    
    string time_channel_id;
    string time_id;
    
    if (table_id.find("math") == 0)
    {
      string math_table_id;
      if (table_id == "math") math_table_id = channel_id;
      else {
        math_table_id = table_id.substr(table_id.find("::")+2, table_id.size());
      }
      
      time_channel_id = get_math_channel_data(math_table_id, channel_id, layer, data_channel);
      time_id = table_id+"::"+time_channel_id;
      
      bool time_channel_in_memory = channels_in_layer.find(time_id) != channels_in_layer.end();
      
      if (!time_channel_in_memory) {
        time_channel = new Channel;
        get_math_channel_data(math_table_id, time_channel_id, layer, time_channel);
        
        channels_in_layer[time_id] = time_channel;
      }
      else {
        time_channel = channels_in_layer[time_id];
      }
      table_id = "math::" + math_table_id;
    }
    else {
      time_channel_id = get_channel_data(table_id, channel_id, layer, data_channel);
      
      time_id = table_id+"::"+time_channel_id;
      bool time_channel_in_memory = channels_in_layer.find(time_id) != channels_in_layer.end();
      
      if (!time_channel_in_memory) {
        time_channel = new Channel;
        get_channel_data(table_id, time_channel_id, layer, time_channel);
        
        string unit_string = get_unit_string(table_id, time_channel_id);
        time_channel->set_unit_string(m_time_channel_unit);
        
        channels_in_layer[time_id] = time_channel;
      }
      else {
        time_channel = channels_in_layer[time_id];
      }
      
      get_channel_data(table_id, time_channel_id, layer, time_channel);
    }
    // set unit strings
    string unit_string = get_unit_string(table_id, channel_id);
    data_channel->set_unit_string(unit_string);
    
    unit_string = get_unit_string(table_id, time_channel_id);
    time_channel->set_unit_string(unit_string);
    
    data_channel->update_time_channel_ptr(time_channel);
    channels_in_layer[id] = data_channel;
    return data_channel;
  }
}

int Table::add_layer()
{
  ++m_number_of_layers;
  m_channels_in_memory.push_back(map<string, Channel *>());
  return m_number_of_layers;
}

int Table::get_number_of_layers() { return m_number_of_layers; }

bool Table::exists_in_layer(string id, int layer)
{
  map<string, Channel *>& channels_in_layer = m_channels_in_memory[layer];

  size_t n_channels_in_memory = channels_in_layer.size();
  
  bool channel_in_memory = n_channels_in_memory > 0 && \
    channels_in_layer.find(id) != channels_in_layer.end();
  
  if (channel_in_memory)
    return true;
  
  else {
    string table_id = id.substr(0, id.rfind("::"));
    string channel_id = id.substr(id.rfind("::")+2, id.size());
    condition_for_sql(channel_id); // TODO: check channel in table
    
    if (table_id.find("math") == 0)
    {
      string math_table_id;
      if (table_id == "math") math_table_id = channel_id;
      else {
        math_table_id = table_id.substr(table_id.find("::")+2, table_id.size());
      }
      
      int mid = get_mid_from_hid(math_table_id, layer); // TODO: clean logic: mid == 0 if !mid_in_layer
      return mid_in_layer(math_table_id) && is_channel_in_table(channel_id, "math_"+to_string(mid));;
      
    } else {
      int tid = get_tid_from_hid(table_id, layer);
      return hid_in_layer(table_id) && is_channel_in_table(channel_id, "data_"+to_string(tid));
    }
  }
}

void compute_math_channels(int layer, string db_filepath, string source_root_dirpath)
{
  close_db();
  
  string python_entry_filepath = source_root_dirpath + "/math/main.py";
//  string command = "python3 ";
  string command = "/Users/kierandwn/miniforge3/bin/conda run -n apple_tensorflow python3 ";
  string cl_args = " " + db_filepath + " " + to_string(layer);
  command += python_entry_filepath + cl_args + "; exit";
  
  system(command.c_str());
  
  open_db();
}


}  // namespace data
}  // namespace insight
