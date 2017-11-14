#include "MainWindow.h"
#include "ui_MainWindow.h"

/*
 * TODO:
 *  >> Produtos <<
 * -> Utilizar o throw quando tiver ocorrido erro na transação do SQL.
 * -> Ajustar o feedback no caso de erro.
 *
 *  >> Clientes <<
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QMYSQL");

    queryModel = new QSqlQueryModel(this);

    db.setHostName("localhost");
    db.setDatabaseName("VendasBD");
    db.setUserName("root");
    db.setPassword("1234567890");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Produto
void MainWindow::on_produto_cadastro_cadastrarButton_clicked()
{
    QString descricao = ui->produto_cadastro_descricao->text();

    if (descricao.length() <= 30 && descricao.length() > 0)
    {

        int cod = 10;

        float valCompra =  ui->produto_cadastro_valCompra->value();

        float valVenda = ui->produto_cadastro_valVenda->value();

        int estoque = ui->produto_cadastro_estoque->value();

        int estoqueMin = ui->produto_cadastro_estMinimo->value();

        QString sqlCommand = "INSERT INTO produtos VALUES (";
        sqlCommand += QString().setNum(cod) + ", ";
        sqlCommand += "\"" + descricao + "\", ";
        sqlCommand += QString().setNum(valCompra) + ", ";
        sqlCommand += QString().setNum(valVenda) + ", ";
        sqlCommand += QString().setNum(estoque) + ", ";
        sqlCommand += QString().setNum(estoqueMin) + ")";

        db.open();

        queryModel->setQuery(sqlCommand);

        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

        db.close();
    }
}

void MainWindow::on_produto_consulta_consultarButton_clicked()
{
    int cod = ui->produto_consulta_cod->value();

    QString sqlCommand = "SELECT * FROM produtos WHERE codigo = ";
    sqlCommand += QString().setNum(cod);

    db.open();

    queryModel->setQuery(sqlCommand);

    std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

    db.close();

    queryModel->query().next();

    ui->produto_consulta_descricao->setText(queryModel->query().value(1).toString());
    ui->produto_consulta_valCompra->setText(queryModel->query().value(2).toString());
    ui->produto_consulta_valVenda->setText(queryModel->query().value(3).toString());
    ui->produto_consulta_estoque->setText(queryModel->query().value(4).toString());
    ui->produto_consulta_estMin->setText(queryModel->query().value(5).toString());
}

void MainWindow::on_produto_excluit_excluirButton_clicked()
{
    int cod = ui->produto_excluir_cod->value();

    QString sqlCommand = "DELETE FROM produtos WHERE codigo = ";
    sqlCommand += QString().setNum(cod);

    db.open();

    queryModel->setQuery(sqlCommand);

    std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

    db.close();

    ui->produto_excluir_feedback->setText("Produto excluido.");
}
