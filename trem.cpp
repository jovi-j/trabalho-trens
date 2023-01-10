#include "trem.h"
#include <QtCore>
#include "mainwindow.h"
#include <iostream>


//Construtor
Trem::Trem(int ID, int x, int y){
    if(ID ==1){
        //inicializa as regiões
        for(int i{0}; i<8; i++){
            regiao[i].release(1);
        }
        for(int i{0}; i<5; i++){
            estados[i] = 0;
        }
        estados[2] = 6;
        //inicializa o mutex
        mutex.release(1);
    }else if(ID == 3){
        regiao[6].acquire(1); // como o trem 3 inicia na região 6, ela inicia trancada
    }

    this->ID = ID;
    this->x = x;
    this->y = y;
    velocidade = 200;
}

void Trem::changeSpeed(int x){
    this->velocidade = x;
}

enum trem{
    TREM1 = 0, TREM2 = 1, TREM3 = 2, TREM4 = 3 , TREM5 = 4
};


void Trem::run(){
    while(true){
        if(velocidade == 200){
            while(velocidade == 200){
                msleep(1);
            }
        }
        switch(ID){
        // ******************************* TREM 1 *******************************************
        case 1:
            if (y == 20 && x <= 460){// trem 1 na aresta de cima
                if(x == 460){
                    y+=10;
                }
                else{
                    if(x == 440){// trem 1 chegando na esquina 1
                        while(estados[TREM1] == 0){
                            mutex.acquire(1);
                            if(estados[TREM2] != 1 && estados[TREM2] != 4 && estados[TREM4] != 3){
                                estados[TREM1] = 1;
                            }
                            mutex.release(1);
                        }
                        regiao[1].acquire(1);
                        x+=10;
                    }else{
                        x+=10;
                    }
                }
            }
            else if(y <= 150 && x == 460){ // trem 1 na região 1 em direção a esquina 6
                if(y == 150){
                    x-=10;
                    regiao[1].release(1);
                }else if(y == 130){//trem 1 chegando na esquina 6
                    while(estados[TREM1] == 1){
                        mutex.acquire(1);
                        if(estados[TREM3] != 3){
                            estados[TREM1] = 3;
                        }
                        mutex.release(1);
                    }
                    regiao[3].acquire(1);
                    y+=10;
                }else{
                    y+=10;
                }
            }
            else if(y == 150 && x >= 190){ // trem 1 na região 3 em direção a esquina 4
                if(x == 310){ // chegou na esquina 5 e soltou a região 3
                    x-=10;
                    regiao[3].release(1);
                }
                else if(x == 190){ // chegou na esquina 4 e soltou a região 2
                    y-=10;
                    regiao[2].release(1);
                    estados[TREM1] = 0;
                }else if(x == 350){ // chegou perto da esquina 5
                    while(estados[TREM1] == 3) { // checa se o trem 3 está na região 2
                        mutex.acquire(1);
                        if(estados[TREM3] != 2){
                            estados[TREM1] = 2;
                        }
                        mutex.release(1);
                    }
                    regiao[2].acquire(1);
                    x-=10;
                }else{ // se não, avança
                    x-=10;
                }
            }else{ // o trem está na aresta esquerda
                y-=10;
            }
            emit updateGUI(ID, x,y);
            break;
            // ******************************* TREM 2 *******************************************
        case 2:
            if(x == 730 && y <= 150){// trem 2 indo em direção a esquina 8
                if(y == 150){ // passou da esquina 8
                    x-=10;
                }else{
                    if(y == 130){// chega perto da esquina 8 e checa se o trem 5 está na região 5
                        while(estados[TREM2] == 0){
                            mutex.acquire(1);
                            if(estados[TREM5] != 5){
                                estados[TREM2] = 5;
                            }
                            mutex.release(1);
                        }
                        regiao[5].acquire(1);
                        y+=10;
                    }
                    else{// se não, avança
                        y+=10;
                    }
                }
            }
            else if(x >= 460 && y == 150){// trem 2 na região 5 indo em direção a esquina 6
                if(x == 460){// chegou a esquina 6
                    y-=10;
                    regiao[4].release(1); // liberou a região 4
                }else if(x == 620){ // se ele chegar perto da esquina 7, checa se o trem 3 está na região 4
                    while(estados[TREM2] == 5){
                        mutex.acquire(1);
                        if(estados[TREM3] != 4){
                            estados[TREM2] = 4;
                        }
                        mutex.release(1);
                    }
                    regiao[4].acquire(1);
                    x-=10;
                }else if(x == 580){// se passou da esquina 7, pode liberar a região 5
                    if(estados[TREM2] == 4){
                        regiao[5].release(1);
                    }
                    x-=10; // e anda
                }else if(x == 480){// se chegou perto da esquina 6, checa se o trem 1 está na região 1
                    while(estados[TREM2] == 4){
                        mutex.acquire(1);
                        if(estados[TREM1] != 1){
                            estados[TREM2] = 1;
                        }
                        mutex.release(1);
                    }
                    regiao[1].acquire(1);
                    x-=10;
                }else{
                    x-=10; // se não estiver em uma esquina, anda
                }
            }else if(x == 460 && y > 20){ // trem 2 indo em direção a esquina 1
                y-=10;// sobe
            }else{// se não for nenhum dos casos acima, o trem está entre a esquina 1 e 2, e anda
                if(x == 480 && y == 20){
                    estados[TREM2] = 0;
                    regiao[1].release(1);
                }
                x+=10;
            }
            emit updateGUI(ID, x,y);
            break;
            // ******************************* TREM 3 *******************************************
        case 3:
            if(x == 330 && y <= 270){ // trem 3 na região 6 indo pra esquina 11
                if(y == 170){
                    regiao[2].release(1);
                }
                if(y == 270){ // chegou na esquina 11
                    x-=10;
                }else{
                    y+=10;
                }
            }else if(x >= 60 && y == 270){// trem 3 indo em direção a esquina 10
                if(x == 310){ // passou da esquina 11, então libera a região 6
                    regiao[6].release(1);
                    mutex.acquire(1);
                    estados[TREM3] = 0;
                    mutex.release(1);
                }
                if(x == 60){
                    y-=10;
                }else{
                    x-=10;
                }
            }
            else if(x == 60 && y >= 150){ //trem 3 indo em direção a esquina 3
                if(y==150){
                    x+=10;
                }else{
                    y-=10;
                }
            }else if(x <= 330 && y == 150){// trem 3 indo em direção a esquina 5
                if(x == 330){ // trem chegou na esquina 5
                    y-=10;
                }
                if(x == 170){// trem 3 chegou perto da esquina 4
                    while(estados[TREM3] == 0){
                        mutex.acquire(1);
                        if(estados[TREM1] != 2 && estados[TREM1] != 3 && estados[TREM4] != 6){
                            estados[TREM3] = 2;
                        }
                        mutex.release(1);
                    }
                    regiao[2].acquire(1);
                    x+=10;
                }else if(x == 310){// trem 3 chegou perto da esquina 5
                    while(estados[TREM3] == 2){
                        mutex.acquire(1);
                        if(estados[TREM4] != 6){
                            estados[TREM3] = 6;
                        }
                        mutex.release(1);
                    }
                    regiao[6].acquire(1);
                    x+=10;
                }else{
                    x+=10;// se não, avança
                }
            }
            emit updateGUI(ID, x,y);
            break;
            // ******************************* TREM 4 *******************************************
        case 4:
            if(x >= 330 && y == 270){ // trem 4 indo em direção a esquina 11
                if(x == 580){
                    if(estados[TREM4] == 7){
                        regiao[7].release(1);
                        estados[TREM4] = 0;
                    }
                    x-=10;
                }
                else if (x == 330){// chegou na esquina 11
                    y-=10;
                }else if(x == 350){// chegou perto da esquina 11
                    while(estados[TREM4] == 0){
                       mutex.acquire(1);
                       if(estados[TREM3] != 6 && estados[TREM3] != 2 && estados[TREM1] != 3){
                            estados[TREM4] = 6;
                       }
                       mutex.release(1);
                    }
                    regiao[6].acquire(1);
                    x-=10;
                }
                else{
                    x-=10;
                }
            }else if(x == 330 && y >=150){// trem 4 em direcao a esquina 5
                if (y == 150){// chegou na esquina 5
                    x+=10;
                }else if(y == 170){// chegou perto da esquina 5
                   while(estados[TREM4] == 6){
                       mutex.acquire(1);
                       if(estados[TREM1] != 3 && estados[TREM1] != 1 && estados[TREM2] != 4){
                            estados[TREM4] = 3;
                       }
                       mutex.release(1);
                   }
                   regiao[3].acquire(1);
                   y-=10;
                }else{
                    y-=10;// se não, avança
                }
            }else if(x <= 600 && y == 150){ // trem 4 indo em direção a esquina 7
                if(x == 350){// passou da esquina 5
                   regiao[6].release(1);// liberou a regiao 6
                   x+=10;
                }
                else if(x == 440){// chegou perto da esquina 6
                    while(estados[TREM4] == 3){
                        mutex.acquire();
                        if(estados[TREM2] != 4 && (estados[TREM5] != 7 && estados[TREM2] != 5)){
                           estados[TREM4] = 4;
                        }
                        mutex.release();
                    }
                    regiao[4].acquire(1);
                    x+=10;
                }else if(x == 480){// passou da esquina 6
                    regiao[3].release();
                    x+=10;
                }
                else if(x == 580){// chegou perto da esquina 7
                    while(estados[TREM4] == 4){
                        mutex.acquire(1);
                        if(estados[TREM5] != 7){
                            estados[TREM4] = 7;
                        }
                        mutex.release(1);
                    }
                    regiao[7].acquire(1);
                    x+=10;
                }
                else if(x == 600){// chegou na esquina 7
                    y+=10;
                }else{
                    x+=10;
                }
            }
            else if(x == 600 && y <= 270){// trem 4 indo em direção a esquina 12
                if(y == 170){// passou da esquina 12
                    regiao[4].release(1); // libera a regiao 4
                    y+=10;
                }
                else if(y == 270){// chegou na esquina 12
                   x-=10;
                }else{
                    y+=10;
                }
            }
            emit updateGUI(ID, x,y);
            break;
            // ******************************* TREM 5 *******************************************
        case 5:
            if(x == 870 && y <= 270){// trem 5 indo em direção a esquina 13
                if(y==270){ // chegou na esquina 13
                    x-=10;
                }else{
                    y+=10;
                }
            }else if(x >= 600 && y == 270){// trem 5 indo em direção a esquina 12
                if(x == 600){// chegou na esquina 12
                    y-=10;
                }else if(x == 620){//chegou perto da esquina 12
                    while(estados[TREM5] == 0){
                        mutex.acquire(1);
                        if(estados[TREM4] != 7 && estados[TREM4] != 4 && estados[TREM2] != 5){
                           estados[TREM5] = 7;
                        }
                        mutex.release(1);
                    }
                    regiao[7].acquire(1);
                    x-=10;
                }else{
                    x-=10; // se não, avança
                }
            }else if(x == 600 && y >= 150){// trem 5 indo em direção a esquina 7
                if(y==150){// chegou na esquina 7
                    x+=10;
                }else if(y == 170){// chegou perto da esquina 7
                   while(estados[TREM5] == 7){// checa se o trem 2 está na região 5
                       mutex.acquire(1);
                       if(estados[TREM2] != 5){
                           estados[TREM5] = 5;
                       }
                       mutex.release(1);
                   }
                   regiao[5].acquire(1);
                   y-=10;
                }else{
                   y-=10;// se não, avança
                }
            }
            else if(x <= 870 && y == 150){// trem 5 indo em direção a esquina 9
                if(x == 620){// se passou da esquina 7, libera a região 7
                    regiao[7].release();
                    x+=10;
                }
                else if(x == 870){ // chegou na esquina 9
                    y-=10;
                }
                else if(x == 750){ // passou da esquina 8
                    mutex.acquire(1);
                    estados[TREM5] = 0;
                    mutex.release(1);
                    regiao[5].release(1);
                    x+=10;
                }else{
                    x+=10;
                }
            }
            emit updateGUI(ID, x,y);
            break;
        default:
            break;
        }
        msleep(velocidade);
    }
}
