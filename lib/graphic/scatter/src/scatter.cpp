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
#include "scatter.h"

#include <string>
#include <cmath>

#include <QMouseEvent>
#include <QWheelEvent>

#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "defaults.h"
#include "insight_graphic_base.h"
#include "grid.h"

namespace insight {
namespace graphic {


ScatterGroup::ScatterGroup(QwtPlot * parent)
    : p_parent(parent),
      m_label(parent),
      m_metrics(parent)
{}

void ScatterGroup::add_xypair(string xchannel_id, string ychannel_id) {
    m_xchannel_name = xchannel_id;
    m_ychannel_name = ychannel_id;
    
}

void ScatterGroup::set_data_from_table(data::Table * table,
                                       double x_lbound=-10e12, double x_hbound=10e12)
{
//    size_t n_scatters = m_nscatter_pairs;
    double xmax, xmin, xmean;
    double ymax, ymin, ymean;
    
    for (size_t i = 0; i < 1; ++i) {
        data::Channel * xchannel = table->get(m_xchannel_name);
        data::Channel * ychannel = table->get(m_ychannel_name);
        size_t n = xchannel->length();
        
        if (i == 0) {
            xmin = xchannel->min();
            xmax = xchannel->max();
            xmean = 0.5 * (xmin + xmax);

            ymin = ychannel->min();
            ymax = ychannel->max();
            ymean = 0.5 * (ymin + ymax);
        }
        
//        x_lbound = max({x_lbound, xchannel->min()});
//        x_hbound = min({x_hbound, xchannel->max()});
        
//        bool below_lbound = true; bool below_hbound = true;
//        int i_lbound = n - 1; int i_hbound = n - 1;
//
//        for (size_t i = 0; i < n; ++i) {
//            if (xdata[i] < x_lbound) {
//                continue;
//            } else if (xdata[i] > x_hbound) {
//                if (below_hbound) {
//                    i_hbound = i - 1;
//                    below_hbound = false;
//                }
//                continue;
//            } else {
//                if (below_lbound) {
//                    i_lbound = i;
//                    below_lbound = false;
//                }
//            }
//        }
//        int n_to_plot = i_hbound - i_lbound;
//        if (n_to_plot < 2) { n_to_plot = 2; i_hbound = i_lbound + 1; }
        
//        m_curves[i]->setSamples(&xdata[i_lbound], &(channel->operator[](i_lbound)), n_to_plot);
        setSamples(xchannel->get_data_ptr(), ychannel->get_data_ptr(), n);
    }
    
    m_xlim[0] = xmin; m_xlim[1] = xmax;
    m_ylim[0] = ymin; m_ylim[1] = ymax;
//
//    set_metric_values(ymin, ymax, ymean);
}

void ScatterGroup::init_scatters() {
  // draw curve on graphic
  vector<int> color;
  for (size_t j = 0; j < 1; ++j) {
    color = kDefaultColorOrder[j];
      
    m_symbol.setStyle(QwtSymbol::Cross);
    m_symbol.setSize(QSize(6, 6));
    m_symbol.setColor(QColor(color[0], color[1], color[2]));
      
    setStyle(NoCurve);
    setSymbol(&m_symbol);
  }
  attach(p_parent);
}

//void ScatterGroup::init_label(data::Table * table) {
//    int label_xcoord = 5;
//    int label_ycoord = (1. - m_normalised_yoffset) * p_parent->height();
//
//    int metrics_xcoord = p_parent->width() - (300 + 5);
//    int metrics_ycoord = ((1. - m_normalised_yoffset) * p_parent->height()) - 16;
//
//    m_label.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
//    m_label.setGeometry(label_xcoord, label_ycoord, 300, 30);
//
//    m_metrics.setStyleSheet("QLabel { font : 10pt 'Courier'; color : rgb(50, 50, 50); }");
//    m_metrics.setGeometry(metrics_xcoord, metrics_ycoord, 300, 30);
//    m_metrics.setAlignment(Qt::AlignRight);
//
//    set_label_values_at(0., table);
//}

//void ScatterGroup::set_label_values_at(double xvalue, data::Table * table) {
//    int channel_names_total_length = 0;
//    size_t n_channels = m_channel_names.size();
//    for (size_t i = 0; i < n_channels; ++i) {
//        channel_names_total_length += m_channel_names[i].length();
//    }
//    char * label_text = new char[channel_names_total_length+(n_channels*67)+1];
//
//    int string_cursor = 0;
//    for (size_t i = 0; i < n_channels; ++i) {
//        string channel_name = m_channel_names[i];
//
//        double value;
//        if (table->exists(channel_name)) {
//          value = table->get(channel_name)->value_at(xvalue);
//        } else {
//          value = 0.;
//        }
//
//        sprintf(&label_text[string_cursor],
//            "%s: <span style=\"color : rgb(%03d, %03d, %03d);\">%*.2f[-];</span><br/>",
//                channel_name.c_str(),
//                kDefaultColorOrder[i][0],
//                kDefaultColorOrder[i][1],
//                kDefaultColorOrder[i][2],
//                7, value
//        );
//        string_cursor += m_channel_names[i].length()+67;
//    }
//
//    m_label.setText(QString(label_text));
//    delete[] label_text;
//}

//void ScatterGroup::set_metric_values(double min, double max, double mean) {
//  char metric_text[38];
//
//  sprintf(metric_text,
//      "%7.2f \xE2\x88\x88 [%7.2f, %7.2f[-]];",
//        mean, min, max
//  );
//  m_metrics.setText(QString::fromUtf8(metric_text));
//}

ScatterDisplay::ScatterDisplay(data::Table * data, layout::Layout * layout)
    : LinkedPlot(data, layout),
      m_data(data),
      m_xlabel(this),
      m_ylabel(this)
{
  p_ui->setupUi(this);

  setAutoFillBackground( true );
  QPalette p = palette();

  p.setColor(QPalette::Window, QColor(255, 255, 255, 255));
  setPalette(p);

  enableAxis(xBottom, false);
  enableAxis(yLeft, false);
}

void ScatterDisplay::init_labels() {
    int label_width = 300;
    int label_height = 15;

    m_xlabel.setStyleSheet(
        "QLabel { color : rgb(50, 50, 50); font : 10pt 'Courier'; }"
    );
    m_ylabel.setStyleSheet(
        "QLabel { color : rgb(50, 50, 50); font : 10pt 'Courier'; }"
    );
    
    m_xlabel.setAlignment(Qt::AlignRight);
    m_ylabel.setAlignment(Qt::AlignLeft);
    
    m_xlabel.setGeometry(
        width() - (label_width + 5),
        height() - 1.02 * label_height,
        label_width,
        label_height
    );
    m_ylabel.setGeometry(
        5,
        5,
        label_width,
        label_height
    );
    set_label_values_at(0.);
}

void ScatterDisplay::set_label_values_at(double tvalue)
{
  // determine channel names
  string xchannel_name, ychannel_name;
  int xchannel_names_total_length = 0;
  int ychannel_names_total_length = 0;
  
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    xchannel_name = m_scatter_pairs[i]->get_xchannel_name();
    ychannel_name = m_scatter_pairs[i]->get_ychannel_name();
      
    xchannel_names_total_length += xchannel_name.length();
    ychannel_names_total_length += ychannel_name.length();
  }
    
  char * xlabel_text = new char[xchannel_names_total_length+(m_nscatter_pairs*67)+1];
  char * ylabel_text = new char[xchannel_names_total_length+(m_nscatter_pairs*67)+1];
  int xstring_cursor = 0;
  int ystring_cursor = 0;
    
  for (int i = 0; i < m_nscatter_pairs; ++i) {
      string xchannel_name = m_scatter_pairs[i]->get_xchannel_name();
      vector<int> color = kDefaultColorOrder[i];
      
      double xvalue, yvalue;
      if (m_data->exists(xchannel_name) && m_data->exists(ychannel_name)) {
        xvalue = m_data->get(xchannel_name)->value_at(tvalue);
        yvalue = m_data->get(ychannel_name)->value_at(tvalue);
      } else {
        xvalue = 0.;
        yvalue = 0.;
      }
      
      sprintf(&xlabel_text[xstring_cursor],
          "%s: <span style=\"color : rgb(%03d, %03d, %03d);\">%*.2f[-];</span><br/>",
              xchannel_name.c_str(),
              color[0],
              color[1],
              color[2],
              7, xvalue
      );
      sprintf(&ylabel_text[xstring_cursor],
          "%s: <span style=\"color : rgb(%03d, %03d, %03d);\">%*.2f[-];</span><br/>",
              ychannel_name.c_str(),
              color[0],
              color[1],
              color[2],
              7, yvalue
      );
      xstring_cursor += xchannel_name.length()+67;
      ystring_cursor += ychannel_name.length()+67;
  }

  // Rotate y-label by transformed pixmap
//  QPixmap pm = m_ylabel.pixmap(Qt::ReturnByValue);
//  pm.fill(Qt::red);
//  QTransform tf;
//  tf.rotate(-90);
//  m_ylabel.setPixmap(pm.transformed(tf));
    
  m_xlabel.setText(QString(xlabel_text));
  m_ylabel.setText(QString(ylabel_text));

  delete[] xlabel_text;
  delete[] ylabel_text;
}

// Apply configuation parameters held in json_config
void ScatterDisplay::apply_config(nlohmann::json * json_config) {
  int i = 0;

  if (json_config->contains("data")) {
    for (auto& channel_pair : json_config->operator[]("data")["xychannels"]) {
      ScatterGroup * scatter_pair = new ScatterGroup(this);
      
      std::cout << channel_pair << std::endl;
      scatter_pair->add_xypair(channel_pair[0], channel_pair[1]);
      
      m_scatter_pairs.push_back(scatter_pair);
      ++i;
    }
    
    if (json_config->contains("group")) {
      m_group_name = json_config->operator[]("group");
    } else {
      m_group_name = "";
    }
  }
  m_nscatter_pairs = i;
}

void ScatterDisplay::init() {
    init_labels();
    for (size_t i = 0; i < m_scatter_pairs.size(); ++i) {
        m_scatter_pairs[i]->init_scatters();
    }
    
    m_xzero_line.setPen(QColor(0, 0, 0, 100));
    m_xzero_line.attach(this);
    
    m_yzero_line.setPen(QColor(0, 0, 0, 100));
    m_yzero_line.attach(this);
    
    update_cursor_position(0.);
    m_crosshair.attach(this);
}

void ScatterDisplay::update_zero_line_limits() {
    double xlimits[2];
    double ylimits[2];
    
    xlim(xlimits);
    ylim(ylimits);
    
    double xdata_xline[2]{0., 0.};
    double ydata_yline[2]{0., 0.};
    
    m_xzero_line.setSamples(xdata_xline, ylimits, 2);
    m_yzero_line.setSamples(xlimits, ydata_yline, 2);
}

void ScatterDisplay::update_after_data_load()
{
//  int channels_to_plot = get_number_of_waveform_groups();
    
  for (int i = 0; i < m_nscatter_pairs; ++i) {
    m_scatter_pairs[i]->set_data_from_table(m_data);
//    m_scatter_pairs[i]->init_label(m_data);
  }
  update_zero_line_limits();
//  update_cursor_position(xlim()[0]);
  replot();
}

void ScatterDisplay::update_view_limits(double xmin, double xmax) {}
//{
//  int channels_to_plot = get_number_of_waveform_groups();
//
//  for (int i = 0; i < channels_to_plot; ++i) {
//    m_waveform_groups[i]->set_data_from_table(m_data, xmin, xmax);
//  }
//  cursor_in_xrange();
//  update_zero_line_limits();
//  setAxisScale(xBottom, xmin, xmax);
//  replot();
//}

void ScatterDisplay::update_cursor_position(double tvalue) {
    string xchannel_name = m_scatter_pairs[0]->get_xchannel_name();
    string ychannel_name = m_scatter_pairs[0]->get_ychannel_name();
    
    if (m_data->exists(xchannel_name) && m_data->exists(ychannel_name)) {
        double xvalue = m_data->get(xchannel_name)->value_at(tvalue);
        double yvalue = m_data->get(ychannel_name)->value_at(tvalue);
        
        double xlimits[2];
        double ylimits[2];
        
        xlim(xlimits);
        ylim(ylimits);
        
    //    cursor_in_xrange();
        m_crosshair.set_xy(xvalue, yvalue, xlimits, ylimits);
        set_label_values_at(tvalue);
        replot();
    }
}

//void ScatterDisplay::cursor_in_xrange() {
//    double * xrange = xlim();
//    if (m_xpos_cursor > xrange[0] && m_xpos_cursor < xrange[1]) { m_cursor.attach(this); }
//    else { m_cursor.detach(); }
//}

void ScatterDisplay::xlim(double * xbounds) {
    xbounds[0] = 10e12; xbounds[1] = -10e12;
    
    for (int i = 0; i < m_nscatter_pairs; ++i) {
        xbounds[0] = min({m_scatter_pairs[i]->xlim()[0], xbounds[0]});
        xbounds[1] = max({m_scatter_pairs[i]->xlim()[1], xbounds[1]});
    }
}

void ScatterDisplay::ylim(double * ybounds) {
    ybounds[0] = 10e12; ybounds[1] = -10e12;
    
    for (int i = 0; i < m_nscatter_pairs; ++i) {
        ybounds[0] = min({m_scatter_pairs[i]->ylim()[0], ybounds[0]});
        ybounds[1] = max({m_scatter_pairs[i]->ylim()[1], ybounds[1]});
    }
}

//void ScatterDisplay::update_label_values_at(double x) {
//    for (int i = 0; i < m_nwaveform_groups; ++i) {
//        m_waveform_groups[i]->set_label_values_at(x, m_data);
//    }
//    set_xlabel_value(x);
//}

//void ScatterDisplay::mousePressEvent(QMouseEvent * event)
//{
//    cout << "press event" << endl;
//    QwtScaleMap map = canvasMap(xBottom);
//    double m_mouse_xpos = map.invTransform(event->x());
//
//    double * widest_xlim = xlim();
//    double xrange = widest_xlim[1] - widest_xlim[0];
//
//    if (abs(m_mouse_xpos - m_xpos_cursor) < (xrange * .05)) { m_drag_cursor = true; }
//    else { update_group_cursor_positions(m_mouse_xpos); }
//}
//
//void ScatterDisplay::mouseMoveEvent(QMouseEvent * event) {
//    if (m_drag_cursor) {
//      cout << "cursor dragging.." << endl;
//      mousePressEvent(event); // update cursor positon
//    } else {
//      m_panning = true;
//
//      double pan_speed_scalar = .001;
//      double delta_x = event->x() - m_mouse_xpos;
//
//      double * widest_xlim = xlim();
//      double xrange = widest_xlim[1] - widest_xlim[0];
//
//      widest_xlim[0] += delta_x * pan_speed_scalar * xrange;
//      widest_xlim[1] += delta_x * pan_speed_scalar * xrange;
//      update_group_view_limits(widest_xlim[0], widest_xlim[1]);
//
//      m_mouse_xpos = event->x();
//      delete[] widest_xlim;
//    }
//}

//void ScatterDisplay::mouseDoubleClickEvent(QMouseEvent * event) {
//    cout << "reached: double-click" << endl;
//}

//void ScatterDisplay::wheelEvent(QWheelEvent * event) {
//    double scroll_speed_scalar = .001;
//    double vertical_scroll_delta = event->angleDelta().y();
//
//    double * widest_xlim = xlim();
//    double xrange = widest_xlim[1] - widest_xlim[0];
//
//    widest_xlim[0] -= scroll_speed_scalar * vertical_scroll_delta * xrange;
//    widest_xlim[1] += scroll_speed_scalar * vertical_scroll_delta * xrange;
//    update_group_view_limits(widest_xlim[0], widest_xlim[1]);
//
//    delete[] widest_xlim;
//}

void ScatterDisplay::reset()
{
    detachItems();
    replot();
}

}  // namespace graphic
}  // namespace insight
