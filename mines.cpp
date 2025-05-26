#include "mines.h"
#include "ui_mines.h"

bool bInitializing = true;




////////////////////////
/// \brief all the dirt
/// drydirt
/// drymud
/// redclay
/// rocks
/// rockydirt
//

QVector<QString> g_CellImages = {
    QString(""),
    QString(":/drydirt"),
    QString(":/drymud"),
    QString(":/redclay"),
    QString(":/rocks"),
    QString(":/rockydirt")
};

void CMines::setCellSizeSlider(){
    //preserve current size setting
    //int cellSize = m_vecMineSizes[ui->sldrCellSize->value()];

    QSize fieldSize = ui->oglMinefield->geometry().size();
    int maxCellSize = qMin(fieldSize.width()/2,fieldSize.height()/2);


    m_vecMineSizes.clear();
    qDebug() << "----- Setting up new mine sizes -----";

    // need a list of square mine sizes to use
    for ( int iSize = 32; iSize < maxCellSize; iSize++ )
    {
        if ( (fieldSize.height() % iSize > 19) || (fieldSize.width() % iSize > 19) )
            continue;

        if ( m_vecMineSizes.size() > 0 && iSize < (m_vecMineSizes.constLast() + 8) )
            continue;

        qDebug() << QString("Inserting mine size: %0\n").arg(iSize) << QString("height r: %0, width r: %1").arg(fieldSize.height() % iSize).arg(fieldSize.width() % iSize);
        m_vecMineSizes.append(iSize);
    }

    if ( m_vecMineSizes.size() == 0 )
        m_vecMineSizes.append(40); // set a default size to always have at least one

    ui->sldrCellSize->setMinimum(0);
    ui->sldrCellSize->setMaximum(m_vecMineSizes.size()-1);
    ui->sldrCellSize->setSingleStep(1);
    ui->sldrCellSize->setPageStep(m_vecMineSizes.size()/2);
    ui->sldrCellSize->setValue(0);

    //set the new mine size
    ui->oglMinefield->setCellSize(m_vecMineSizes[0]);
}

void CMines::setMinesSlider(){
    int mineSize = m_vecMineSizes[ui->sldrCellSize->value()];
    int maxMines = ui->oglMinefield->getCellCount(false,mineSize) - 2;

    int sldrMines = ui->sldrMines->value();

    ui->sldrMines->setMinimum(1);
    ui->sldrMines->setMaximum(maxMines);
    ui->sldrMines->setSingleStep(maxMines/8);
    ui->sldrMines->setPageStep(maxMines/4);
    ui->sldrMines->setValue(qMin(maxMines,sldrMines));

    ui->lblMines->setText(QString("Mines: %0").arg(ui->sldrMines->value()));
}

void CMines::setSliderSteps(){
    setCellSizeSlider();
    setMinesSlider();
}

void CMines::resetGame(){
    //repaint
    ui->oglMinefield->update();

    //reset time
    m_timer->stop();
    ui->lblCurrentTime->setText("00:00.00");
    ui->oglMinefield->setExternalTimer(m_timer);
}

CMines::CMines(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMines)
{
    ui->setupUi(this);

    ui->sldrCellSize->connect(ui->sldrCellSize,&QSlider::valueChanged,[this]()
    {
        int mineSize = m_vecMineSizes[ui->sldrCellSize->value()];
        qDebug() << QString("New Mine Size Is: %0 - slider value: %1").arg(mineSize).arg(ui->sldrCellSize->value());
        ui->oglMinefield->setCellSize(mineSize);

        setMinesSlider();

        ui->oglMinefield->generateMinefield(ui->sldrMines->value());

        resetGame();
    });

    ui->sldrMines->connect(ui->sldrMines,&QSlider::valueChanged,[this]()
    {
        ui->oglMinefield->generateMinefield(ui->sldrMines->value());
        ui->lblMines->setText(QString("Mines: %0").arg(ui->sldrMines->value()));

        resetGame();
    });

    ui->btnNormal->connect(ui->btnNormal, &QPushButton::clicked, [this](){
        ui->oglMinefield->setClickType(ClickType::Normal);
    });

    ui->btnFlag->connect(ui->btnFlag, &QPushButton::clicked, [this](){
        ui->oglMinefield->setClickType(ClickType::Flag);
    });

    ui->btnReset->connect(ui->btnReset,&QPushButton::clicked,[this](){
        ui->oglMinefield->setCellSize(ui->sldrCellSize->value());
        ui->oglMinefield->generateMinefield(ui->sldrMines->value());

        resetGame();
    });

    ui->btnChangeCellImage->connect(ui->btnChangeCellImage,&QPushButton::clicked,[this](){
        static int currentImage = 4;

        currentImage++;
        if ( currentImage >= ::g_CellImages.count() ){
            currentImage = 0;
        }

        ui->oglMinefield->setUpDownCellImages(::g_CellImages[currentImage]);
        ui->oglMinefield->update();
        ui->oglMinefield->repaint();
    });

    ui->btnClose->connect(ui->btnClose, &QPushButton::clicked, qApp, &QCoreApplication::quit);

    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setInterval(10);

    connect(m_timer, &QTimer::timeout, [this](){
        qint64 elapsedMs = ui->oglMinefield->getGameTime(); // Get elapsed time from QElapsedTimer

        QTime time(0, 0, 0, 0); // Start from zero
        time = time.addMSecs(elapsedMs); // Convert elapsed time into QTime format

        QString formattedTime = time.toString("mm:ss.zzz");
        formattedTime.truncate(8); // Ensure MM:SS:hh format

        ui->lblCurrentTime->setText(formattedTime);
        ui->lblCurrentTime->update();
    });

    ui->oglMinefield->setExternalTimer(m_timer);

    bInitializing = false;
}

void CMines::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    QTimer::singleShot(100, [this](){
        setSliderSteps();
        ui->oglMinefield->setCellSize(ui->sldrCellSize->value());
        ui->oglMinefield->generateMinefield(ui->sldrMines->value());
        resetGame();
    });
}


CMines::~CMines()
{
    delete ui;
}

