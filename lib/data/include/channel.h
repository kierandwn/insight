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

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

namespace insight {
namespace data {


// template <typename Tp>
class Channel {
private:
    vector<double> m_items;
    size_t len = 0;
    
    Channel * p_time_channel;

public:
    Channel() : p_time_channel(this) {}
    Channel(Channel * t) : p_time_channel(t) {}
    ~Channel() {}

    void push(double val) { m_items.push_back(val); len += 1; }
    // double * buffer() { return &m_items[count]; len += 1; }

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
    
    double value_at(double xval) {
      double decreasing_diff = abs(p_time_channel->operator[](0) - xval);
      double diff;
    
      size_t i;
      for (i = 1; i < len; ++i) {
        diff = abs(p_time_channel->operator[](i) - xval);
        if (diff > decreasing_diff) { break; } else { decreasing_diff = diff; }
      }
      return m_items[i];
    }
    
    void update_time_channel_ptr(Channel * t) { p_time_channel = t; }
    
    double * get_time_data_ptr() { return &p_time_channel->operator[](0); }
    Channel * get_time_ref() { return p_time_channel; }

    double& operator[] (int i) { return m_items[i]; }
};


}  // namespace data
}  // namespace insight
#endif // DATACHANNEL_H
