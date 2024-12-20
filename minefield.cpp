#include "minefield.h"

#include <iostream>

#include <QtMath>

CMinefield::CMinefield(QWidget *parent) : QOpenGLWidget{parent}
{
    //get mouse move / hover events
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover, true);

    setAutoFillBackground(false);
    setCellSize(50);
}

CMinefield::~CMinefield()
{
    if ( m_pFontMetrics != nullptr )
        delete m_pFontMetrics;
}

void CMinefield::setClearColor(QColor Color)
{
    m_ClearColor = Color;
}

void CMinefield::setBounds(QRect Rect)
{
    setGeometry(Rect);
}

int CMinefield::getBombCount()
{
    return m_iBombCount;
}

uint CMinefield::getCellSize()
{
    return m_uiCellSize;
}

QPixmap CMinefield::drawGrid()
{
    QPixmap grid = QPixmap(geometry().width(), geometry().height());
    QPainter painter = QPainter(&grid);

    painter.setBrush(Qt::GlobalColor::lightGray);
    painter.drawRect(geometry());

    int rows = m_Minefield.count();
    int cols = (rows > 0 ? m_Minefield[0].count() : 0 );

    uint left = m_ExtraSpace[0];
    uint top = m_ExtraSpace[1];
    uint right = left + (geometry().width()-m_ExtraSpace[0]*2);
    uint bottom = top + (geometry().height()-m_ExtraSpace[1]*2);

    painter.setBrush(Qt::GlobalColor::black);

    uint startX;
    uint startY = top;
    uint endX;
    uint endY = bottom;

    //vertical lines
    for( uint xline = 1; xline < static_cast<uint>(cols); xline++ )
    {
        startX = xline*m_CellSize[0] + m_ExtraSpace[0]/2;
        endX = xline*m_CellSize[0] + m_ExtraSpace[0]/2;

        painter.drawLine(startX,startY,endX,endY);
    }

    startX = left;
    endX = right;

    //horizontal lines
    for( uint yline = 1; yline < static_cast<uint>(rows); yline++ )
    {
        startY = yline*m_CellSize[1] + m_ExtraSpace[1]/2;
        endY = yline*m_CellSize[1] + m_ExtraSpace[1]/2;

        painter.drawLine(startX,startY,endX,endY);
    }

    painter.end();

    return grid;
}

QPixmap CMinefield::drawPushBox()
{
    //Create pushBox, make sure it's size is smaller than the cell
    QPixmap pushBox = QPixmap(m_CellSize[0]-1,m_CellSize[1]-1);
    QPainter painter = QPainter(&pushBox);

    uint left = 0;
    uint top = 0;
    uint right = pushBox.width()-1;
    uint bottom = pushBox.height()-1;

    QRect InsideRect = QRect(1,1,right-2,bottom-2);

    //light
    painter.setPen(Qt::GlobalColor::white);
    painter.drawLine(left, top, right, top);
    painter.drawLine(left, top, left, bottom);
    //shadow
    painter.setPen(Qt::GlobalColor::darkGray);
    painter.drawLine(right, top, right, bottom);
    painter.drawLine(left, bottom, right, bottom);
    //box main
    painter.setBrush(Qt::GlobalColor::gray);
    painter.setPen(Qt::GlobalColor::gray);
    painter.drawRect(InsideRect);

    painter.end();

    return pushBox;
}

QPixmap CMinefield::drawPushedBox()
{
    //Create pushBox, make sure it's size is smaller than the cell
    QPixmap pushedBox = QPixmap(m_CellSize[0]-1,m_CellSize[1]-1);
    QPainter painter = QPainter(&pushedBox);

    uint left = 0;
    uint top = 0;
    uint right = pushedBox.width()-1;
    uint bottom = pushedBox.height()-1;

    QRect InsideRect = QRect(1,1,right-2,bottom-2);

    //shadow
    painter.setPen(Qt::GlobalColor::darkGray);
    painter.drawLine(left, top, right, top);
    painter.drawLine(left, top, left, bottom);
    //light
    painter.setPen(Qt::GlobalColor::white);
    painter.drawLine(right, top, right, bottom);
    painter.drawLine(left, bottom, right, bottom);
    //box main
    painter.setBrush(Qt::GlobalColor::gray);
    painter.setPen(Qt::GlobalColor::gray);
    painter.drawRect(InsideRect);

    painter.end();

    return pushedBox;
}

QPixmap CMinefield::drawFlag()
{
    QPixmap flag = QPixmap(":/Flag.png");
    QPointI flagSize = QPointI(flag.width(), flag.height());
    uint uiSize = fmin(m_CellSize[0],m_CellSize[1]);

    float fMod = static_cast<float>(uiSize) / static_cast<float>(flagSize[0]);
    fMod *= 3.0f/5.0f;

    QPointF newSize = QPointF( static_cast<float>(flagSize[0])*fMod,
                               static_cast<float>(flagSize[1])*fMod);

    flagSize = QPointI(newSize.x(),newSize.y());

    return flag.scaled(flagSize.rx(),flagSize.ry());
}

QPixmap CMinefield::drawValues()
{
    QPixmap values = QPixmap(geometry().width(),geometry().height());
    values.fill(Qt::transparent);
    QPainter painter(&values);
    QColor fontColor;

    painter.setBackgroundMode(Qt::BGMode::TransparentMode);
    painter.setBackground(QBrush(Qt::transparent));

    painter.fillRect(geometry(),QBrush(Qt::transparent));

    int rows = m_Minefield.count();
    int cols = (rows > 0 ? m_Minefield[0].count() : 0 );

    painter.setFont(m_CellFont);

    for( int y = 0; y < rows; y++ )
    {
       for( int x = 0; x < cols; x++ )
       {
           uint left = m_ExtraSpace[0]/2 + (m_CellSize[0]*x) + 1;
           uint top = m_ExtraSpace[1]/2 + (m_CellSize[1]*y) + 1;
           uint width = m_CellSize[0] - 1;
           uint height = m_CellSize[1] - 1;

           QRect InsideRect = QRect(left+1,top+1,width-1,height-1);

           //Update rect
           m_Minefield[y][x].m_Rect = InsideRect;

           //If no value then continue onward
           if ( m_Minefield[y][x].m_Value == ' ' )
               continue;

           //For Centering The Font
           int charWidth = m_pFontMetrics->horizontalAdvance(QChar(m_Minefield[y][x].m_Value));
           QRect fontRect = QRect(InsideRect.left() + InsideRect.width()/2 - charWidth/2,
                                  InsideRect.top(),
                                  charWidth,
                                  InsideRect.height());

           switch(static_cast<int>(m_Minefield[y][x].m_Value))
           {
           case '1':
               fontColor = Qt::GlobalColor::darkBlue;
               break;
           case '2':
               fontColor = Qt::GlobalColor::darkCyan;
               break;
           case '3':
               fontColor = QColor::fromRgb(160,64,0); //DarkOrange
               break;
           case '4':
               fontColor = Qt::GlobalColor::darkGreen;
               break;
           case '5':
               fontColor = Qt::GlobalColor::darkMagenta;
               break;
           case '6':
               fontColor = Qt::GlobalColor::darkYellow;
               break;
           case '7':
               fontColor = QColor::fromRgb(115,77,70); //Brown
               break;
           case '8':
               fontColor = Qt::GlobalColor::darkRed;
               break;
           case '*':
               fontColor = Qt::black;
               break;
           }

           painter.setPen(fontColor);
           painter.setBrush(fontColor);
           painter.drawText(fontRect,QString(m_Minefield[y][x].m_Value));
        }
    }

    painter.end();
    return values;
}



/*********** TODO *************/
void CMinefield::drawMinefield(QPainter* Painter)
{
    //Draw Pixmaps For Each Necessary Item (should be lightweight)
    if ( m_bDirtyUpdate )
    {
        m_pxmGrid = drawGrid();
        m_pxmValues = drawValues();
        m_pxmPushBox = drawPushBox();
        m_pxmPushedBox = drawPushedBox();
        m_pxmFlag = drawFlag();
        m_bDirtyUpdate = false;
    }

    Painter->drawPixmap(geometry(),m_pxmGrid);
    Painter->drawPixmap(geometry(),m_pxmValues);

    for( int y = 0; y < m_Minefield.count(); y++ )
        for( int x = 0; x < m_Minefield[y].count(); x++ )
        {
            if ( m_Minefield[y][x].m_DrawBox )
            {
                Painter->drawPixmap(m_Minefield[y][x].m_Rect,m_pxmPushBox);
                if ( m_Minefield[y][x].m_HasFlag )
                    Painter->drawPixmap(m_Minefield[y][x].m_Rect,m_pxmFlag);
            }
        }

    if ( m_HoverCell[0] != -1 )
        if ( m_Minefield[m_HoverCell[1]][m_HoverCell[0]].m_DrawBox )
        {
            Painter->drawPixmap(m_Minefield[m_HoverCell[1]][m_HoverCell[0]].m_Rect,m_pxmPushedBox);
            if ( m_Minefield[m_HoverCell[1]][m_HoverCell[0]].m_HasFlag )
                Painter->drawPixmap(m_Minefield[m_HoverCell[1]][m_HoverCell[0]].m_Rect,m_pxmFlag);
        }

    if ( m_Bomb[0] != -1 )
    {
        Painter->setBrush(QBrush(Qt::red));
        QRect insideRect = m_Minefield[m_Bomb[1]][m_Bomb[0]].m_Rect;
        insideRect.adjust(0,-1,0,-1);
        Painter->drawRect(insideRect);
    }

}

//Find a cell size the fills the space closest to the desired
// cell size without excessive growth
QPointI offerNewCellSize(uint width, uint height, QPointI* OfferedSize)
{
    QPointI initialSize = *OfferedSize;
    QPointI extraSpace;

    QPointI coords = QPointI(width,height);
    uint checkSize = 0;
    uint closest;
    uint space = 0;

    for(uint coord = 0; coord <= 1; coord++ )
    {
        closest = 999;

        for( int sign = -1; sign <= 1; sign++ )
        {
            if (sign == 0)
                continue;

            checkSize = initialSize[coord];

            for( int i = 0; i < 5; i++ )
            {
                checkSize += i*sign;
                int value = coords[coord];
                space = value % checkSize;
                //space = coords[coord] % checkSize;

                if ( space < closest )
                {
                    OfferedSize->setIndex(coord,checkSize);
                    extraSpace.setIndex(coord,space);
                    closest = space;
                }//endif
            }//end for( int i...
        }//end for(int sign...
    }//end for(uint coord...

    //qDebug() << "debug: " << "Initial Size: " << initialSize[0] << ":" << initialSize[1] << "Offered Size: " << (*OfferedSize)[0] << ":" << (*OfferedSize)[1] << "Extra Space: " << extraSpace[0] << ":" << extraSpace[1];

    return extraSpace;
}//end QPoint offerNewCellSize

void CMinefield::setCellSize(uint CellSize)
{
    if ( CellSize < 32 )
        return;

    uint width = this->geometry().width();
    uint height = this->geometry().height();

    QPointI cellSize = QPointI(CellSize,CellSize);
    m_uiCellSize = CellSize;

    //calculate size based upon nearest amount to fill entire area
    m_ExtraSpace = offerNewCellSize(width,height,&cellSize);

    m_CellSize = cellSize;
    uint uiSize = fmin(cellSize[0],cellSize[1]);
    uint uiPtSize = static_cast<uint>((static_cast<float>(uiSize) / 96.0f)*72.0f);
    m_CellFont = QFont("SansSerif", uiPtSize, QFont::Bold, false);

    if ( m_pFontMetrics != nullptr )
        delete m_pFontMetrics;

    m_pFontMetrics = new QFontMetrics(m_CellFont);

    m_HoverCell = QPointI(-1,-1);
    m_HitBox = QPointI(-1,-1);
    m_Bomb = QPointI(-1,-1);

    m_iCellCount = (width / m_CellSize[0]) * (height / m_CellSize[1]);

    m_bDirtyUpdate = true;
}

void CMinefield::generateMinefield(uint NumberOfBombs)
{
    int rows = geometry().height() / m_CellSize[1];
    int cols = geometry().width() / m_CellSize[0];
    int count = 0;

    int UseThisManyBombs = NumberOfBombs;

    m_Bomb = QPointI(-1,-1);
    m_HoverCell = QPointI(-1,-1);
    m_HitBox = QPointI(-1,-1);
    m_GameState = GameState::Playing;

    m_bDirtyUpdate = true;

    m_iBombCount = NumberOfBombs;


    if ( m_Minefield.count() > 0 )
        m_Minefield.clear();

    //Generate all the cells
    for( int y = 0; y < rows; y++ )
    {
        QVector<MineFieldCell> row;
        for( int x = 0; x < cols; x++ )
        {
            MineFieldCell mfc;
            mfc.m_DrawBox = true; //( y == x || (x>0 && y>0 && y % x == 0) ? false : true);
            mfc.m_Value = ' ';
            row.push_back(mfc);
        }
        m_Minefield.push_back(row);
    }

    //Change the number of bombs if there are too many...
    if ( m_iCellCount / 8 <= m_iBombCount )
        UseThisManyBombs = m_iCellCount / 8;


    //Randomly select bomb locations
    for( int bomb = 0; bomb < static_cast<int>(UseThisManyBombs); bomb++ )
    {
        int col = std::rand() % cols;
        int row = std::rand() % rows;

        if ( m_Minefield[row][col].m_Value != '*' )
            m_Minefield[row][col].m_Value = '*';
        else
            bomb--;
    }

    //Calculate all the numbers around the bombs!
    for( int y = 0; y < rows; y++ )
    {
        for( int x = 0; x < cols; x++ )
        {
            count = 0;
            if ( m_Minefield[y][x].m_Value != '*' )
            {
                //check in all directions for nearest bombs
                count += (x-1 >= 0 && m_Minefield[y][x-1].m_Value == '*' ? 1 : 0);
                count += (x+1 < cols && m_Minefield[y][x+1].m_Value == '*' ? 1 : 0);
                count += (x-1 >= 0 && y-1 >= 0 && m_Minefield[y-1][x-1].m_Value == '*' ? 1 : 0);
                count += (x-1 >= 0 && y+1 < m_Minefield.size() && m_Minefield[y+1][x-1].m_Value == '*' ? 1 : 0);
                count += (x+1 < cols && y-1 >= 0 && m_Minefield[y-1][x+1].m_Value == '*' ? 1 : 0);
                count += (x+1 < cols && y+1 < m_Minefield.size() && m_Minefield[y+1][x+1].m_Value == '*' ? 1 : 0);
                count += (y-1 >= 0 && m_Minefield[y-1][x].m_Value == '*' ? 1 : 0);
                count += (y+1 < rows && m_Minefield[y+1][x].m_Value == '*' ? 1 : 0);

                //set the number of nearest bombs
                if (count > 0)
                    m_Minefield[y][x].m_Value = std::to_string(count)[0];
            }//end if ( m_Minefield[y][x].m_Value
        }//end for( int x = 0
    }//end for( int y
}




bool CMinefield::canCornerBeUncovered(QPointI cornerCoord, QPointI sourceCoord)
{
    int rows = m_Minefield.count();
    int cols = m_Minefield[0].count();

    int x = cornerCoord[0];
    int y = cornerCoord[1];

    if ( x >= cols || x < 0 )
        return false;

    if ( y >= rows || y < 0 )
        return false;

    if ( y == 1 && x == 2 )
        qDebug() << "found desired box";

    MineFieldCell* cell = &(m_Minefield[y][x]);
    bool top = (y-1 >= 0 ? m_Minefield[y-1][x].m_HasFlag : false );
    bool left = (x-1 >= 0 ? m_Minefield[y][x-1].m_HasFlag : false );
    bool right = (x+1 < cols ? m_Minefield[y][x+1].m_HasFlag : false );
    bool bottom = (y+1 < rows ? m_Minefield[y+1][x].m_HasFlag : false );

    if ( cornerCoord == QPointI(sourceCoord[0]+1, sourceCoord[1]+1))
        if ( top && left )
            return false;

    if ( cornerCoord == QPointI(sourceCoord[0]-1, sourceCoord[1]+1))
        if ( top && right )
            return false;

    if ( cornerCoord == QPointI(sourceCoord[0]+1, sourceCoord[1]-1))
        if ( bottom && left )
            return false;

    if ( cornerCoord == QPointI(sourceCoord[0]-1, sourceCoord[1]-1))
        if ( bottom && right )
            return false;

    if ( cell->m_Value != '*' && cell->m_Value != ' ' && !(cell->m_HasFlag) )
        return true;

    return false;
}

void CMinefield::checkForHitEmptyAndUpdate()
{
    if ( m_HitBox == QPointI(-1,-1) ||
         m_Minefield[m_HitBox[1]][m_HitBox[0]].m_DrawBox == false )
        return;

    int rows = m_Minefield.count();
    int cols = m_Minefield[0].count();

    QVector<QPointI> checkList;
    QVector<QPointI> foundList;

    //Find all the empty space around a hit
    if( m_Minefield[m_HitBox[1]][m_HitBox[0]].m_Value == ' ' )
        checkList.push_back(m_HitBox);
    else //if not empty space keep going
        foundList.push_back(m_HitBox);

    ////
    /// TODO MUST CHECK FOR BOMB STRIKE
    ////


    while( checkList.isEmpty() == false )
    {
        QPointI check = checkList.takeFirst();
        if ( foundList.contains(check) || m_Minefield[check[1]][check[0]].m_HasFlag )
            continue;

        foundList.push_back(check);

        int x = check[0];
        int y = check[1];

        if (x-1 >= 0 && m_Minefield[y][x-1].m_Value == ' ')
        {
            if( !foundList.contains(QPointI(x-1,y)) )
                checkList.push_back(QPointI(x-1,y));
        }
        else if(x-1 >= 0 && m_Minefield[y][x-1].m_Value != '*')
            if( !foundList.contains(QPointI(x-1,y)) )
                foundList.push_back(QPointI(x-1,y));

        if (x+1 < cols && m_Minefield[y][x+1].m_Value == ' ')
        {
            if( !foundList.contains(QPointI(x+1,y)) )
                checkList.push_back(QPointI(x+1,y));
        }
        else if(x+1 < cols && m_Minefield[y][x+1].m_Value != '*')
            if( !foundList.contains(QPointI(x+1,y)) )
                foundList.push_back(QPointI(x+1,y));

        if (y-1 >= 0 && m_Minefield[y-1][x].m_Value == ' ')
        {
            if( !foundList.contains(QPointI(x,y-1)) )
                checkList.push_back(QPointI(x,y-1));
        }
        else if(y-1 >= 0 && m_Minefield[y-1][x].m_Value != '*')
            if( !foundList.contains(QPointI(x,y-1)) )
                foundList.push_back(QPointI(x,y-1));

        if (y+1 < rows && m_Minefield[y+1][x].m_Value == ' ')
        {
            if( !foundList.contains(QPointI(x,y+1)) )
                checkList.push_back(QPointI(x,y+1));
        }
        else if(y+1 < rows && m_Minefield[y+1][x].m_Value != '*')
            if( !foundList.contains(QPointI(x,y+1)) )
                foundList.push_back(QPointI(x,y+1));

        //Check the adjacent corners for numbers only
        // and uncover them if they are only numbers instead
        // of being blank boxes with untouching sides
        if( canCornerBeUncovered(QPointI(x-1,y-1),QPointI(x,y)) )
            if( !foundList.contains(QPointI(x-1,y-1)) )
                foundList.push_back(QPointI(x-1,y-1));

        if( canCornerBeUncovered(QPointI(x-1,y+1),QPointI(x,y)) )
            if( !foundList.contains(QPointI(x-1,y+1)) )
                foundList.push_back(QPointI(x-1,y+1));

        if( canCornerBeUncovered(QPointI(x+1,y-1),QPointI(x,y)) )
            if( !foundList.contains(QPointI(x+1,y-1)) )
                foundList.push_back(QPointI(x+1,y-1));

        if( canCornerBeUncovered(QPointI(x+1,y+1),QPointI(x,y)) )
            if( !foundList.contains(QPointI(x+1,y+1)) )
                foundList.push_back(QPointI(x+1,y+1));
    }

    for( QPointI pt:foundList )
    {
        if ( m_Minefield[pt[1]][pt[0]].m_HasFlag )
            continue;

        m_Minefield[pt[1]][pt[0]].m_DrawBox = false;
    }
}


void CMinefield::mousePressEvent(QMouseEvent *event)
{
    QPainter qpMeasure(this);
    QPoint clickPos = event->pos();
    QRect clickArea = QRect(0,0,0,0);

    if ( m_GameState != GameState::Playing )
    {
        mouseDoubleClickEvent(event);
        return;
    }

    float fDpcmX = (float)qpMeasure.device()->logicalDpiX() / 2.54f;
    float fDpcmY = (float)qpMeasure.device()->logicalDpiY() / 2.54f;

    //Click area is considered the 'nice' area of being clicked in
    // allowing for 'fat' finger touch to select a specific spot
    // within a region and allowing for making mistakes but
    // still having the pleasure of using touch
    clickArea.setX(clickPos.x() - (int)(fDpcmX/2.0f) );
    clickArea.setY(clickPos.y() - (int)(fDpcmY/2.0f) );
    clickArea.setWidth((int)(fDpcmX) );
    clickArea.setHeight((int)(fDpcmY) );

    QPointI closest = QPointI(-1,-1);

    for( int row=0; row<m_Minefield.count(); row++ )
        for( int col=0; col<m_Minefield[row].count(); col++ )
        {
            if (clickArea.intersects(m_Minefield[row][col].m_Rect))
            {
                if ( closest[0] != -1 )
                {
                    QRect rectNew = m_Minefield[row][col].m_Rect;
                    QRect rectClosest = m_Minefield[closest[1]][closest[0]].m_Rect;
                    ulong uNew = CUtility::getCrossProduct(rectNew.center(), clickPos);
                    ulong uClosest = CUtility::getCrossProduct(rectClosest.center(), clickPos);
                    if ( uNew < uClosest )
                        closest = QPointI(col,row);
                }
                else
                   closest = QPointI(col,row);
            }
        }

    if ( closest == QPointI(-1,-1) )
        std::cerr << "debug: nothing to output or do" << std::endl;
    else
    {
        m_HitBox = closest;

        if ( event->deviceType() == QInputDevice::DeviceType::Mouse && event->button() == Qt::RightButton)
            if ( m_Minefield[m_HitBox[1]][m_HitBox[0]].m_DrawBox )
            {
                m_Minefield[m_HitBox[1]][m_HitBox[0]].m_HasFlag = !(m_Minefield[m_HitBox[1]][m_HitBox[0]].m_HasFlag);
                m_HitBox = QPointI(-1,-1);
                this->update();
                return;
            }

        if ( m_Minefield[m_HitBox[1]][m_HitBox[0]].m_HasFlag )
        {
            m_HitBox = QPointI(-1,-1);
            return;
        }

        if ( m_Minefield[m_HitBox[1]][m_HitBox[0]].m_Value == '*' )
        {
            for( int y=0; y < m_Minefield.count(); y++ )
                for( int x=0; x < m_Minefield[y].count(); x++ )
                    m_Minefield[y][x].m_DrawBox = false;

            m_Bomb = closest;
            m_GameState = GameState::Lost;
        }
        else
            checkForHitEmptyAndUpdate();

        this->update();
    }
}

//On a platform that allows the mouse, or sense touch devices
// that can detect a near touch without physical contact
// this will allow the 'hover' effect to make cards pop-up
// to easily identify selection
void CMinefield::mouseMoveEvent(QMouseEvent* event)
{
    QPainter qpMeasure(this);
    QPoint clickPos = event->pos();
    QRect clickArea = QRect(0,0,0,0);

    if ( m_GameState != GameState::Playing )
        return;

    float fDpcmX = (float)qpMeasure.device()->logicalDpiX() / 2.54f;
    float fDpcmY = (float)qpMeasure.device()->logicalDpiY() / 2.54f;

    //Click area is considered the 'nice' area of being clicked in
    // allowing for 'fat' finger touch to select a specific spot
    // within a region and allowing for making mistakes but
    // still having the pleasure of using touch
    clickArea.setX(clickPos.x() - (int)(fDpcmX/2.0f) );
    clickArea.setY(clickPos.y() - (int)(fDpcmY/2.0f) );
    clickArea.setWidth((int)(fDpcmX) );
    clickArea.setHeight((int)(fDpcmY) );

    QPointI closest = QPointI(-1,-1);

    for( int row=0; row<m_Minefield.count(); row++ )
        for( int col=0; col<m_Minefield[row].count(); col++ )
        {
            if (clickArea.intersects(m_Minefield[row][col].m_Rect))
            {
                if ( closest[0] != -1 )
                {
                    QRect rectNew = m_Minefield[row][col].m_Rect;
                    QRect rectClosest = m_Minefield[closest[1]][closest[0]].m_Rect;
                    ulong uNew = CUtility::getCrossProduct(rectNew.center(), clickPos);
                    ulong uClosest = CUtility::getCrossProduct(rectClosest.center(), clickPos);
                    if ( uNew < uClosest )
                        closest = QPointI(col,row);
                }
                else
                   closest = QPointI(col,row);

            }
        }

    bool bUpdate = false;
    if ( m_HoverCell != closest && m_HoverCell[0] != -1 && closest[0] != -1 )
        bUpdate = true;

    m_HoverCell = closest;

    if ( bUpdate )
        this->update();
}

void CMinefield::wheelEvent(QWheelEvent* event)
{
    int cellSize = (int)m_uiCellSize + event->angleDelta().ry() / 8;

    if ( cellSize <= 5 )
        return;

    if ( cellSize >= geometry().width() / 4 )
        return;

    setCellSize(static_cast<uint>(cellSize));
    generateMinefield(m_iBombCount);

    this->update();
}

//TODO
void CMinefield::mouseDoubleClickEvent(QMouseEvent*)
{
    //Reset on a double click
    generateMinefield(m_iBombCount);

    //possible marking a mine
    this->update();
    this->repaint();
}

void CMinefield::paintEvent(QPaintEvent *event)
{
    QPainter Painter(this);
    QBrush brushBackground = QBrush(m_ClearColor);

    //Painter.begin(this);
    //glEnable(GL_MULTISAMPLE);
    //glEnable(GL_LINE_SMOOTH);
    Painter.setRenderHint(QPainter::Antialiasing);
    Painter.setRenderHint(QPainter::SmoothPixmapTransform);
    Painter.fillRect(event->rect(),brushBackground);
    Painter.setFont(m_CellFont);


    drawMinefield(&Painter);

    //check for win
    if ( m_Bomb[0] == -1 && m_GameState == GameState::Playing )
    {
        int count = 0;
        for( QVector<MineFieldCell> vmfc: m_Minefield )
            for( MineFieldCell mfc: vmfc )
            {
                if ( mfc.m_DrawBox == true && mfc.m_Value != '*' )
                    count++;
            }

        if ( count == 0 )
            m_GameState = GameState::Won;
    }

    QVector<QString> GameMessage;

    if ( m_GameState == GameState::Won )
    {
        GameMessage.append("You Won!!!");
    }
    else if ( m_GameState == GameState::Lost )
    {
        GameMessage.append("You Lost :'(");
    }

    if ( m_GameState != GameState::Playing )
    {
        GameMessage.append(" ");
        GameMessage.append("Click To Restart");
        GameMessage.append(" ");
        GameMessage.append("Change Size With");
        GameMessage.append("Mouse Wheel");

        Painter.setBrush(QColor::fromRgbF(1.0f,1.0f,1.0f,.50f));
        Painter.drawRect(geometry());

        Painter.setPen(Qt::red);
        QFont fMessage = QFont("SansSerif", 48, QFont::Bold, false);
        QFontMetrics fMetrics = QFontMetrics(fMessage,Painter.paintEngine()->paintDevice());

        if ( fMetrics.maxWidth() * 16 > geometry().width() )
        {
            fMessage = QFont("SansSerif", 24, QFont::Bold, false);
            fMetrics = QFontMetrics(fMessage,Painter.paintEngine()->paintDevice());
        }

        Painter.setFont(fMessage);

        QPoint drawPoint = geometry().center();
        drawPoint.setY(drawPoint.y() - ((fMetrics.height() * (GameMessage.count()+1))/2));

        for( QString line: GameMessage )
        {
            drawPoint.setX(drawPoint.rx() - fMetrics.boundingRect(line).width()/2);
            drawPoint.setY(drawPoint.ry() + fMetrics.height());
            Painter.drawText(drawPoint,line);
            drawPoint.setX(geometry().center().rx());
        }
    }

    /***********
     * drawBoxes
     */

    Painter.end();
}
