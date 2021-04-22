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
#include "vlabel.h"

#include <QWidget>
#include <QLabel>
#include <QSize>

#include <QStaticText>
#include <QTextDocument>
#include <QPaintEvent>
#include <QStylePainter>

namespace insight {
namespace graphic {

VLabel::VLabel(QWidget * parent) : QLabel(parent) {}
VLabel::VLabel(const QString& text, QWidget * parent) : QLabel(text, parent) {}
  
void VLabel::paintEvent(QPaintEvent *) {
  QStylePainter painter(this);
//  QTextDocument tdoc;
//
//  tdoc.setHtml(text().toStdString().c_str());
  
  //  painter.setPen(Qt::black);
  painter.setBrush(Qt::Dense1Pattern);
  
  painter.translate(sizeHint().width(), 0.);
  painter.rotate(90);
  
  painter.drawStaticText(0, 0, QStaticText(text()));
//  tdoc.drawContents(&painter);
}
  
QSize VLabel::minimumSizeHint() const {
  QSize s = QLabel::minimumSizeHint();
  return QSize(s.height(), s.width());
}

QSize VLabel::sizeHint() const {
  QSize s = QLabel::sizeHint();
  return QSize(s.height(), s.width());
}

}  // namespace graphic
}  // namespace insight
