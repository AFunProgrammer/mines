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
