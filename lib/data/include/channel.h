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
