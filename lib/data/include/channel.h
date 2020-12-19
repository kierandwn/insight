#ifndef DATACHANNEL_H
#define DATACHANNEL_H

// template <typename Tp>
class channel {
private:
    double * m_dataArray;

    double m_get(int);
    void m_set(int, double);

public:
    channel(int ln, double * raw)
    {

    };
};

#endif // DATACHANNEL_H
