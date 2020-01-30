#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include <QGraphicsScene>

#include "canvas.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    int algoritmo_selecionado();
    int tipoRotacao();
private slots:

    void on_btnPoligono_clicked();

    void on_btnCirculo_clicked();

    void on_listPoligonos_clicked(const QModelIndex &index);

    void on_btnExcluir_clicked();

    void on_btnTranslada_clicked();

    void on_btnEscalonar_clicked();

    void on_btnRotaciona_clicked();

    void on_btnClipping_clicked();

    void on_zoomMenos_clicked();

    void on_zomMais_clicked();

    void on_btnCasteljau_clicked();

    void on_btnHermit_clicked();

    void on_btnBezier_clicked();

protected:
    virtual void keyPressEvent(QKeyEvent *) override;

private:
    Ui::Dialog *ui;
    QGraphicsScene *scene;
    Canvas *display;

};

#endif // DIALOG_H
