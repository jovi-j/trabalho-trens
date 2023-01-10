#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Cria o trem com seu (ID, posição X, posição Y)
    trem1 = new Trem(1,330,20);
    trem2 = new Trem(2,730,80);
    trem3 = new Trem(3,330,210);
    trem4 = new Trem(4,460,270);
    trem5 = new Trem(5,870,210);

    /*
     * Conecta o sinal UPDATEGUI à função UPDATEINTERFACE.
     * Ou seja, sempre que o sinal UPDATEGUI foi chamado, será executada a função UPDATEINTERFACE.
     * Os 3 parâmetros INT do sinal serão utilizados na função.
     * Trem1 e Trem2 são os objetos que podem chamar o sinal. Se um outro objeto chamar o
     * sinal UPDATEGUI, não haverá execução da função UPDATEINTERFACE
     */
    connect(trem1,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem2,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem3,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem4,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));
    connect(trem5,SIGNAL(updateGUI(int,int,int)),SLOT(updateInterface(int,int,int)));

    trem1->start();
    trem2->start();
    trem3->start();
    trem4->start();
    trem5->start();

}

void MainWindow::on_horizontalSlider_1_sliderReleased(){
    trem1->changeSpeed(200 - ui->horizontalSlider_1->value());
}

void MainWindow::on_horizontalSlider_2_sliderReleased(){
    trem2->changeSpeed(200 - ui->horizontalSlider_2->value());
}

void MainWindow::on_horizontalSlider_3_sliderReleased(){
    trem3->changeSpeed(200 - ui->horizontalSlider_3->value());
}

void MainWindow::on_horizontalSlider_4_sliderReleased(){
    trem4->changeSpeed(200 - ui->horizontalSlider_4->value());
}

void MainWindow::on_horizontalSlider_5_sliderReleased(){
    trem5->changeSpeed(200 - ui->horizontalSlider_5->value());
}


void MainWindow::updateInterface(int id, int x, int y){
    switch(id){
    case 1:
        ui->label_trem1->setGeometry(x,y,21,17);
        break;
    case 2:
        ui->label_trem2->setGeometry(x,y,21,17);
        break;
    case 3:
        ui->label_trem3->setGeometry(x,y,21,17);
        break;
    case 4:
        ui->label_trem4->setGeometry(x,y,21,17);
        break;
    case 5:
        ui->label_trem5->setGeometry(x,y,21,17);
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
