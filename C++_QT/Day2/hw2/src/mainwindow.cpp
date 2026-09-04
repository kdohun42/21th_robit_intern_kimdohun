#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushHistory()
{
    history.push_back(ui->lineEdit->text());   // 바뀌기 직전 텍스트를 저장
}

void MainWindow::on_pushButton_clicked()   // +
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "+");
}

void MainWindow::on_pushButton_2_clicked() // -
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "-");
}

void MainWindow::on_pushButton_3_clicked() // =
{
    pushHistory();

    QString expr = ui->lineEdit->text();

    std::vector<int> numbers;
    std::vector<QChar> operators;
    QString numBuffer;

    for (QChar ch : expr) {
        if (ch.isDigit()) {
            numBuffer += ch;
        } else if (ch == '+' || ch == '-') {
            if (!numBuffer.isEmpty()) {
                numbers.push_back(numBuffer.toInt());
                numBuffer.clear();
            }
            operators.push_back(ch);
        }
    }
    if (!numBuffer.isEmpty()) {
        numbers.push_back(numBuffer.toInt());
    }

    if (numbers.empty()) {
        return;
    }

    int result = numbers[0];
    for (size_t i = 0; i < operators.size(); ++i) {
        if (operators[i] == '+') {
            result += numbers[i + 1];
        } else if (operators[i] == '-') {
            result -= numbers[i + 1];
        }
    }

    ui->lineEdit->setText(QString::number(result));
}

void MainWindow::on_pushButton_4_clicked() // Undo
{
    if (history.empty()) {
        return;   // 되돌릴 게 없으면 아무것도 안 함
    }
    QString prev = history.back();
    history.pop_back();
    ui->lineEdit->setText(prev);
}

void MainWindow::on_pushButton_5_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "1");
}

void MainWindow::on_pushButton_6_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "2");
}

void MainWindow::on_pushButton_7_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "3");
}

void MainWindow::on_pushButton_8_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "4");
}

void MainWindow::on_pushButton_9_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "5");
}

void MainWindow::on_pushButton_10_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "6");
}

void MainWindow::on_pushButton_11_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "7");
}

void MainWindow::on_pushButton_12_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "8");
}

void MainWindow::on_pushButton_13_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "9");
}

void MainWindow::on_pushButton_14_clicked()
{
    pushHistory();
    ui->lineEdit->setText(ui->lineEdit->text() + "0");
}

void MainWindow::on_pushButton_15_clicked()
{
    pushHistory();
    ui->lineEdit->clear();
}
