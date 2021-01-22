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
#ifndef DATACHANNEL_H
#define DATACHANNEL_H

#include <iostream>
#include <vector>

using namespace std;

// template <typename Tp>
class channel {
private:
    vector<double> m_dataArray;

    double m_get(int);
    void m_set(int, double);

    size_t len = 0;

public:
    channel() {}
    // channel() {}

    ~channel() {}

    void push(double val) { m_dataArray.push_back(val); len += 1; }
    // double * buffer() { return &m_dataArray[count]; len += 1; }

    size_t length () { return len; }
    double * begin() { return &m_dataArray[0]; }

    double * operator[] (int i) { return &m_dataArray[i]; }
    // operator double * () const { return m_dataArray; }
};

#endif // DATACHANNEL_H
