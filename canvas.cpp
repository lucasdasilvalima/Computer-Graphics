#include "canvas.h"
#include "dialog.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    mundo = new Janela(-250, -250, 250, 250);
    viewPort= new Janela(0, 0, 500, 500);
    contId = 0;
    clip = new_pol = new_circle = false;

    polAux.id = contId++;
    polAux.tipoDesenho = 0; // vamos usar dda
    polAux.tipo = "ORDENADA"; // eixo do y
    polAux.pontos.push_back(Ponto(0, mundo->getYmin()));
    polAux.pontos.push_back(Ponto(0, mundo->getYmax()));
    poligonos.push_back(polAux);

    polAux.pontos.clear();

    polAux.id = contId++;
    polAux.tipo = "ABSCISSA"; // eixo do x
    polAux.pontos.push_back(Ponto(-250, 0));
    polAux.pontos.push_back(Ponto(mundo->getXmax(), 0));
    poligonos.push_back(polAux);

    polAux.pontos.clear();
}

void Canvas::desenha(QPainter *painter)
{
    if (clip)
        for (int i = 0; i < poligonosClip.size(); i++)
            poligonosClip[i].desenha(painter, *mundo, *viewPort);
    else {
        for (int i = 0; i < poligonos.size(); i++)
            poligonos[i].desenha(painter, *mundo, *viewPort);
    }
}

void Canvas::remove(int index)
{
    if (index != 0 && index != 1)
        poligonos.erase(poligonos.begin()+index);
    update();
}

void Canvas::mostrarListaPoligonos(QListWidget *myLocalListPoligonos)
{

    myLocalListPoligonos->clear();
    for (int i = 2; i < poligonos.size(); i++)
        poligonos[i].mostraPol(myLocalListPoligonos);
}

void Canvas::mostraListaPontos(QListWidget *myListPontos, int index)
{
    myListPontos->clear();
    poligonos[index].mostraPon(myListPontos);
}

void Canvas::casteljau(int index)
{
    poligonos.push_back(poligonos[index].casteljau(poligonos.size()));
    contId++;
    update();
}

void Canvas::hermit(int index)
{
    poligonos.push_back(poligonos[index].hermit(poligonos.size()));
    contId++;
    update();
}

void Canvas::bezier(int index)
{
    poligonos.push_back(poligonos[index].bezier(contId++));
    update();
}

void Canvas::moverEsquerda()
{
    mundo->xMax += 10;
    mundo->xMin += 10;
    poligonos[1].pontos[0].setX(mundo->getXmin());//eixo x
    poligonos[1].pontos[1].setX(mundo->getXmax());//eixo x

    if (clip){
        clippingWindow->xMax += 10;
        clippingWindow->xMin += 10;

        for (int i = 0; i < poligonosClip[0].pontos.size(); i++)
            poligonosClip[0].pontos[i].translada(Ponto(10, 0));
        poligonosClip.erase(poligonosClip.begin()+1, poligonosClip.end());
        for (int i = 0; i < poligonos.size(); i++)
            poligonos[i].clipping(*clippingWindow, &poligonosClip);
    }
    update();
}

void Canvas::moverDireita()
{
    mundo->xMax -= 10;
    mundo->xMin -= 10;
    poligonos[1].pontos[0].setX(mundo->getXmin());//eixo x
    poligonos[1].pontos[1].setX(mundo->getXmax());//eixo x
    if (clip){
        clippingWindow->xMax -= 10;
        clippingWindow->xMin -= 10;

        for (int i = 0; i < poligonosClip[0].pontos.size(); i++)
            poligonosClip[0].pontos[i].translada(Ponto(-10, 0));
        poligonosClip.erase(poligonosClip.begin()+1, poligonosClip.end());
        for (int i = 0; i < poligonos.size(); i++)
            poligonos[i].clipping(*clippingWindow, &poligonosClip);
    }
    update();
}

void Canvas::moverCima()
{
    mundo->yMax -= 10;
    mundo->yMin -= 10;
    poligonos[0].pontos[0].setY(mundo->getYmin());//eixo y
    poligonos[0].pontos[1].setY(mundo->getYmax());//eixo y
    if (clip){
        clippingWindow->yMax -= 10;
        clippingWindow->yMin -= 10;

        for (int i = 0; i < poligonosClip[0].pontos.size(); i++)
            poligonosClip[0].pontos[i].translada(Ponto(0, -10));
        poligonosClip.erase(poligonosClip.begin()+1, poligonosClip.end());
        for (int i = 0; i < poligonos.size(); i++)
            poligonos[i].clipping(*clippingWindow, &poligonosClip);
    }
    update();
}

void Canvas::moverBaixo()
{
    mundo->yMax += 10;
    mundo->yMin += 10;
    poligonos[0].pontos[0].setY(mundo->getYmin());//eixo y
    poligonos[0].pontos[1].setY(mundo->getYmax());//eixo y
    if (clip){
        clippingWindow->yMax += 10;
        clippingWindow->yMin += 10;
        for (int i = 0; i < poligonosClip[0].pontos.size(); i++)
            poligonosClip[0].pontos[i].translada(Ponto(0, 10));

        poligonosClip.erase(poligonosClip.begin()+1, poligonosClip.end());
        for (int i = 0; i < poligonos.size(); i++)
            poligonos[i].clipping(*clippingWindow, &poligonosClip);
    }
    update();
}

void Canvas::zomMais()
{
    mundo->xMax *= 2;
    mundo->xMin *= 2;
    poligonos[1].pontos[0].setX(mundo->getXmin());//eixo x
    poligonos[1].pontos[1].setX(mundo->getXmax());//eixo x
    mundo->yMax *= 2;
    mundo->yMin *= 2;
    poligonos[0].pontos[0].setY(mundo->getYmin());//eixo y
    poligonos[0].pontos[1].setY(mundo->getYmax());//eixo y
    update();
}

void Canvas::zomMenos()
{
    mundo->xMax *= 0.5;
    mundo->xMin *= 0.5;
    poligonos[1].pontos[0].setX(mundo->getXmin());//eixo x
    poligonos[1].pontos[1].setX(mundo->getXmax());//eixo x
    mundo->yMax *= 0.5;
    mundo->yMin *= 0.5;
    poligonos[0].pontos[0].setY(mundo->getYmin());//eixo y
    poligonos[0].pontos[1].setY(mundo->getYmax());//eixo y
    update();
}

void Canvas::translada(int index, Ponto p)
{
    poligonos[index].translada(p);
}

void Canvas::escalona(int index, double fator)
{
    poligonos[index].escalona(fator);
}

void Canvas::rotacionar(int index, int tipo, double graus)
{
    poligonos[index].rotacionar(tipo, graus);
}

double Canvas::xVp2World(int x, Janela mundo, Janela vp)
{
    return ((x-vp.getXmin())/(vp.getXmax()-vp.getXmin()))*(mundo.getXmax()-mundo.getXmin()) + mundo.getXmin();
}

double Canvas::yVp2World(int y, Janela mundo, Janela vp)
{
    return (1-(y-vp.getYmin())/(vp.getYmax()-vp.getYmin()))*(mundo.getYmax()-mundo.getYmin()) + mundo.getYmin();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter qp(this);
    desenha(&qp);
    if (new_pol) // CASO O POLIGONO ESTEJA SENTO DESENHADO E PRECISA SER ATUALIZADO
            polAux.desenha(&qp, *mundo, *viewPort);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->pos();
    double x = xVp2World(p.x(), *mundo, *viewPort),
           y = yVp2World(p.y(), *mundo, *viewPort);
    labelMundo->setText(QString("MUNDO ( ")+
                        QString::number(x)+
                        QString(", ")+
                        QString::number(y)+
                        QString(" )"));
    labelViewPort->setText(QString("VIEW PORT ( ") + QString::number(p.x()) + QString(", ") + QString::number(p.y()) + QString(" )"));
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    Ponto p;
    p.setX(xVp2World(pos.x(), *mundo, *viewPort));
    p.setY(yVp2World(pos.y(), *mundo, *viewPort));

    if (new_pol && !new_circle){ // O CARA QUER FAZER UM NOVO POLIGONO!
        if (event->buttons() == Qt::LeftButton){
            polAux.tipoDesenho = (idAlgoritmo); // ALTERAR PARA A FUNÇÃO QUE PERMITE USARIO ESCOLHER
            polAux.pontos.push_back(Ponto(p.getX(), p.getY()));
            update();// ATUALIZAR ELEMETOS VISUAIS
        }
        else{
            new_pol = false;
            polAux.id = (contId++);
            polAux.tipo = "POLIGONO";
            poligonos.push_back(polAux);
            polAux.pontos.clear();
            update();
            mostrarListaPoligonos(myListPoligonos);
        }
    }
    else if (new_circle && !new_pol) {
        if (event->buttons() == Qt::LeftButton){
            polAux.tipoDesenho = idAlgoritmo;
            polAux.pontos.push_back(Ponto(p.getX(), p.getY()));
        }
        else {
            new_circle = false;
            polAux.id = (contId++);
            polAux.tipo = "CIRCUNFERÊNCIA";
            polAux.gerarPontosCircnferencia();
            poligonos.push_back(polAux);
            polAux.pontos.clear();
            update();
            mostrarListaPoligonos(myListPoligonos);
        }
    }
}


void Canvas::clipping(){
    if (!clip){
        //clippingWindow = new Janela(mundo->getXmin()/4, mundo->getYmin()/4, mundo->getXmax()/4, mundo->getYmax()/4);
        clippingWindow = new Janela(xVp2World(188, *mundo, *viewPort), yVp2World(312, *mundo, *viewPort), xVp2World(312, *mundo, *viewPort), yVp2World(188, *mundo, *viewPort));
        polAux.tipoDesenho = 2;
        polAux.tipo = "janela Clipping";
        if (!poligonosClip.isEmpty())
            poligonosClip.clear();
        polAux.pontos.push_back(Ponto(clippingWindow->getXmin(), clippingWindow->getYmax())); // ponto superior esquerda
        polAux.pontos.push_back(Ponto(clippingWindow->getXmax(), clippingWindow->getYmax())); // ponto superior direita
        polAux.pontos.push_back(Ponto(clippingWindow->getXmax(), clippingWindow->getYmin())); // ponto inferior direita
        polAux.pontos.push_back(Ponto(clippingWindow->getXmin(), clippingWindow->getYmin())); // ponto inferior esquerda
        polAux.pontos.push_back(Ponto(clippingWindow->getXmin(), clippingWindow->getYmax())); // ponto superior esquerda
        poligonosClip.push_back(polAux);
        polAux.pontos.clear();
        for (int i = 0; i < (poligonos.size()); i++)
            poligonos[i].clipping(*clippingWindow, &poligonosClip);
    }
    clip = !clip;
    update();
}
