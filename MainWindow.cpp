#include "MainWindow.h"
#include "ui_MainWindow.h"

/*
 * TODO:
 *  >> Geral <<
 * -> Colocar as operações com transações.
 *
 *  >> Produtos <<
 * -> Utilizar o throw quando tiver ocorrido erro na transação do SQL.
 * -> Ajustar o feedback no caso de erro.
 *
 *  >> Clientes <<
 * -> Utilizar o throw quando tiver ocorrido erro na transação do SQL.
 * -> Ajustar o feedback no caso de erro.
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

    beginTransaction = "START TRANSACTION";
    endTransaction = "COMMIT";
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
        QString sqlCommand = "select cod from codreferencia where entidade = \"produtos\"";
        db.open();
        queryModel->setQuery(sqlCommand);
        db.close();
        queryModel->query().first();
        int cod = queryModel->query().value(0).toInt();

        float valCompra =  ui->produto_cadastro_valCompra->value();

        float valVenda = ui->produto_cadastro_valVenda->value();

        int estoque = ui->produto_cadastro_estoque->value();

        int estoqueMin = ui->produto_cadastro_estMinimo->value();

        db.open();
        queryModel->setQuery(beginTransaction);

        sqlCommand = "INSERT INTO produtos VALUES (";
        sqlCommand += QString().setNum(cod) + ", ";
        sqlCommand += "\"" + descricao + "\", ";
        sqlCommand += QString().setNum(valCompra) + ", ";
        sqlCommand += QString().setNum(valVenda) + ", ";
        sqlCommand += QString().setNum(estoque) + ", ";
        sqlCommand += QString().setNum(estoqueMin) + ")";
        queryModel->setQuery(sqlCommand);

        query = queryModel->query();

        queryModel->setQuery(endTransaction);
        db.close();

        QString error = query.lastError().text();

        std::cout << "Erro: \"" + error.toStdString() + "\"\n";

        if (error == " ")
        {
            ui->produto_cadastro_feedback->setText("Codigo: " + QString().setNum(cod));
        }
        else
        {
            ui->produto_cadastro_feedback->setText("Erro no cadastro.");

            std::cout << error.toStdString() << std::endl;
        }
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

    db.open();
    queryModel->setQuery(beginTransaction);

    QString sqlCommand = "SELECT * FROM produtos WHERE codigo = ";
    sqlCommand += QString().setNum(cod);
    queryModel->setQuery(sqlCommand);

    query = queryModel->query();

    queryModel->setQuery(endTransaction);
    db.close();

    if (!query.next())
    {
        ui->produto_consulta_descricao->setText("Código invalido!");
        ui->produto_consulta_valCompra->setText("Valor Compra");
        ui->produto_consulta_valVenda->setText("Valor Venda.");
        ui->produto_consulta_estoque->setText("Quantidade Estoque");
        ui->produto_consulta_estMin->setText("Estoque Minimo");
    }
    else
    {
        ui->produto_consulta_descricao->setText(query.value(1).toString());
        ui->produto_consulta_valCompra->setText(query.value(2).toString());
        ui->produto_consulta_valVenda->setText(query.value(3).toString());
        ui->produto_consulta_estoque->setText(query.value(4).toString());
        ui->produto_consulta_estMin->setText(query.value(5).toString());
    }
}

void MainWindow::on_produto_excluit_excluirButton_clicked()
{
    int cod = ui->produto_excluir_cod->value();

    db.open();
    queryModel->setQuery(beginTransaction);

    QString sqlCommand = "SELECT * FROM produtos WHERE codigo = ";
    sqlCommand += QString().setNum(cod);
    queryModel->setQuery(sqlCommand);

    query = queryModel->query();

    queryModel->setQuery(endTransaction);
    db.close();

    if (query.next())
    {  
        db.open();
        queryModel->setQuery(beginTransaction);

        sqlCommand = "DELETE FROM produtos WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        query = queryModel->query();

        queryModel->setQuery(endTransaction);
        db.close();

        QString error = query.lastError().text();

        if (error == " ")
        {
            ui->produto_excluir_feedback->setText("Produto excluido.");
        }
        else
        {
            ui->produto_excluir_feedback->setText("Falha ao excluir produto.");

            std::cout << error.toStdString() << std::endl;
        }
    }
    else
    {
        ui->produto_excluir_feedback->setText("Produto não encontrado");
    }
}

void MainWindow::on_produto_alterar_consultarButton_clicked()
{
    std::cout << "P A CS" << std::endl;

    int cod = ui->produto_alterar_cod->value();

    db.open();
    queryModel->setQuery(beginTransaction);

    QString sqlCommand = "SELECT * FROM produtos WHERE codigo = ";
    sqlCommand += QString().setNum(cod);
    queryModel->setQuery(sqlCommand);

    query = queryModel->query();

    queryModel->setQuery(endTransaction);
    db.close();

    if (!query.next())
    {
        ui->produto_alterar_descricao->setText("Código invalido!");
        ui->produto_alterar_valCompra->setValue(0);
        ui->produto_alterar_valVenda->setValue(0);
        ui->produto_alterar_estoque->setValue(0);
        ui->produto_alterar_estMin->setValue(0);
    }
    else
    {
        ui->produto_alterar_descricao->setText(query.value(1).toString());
        ui->produto_alterar_valCompra->setValue(query.value(2).toFloat());
        ui->produto_alterar_valVenda->setValue(query.value(3).toFloat());
        ui->produto_alterar_estoque->setValue(query.value(4).toInt());
        ui->produto_alterar_estMin->setValue(query.value(5).toInt());

        enableProdutoAlterar();
    }
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

        db.open();
        queryModel->setQuery(beginTransaction);

        QString sqlCommand = "UPDATE produtos SET descricao = ";
        sqlCommand += "\"" + descricao + "\"";
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        sqlCommand = "UPDATE produtos SET valorCompra = ";
        sqlCommand += QString().setNum(valCompra);
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        sqlCommand = "UPDATE produtos SET valorVenda = ";
        sqlCommand += QString().setNum(valVenda);
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        sqlCommand = "UPDATE produtos SET quantidadeEst = ";
        sqlCommand += QString().setNum(estoque);
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        sqlCommand = "UPDATE produtos SET estMin = ";
        sqlCommand += QString().setNum(estoqueMin);
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        queryModel->setQuery(endTransaction);
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
    QString sqlCommand = "SELECT cod FROM codreferencia WHERE entidade = \"clientes\"";
    db.open();
    queryModel->setQuery(sqlCommand);
    db.close();
    queryModel->query().first();
    int cod = queryModel->query().value(0).toInt();

    std::cout << "Codigo: " << cod << std::endl;

    QString nome = ui->cliente_cadastro_nome->text();

    QString endereco = ui->cliente_cadastro_endereco->text();

    QString telefone = ui->cliente_cadastro_telefone->text();

    if (nome.length() > 0 && endereco.length() > 0)
    {
        db.open();
        queryModel->setQuery(beginTransaction);

        sqlCommand = "INSERT INTO clientes VALUES (";
        sqlCommand += QString().setNum(cod) + ", ";
        sqlCommand += "\"" + nome + "\", ";
        sqlCommand += "\"" + endereco + "\", ";
        sqlCommand += "\"" + telefone + "\")";
        queryModel->setQuery(sqlCommand);

        query = queryModel->query();

        queryModel->setQuery(endTransaction);
        db.close();

        QString error = query.lastError().text();

        if (error == " ")
        {
            ui->cliente_cadastro_feedback->setText("Codigo: " + QString().setNum(cod));
        }
        else
        {
            ui->cliente_cadastro_feedback->setText("Error no cadastro");
        }
    }
    else
    {
        ui->cliente_cadastro_feedback->setText("Dados invalidos.");
    }
}

void MainWindow::on_cliente_consulta_consultarButton_clicked()
{
    int cod = ui->cliente_consulta_cod->value();

    db.open();
    queryModel->setQuery(beginTransaction);

    QString sqlCommand = "SELECT * FROM clientes WHERE codigo = ";
    sqlCommand += QString().setNum(cod);
    queryModel->setQuery(sqlCommand);

    query = queryModel->query();

    queryModel->setQuery(endTransaction);
    db.close();

    if (!query.next())
    {
        ui->cliente_consulta_nome->setText("Cliente não encontrado");
        ui->cliente_consulta_endereco->setText("Endereco");
        ui->cliente_consulta_telefone->setText("Telefone");
    }
    else
    {
        ui->cliente_consulta_nome->setText(query.value(1).toString());
        ui->cliente_consulta_endereco->setText(query.value(2).toString());
        ui->cliente_consulta_telefone->setText(query.value(3).toString());
    }
}

void MainWindow::on_cliente_excluir_excluirButton_clicked()
{
    int cod = ui->cliente_excluir_cod->value();

    db.open();
    queryModel->setQuery(beginTransaction);

    QString sqlCommand = "SELECT * FROM clientes WHERE codigo = ";
    sqlCommand += QString().setNum(cod);
    queryModel->setQuery(sqlCommand);

    query = queryModel->query();

    queryModel->setQuery(endTransaction);
    db.close();

    if (query.next())
    {
        db.open();
        queryModel->setQuery(beginTransaction);

        sqlCommand = "DELETE FROM clientes WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        query = queryModel->query();

        queryModel->setQuery(endTransaction);
        db.close();

        QString error = query.lastError().text();

        if (error == " ")
        {
            ui->cliente_excluir_feedback->setText("Cliente excluido.");
        }
        else
        {
            ui->cliente_excluir_feedback->setText("Falha ao excluir cliente.");

            std::cout << error.toStdString() << std::endl;
        }
    }
    else
    {
        ui->cliente_excluir_feedback->setText("Cliente não encontrado");
    }
}

void MainWindow::on_cliente_alterar_consultarButton_clicked()
{
    std::cout << "C A CS" << std::endl;

    int cod = ui->cliente_alterar_cod->value();

    db.open();
    queryModel->setQuery(beginTransaction);

    QString sqlCommand = "SELECT * FROM clientes WHERE codigo = ";
    sqlCommand += QString().setNum(cod);
    queryModel->setQuery(sqlCommand);

    query = queryModel->query();

    queryModel->setQuery(endTransaction);
    db.close();

    if (!query.next())
    {
        ui->cliente_alterar_nome->setText("Cliente não encontrado");
        ui->cliente_alterar_endereco->setText("Endereco");
        ui->cliente_alterar_telefone->setText("Telefone");
    }
    else
    {
        ui->cliente_alterar_nome->setText(query.value(1).toString());
        ui->cliente_alterar_endereco->setText(query.value(2).toString());
        ui->cliente_alterar_telefone->setText(query.value(3).toString());

        enableClienteAlterar();
    }
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

        db.open();
        queryModel->setQuery(beginTransaction);

        QString sqlCommand = "UPDATE clientes SET nome = ";
        sqlCommand += "\"" + nome + "\"";
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        sqlCommand = "UPDATE clientes SET endereco = ";
        sqlCommand += "\"" + endereco + "\"";
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        sqlCommand = "UPDATE clientes SET telefone = ";
        sqlCommand += "\"" + telefone + "\"";
        sqlCommand += " WHERE codigo = ";
        sqlCommand += QString().setNum(cod);
        queryModel->setQuery(sqlCommand);

        queryModel->setQuery(endTransaction);
        db.close();

        disableClienteAlterar();
    }
}

// Nota
void MainWindow::enableNota()
{
    ui->nota_cliente_cod->setEnabled(true);
    ui->nota_vencimento->setEnabled(true);
    ui->nota_gerarNotaButton->setEnabled(true);

    ui->nota_produto_cod->setEnabled(false);
    ui->nota_produto_consultar->setEnabled(false);
    ui->nota_finalizarButton->setEnabled(false);
}

void MainWindow::disableNota()
{
    ui->nota_cliente_cod->setEnabled(false);
    ui->nota_vencimento->setEnabled(false);
    ui->nota_gerarNotaButton->setEnabled(false);

    ui->nota_produto_cod->setEnabled(true);
    ui->nota_produto_consultar->setEnabled(true);
    ui->nota_finalizarButton->setEnabled(true);
}

void MainWindow::on_nota_gerarNotaButton_clicked()
{
    bool isPrazo = ui->nota_isPrazo->isChecked();

    if (isPrazo)
    {
        QString sqlCommand = "select cod from codreferencia where entidade = \"vendas\"";
        db.open();
        queryModel->setQuery(sqlCommand);
        db.close();
        queryModel->query().first();
        int cod = queryModel->query().value(0).toInt();

        int codCliente = ui->nota_cliente_cod->value();

        QDate vencimento = ui->nota_vencimento->date();

        QString vencimentoString = QString().setNum(vencimento.year()) + "-" + QString().setNum(vencimento.month()) + "-" + QString().setNum(vencimento.day());

        QDate venda = QDate::currentDate();

        QString vendaString = QString().setNum(venda.year()) + "-" + QString().setNum(venda.month()) + "-" + QString().setNum(venda.day());

        QString isPrazoStirng = (isPrazo ? "true" : "false");

        db.open();
        queryModel->setQuery(beginTransaction);

        sqlCommand = "INSERT INTO vendas VALUES (";
        sqlCommand += QString().setNum(cod) + ", ";
        sqlCommand += "\"" + vendaString + "\", ";
        sqlCommand += "\"" + vencimentoString + "\", ";
        sqlCommand += isPrazoStirng + ", ";
        sqlCommand += QString().setNum(codCliente) + ")";
        queryModel->setQuery(sqlCommand);

        query = queryModel->query();

        queryModel->setQuery(endTransaction);
        db.close();

        numVenda = cod;

        QString error = query.lastError().text();

        if (error == " ")
        {
            ui->nota_feedback->setText("Codigo: " + QString().setNum(cod));

            disableNota();
        }
        else
        {
            ui->nota_feedback->setText("Error na geração");

            std::cout << error.toStdString() << std::endl;
        }
    }
    else if (!isPrazo)
    {
        QString sqlCommand = "select cod from codreferencia where entidade = \"vendas\"";
        db.open();
        queryModel->setQuery(sqlCommand);
        db.close();
        queryModel->query().first();
        int cod = queryModel->query().value(0).toInt();

        int codCliente = ui->nota_cliente_cod->value();

        QDate data = QDate::currentDate();

        QString dataString = QString().setNum(data.year()) + "-" + QString().setNum(data.month()) + "-" + QString().setNum(data.day());

        QString isPrazoString = (isPrazo ? "true" : "false");

        db.open();
        queryModel->setQuery(beginTransaction);

        sqlCommand = "INSERT INTO vendas VALUES (";
        sqlCommand += QString().setNum(cod) + ", ";
        sqlCommand += "\"" + dataString + "\", ";
        sqlCommand += "\"" + dataString + "\", ";
        sqlCommand += isPrazoString + ", ";
        sqlCommand += QString().setNum(codCliente) + ")";
        queryModel->setQuery(sqlCommand);

        query = queryModel->query();

        queryModel->setQuery(endTransaction);
        db.close();

        numVenda = cod;

        QString error = query.lastError().text();

        if (error == " ")
        {
            ui->nota_feedback->setText("Codigo: " + QString().setNum(cod));

            disableNota();
        }
        else
        {
            ui->nota_feedback->setText("Error na geração");

            std::cout << error.toStdString() << std::endl;
        }
    }
}

void MainWindow::on_nota_isVista_clicked()
{
    ui->nota_cliente_cod->setValue(-1);
    ui->nota_cliente_cod->setEnabled(false);

    ui->nota_vencimento->setDate(QDate::currentDate());
    ui->nota_vencimento->setEnabled(false);
}

void MainWindow::on_nota_isPrazo_clicked()
{
    ui->nota_cliente_cod->setValue(0);
    ui->nota_cliente_cod->setEnabled(true);

    ui->nota_vencimento->setDate(QDate(2000, 1, 1));
    ui->nota_vencimento->setEnabled(true);
}

void MainWindow::on_nota_produto_consultar_clicked()
{
    if (ui->nota_produto_consultar->text() == "Consultar")
    {
        int codProduto = ui->nota_produto_cod->value();

        db.open();
        queryModel->setQuery(beginTransaction);

        QString sqlCommand = "SELECT * FROM produtos WHERE codigo = ";
        sqlCommand += QString().setNum(codProduto);
        queryModel->setQuery(sqlCommand);

        query = queryModel->query();

        queryModel->setQuery(endTransaction);
        db.close();

        if (!query.next())
        {
            ui->nota_produto_descricao->setText("Codigo invalido");
        }
        else
        {
            ui->nota_produto_descricao->setText(query.value(1).toString());

            ui->nota_produto_cod->setEnabled(false);
            ui->nota_produto_cod->setEnabled(false);

            ui->nota_produto_qtd->setEnabled(true);
            ui->nota_produto_valPago->setEnabled(true);
            ui->nota_produto_addButton->setEnabled(true);

            ui->nota_produto_consultar->setText("Outro Produto");
        }
    }
    else
    {
        ui->nota_produto_cod->setEnabled(true);
        ui->nota_produto_cod->setEnabled(true);

        ui->nota_produto_qtd->setEnabled(false);
        ui->nota_produto_valPago->setEnabled(false);
        ui->nota_produto_addButton->setEnabled(false);

        ui->nota_produto_consultar->setText("Descricao");

        ui->nota_produto_consultar->setText("Consultar");
    }
}

void MainWindow::on_nota_produto_addButton_clicked()
{
    int codProduto = ui->nota_produto_cod->value();

    int qtdDesejada = ui->nota_produto_qtd->value();

    float valPago = ui->nota_produto_valPago->value();

    db.open();
    queryModel->setQuery(beginTransaction);

    QString sqlCommand = "INSERT INTO itensvendidos VALUES (";
    sqlCommand += QString().setNum(numVenda) + ", ";
    sqlCommand += QString().setNum(codProduto) + ", ";
    sqlCommand += QString().setNum(qtdDesejada) + ", ";
    sqlCommand += QString().setNum(valPago) + ")";
    queryModel->setQuery(sqlCommand);

    query = queryModel->query();

    queryModel->setQuery(endTransaction);
    db.close();

    QString error = query.lastError().text();

    if (error == " ")
    {
        ui->nota_produto_cod->setValue(0);

        ui->nota_produto_cod->setEnabled(true);

        ui->nota_produto_consultar->setEnabled(true);

        ui->nota_produto_consultar->setText("Consultar");

        ui->nota_produto_descricao->setText("Descricao");

        ui->nota_produto_qtd->setValue(0);

        ui->nota_produto_addButton->setEnabled(false);

        ui->nota_produto_valPago->setEnabled(false);

        ui->nota_produto_feedback->setText("Produto adicionado.");
    }
    else
    {
        ui->nota_produto_feedback->setText("Falha ao adicionar o produto");

        std::cout << error.toStdString() << std::endl;
    }
}

void MainWindow::on_nota_finalizarButton_clicked()
{
    ui->nota_produto_feedback->setText("Nota finalizada.");

    ui->nota_isPrazo->setChecked(false);
    ui->nota_isVista->setChecked(false);

    ui->nota_cliente_cod->setEnabled(true);
    ui->nota_cliente_cod->setValue(0);

    ui->nota_vencimento->setEnabled(true);
    ui->nota_vencimento->setDate(QDate(2000, 1, 1));

    ui->nota_gerarNotaButton->setEnabled(true);

    ui->nota_feedback->setText("");

    ui->nota_produto_cod->setEnabled(false);
    ui->nota_produto_cod->setValue(0);

    ui->nota_produto_consultar->setEnabled(false);

    ui->nota_produto_descricao->setText("Descrição.");

    ui->nota_produto_qtd->setEnabled(false);
    ui->nota_produto_qtd->setValue(0);

    ui->nota_produto_valPago->setEnabled(false);
    ui->nota_produto_valPago->setValue(0);

    ui->nota_produto_addButton->setEnabled(false);

    ui->nota_finalizarButton->setEnabled(false);
}

// Check Geral
void MainWindow::on_check_produtos_abaixo_button_clicked()
{
    db.open();
    queryModel->setQuery(beginTransaction);

    QString sqlCommand = "SELECT * FROM produtos WHERE quantidadeEst < estMin";
    queryModel->setQuery(sqlCommand);

    query = queryModel->query();

    queryModel->setQuery(endTransaction);
    db.close();

    ui->check_list_produtos_abaixo->clear();

    while(query.next())
    {
        QString produto = "Código = " + query.value(0).toString() + " ~ Descrição: " + query.value(1).toString();

        ui->check_list_produtos_abaixo->addItem(produto);
    }
}

void MainWindow::on_check_entreDatasButton_clicked()
{
    QDate final = ui->check_data_final->date();
    QDate inicio = ui->check_data_inicio->date();

    if (inicio > final)
    {
        ui->check_intervalo_feedback->setText("Intervalo invalido.");
    }
    else
    {
        ui->check_intervalo_feedback->setText("");

        QString finalString = QString().setNum(final.year()) + "-" + QString().setNum(final.month()) + "-" + QString().setNum(final.day());
        QString inicioString = QString().setNum(inicio.year()) + "-" + QString().setNum(inicio.month()) + "-" + QString().setNum(inicio.day());

        db.open();
        queryModel->setQuery(beginTransaction);

        QString sqlCommand = "SELECT * FROM vendas WHERE dataVenda <= ";
        sqlCommand += "\"" + finalString + "\"";
        sqlCommand += " AND dataVenda >= ";
        sqlCommand += "\"" + inicioString + "\"";
        queryModel->setQuery(sqlCommand);

        query = queryModel->query();

        queryModel->setQuery(endTransaction);
        db.close();

        ui->check_list_intervalo->clear();

        while(query.next())
        {
            QString venda = "Número = " + query.value(0).toString();

            ui->check_list_intervalo->addItem(venda);
        }
    }
}
