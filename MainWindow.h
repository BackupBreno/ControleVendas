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

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;
    QSqlQueryModel *queryModel;
};

#endif // MAINWINDOW_H
