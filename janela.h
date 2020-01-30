#ifndef JANELA_H
#define JANELA_H


class Janela
{
public:
    Janela(double, double, double, double);
    double getXmax();
    double getYmax();
    double getXmin();
    double getYmin();
    double xMin;
    double xMax;
    double yMin;
    double yMax;
};

#endif // JANELA_H
