#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "temperature.h"
#include "QTimer"

#include <thread>
#include <chrono>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //give serial communication a head start
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //timer to update temperature readings
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::UpdateTemp);
    timer->start(200);

    /*
    //to manually update temperature with buttons
    QPushButton *uButton;
    uButton = MainWindow::findChild<QPushButton *>("updateButton");
    ui->f_temp_display->setText("Waiting...");
    ui->c_temp_display->setText("Waiting...");
    connect(uButton, SIGNAL(released()), this, SLOT(UpdateTemp()));
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateTemp()
{
    Temperature tr;
    //get value as Fahrenheit
    double f = tr.getTemp();
    //convert to Celsius
    double c = (f - 32.0) * 5.0 / 9.0;
    //format output to only one decimal place
    int c_int = int(c * 10.0);
    c = double(c_int) / 10.0;
    ui->f_temp_display->setText(QString::number(f));
    ui->c_temp_display->setText(QString::number(c));
}

