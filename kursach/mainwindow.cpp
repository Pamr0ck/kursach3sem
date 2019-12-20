#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arch = new archivator;
    abt = new about;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_DO_clicked()
{
    if(ui->code->isChecked()) code = true;
    if(ui->decode->isChecked()) code = false;
    QMessageBox* msgb = new QMessageBox("Внимание",
                                        "Обьем сообщения может сказать на работе программы. Предлагаю вам сохранить данные сразу в файл",
                                        QMessageBox::Information,
                                        QMessageBox::Yes,
                                        QMessageBox::No,
                                        QMessageBox::Cancel | QMessageBox::Escape);
    int n = msgb->exec();
    delete msgb;
    if (n == QMessageBox::Yes)
    {
      save_without_show();
      return;
    }
//    QMessageBox::warning(this, "Внимание", "Оюъем сообщения может сказать на работе программы. Предлагаю вам сохранить данные сразу в файл");

    if(code){
        ui->output->setPlainText( arch->DoCode(ui->input->toPlainText()));
    }
    else{
        ui->output->setPlainText( arch->DoDeCode(ui->input->toPlainText()));
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog().getOpenFileName();

    if (!fileName.isNull()){

        QFile file(fileName);

        if (file.open(file.ReadOnly)){
            QString data = file.readAll();
            ui->input->setPlainText(data);
        }
        else {
            QMessageBox::warning(this, "Error", "Error to open file");
        }
    }
}

void MainWindow::save_without_show(){
    QString fileName = QFileDialog().getSaveFileName();

    if (!fileName.isNull()){

        QFile file(fileName);

        if (file.open(file.WriteOnly)){
            file.write(arch->DoDeCode(ui->input->toPlainText()).toUtf8());
        }
        else {
            QMessageBox::warning(this, "Error", "Error to save file");
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog().getSaveFileName();

    if (!fileName.isNull()){

        QFile file(fileName);

        if (file.open(file.WriteOnly)){
            file.write(ui->output->toPlainText().toUtf8());
        }
        else {
            QMessageBox::warning(this, "Error", "Error to save file");
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    abt->show();
}
