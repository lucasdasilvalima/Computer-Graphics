#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    display = new Canvas(this);
    display->resize(500, 500);

    display->labelMundo = ui->lblMundo; // ALTERAR
    display->labelViewPort = ui->lblViewPort; // alterar
    display->myListPoligonos = ui->listPoligonos;

    setMouseTracking(true);
    ui->graphicsView->setMouseTracking(true);
    display->setMouseTracking(true);

    ui->graphicsView->setScene(scene);

    display->move(2, 2); /* ESTOU MOVENDO O MEU DISPLAY PARA A POSICAO 2, 2 (EM RELAÇÃO AO 'PARENT' DO OBJETO). OU SEJA DO QDIALOG WINDOW
                          * APENAS PARA DEIXAR O DISPLAY NA MESMA POSIÇÃO DA MINHA SCENE
                          * ==========================================================================
                          * display->resize(500, 500);
                          */
    scene->addWidget(display);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnPoligono_clicked()
{
    display->new_pol = true;
    display->idAlgoritmo = algoritmo_selecionado();
}

void Dialog::on_btnCirculo_clicked()
{
    display->new_circle = true;
    display->idAlgoritmo = algoritmo_selecionado();
}

void Dialog::on_listPoligonos_clicked(const QModelIndex &index)
{
    display->mostraListaPontos(ui->listPontos, index.row()+2); // A LISTA DE POLIGONOS TEM DOIS ANTERIORES AO PRIMEIRO QUE É DESENHADO
}

int Dialog::algoritmo_selecionado()
{ // Para: DDA == 1; Bresiham == 2; Qt == 3.
    return ui->rdbtQt->isChecked() ? 3 : ui->rdbtDda->isChecked() ? 1 : 2;
}

int Dialog::tipoRotacao()
{
    return ui->rdbtHomo->isChecked() ? 2 : 1;
}

void Dialog::on_btnExcluir_clicked()
{
    QString s;
    if (!ui->listPoligonos->selectedItems().isEmpty()){
        try {
            s = ui->listPoligonos->selectedItems()[0]->text();
            display->remove(s.split(' ')[0].toInt()); // COMO TENHO ACESSSO AO TEXTO SEI QUE O PRIMEIRTO ELEMENTO DEPOIS DE UM ' '(ESPAÇO) É O NUMERO DE ID
        } catch (QException e) {
            qDebug() << "NÃO FOI POSSIVEL PEGAR O TEXTO NA POSIÇÃO 0";
        }
    }
    display->mostrarListaPoligonos(ui->listPoligonos);
}

void Dialog::on_btnTranslada_clicked()
{
    QString s;
    int i = -1;
    Ponto p(ui->spbTx->value(), ui->spbTy->value());
    if (!ui->listPoligonos->selectedItems().isEmpty()){
        try {
            s = ui->listPoligonos->selectedItems()[0]->text();
            i = s.split(' ')[0].toInt();
        } catch (QException e) {
            qDebug() << "NÃO FOI POSSIVEL PEGAR O TEXTO NA POSIÇÃO 0";
        }
        display->translada(i, p); // COMO TENHO ACESSSO AO TEXTO SEI QUE O PRIMEIRTO ELEMENTO DEPOIS DE UM ' '(ESPAÇO) É O NUMERO DE ID
    }
    display->mostraListaPontos(ui->listPontos, i);
    display->update();
}

void Dialog::on_btnEscalonar_clicked()
{
    QString s;
    int i = -1;
    double fator = ui->spbEsc->value();
    if (!ui->listPoligonos->selectedItems().isEmpty()){
        try {
            s = ui->listPoligonos->selectedItems()[0]->text();
            i = s.split(' ')[0].toInt();
        } catch (QException e) {
            qDebug() << "NÃO FOI POSSIVEL PEGAR O TEXTO NA POSIÇÃO 0";
        }
        display->escalona(i, fator); // COMO TENHO ACESSSO AO TEXTO SEI QUE O PRIMEIRTO ELEMENTO DEPOIS DE UM ' '(ESPAÇO) É O NUMERO DE ID
    }
    display->mostraListaPontos(ui->listPontos, i);
    display->update();
}

void Dialog::on_btnRotaciona_clicked()
{
    QString s;
    int i = -1;
    double angulo = ui->spbR->value();
    if (!ui->listPoligonos->selectedItems().isEmpty()){
        try {
            s = ui->listPoligonos->selectedItems()[0]->text();
            i = s.split(' ')[0].toInt();
        } catch (QException e) {
            qDebug() << "NÃO FOI POSSIVEL PEGAR O TEXTO NA POSIÇÃO 0";
        }
        display->rotacionar(i, tipoRotacao(), angulo); // COMO TENHO ACESSSO AO TEXTO SEI QUE O PRIMEIRTO ELEMENTO DEPOIS DE UM ' '(ESPAÇO) É O NUMERO DE ID
    }

    display->mostraListaPontos(ui->listPontos, i);
    display->update();
}

void Dialog::on_btnClipping_clicked()
{
    display->clipping();
}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == 65) // LETRA A
            display->moverEsquerda();

    if (event->key() == 87) // LETRA W
            display->moverCima();

    if (event->key() == 68) //LETRA D
            display->moverDireita();

    if (event->key() == 83) // LETRA S
            display->moverBaixo();
}

void Dialog::on_zoomMenos_clicked()
{
    display->zomMais();
}

void Dialog::on_zomMais_clicked()
{
    display->zomMenos();
}

void Dialog::on_btnCasteljau_clicked()
{
    QString s;
    int i = -1;
    if (!ui->listPoligonos->selectedItems().isEmpty()){
        try {
            s = ui->listPoligonos->selectedItems()[0]->text();
            i = s.split(' ')[0].toInt();
        } catch (QException e) {
            qDebug() << "NÃO FOI POSSIVEL PEGAR O TEXTO NA POSIÇÃO 0";
        }
        display->casteljau(i); // COMO TENHO ACESSSO AO TEXTO SEI QUE O PRIMEIRTO ELEMENTO DEPOIS DE UM ' '(ESPAÇO) É O NUMERO DE ID
    }
    display->mostrarListaPoligonos(ui->listPoligonos);
    display->update();
    qDebug() << display->poligonos.size();
}

void Dialog::on_btnHermit_clicked()
{
    QString s;
    int i = -1;
    if (!ui->listPoligonos->selectedItems().isEmpty()){
        try {
            s = ui->listPoligonos->selectedItems()[0]->text();
            i = s.split(' ')[0].toInt();
        } catch (QException e) {
            qDebug() << "NÃO FOI POSSIVEL PEGAR O TEXTO NA POSIÇÃO 0";
        }
        display->hermit(i); // COMO TENHO ACESSSO AO TEXTO SEI QUE O PRIMEIRTO ELEMENTO DEPOIS DE UM ' '(ESPAÇO) É O NUMERO DE ID

    }

    display->mostrarListaPoligonos(ui->listPoligonos);
    display->update();
}

void Dialog::on_btnBezier_clicked()
{
    QString s;
    int i = -1;
    if (!ui->listPoligonos->selectedItems().isEmpty()){
        try {
            s = ui->listPoligonos->selectedItems()[0]->text();
            i = s.split(' ')[0].toInt();// COMO TENHO ACESSSO AO TEXTO SEI QUE O PRIMEIRTO ELEMENTO DEPOIS DE UM ' '(ESPAÇO) É O NUMERO DE ID
        } catch (QException e) {
            qDebug() << "NÃO FOI POSSIVEL PEGAR O TEXTO NA POSIÇÃO 0";
        }
        display->bezier(i);
    }

    display->mostrarListaPoligonos(ui->listPoligonos);
    display->update();
}
