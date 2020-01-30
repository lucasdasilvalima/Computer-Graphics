#ifndef MYDISPLAY_H
#define MYDISPLAY_H

#include <QWidget>
#include <QtGui>
#include <QtCore>
#include <QLabel>

#include "poligono.h"
#include "janela.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    bool new_pol;
    bool new_circle;
    bool clip;
    int idAlgoritmo;

    QLabel *labelMundo;
    QLabel *labelViewPort;

    QVector<Poligono> poligonos;
    QVector<Poligono> poligonosClip;

    QListWidget *myListPoligonos;

    void clipping();

    void desenha(QPainter *painter);
    void remove(int index);
    void mostrarListaPoligonos(QListWidget *myListPoligono);
    void mostraListaPontos(QListWidget *myListPontos, int);

    void casteljau(int);
    void hermit(int);
    void bezier(int);

    void moverEsquerda();
    void moverDireita();
    void moverCima();
    void moverBaixo();
    void zomMais();
    void zomMenos();

    void translada(int, Ponto); // passe por paramentros o index do poligono cordenadas em x e y para mover
    void escalona(int, double); // passe por paramentros o index do poligono fator em escala para alterar
    void rotacionar(int, int, double); // passe por paramentros o index do poligono e qual tipo de rotação 0 para normal 1 para homogenia valor do angulo

    double xVp2World(int, Janela, Janela);
    double yVp2World(int, Janela, Janela);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    int contId;
    Poligono polAux;
    Janela *mundo;
    Janela *viewPort;
    Janela *clippingWindow;
};

#endif // MYDISPLAY_H
