#include "janela.h"

Janela::Janela(double xMin, double yMin, double xMax, double yMax)
{
    this->xMax = xMax;
    this->xMin = xMin;
    this->yMax = yMax;
    this->yMin = yMin;
}
double Janela::getXmax(){ return this->xMax; }
double Janela::getXmin(){ return this->xMin; }
double Janela::getYmax(){ return this->yMax; }
double Janela::getYmin(){ return this->yMin; }
