#ifndef UTILITY_H
#define UTILITY_H

#endif // UTILITY_H

#pragma once
#include <QPoint>

class QPointI: public QPoint
{
public:
    constexpr QPointI() noexcept: QPoint() {}
    constexpr QPointI(int xpos, int ypos) noexcept : QPoint(xpos,ypos) {}

    int m_unused = -1;

    int& operator[](int idx)
    {
        if (idx == 0)
            return this->rx();
        else if(idx == 1)
            return this->ry();
        else
            return m_unused;
    }

    void setIndex(int idx, int value)
    {
        if ( idx == 0 )
            this->setX(value);
        else if( idx == 1 )
            this->setY(value);
    }
};

class CUtility
{
public:
    CUtility();

    static inline ulong getCrossProduct(QPoint ptA, QPoint ptB)
    {
        float fXProduct = sqrt(((ptA.x() - ptB.x()) * (ptA.x() - ptB.x())) + ((ptA.y() - ptB.y()) * (ptA.y() - ptB.y())));

        return (ulong)fXProduct;
    }
};
