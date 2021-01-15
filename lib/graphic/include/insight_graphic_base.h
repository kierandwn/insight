#ifndef INSIGHT_GRAPHIC_BASE_H
#define INSIGHT_GRAPHIC_BASE_H

#include <QWidget>
#include <QDesignerCustomWidgetInterface>

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

    virtual void reset () {}
    virtual void update() {}

    void update_data_ref(table * data) { data_ = data; }
};

#endif // INSIGHT_GRAPHIC_BASE_H
