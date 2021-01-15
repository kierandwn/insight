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
#ifndef INSIGHT_GRAPHIC_BASE_H
#define INSIGHT_GRAPHIC_BASE_H

#include <QWidget>
#include <QDesignerCustomWidgetInterface>

#include "lib/rapidjson/include/rapidjson/document.h"
#include "lib/data/include/table.h"

using namespace std;


class InsightBaseGraphic //, public virtual QDesignerCustomWidgetInterface
{
protected:
    table * data_;

    bool initialized_ = false;

public:
    InsightBaseGraphic() {}
    InsightBaseGraphic(table * data)
        : data_(data)
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

    virtual void apply_config(rapidjson::Value&);
    virtual void reset () {}
    virtual void update() {}

    void update_data_ref(table * data) { data_ = data; }
};

#endif // INSIGHT_GRAPHIC_BASE_H
