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

private:
    Ui::CMines *ui;
};
#endif // CMINES_H
