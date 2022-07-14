// Copyright (c) 2020 Kieran Downie. All rights reserved.
//
// This file is part of insight.
//
// insight is free software : you can redistribute it and /
// or modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// insight is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with insight.  If not, see <https://www.gnu.org/licenses/>.
//
//#include "scattergroup.h"
#include "crosshair.h"
#include "defaults.h"

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>

namespace insight {
namespace graphic {

void Crosshair::set_xy(double x, double y, double * xlim, double * ylim)
{
  double horzbar_ydata[2]{y, y};
  double vertbar_xdata[2]{x, x};
    
  m_horzbar.setSamples(xlim, horzbar_ydata, 2);
  m_vertbar.setSamples(vertbar_xdata, ylim, 2);
  m_centrepoint.setSamples(&x, &y, 1);
  
  m_xy[0] = x;
  m_xy[1] = y;
}


void CrosshairGroup::set_xy(double x, double y, int layer, double * xlim, double * ylim)
{
  set_label_values(layer, x, y);
  m_crosshairs[layer]->set_xy(x, y, xlim, ylim);
  
  m_xy[layer][0] = x;
  m_xy[layer][1] = y;
  
  m_xbounds[0] = xlim[0];
  m_xbounds[1] = xlim[1];
  m_ybounds[0] = ylim[0];
  m_ybounds[1] = ylim[1];
}

void CrosshairGroup::set_xy(double x, double y, int layer)
{
  set_label_values(layer, x, y);
  m_crosshairs[layer]->set_xy(x, y, m_xbounds, m_ybounds);
  
  m_xy[layer][0] = x;
  m_xy[layer][1] = y;
}

void CrosshairGroup::update_plot_limits(double * xlimits, double * ylimits)
{
  m_xbounds[0] = xlimits[0];
  m_xbounds[1] = xlimits[1];
  m_ybounds[0] = ylimits[0];
  m_ybounds[1] = ylimits[1];
  
  for (Crosshair * c: m_crosshairs)
    c->set_xy(c->x(), c->y(), xlimits, ylimits);
}

void CrosshairGroup::set_label_values(int layer, double xvalue, double yvalue)
{
  m_xy[layer][0] = xvalue;
  m_xy[layer][1] = yvalue;
  
  render_labels();
}

void CrosshairGroup::render_labels()
{
//  char * xlabel_text = new char[m_xchannel_name.size() + 3 + m_nlayers * (m_xchannel_unit_string.size() + 63)];
//  char * ylabel_text = new char[m_ychannel_name.size() + 3 + m_nlayers * (m_ychannel_unit_string.size() + 63)];
//  char * xlabel_text = new char[1000];
//  char * ylabel_text = new char[1000];
  string xlabel_text = m_xchannel_name + ": ";
  string ylabel_text = m_ychannel_name + ": ";
  
  int xlabel_cursor = 0;
  int ylabel_cursor = 0;
  
//  sprintf(xlabel_text, "%s: ", m_xchannel_name.c_str());
//  sprintf(ylabel_text, "%s: ", m_ychannel_name.c_str());
  
  xlabel_cursor += m_xchannel_name.length() + 2;
  ylabel_cursor += m_ychannel_name.length() + 2;
  
  for (int i = 0; i < m_nlayers; ++i)
  {
    char * xlabel_chars = new char[1000];
    char * ylabel_chars = new char[1000];
    
    double xvalue = m_xy[i][0];
    double yvalue = m_xy[i][1];
    
    vector<int> color = m_colors[i];
    
//    sprintf(&xlabel_text[xlabel_cursor],
//        "<span style=\"color : rgb(%03d,%03d,%03d);\">L%02d:%*.2f[%s];</span>",
//            color[0], color[1], color[2], i + 1, 7, xvalue, m_xchannel_unit_string.c_str()
//    );
//    sprintf(&ylabel_text[ylabel_cursor],
//        "<span style=\"color : rgb(%03d,%03d,%03d);\">L%02d:%*.2f[%s];</span>",
//            color[0], color[1], color[2], i + 1, 7, yvalue, m_ychannel_unit_string.c_str()
//    );
    
    sprintf(xlabel_chars,
        "<span style=\"color : rgb(%03d,%03d,%03d);\">L%02d:%*.2f[%s];</span> ",
            color[0], color[1], color[2], i + 1, 7, xvalue, m_xchannel_unit_string.c_str()
    );
    sprintf(ylabel_chars,
        "<span style=\"color : rgb(%03d,%03d,%03d);\">L%02d:%*.2f[%s];</span> ",
            color[0], color[1], color[2], i + 1, 7, yvalue, m_ychannel_unit_string.c_str()
    );
    xlabel_text += xlabel_chars;
    ylabel_text += ylabel_chars;
    
    xlabel_cursor += 63 + m_xchannel_unit_string.length();
    ylabel_cursor += 63 + m_ychannel_unit_string.length();
    
    delete[] xlabel_chars;
    delete[] ylabel_chars;
  }
  
  m_xlabel.setText(QString(xlabel_text.c_str()));
  m_ylabel.setText(QString(ylabel_text.c_str()));

//  delete[] xlabel_text;
//  delete[] ylabel_text;
  
  // update label positions
  m_xlabel.setGeometry(
    p_parent->width()  - (m_xlabel.sizeHint().width()  + 5),
    p_parent->height() - (m_xlabel.sizeHint().height() + .5),
    m_xlabel.sizeHint().width(),
    m_xlabel.sizeHint().height()
  );
    
  m_ylabel.setGeometry(
    5,
    5,
    m_ylabel.sizeHint().width(),
    m_ylabel.sizeHint().height()
  );
}

//void CrosshairGroup::render_labels()
//{
////  char * xlabel_text = new char[m_xchannel_name.size() + 3 + m_nlayers * (m_xchannel_unit_string.size() + 63)];
////  char * ylabel_text = new char[m_ychannel_name.size() + 3 + m_nlayers * (m_ychannel_unit_string.size() + 63)];
////  char * xlabel_text = new char[1000];
////  char * ylabel_text = new char[1000];
//  string xlabel_text = m_xchannel_name + ": ";
//  string ylabel_text = m_ychannel_name + ": ";

//  int xlabel_cursor = 0;
//  int ylabel_cursor = 0;

////  sprintf(xlabel_text, "%s: ", m_xchannel_name.c_str());
////  sprintf(ylabel_text, "%s: ", m_ychannel_name.c_str());

//  xlabel_cursor += m_xchannel_name.length() + 2;
//  ylabel_cursor += m_ychannel_name.length() + 2;

//  for (int i = 0; i < m_nlayers; ++i)
//  {
//    char * xlabel_chars = new char[1000];
//    char * ylabel_chars = new char[1000];

//    double xvalue = m_xy[i][0];
//    double yvalue = m_xy[i][1];

//    vector<int> color = m_colors[i];

////    sprintf(&xlabel_text[xlabel_cursor],
////        "<span style=\"color : rgb(%03d,%03d,%03d);\">L%02d:%*.2f[%s];</span>",
////            color[0], color[1], color[2], i + 1, 7, xvalue, m_xchannel_unit_string.c_str()
////    );
////    sprintf(&ylabel_text[ylabel_cursor],
////        "<span style=\"color : rgb(%03d,%03d,%03d);\">L%02d:%*.2f[%s];</span>",
////            color[0], color[1], color[2], i + 1, 7, yvalue, m_ychannel_unit_string.c_str()
////    );

//    sprintf(xlabel_chars,
//        "<span style=\"color : rgb(%03d,%03d,%03d);\">L%02d:%*.2f[%s];</span> ",
//            color[0], color[1], color[2], i + 1, 7, xvalue, m_xchannel_unit_string.c_str()
//    );
//    sprintf(ylabel_chars,
//        "<span style=\"color : rgb(%03d,%03d,%03d);\">L%02d:%*.2f[%s];</span> ",
//            color[0], color[1], color[2], i + 1, 7, yvalue, m_ychannel_unit_string.c_str()
//    );
//    xlabel_text += xlabel_chars;
//    ylabel_text += ylabel_chars;

//    xlabel_cursor += 63 + m_xchannel_unit_string.length();
//    ylabel_cursor += 63 + m_ychannel_unit_string.length();

//    delete[] xlabel_chars;
//    delete[] ylabel_chars;
//  }

//  m_xlabel.setText(QString(xlabel_text.c_str()));
//  m_ylabel.setText(QString(ylabel_text.c_str()));

////  delete[] xlabel_text;
////  delete[] ylabel_text;

//  // update label positions
//  m_xlabel.setGeometry(
//    p_parent->width()  - (m_xlabel.sizeHint().width()  + 5),
//    p_parent->height() - (m_xlabel.sizeHint().height() + .5),
//    m_xlabel.sizeHint().width(),
//    m_xlabel.sizeHint().height()
//  );

//  m_ylabel.setGeometry(
//    5,
//    5,
//    m_ylabel.sizeHint().width(),
//    m_ylabel.sizeHint().height()
//  );
//}


}  // namespace graphic
}  // namespace insight

