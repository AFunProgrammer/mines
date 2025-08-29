/*
 * This file is part of mines.
 *
 * Copyright (C) 2024-2025 InspiringFamily.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

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
