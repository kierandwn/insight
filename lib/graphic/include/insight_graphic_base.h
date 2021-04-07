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
#ifndef INSIGHT_GRAPHIC_BASE_H
#define INSIGHT_GRAPHIC_BASE_H
#pragma once

#include <qwt_plot.h>

//#include <QWidget>
//#include <QtUiPlugin/QDesignerCustomWidgetInterface>

#include <vector>

#include "table.h"
#include "lib/json/single_include/nlohmann/json.hpp"

namespace insight {
namespace graphic {

using namespace std;


class Base : public QwtPlot { //, public virtual QDesignerCustomWidgetInterface {
 protected:
  data::Table * m_data;
   
  string m_group_name = "";

 public:
//  Base() {}
  Base(data::Table * data) : m_data(data) {}

//    bool isContainer() const override { return false; }
//    bool isInitialized() const override { return initialized_; }

//    QIcon icon() const override { return QIcon(); }
//    QString group() const override { return QStringLiteral("Inisght Graphics Classes"); }

//    void initialize(QDesignerFormEditorInterface *) override
//    {
//        if (initialized_)
//            return;

//        initialized_ = true;
//    }

//    QString toolTip() const override { return QString(); }
//    QString whatsThis() const override { return QString(); }
  
  virtual void update_after_data_load () {}
  virtual void update_after_resize() {}
  
  virtual void update_cursor_position(double) {}
  virtual void update_view_limits(double, double) {}
  
  virtual void init()   = 0;
  virtual void apply_config(nlohmann::json *) = 0;
  virtual void reset () = 0;
    
  data::Table * get_data_table_ref() { return m_data; }
  
  string group() { return m_group_name; }
  void update_data_ref(data::Table * data) { m_data = data; }
};

}  // namespace graphic
}  // namespace insight
#endif // INSIGHT_GRAPHIC_BASE_H
