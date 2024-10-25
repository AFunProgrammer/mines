#include "mines.h"
#include "ui_mines.h"

bool bInitializing = true;

CMines::CMines(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMines)
{
    ui->setupUi(this);

//    connect(screen(),&QScreen::orientationChanged,this,&CMines::orientationChanged);

    m_Minefield.setParent(ui->centralwidget);
    m_Minefield.show();
    m_Minefield.raise();

    m_Minefield.setCellSize(50);
    m_Minefield.generateMinefield(20);

    bInitializing = false;
}

CMines::~CMines()
{
    delete ui;
}

/************ Looking for sizing issue with orientation changing but this isn't it... *********
void CMines::orientationChanged(Qt::ScreenOrientation)
{
    if ( bInitializing )
        return;

    resizeEvent(nullptr);
}
**********************************************************************************************/

void CMines::resizeEvent(QResizeEvent*)
{
#if defined(Q_OS_ANDROID)
    int iWidth = screen()->availableGeometry().width();
    int iHeight = screen()->availableGeometry().height();
    //this->setGeometry(QRect(0,0,iWidth,iHeight));
#else
    int iWidth = window()->childrenRect().width();
    int iHeight = window()->childrenRect().height();
#endif

    //This is, as far as I can tell, the entire client area of the window
    QRect rctDrawBox = QRect(0,
                             0,
                             iWidth,
                             iHeight);

    m_Minefield.setBounds(rctDrawBox);
    m_Minefield.setCellSize(m_Minefield.getCellSize());
    m_Minefield.generateMinefield(20);
    m_Minefield.update();
}
