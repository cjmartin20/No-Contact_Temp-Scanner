#include "tempwindow.h"
#include "temp_receiver.h"
#include "ui_tempwindow.h"

#include <thread>
#include <chrono>


const char currentTemp[10] = "25.999";

TempWindow::TempWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TempWindow)
{
    ui->setupUi(this);
    temp_receiver tr;
    int j = 0;
    if(1 < 0){//change to successful connection
        ui->Display->setText(("Unable to connect to device."));
    }
    else{
        while(1){
            ui->Display->setText(QString::number(tr.getTemp()));
        }
    }
}

TempWindow::~TempWindow()
{
    delete ui;
}


