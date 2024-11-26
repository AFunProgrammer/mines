#include "mines.h"
#include "ui_mines.h"

bool bInitializing = true;

CMines::CMines(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMines)
{
    ui->setupUi(this);


    ui->btnExit->connect(ui->btnExit, &QPushButton::clicked, qApp, &QCoreApplication::quit);


    bInitializing = false;
}

void CMines::showEvent(QShowEvent *event) {
    ui->oglMinefield->setCellSize(50);
    ui->oglMinefield->generateMinefield(10);
    //    connect(screen(),&QScreen::orientationChanged,this,&CMines::orientationChanged);

    QWidget::showEvent(event);
}

CMines::~CMines()
{
    delete ui;
}

