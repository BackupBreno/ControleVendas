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
void MainWindow::enableProdutoAlterar()
{
    ui->produto_alterar_cod->setEnabled(false);
    ui->produto_alterar_consultarButton->setEnabled(false);

    ui->produto_alterar_descricao->setEnabled(true);
    ui->produto_alterar_valCompra->setEnabled(true);
    ui->produto_alterar_valVenda->setEnabled(true);
    ui->produto_alterar_estoque->setEnabled(true);
    ui->produto_alterar_estMin->setEnabled(true);
    ui->produto_alterar_alterarButton->setEnabled(true);
}

void MainWindow::disableProdutoAlterar()
{
    ui->produto_alterar_cod->setEnabled(true);
    ui->produto_alterar_consultarButton->setEnabled(true);

    ui->produto_alterar_descricao->setEnabled(false);
    ui->produto_alterar_valCompra->setEnabled(false);
    ui->produto_alterar_valVenda->setEnabled(false);
    ui->produto_alterar_estoque->setEnabled(false);
    ui->produto_alterar_estMin->setEnabled(false);
    ui->produto_alterar_alterarButton->setEnabled(false);

    ui->produto_alterar_descricao->setText("");
    ui->produto_alterar_valCompra->setValue(0);
    ui->produto_alterar_valVenda->setValue(0);
    ui->produto_alterar_estoque->setValue(0);
    ui->produto_alterar_estMin->setValue(0);
}

void MainWindow::on_produto_cadastro_cadastrarButton_clicked()
{
    std::cout << "P C" << std::endl;

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

        ui->produto_cadastro_feedback->setText("Codigo: " + QString().setNum(cod));
    }
    else
    {
        ui->produto_cadastro_feedback->setText("Descrição invalida.");
    }
}

void MainWindow::on_produto_consulta_consultarButton_clicked()
{
    std::cout << "P CS" << std::endl;

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
    std::cout << "P X" << std::endl;

    int cod = ui->produto_excluir_cod->value();

    QString sqlCommand = "DELETE FROM produtos WHERE codigo = ";
    sqlCommand += QString().setNum(cod);

    db.open();

    queryModel->setQuery(sqlCommand);

    std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

    db.close();

    ui->produto_excluir_feedback->setText("Produto excluido.");
}

void MainWindow::on_produto_alterar_consultarButton_clicked()
{
    std::cout << "P A CS" << std::endl;

    int cod = ui->produto_alterar_cod->value();

    QString sqlCommand = "SELECT * FROM produtos WHERE codigo = ";
    sqlCommand += QString().setNum(cod);

    db.open();

    queryModel->setQuery(sqlCommand);

    std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

    db.close();

    queryModel->query().next();

    ui->produto_alterar_descricao->setText(queryModel->query().value(1).toString());
    ui->produto_alterar_valCompra->setValue(queryModel->query().value(2).toFloat());
    ui->produto_alterar_valVenda->setValue(queryModel->query().value(3).toFloat());
    ui->produto_alterar_estoque->setValue(queryModel->query().value(4).toInt());
    ui->produto_alterar_estMin->setValue(queryModel->query().value(5).toInt());

    enableProdutoAlterar();
}

void MainWindow::on_produto_alterar_alterarButton_clicked()
{

    std::cout << "P A A" << std::endl;

    QString descricao = ui->produto_alterar_descricao->text();

    if (descricao.length() <= 30 && descricao.length() > 0)
    {
        int cod = ui->produto_alterar_cod->value();

        float valCompra = ui->produto_alterar_valCompra->value();

        float valVenda = ui->produto_alterar_valVenda->value();

        int estoque = ui->produto_alterar_estoque->value();

        int estoqueMin = ui->produto_alterar_estMin->value();

        QString sqlCommand = "UPDATE produtos SET descricao = ";
        sqlCommand += "\"" + descricao + "\"";
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);

        db.open();
        queryModel->setQuery(sqlCommand);
        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;
        db.close();

        sqlCommand = "UPDATE produtos SET valorCompra = ";
        sqlCommand += QString().setNum(valCompra);
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);

        db.open();
        queryModel->setQuery(sqlCommand);
        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;
        db.close();

        sqlCommand = "UPDATE produtos SET valorVenda = ";
        sqlCommand += QString().setNum(valVenda);
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);

        db.open();
        queryModel->setQuery(sqlCommand);
        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;
        db.close();

        sqlCommand = "UPDATE produtos SET quantidadeEst = ";
        sqlCommand += QString().setNum(estoque);
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);

        db.open();
        queryModel->setQuery(sqlCommand);
        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;
        db.close();

        sqlCommand = "UPDATE produtos SET estMin = ";
        sqlCommand += QString().setNum(estoqueMin);
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);

        db.open();
        queryModel->setQuery(sqlCommand);
        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;
        db.close();

        disableProdutoAlterar();
    }
}

// Cliente
void MainWindow::enableClienteAlterar()
{
    ui->cliente_alterar_cod->setEnabled(false);
    ui->cliente_alterar_consultarButton->setEnabled(false);

    ui->cliente_alterar_nome->setEnabled(true);
    ui->cliente_alterar_endereco->setEnabled(true);
    ui->cliente_alterar_telefone->setEnabled(true);
    ui->cliente_alterar_alterarButton->setEnabled(true);
}

void MainWindow::disableClienteAlterar()
{
    ui->cliente_alterar_cod->setEnabled(true);
    ui->cliente_alterar_consultarButton->setEnabled(true);

    ui->cliente_alterar_nome->setEnabled(false);
    ui->cliente_alterar_endereco->setEnabled(false);
    ui->cliente_alterar_telefone->setEnabled(false);
    ui->cliente_alterar_alterarButton->setEnabled(false);

    ui->cliente_alterar_nome->setText("");
    ui->cliente_alterar_endereco->setText("");
    ui->cliente_alterar_telefone->setText("");
}

void MainWindow::on_cliente_cadastro_cadastrarButton_clicked()
{
    std::cout << "C C" << std::endl;

    int cod = 10;

    QString nome = ui->cliente_cadastro_nome->text();

    QString endereco = ui->cliente_cadastro_endereco->text();

    QString telefone = ui->cliente_cadastro_telefone->text();

    if (nome.length() > 0 && endereco.length() > 0)
    {
        QString sqlCommand = "INSERT INTO clientes VALUES (";
        sqlCommand += QString().setNum(cod) + ", ";
        sqlCommand += "\"" + nome + "\", ";
        sqlCommand += "\"" + endereco + "\", ";;
        sqlCommand += "\"" + telefone + "\")";

        db.open();

        queryModel->setQuery(sqlCommand);

        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

        db.close();

        ui->cliente_cadastro_feedback->setText("Codigo: " + QString().setNum(cod));
    }
    else
    {
        ui->cliente_cadastro_feedback->setText("Dados invalidos.");
    }
}

void MainWindow::on_cliente_consulta_consultarButton_clicked()
{
    std::cout << "C CS" << std::endl;

    int cod = ui->cliente_consulta_cod->value();

    QString sqlCommand = "SELECT * FROM clientes WHERE codigo = ";
    sqlCommand += QString().setNum(cod);

    db.open();

    queryModel->setQuery(sqlCommand);

    std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

    db.close();

    queryModel->query().next();

    ui->cliente_consulta_nome->setText(queryModel->query().value(1).toString());
    ui->cliente_consulta_endereco->setText(queryModel->query().value(2).toString());
    ui->cliente_consulta_telefone->setText(queryModel->query().value(3).toString());
}

void MainWindow::on_cliente_excluir_excluirButton_clicked()
{
    std::cout << "C X" << std::endl;

    int cod = ui->cliente_excluir_cod->value();

    QString sqlCommand = "DELETE FROM clientes WHERE codigo = ";
    sqlCommand += QString().setNum(cod);

    db.open();

    queryModel->setQuery(sqlCommand);

    std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

    db.close();

    ui->cliente_excluir_feedback->setText("Cliente excluido.");
}

void MainWindow::on_cliente_alterar_consultarButton_clicked()
{
    std::cout << "C A CS" << std::endl;

    int cod = ui->cliente_alterar_cod->value();

    QString sqlCommand = "SELECT * FROM clientes WHERE codigo = ";
    sqlCommand += QString().setNum(cod);

    db.open();

    queryModel->setQuery(sqlCommand);

    std::cout << queryModel->query().lastError().text().toStdString() << std::endl;

    db.close();

    queryModel->query().next();

    ui->cliente_alterar_nome->setText(queryModel->query().value(1).toString());
    ui->cliente_alterar_endereco->setText(queryModel->query().value(2).toString());
    ui->cliente_alterar_telefone->setText(queryModel->query().value(3).toString());

    enableClienteAlterar();
}

void MainWindow::on_cliente_alterar_alterarButton_clicked()
{
    std::cout << "C A A" << std::endl;

    QString nome = ui->cliente_alterar_nome->text();

    QString endereco = ui->cliente_alterar_endereco->text();

    QString telefone = ui->cliente_alterar_telefone->text();

    if (nome.length() > 0 && endereco.length() > 0)
    {
        int cod = ui->cliente_alterar_cod->value();

        QString sqlCommand = "UPDATE clientes SET nome = ";
        sqlCommand += "\"" + nome + "\"";
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);

        db.open();
        queryModel->setQuery(sqlCommand);
        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;
        db.close();

        sqlCommand = "UPDATE clientes SET endereco = ";
        sqlCommand += "\"" + endereco + "\"";
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);

        db.open();
        queryModel->setQuery(sqlCommand);
        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;
        db.close();

        sqlCommand = "UPDATE clientes SET telefone = ";
        sqlCommand += "\"" + telefone + "\"";
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);

        db.open();
        queryModel->setQuery(sqlCommand);
        std::cout << queryModel->query().lastError().text().toStdString() << std::endl;
        db.close();

        disableClienteAlterar();
    }
}
