#include <QString>
#include <QListWidget>
#include <qmath.h>

#include "ponto.h"
#include "janela.h"

Ponto::Ponto(double pX, double pY)
{
    this->x = pX;
    this->y = pY;
}

QString Ponto::toString(){ return "X: " + QString::number(x) + ",\t Y: " + QString::number(y) + '\t'; }

void Ponto::mostraPon(QListWidget *list)
{
    list->addItem(toString());
}

void Ponto::translada(Ponto p)
{
    x += p.getX();
    y += p.getY();
}

void Ponto::escalona(double fator)
{
    x *= fator;
    y *= fator;
}

void Ponto::rotacionar(Ponto p, double rad)
{
    double ax = x;
    x = (x - p.getX())*cos(rad) - (y-p.getY())*sin(rad) + p.getX();
    y = (ax - p.getX())*sin(rad) + (y-p.getY())*cos(rad) + p.getY();
}

double Ponto::distancia(Ponto p)
{
    return sqrt((p.getX() - x)*(p.getX() - x) + (p.getY() - y)*(p.getY() - y));
}

Ponto Ponto::medio(Ponto p)
{
    Ponto r;
    r.x = (x+(p.x - x)/2);
    r.y = (y+(p.y - y)/2);
    return r;
}

Ponto Ponto::operator +(Ponto p)
{
    return Ponto(p.getX()+x, p.getY()+y);
}

Ponto Ponto::operator *(double n)
{
    return Ponto(n*x, n*y);
}

void Ponto::rotacionar(double rad)
{
    double ax = x;
    x = x*cos(rad) - y*sin(rad); // OPERAÇÕES PRECISAM DE AJUSTES
    y = ax*sin(rad) + y*cos(rad);
}

int Ponto::xW2Vp(Janela mundo, Janela vp)
{
    return ( (x-mundo.getXmin())/(mundo.getXmax()-mundo.getXmin()))*(vp.getXmax()-vp.getXmin());
}

int Ponto::yW2Vp(Janela mundo, Janela vp)
{
    return (1-(y-mundo.getYmin())/(mundo.getYmax()-mundo.getYmin()))*(vp.getYmax()-vp.getYmin());
}
void Ponto::setX(double x){ this->x = x; }
void Ponto::setY(double y){ this->y = y; }
double Ponto::getX(){ return this->x; }
double Ponto::getY(){ return this->y; }
