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
#ifndef DATACHANNEL_H
#define DATACHANNEL_H
#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

namespace insight {
namespace data {

using namespace std;


// template <typename Tp>
class Channel {
private:
  vector<double> m_items;
  size_t len = 0;
  
  Channel * p_time_channel;
  
  string m_unit_string = "-";

public:
  Channel() : p_time_channel(this) {}
  Channel(Channel * t) : p_time_channel(t) {}
  ~Channel() {}

  void push(double val) { m_items.push_back(val); len += 1; }

  size_t length () { return len; }
  double * begin() { return &m_items[0]; }

  double max() {
    vector<double>::iterator it = std::max_element(m_items.begin(), m_items.end());
    return *it;
  }
  double min() {
    vector<double>::iterator it = std::min_element(m_items.begin(), m_items.end());
    return *it;
  }
  
  double value_at(double tval) {
    double decreasing_diff = abs(p_time_channel->operator[](0) - tval);
    double diff;
  
    size_t i;
    for (i = 1; i < m_items.size(); ++i) {
      diff = abs(p_time_channel->operator[](i) - tval);
      if (diff > decreasing_diff) { break; } else { decreasing_diff = diff; }
    }
    return m_items[i-1];
  }
  
  Channel * resample_on(Channel * time_to_resample_on, size_t n) { // TODO: BEWARE memory leak
    Channel * result = new Channel(time_to_resample_on);
    result->set_unit_string(m_unit_string);
    
    size_t k = 0;
    for (size_t i = 0; i < n; ++i)
    {
      if (time_to_resample_on->operator[](i) < p_time_channel->operator[](0)) {
        result->push(m_items[0]);
      } else // outwith bounds, above maximum
      if (time_to_resample_on->operator[](i) > p_time_channel->operator[](len - 1)) {
        result->push(m_items[len - 1]);
      } else { // interpolate between neighbouring points
        while (!(time_to_resample_on->operator[](i) >= p_time_channel->operator[](k) && time_to_resample_on->operator[](i) <= p_time_channel->operator[](k + 1)))
        { ++k; }
        
        double  dt = p_time_channel->operator[](k + 1) - p_time_channel->operator[](k);
        double ddt = time_to_resample_on->operator[](i) - p_time_channel->operator[](k);
        result->push(m_items[k] + (m_items[k + 1] - m_items[k]) * (ddt / dt));
      }
    }
    return result;
  }
  
  void update_time_channel_ptr(Channel * t) { p_time_channel = t; }
  
  double * get_data_ptr() { return &operator[](0); }
  
  double * get_time_data_ptr() { return &p_time_channel->operator[](0); }
  Channel * get_time_ref() { return p_time_channel; }
  
  string get_unit_string() { return m_unit_string; }
  void set_unit_string(string unit_string) { m_unit_string = unit_string; }

  double& operator[] (int i) { return m_items[i]; }
};


}  // namespace data
}  // namespace insight
#endif // DATACHANNEL_H
