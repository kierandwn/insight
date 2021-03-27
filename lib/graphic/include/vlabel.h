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
#ifndef VLABEL_H_
#define VLABEL_H_
#pragma once

#include <QWidget>
#include <QLabel>
#include <QSize>

#include <QStaticText>
#include <QPaintEvent>
#include <QStylePainter>

namespace insight {
namespace graphic {

class VLabel : public QLabel
{
  Q_OBJECT
  
 public:
  VLabel(QWidget *parent);
  VLabel(const QString& text, QWidget * parent);
  
 protected:
  void paintEvent(QPaintEvent *);
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
};

}  // namespace graphic
}  // namespace insight
#endif // VLABEL_H_
