#ifndef POLIGONO_H
#define POLIGONO_H
#include <QVector>
#include <QMainWindow>
#include <QString>
#include <QListWidget>

#include "janela.h"
#include "ponto.h"

class Poligono
{
public:
    Poligono();

    // Defining region codes
    static const int INSIDE = 0; // 0000
    static const int LEFT = 1;   // 0001
    static const int RIGHT = 2;  // 0010
    static const int BOTTOM = 4; // 0100
    static const int TOP = 8;    // 1000

    int id;
    int tipoDesenho;

    QString tipo;
    QVector<Ponto> pontos;

    void DDA(QPainter *, Janela, Janela);
    void bresenhams(QPainter *, Janela, Janela);
    void qtDesenha(QPainter *, Janela, Janela);

    Poligono casteljau(int);
    Poligono hermit(int);
    Poligono bezier(int);

    void fazerHermit(Ponto, Ponto, Ponto, Ponto, Poligono*);
    void fazerCasteljau(Ponto,Ponto,Ponto,Poligono *);
    void gerarPontosCircnferencia();
    void adicionaPontosOctantes(double , double, double, double);

    void clipping(Janela, QVector<Poligono> *);
    void cohenClipping(Ponto, Ponto, Janela, Poligono *);
    int computeCode(Ponto, Janela);

    void translada(Ponto);
    void escalona(double);
    void rotacionar(int, double);

    void desenha(QPainter *, Janela , Janela);
    void mostraPol(QListWidget *);
    void mostraPon(QListWidget *);

    Ponto centroPoligono();
    QString toString();
};

#endif // POLIGONO_H
