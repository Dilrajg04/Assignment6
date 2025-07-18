#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addBtn_clicked();
    void on_findBtn_clicked();
    void on_removeBtn_clicked();
    void on_clearBtn_clicked();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
