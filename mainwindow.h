#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "trem.h"
#include <QSemaphore>
#include <QSlider>
#include <QAbstractSlider>

static QSemaphore regiao[8];// semaforos das regioes criticas
static QSemaphore mutex;// mutex para controlar as mudanças de estado

static std::array<int, 5> estados{0, 0, 6, 0, 0}; // array que guarda o estado de cada trem

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void updateInterface(int,int,int);

private slots:
    void on_horizontalSlider_1_sliderReleased();
    void on_horizontalSlider_2_sliderReleased();
    void on_horizontalSlider_3_sliderReleased();
    void on_horizontalSlider_4_sliderReleased();
    void on_horizontalSlider_5_sliderReleased();

private:
    Ui::MainWindow *ui;
    Trem *trem1;
    Trem *trem2;
    Trem *trem3;
    Trem *trem4;
    Trem *trem5;

};

#endif // MAINWINDOW_H
