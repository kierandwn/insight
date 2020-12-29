#ifndef DATACHANNEL_H
#define DATACHANNEL_H

#include <iostream>

// template <typename Tp>
class channel {
private:
    double * m_dataArray;

    double m_get(int);
    void m_set(int, double);

    int count = 0;

public:
    channel() {
        m_dataArray = new double[101]{ 0. };
        printf("hello.\n");
    }
    // channel() {}

    ~channel() {
        delete[] m_dataArray;
    }

    void push(double val) { m_dataArray[count] = val; count += 1; }
    double * buffer() { return &m_dataArray[count]; count += 1; }

    operator double * () const { return m_dataArray; }
};

#endif // DATACHANNEL_H
