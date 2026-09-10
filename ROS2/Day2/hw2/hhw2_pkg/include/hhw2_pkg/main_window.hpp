#ifndef hhw2_pkg_MAIN_WINDOW_HPP_
#define hhw2_pkg_MAIN_WINDOW_HPP_

#include <QMainWindow>
#include <QCloseEvent>

#include "qnode.hpp"


namespace Ui
{
class MainWindowDesign;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindowDesign* ui;

    QNode* qnode;

    int selectedWidth;

    int selectedR;
    int selectedG;
    int selectedB;
};

#endif
