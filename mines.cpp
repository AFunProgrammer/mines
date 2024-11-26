#include "mines.h"
#include "ui_mines.h"

bool bInitializing = true;

void CMines::setMineSliderSteps(){
    int maxBombs = ui->oglMinefield->getCellCount(false,ui->sldrCellSize->value()) / 2;
    int setBombs = qMin(maxBombs,ui->sldrMines->value());

    ui->sldrMines->setMaximum(maxBombs);
    ui->sldrMines->setSingleStep(maxBombs/10);
    ui->sldrMines->setPageStep(maxBombs/4);

    ui->sldrMines->setValue(setBombs);
}


CMines::CMines(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMines)
{
    ui->setupUi(this);

    ui->btnReset->connect(ui->btnReset,&QPushButton::clicked,[this](){
        ui->oglMinefield->setCellSize(ui->sldrCellSize->value());
        ui->oglMinefield->generateMinefield(ui->sldrMines->value());
        //repaint
        ui->oglMinefield->update();
        ui->oglMinefield->repaint();
    });

    ui->sldrCellSize->connect(ui->sldrCellSize,&QSlider::valueChanged,[this](int value)
    {
        ui->oglMinefield->setCellSize(ui->sldrCellSize->value());

        setMineSliderSteps();

        ui->oglMinefield->generateMinefield(ui->sldrMines->value());
        //repaint
        ui->oglMinefield->update();
        ui->oglMinefield->repaint();

    });

    ui->sldrMines->connect(ui->sldrMines,&QSlider::valueChanged,[this](int value)
    {
        ui->oglMinefield->generateMinefield(ui->sldrMines->value());
        //repaint
        ui->oglMinefield->update();
        ui->oglMinefield->repaint();
    });


    ui->btnExit->connect(ui->btnExit, &QPushButton::clicked, qApp, &QCoreApplication::quit);


    bInitializing = false;
}

void CMines::showEvent(QShowEvent *event) {
    setMineSliderSteps();

    ui->oglMinefield->setCellSize(ui->sldrCellSize->value());
    ui->oglMinefield->generateMinefield(ui->sldrMines->value());
    //    connect(screen(),&QScreen::orientationChanged,this,&CMines::orientationChanged);

    QWidget::showEvent(event);
}

CMines::~CMines()
{
    delete ui;
}

