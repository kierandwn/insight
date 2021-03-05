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

#include <vector>

#include <QWidget>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

//#include "mainwindow.h"
#include "table.h"
#include "lib/json/single_include/nlohmann/json.hpp"

using namespace std;

namespace insight {
namespace graphic {


class Base { //, public virtual QDesignerCustomWidgetInterface {
 protected:
//  layout::ApplicationMainWindow * m_app;
  data::Table * m_data;
    
  string m_group_name = "a";

  bool initialized_ = false;

 public:
  Base() {}
//  Base(ApplicationMainWindow * app, data::Table * data)
//      : m_data(data),
//        m_app(app)
//  {}
    Base(data::Table * data)
        : m_data(data)
    {}

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

  virtual void apply_config(nlohmann::json *) = 0;
  virtual void update_cursor_position(double xval) = 0;
  
  virtual void reset () = 0;
  virtual void update() = 0;
  virtual void init()   = 0;
    
  string group() { return m_group_name; }
    
  void update_data_ref(data::Table * data) { m_data = data; }
    
//  void update_all_group_cursor_positions(double xval) {
//    Base * graphic_ptr = this;
//    while (this != next()) {
//      graphic_ptr->update_cursor_position(xval);
//      graphic_ptr = next();
//    }
//  }
};

}  // namespace graphic
}  // namespace insight
#endif // INSIGHT_GRAPHIC_BASE_H
