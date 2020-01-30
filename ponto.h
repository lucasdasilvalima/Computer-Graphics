#ifndef PONTO_H
#define PONTO_H
#include <QString>
#include <QListWidget>

#include "janela.h"

class Ponto
{
public:
    Ponto(double pX = 0, double pY = 0);
    int xW2Vp(Janela mundo , Janela vp);
    int yW2Vp(Janela mundo , Janela vp);
    QString toString();
    void setX(double x);
    void setY(double y);
    double getX();
    double getY();
    void mostraPon(QListWidget *);

    void translada(Ponto);
    void escalona(double);
    void rotacionar(double);
    void rotacionar(Ponto, double);
    double distancia(Ponto);
    Ponto medio(Ponto);

    Ponto operator +(Ponto p);
    Ponto operator *(double n);
private:
     double x;
     double y;
};

#endif // PONTO_H
