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
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void setSliderSteps();
    void setCellSizeSlider();
    void setMinesSlider();

    Ui::CMines *ui;
};
#endif // CMINES_H
