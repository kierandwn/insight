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

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include <string>
#include <algorithm>

namespace insight {
namespace data {

using namespace std;

void condition_for_sql(string& s) {
  replace(s.begin(), s.end(), '[', '_');
  replace(s.begin(), s.end(), ']', '_');
  return;
}

void establish_db(string filepath) {
  const QString DRIVER("QSQLITE");
   
  if ( QSqlDatabase::isDriverAvailable(DRIVER) ) {
    QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
    db.setDatabaseName(filepath.c_str());
    
    if(!db.open())
      qWarning() << "ERROR: " << db.lastError();
  }
  return;
}

void add_table(string filename, vector<string> channel_names, string time_channel_name)
{
  string query_string = "CREATE TABLE " + filename + " ( idx INT PRIMARY KEY";
  
  for ( string channel_name : channel_names ) {
    condition_for_sql(channel_name);
    query_string += ", "+channel_name+" DOUBLE PRECISION";
  }
  query_string += " )";
  
  QSqlQuery q(query_string.c_str());
  if(!q.isActive())
      qWarning() << "ERROR: " << q.lastError().text();
}

void add_to_table(string table_name, string data_row, int idx) {
//  replace(data_row.begin(), data_row.end(), ",", " ");
  
  string query_text ="INSERT INTO " + table_name + " VALUES(" + to_string(idx) + " " + data_row + ")";
  
  QSqlQuery q(query_text.c_str());
//  cout << query_text.c_str() << endl;
  
  if(!q.isActive())
    qWarning() << "ERROR: " << q.lastError().text();
}



}  // namespace data
}  // namespace insight
