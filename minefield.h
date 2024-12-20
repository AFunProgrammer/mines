#ifndef CMINEFIELD_H
#define CMINEFIELD_H

#pragma once
#include <QMouseEvent>
#include <QPainter>
#include <QtSvg/QtSvg>
#include <QVector>
#include <QOpenGLWidget>

#include "utility.h"

enum class GameState
{
    Playing = 0x0,
    Lost = 0x1,
    Won = 0x2
};

typedef struct _MineFieldCell
{
    char m_Value = '0'; //default is nothing in/near cell
    bool m_DrawBox = true; //default is to draw box
    bool m_HasFlag = false; //default is not to have a Flag
    QRect m_Rect = QRect(0,0,0,0);
}MineFieldCell;

class CMinefield : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit CMinefield(QWidget *parent = nullptr);
    ~CMinefield();

//single class needs
private:
    QSvgRenderer m_SvgRenderer;
    QVector<QVector<MineFieldCell>> m_Minefield;

    QColor m_ClearColor = Qt::black;

    bool m_bDirtyUpdate = false;
    GameState m_GameState = GameState::Playing;

    int  m_iBombCount = 0;
    int  m_iCellCount = 0;

    QPointI m_HoverCell = QPointI(-1,-1);
    QPointI m_HitBox = QPointI(-1,-1);
    QPointI m_Bomb = QPointI(-1,-1);

    QPixmap m_pxmGrid;
    QPixmap m_pxmValues;;
    QPixmap m_pxmPushBox;
    QPixmap m_pxmPushedBox;
    QPixmap m_pxmFlag;

    QPointI m_ExtraSpace;
    QPointI m_CellSize = QPointI(50,50); //default box size in pixels, determines the number of boxes
    uint    m_uiCellSize = 50;
    // font point to cell = (pixel_size / 96) * 72;
    QFont m_CellFont; //every time the size is set then this also needs to be set
    QFontMetrics* m_pFontMetrics = nullptr;

private:
    QPixmap drawGrid();
    QPixmap drawValues();
    QPixmap drawPushBox();
    QPixmap drawPushedBox();
    QPixmap drawFlag();

    void drawMinefield(QPainter* Painter);

    bool canCornerBeUncovered(QPointI cornerCoord, QPointI sourceCoord);
    void checkForHitEmptyAndUpdate();

public:
    int getBombCount();
    void generateMinefield(uint NumberOfBombs);

    uint getCellSize();
    void setCellSize(uint CellSize);

    void setBounds(QRect Rect);
    void setClearColor(QColor Color);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent* event) override;

};

#endif // CMINEFIELD_H
