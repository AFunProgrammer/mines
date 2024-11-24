#include "mines.h"
#include "ui_mines.h"

bool bInitializing = true;

CMines::CMines(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMines)
{
    ui->setupUi(this);

    ui->oglMinefield->setCellSize(50);
    ui->oglMinefield->generateMinefield(10);
//    connect(screen(),&QScreen::orientationChanged,this,&CMines::orientationChanged);

    bInitializing = false;
}

CMines::~CMines()
{
    delete ui;
}

