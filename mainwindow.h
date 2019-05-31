#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gif.h"
#include <QMainWindow>
#include <QSpinBox>
#include <QMovie>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSpinBox *spinBox_width, *spinBox_height, *spinBox_delay;
    QMovie *movie;
    QString path, filepath_gif;
    QStringList filepaths;
    GifWriter GW;
    void open(QStringList paths);
    QString BS(long b);

private slots:
    void on_action_open_triggered();
    void on_action_quit_triggered();
    void on_action_about_triggered();
    void on_action_aboutQt_triggered();
    void on_actionMake_triggered();
    void on_actionDelete_triggered();
    void on_actionSort_triggered();

};

#endif // MAINWINDOW_H