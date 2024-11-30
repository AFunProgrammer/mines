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
    QSize fieldSize = ui->oglMinefield->geometry().size();
    int maxCellSize = qMin(fieldSize.width()/3,fieldSize.height()/3);

    int sldrCellSize = ui->sldrCellSize->value();

    ui->sldrCellSize->setMinimum(40);
    ui->sldrCellSize->setMaximum(maxCellSize);
    ui->sldrCellSize->setSingleStep(maxCellSize/6);
    ui->sldrCellSize->setPageStep(maxCellSize/3);
    ui->sldrCellSize->setValue(sldrCellSize);
}

void CMines::setMinesSlider(){
    int maxMines = ui->oglMinefield->getCellCount(false,ui->sldrCellSize->value()) - 2;

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


CMines::CMines(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMines)
{
    ui->setupUi(this);

    ui->sldrCellSize->connect(ui->sldrCellSize,&QSlider::valueChanged,[this]()
    {
        ui->oglMinefield->setCellSize(ui->sldrCellSize->value());

        setMinesSlider();

        ui->oglMinefield->generateMinefield(ui->sldrMines->value());
        //repaint
        ui->oglMinefield->update();
        ui->oglMinefield->repaint();

    });

    ui->sldrMines->connect(ui->sldrMines,&QSlider::valueChanged,[this]()
    {
        ui->oglMinefield->generateMinefield(ui->sldrMines->value());
        ui->lblMines->setText(QString("Mines: %0").arg(ui->sldrMines->value()));

        //repaint
        ui->oglMinefield->update();
        ui->oglMinefield->repaint();
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
        //repaint
        ui->oglMinefield->update();
        ui->oglMinefield->repaint();
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

    ui->btnExit->connect(ui->btnExit, &QPushButton::clicked, qApp, &QCoreApplication::quit);


    bInitializing = false;
}

void CMines::showEvent(QShowEvent *event) {
    setSliderSteps();

    ui->oglMinefield->setCellSize(ui->sldrCellSize->value());
    ui->oglMinefield->generateMinefield(ui->sldrMines->value());
    //    connect(screen(),&QScreen::orientationChanged,this,&CMines::orientationChanged);

    QWidget::showEvent(event);
}

void CMines::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    setSliderSteps();

    ui->oglMinefield->setCellSize(ui->sldrCellSize->value());
    ui->oglMinefield->generateMinefield(ui->sldrMines->value());
}



CMines::~CMines()
{
    delete ui;
}

