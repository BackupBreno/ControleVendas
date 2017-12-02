#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void enableProdutoAlterar();

    void disableProdutoAlterar();

    void on_produto_cadastro_cadastrarButton_clicked();

    void on_produto_consulta_consultarButton_clicked();

    void on_produto_excluit_excluirButton_clicked();

    void on_produto_alterar_consultarButton_clicked();

    void on_produto_alterar_alterarButton_clicked();

    void enableClienteAlterar();

    void disableClienteAlterar();

    void on_cliente_cadastro_cadastrarButton_clicked();

    void on_cliente_consulta_consultarButton_clicked();

    void on_cliente_excluir_excluirButton_clicked();

    void on_cliente_alterar_consultarButton_clicked();

    void on_cliente_alterar_alterarButton_clicked();

    void enableNota();

    void disableNota();

    void on_nota_gerarNotaButton_clicked();

    void on_nota_isVista_clicked();

    void on_nota_isPrazo_clicked();

    void on_nota_produto_consultar_clicked();

    void on_nota_produto_addButton_clicked();

    void on_nota_finalizarButton_clicked();

    void on_check_produtos_abaixo_button_clicked();

    void on_check_entreDatasButton_clicked();

private:
    Ui::MainWindow *ui;

    int numVenda;

    QSqlDatabase db;
    QSqlQueryModel *queryModel;
    QSqlQuery query;

    QString beginTransaction;
    QString endTransaction;
};

#endif // MAINWINDOW_H
