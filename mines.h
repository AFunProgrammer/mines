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

#ifndef CMINES_H
#define CMINES_H

#pragma once

#include <QMainWindow>

#include "minefield.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CMines; }
QT_END_NAMESPACE

class CMines : public QMainWindow
{
    Q_OBJECT

public:
    CMines(QWidget *parent = nullptr);
    ~CMines();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setSliderSteps();
    void setCellSizeSlider();
    void setMinesSlider();

    void resetGame();

    // need a list of square mine sizes to use
    QVector<int> m_vecMineSizes = QVector<int>({40});

    // need to wait for ui to size itself first
    bool m_uiLoaded = false;
    QTimer *m_timer = nullptr;

    Ui::CMines *ui;
};
#endif // CMINES_H
