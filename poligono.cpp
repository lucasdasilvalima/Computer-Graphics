#include <qmath.h>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QListWidget>
#include <QDebug>

#include "ponto.h"
#include "poligono.h"

#ifndef SINAL
#define SINAL(x) ((x) < 0 ? (-1) : (1))
#endif
#ifndef ABSOLUTO
#define ABSOLUTO(x) ((x) < 0 ? (-x) : (x))
#endif

Poligono::Poligono(){ }

void Poligono::DDA(QPainter *qp, Janela mundo, Janela vp)
{
    QPen pen(QColor(245, 99, 88), 2, Qt::SolidLine);

    qp->setPen(pen);
    int cont;
    for (cont = 1; cont < pontos.size(); cont++){
        // calculate dx & dy
        int X0 = pontos[cont].xW2Vp(mundo, vp),
                Y0 = pontos[cont].yW2Vp(mundo, vp),
                X1 = pontos[cont-1].xW2Vp(mundo, vp),
                Y1 = pontos[cont-1].yW2Vp(mundo, vp);
        int dx = X1 - X0;
        int dy = Y1 - Y0;

        // calculate steps required for generating pixels
        int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

        // calculate increment in x & y for each steps
        float Xinc = dx / (float) steps;
        float Yinc = dy / (float) steps;

        // Put pixel for each step
        float X = X0;
        float Y = Y0;
        for (int i = 0; i <= steps; i++){
            qp->drawPoint(X, Y); // put pixel at (X,Y)
            X += Xinc;           // increment in x at each step
            Y += Yinc;           // increment in y at each step
        }
    }
}

void Poligono::bresenhams(QPainter *e, Janela mundo, Janela vp)
{
    QPen pen(QColor(33, 141, 166), 2, Qt::SolidLine);
    e->setPen(pen);

    bool interChange;
    int k, deltaX, deltaY, sinalX, sinalY, x, y, erro;
    for (k = 1; k < pontos.size(); k++){
        deltaX = ABSOLUTO ( ( pontos[k].xW2Vp(mundo, vp) - pontos[k-1].xW2Vp(mundo, vp) ) );
        deltaY = ABSOLUTO ( (pontos[k].yW2Vp(mundo, vp) - pontos[k-1].yW2Vp(mundo, vp)) );
        sinalX = SINAL ( (pontos[k].xW2Vp(mundo, vp) - pontos[k-1].xW2Vp(mundo, vp)) );
        sinalY = SINAL ( (pontos[k].yW2Vp(mundo, vp) - pontos[k-1].yW2Vp(mundo, vp)) );
        x = pontos[k-1].xW2Vp(mundo, vp);
        y = pontos[k-1].yW2Vp(mundo, vp);
        if (sinalX < 0)
            x -= 1;
        if(sinalY < 0)
            y -= 1;
        interChange = false;
        if (deltaX < deltaY){
            int tmp = deltaX;
            deltaX = deltaY;
            deltaY = tmp;
            interChange = true;
        }
        erro = 2 * deltaY - deltaX;
        for (int i = 0; i < deltaX; i++){
            e->drawPoint(x, y);
            while(erro >= 0){
                if (interChange)
                    x += sinalX;
                else
                    y += sinalY;
                erro -= 2 * deltaX;
            } // WHILE
            if (interChange)
                y += sinalY;
            else
                x += sinalX;
            erro += 2 * deltaY;
        }//FOR
    }
}

void Poligono::qtDesenha(QPainter *e, Janela mundo, Janela vp)
{
    QPen pen(QColor(186, 242, 232), 2, Qt::SolidLine);
    e->setPen(pen);

    for (int i = 1; i < pontos.size(); i++){
        e->drawLine(pontos[i-1].xW2Vp(mundo, vp), pontos[i-1].yW2Vp(mundo, vp)
                    ,pontos[i].xW2Vp(mundo, vp), pontos[i].yW2Vp(mundo, vp));
    }
}

Poligono Poligono::casteljau(int last_id)
{
    Poligono p;
    p.tipo = "CURVA CASTELJAU";
    p.id = last_id;
    p.tipoDesenho = tipoDesenho;
    p.pontos.push_back(pontos[0]);
    if (pontos.size() >= 3)
        fazerCasteljau(pontos[0], pontos[1], pontos[2], &p);
    p.pontos.push_back(pontos[2]);
    return p;
}

Poligono Poligono::hermit(int last_id)
{
    Poligono p;
    p.id = last_id;
    p.tipo = "CRUVA HERMIT";
    p.tipoDesenho = tipoDesenho;
    p.pontos.push_back(pontos[0]);
    if (pontos.size() >= 4)
        fazerHermit(pontos[0], pontos[1], pontos[2], pontos[3], &p);
    pontos.push_back(pontos[3]);
    return p;
}

Poligono Poligono::bezier(int last_id)
{
    Poligono p;
    p.id = last_id;
    p.tipo = "CRUVA BEZIER";
    p.tipoDesenho = tipoDesenho;
    p.pontos.push_back(pontos[0]);
    if (pontos.size() >= 4)
        //fazerBezier();
    pontos.push_back(pontos[3]);
    return p;
}

QVector<Ponto> matrizCoeficiente(Ponto p1, Ponto p4, Ponto r1, Ponto r4)
{
    QVector<Ponto> pts;
    pts.push_back(Ponto());
    pts[0].setX(2*p1.getX() - 2*p4.getX() + r1.getX() + r4.getX());
    pts[0].setY(2*p1.getY() - 2*p4.getY() + r1.getY() + r4.getY());
    /* C1 */
    pts.push_back(Ponto());
    pts[1].setX(-3*p1.getX() + 3*p4.getX() - 2*r1.getX() - r4.getX());
    pts[1].setY(-3*p1.getY() + 3*p4.getY() - 2*r1.getY() - r4.getY());
    /* C2 */
    pts.push_back(r1);

    /* C3 */
    pts.push_back(p1);

    /* C4 */
    return pts;
}
void Poligono::fazerHermit(Ponto p1, Ponto p2, Ponto p3, Ponto p4, Poligono *p)
{   // P1, P4, R1, R4 -> R1 = P2 - P1 E R4 = P3 - P4
    QVector<Ponto> mc = matrizCoeficiente(p1, p4, p1.distancia(p2), p3.distancia(p4));
    // for para achar os x e y
    for (double t = 0; t <= 1; t += 0.01){
        Ponto a = (mc[0]*(t*t*t) + mc[1]*(t*t) + (mc[2]*t) + mc[3]);
        p->pontos.push_back(a);
    }
}

void dividirCurva(Ponto p1, Ponto p2, Ponto p3, Ponto *e, Ponto *c, Ponto *d)
{
    *e = p1.medio(p2);
    *d = p2.medio(p3);
    *c = e->medio(*d);
}

void Poligono::fazerCasteljau(Ponto p1, Ponto p2, Ponto p3, Poligono *p)
{
    double LIMIAR = 1;
    if (p1.distancia(p2) < LIMIAR)
        p->pontos.push_back(p2);
    else {
        Ponto e1,d1,c;
        dividirCurva(p1, p2, p3, &e1, &c, &d1);
        fazerCasteljau(p1, e1, c, p); // ESQUERDA
        fazerCasteljau(c, d1, p3, p); // DIREITA
    }
}

void Poligono::translada(Ponto p)
{
    for (int i = 0; i < pontos.size(); i++)
        pontos[i].translada(p);
}

void Poligono::escalona(double fator)
{
    for (int i = 0; i < pontos.size(); i++)
        pontos[i].escalona(fator);
}

void Poligono::rotacionar(int tipo, double graus)
{
    double rad = graus * M_PI / 180;
    if (tipo == 1)
        for (int i = 0; i < pontos.size(); i++)
            pontos[i].rotacionar(rad); // AQUI JA FAZEMOS UM CALCULO UNICO PARA TODOS OS PONTOS
    else {
        for (int i = 0; i < pontos.size(); i++)
            pontos[i].rotacionar(centroPoligono() ,rad);
    }
}

void Poligono::desenha(QPainter *e, Janela mundo, Janela vp)
{
    switch (tipoDesenho) {
    case 1:
        DDA(e, mundo, vp);
        break;
    case 2:
        bresenhams(e, mundo, vp);
        break;
    default:
        qtDesenha(e, mundo, vp);
        break;
    }
}

QString Poligono::toString()
{
    return QString::number(id) +
            " - " +
            tipo.toUpper() + " DE " +
            (tipoDesenho == 1 ? "DDA" : tipoDesenho == 2 ? "BRESENHAMS" : "QT-LINE") +
            " COM: " + QString::number(pontos.size()) + " PONTOS.";
}

void Poligono::mostraPol(QListWidget *list)
{

    list->addItem(toString());
}

void Poligono::mostraPon(QListWidget *list)
{
    for (int i = 0; i < pontos.size(); i++)
        pontos[i].mostraPon(list);
}

Ponto Poligono::centroPoligono()
{
    double sx, sy;
    sx = sy = 0;
    for (int i = 0; i < pontos.size(); i++){
        sx += pontos[i].getX();
        sy += pontos[i].getY();
    }

    return Ponto(sx/pontos.size(), sy/pontos.size());
}

void Poligono::adicionaPontosOctantes(double xc, double yc, double x, double y){
  pontos.push_back(Ponto(xc+x, yc+y)); // 1 octante
  pontos.push_back(Ponto(xc+y, yc+x));
  pontos.push_back(Ponto(xc+y, yc-x));
  pontos.push_back(Ponto(xc+x, yc-y));
  pontos.push_back(Ponto(xc-x, yc-y));
  pontos.push_back(Ponto(xc-y, yc-x));
  pontos.push_back(Ponto(xc-y, yc+x));
  pontos.push_back(Ponto(xc-x, yc+y)); // 8 octante "HORARIO!!"
}

void Poligono::gerarPontosCircnferencia(){
  if (pontos.size() >= 2){
    double x1 = pontos[0].getX(), // x e y normal de mundo
        x2 = pontos[1].getX(),
        y1 = pontos[0].getY(),
        y2 = pontos[1].getY(),
        r = sqrt(pow(x2-x1,2) + pow(y2-y1,2));
    double x, y, p;
    x = 0;
    y = r;
    adicionaPontosOctantes(x1, y1, x, y);
    p = 1- r;
    while(x < y){
      if (p < 0)
        x++;
      else{
        x++;
        y--;
      }
      if(p<0)
        p += 2 * x + 1;
      else
        p += 2 * (x-y) + 1;
      adicionaPontosOctantes(x1, y1, x, y);
    }
  }
}

void Poligono::clipping(Janela clipping, QVector<Poligono> *poligonosClip){
    Poligono paux;
    paux.tipoDesenho = tipoDesenho;
    paux.tipo = tipo;
    for (int i = 1; i < pontos.size(); i++){
        cohenClipping(pontos[i-1], pontos[i], clipping, &paux);
        poligonosClip->push_back(paux);
    }
}

void Poligono::cohenClipping(Ponto p1, Ponto p2, Janela clip, Poligono *p)
{
    // Compute region codes for P1, P2
    int code1 = computeCode(p1, clip);
    int code2 = computeCode(p2, clip);

    // Initialize line as outside the rectangular window
    bool accept = false;

    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            // If both endpoints lie within rectangle
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }
        else
        {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            double x, y;

            // At least one endpoint is outside the
            // rectangle, pick it.
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;

            // Find intersection point;
            // using formulas y = p1.getY() + slope * (x - p1.getX()),
            // x = p1.getX() + (1 / slope) * (y - p1.getY())
            if (code_out & TOP)
            {
                // point is above the clip rectangle
                x = p1.getX() + (p2.getX() - p1.getX()) * (clip.getYmax() - p1.getY()) / (p2.getY() - p1.getY());
                y = clip.getYmax();
            }
            else if (code_out & BOTTOM)
            {
                // point is below the rectangle
                x = p1.getX() + (p2.getX() - p1.getX()) * (clip.getYmin() - p1.getY()) / (p2.getY() - p1.getY());
                y = clip.getYmin();
            }
            else if (code_out & RIGHT)
            {
                // point is to the right of rectangle
                y = p1.getY() + (p2.getY() - p1.getY()) * (clip.getXmax() - p1.getX()) / (p2.getX() - p1.getX());
                x = clip.getXmax();
            }
            else if (code_out & LEFT)
            {
                // point is to the left of rectangle
                y = p1.getY() + (p2.getY() - p1.getY()) * (clip.getXmin() - p1.getX()) / (p2.getX() - p1.getX());
                x = clip.getXmin();
            }

            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1)
            {
                p1.setX(x);
                p1.setY(y);
                code1 = computeCode(p1, clip);
            }
            else
            {
                p2.setX(x);
                p2.setY(y);
                code2 = computeCode(p2, clip);
            }
        }
    }
    if (accept)
    {
       p->pontos.push_back(p1);
       p->pontos.push_back(p2);
    }
}

int Poligono::computeCode(Ponto p, Janela clip)
{
    int code = INSIDE;

    if (p.getX() < clip.getXmin())       // to the left of rectangle
        code |= LEFT;
    else if (p.getX() > clip.getXmax())  // to the right of rectangle
        code |= RIGHT;
    if (p.getY() < clip.getYmin())       // below the rectangle
        code |= BOTTOM;
    else if (p.getY() > clip.getYmax())  // above the rectangle
        code |= TOP;

    return code;
}
