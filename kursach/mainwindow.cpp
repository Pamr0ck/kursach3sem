#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arch = new archivator;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_DO_clicked()
{
    if(ui->code->isChecked()) code = true;
    if(ui->decode->isChecked()) code = false;
    if(code){
        ui->output->setPlainText( arch->DoCode(ui->input->toPlainText()));
    }
}
