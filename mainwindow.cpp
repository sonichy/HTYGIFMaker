#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    path = "";
    QLabel *label = new QLabel("宽");
    ui->mainToolBar->addWidget(label);
    spinBox_width = new QSpinBox;
    spinBox_width->setRange(10,1000);
    ui->mainToolBar->addWidget(spinBox_width);
    label = new QLabel("高");
    ui->mainToolBar->addWidget(label);
    spinBox_height = new QSpinBox;
    spinBox_height->setRange(10,1000);
    ui->mainToolBar->addWidget(spinBox_height);
    label = new QLabel("延时");
    ui->mainToolBar->addWidget(label);
    spinBox_delay = new QSpinBox;
    spinBox_delay->setValue(50);
    spinBox_delay->setRange(10,1000);
    ui->mainToolBar->addWidget(spinBox_delay);
    label = new QLabel("ms");
    ui->mainToolBar->addWidget(label);
    movie = new QMovie;
    ui->label->setMovie(movie);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open(QStringList filepaths)
{
    filepaths.sort();
    QPixmap pixmap(filepaths.at(0));
    spinBox_width->setValue(pixmap.width());
    spinBox_height->setValue(pixmap.height());
    for(int i=0; i<filepaths.count(); i++){
        QString fp = filepaths.at(i);
        QListWidgetItem *LWI = new QListWidgetItem(QIcon(fp), QFileInfo(fp).fileName());
        LWI->setToolTip(fp);
        ui->listWidget->insertItem(ui->listWidget->count() + 1, LWI);
    }
    ui->statusBar->showMessage("共 "+ QString::number(ui->listWidget->count()) + " 帧");
}

void MainWindow::on_action_open_triggered()
{
    if (path == "") path = ".";
    filepaths = QFileDialog::getOpenFileNames(this, "打开图片", path, "图片(*.jpg *.jpeg *.png *.bmp)");
    if(filepaths.count() != 0){
        path = filepaths.at(0);
        open(filepaths);
        movie->stop();
    }
}

void MainWindow::on_action_quit_triggered()
{
    qApp->quit();
}

void MainWindow::on_action_aboutQt_triggered()
{
    QMessageBox::aboutQt(NULL, "关于 Qt");
}

void MainWindow::on_action_about_triggered()
{
    QMessageBox aboutMB(QMessageBox::NoIcon, "关于", "海天鹰GIF生成器 1.0\n一款基于 Qt + gif.h 的GIF生成器。\n作者：黄颖\nE-mail: sonichy@163.com\n主页：http://github.com/sonichy\nGIF合成库：https://github.com/ginsweater/gif-h");
    aboutMB.setIconPixmap(QPixmap(":/icon.png"));
    aboutMB.exec();
}

void MainWindow::on_actionMake_triggered()
{
    if(ui->listWidget->count() > 0){
        ui->mainToolBar->setEnabled(false);
        QDateTime time = QDateTime::currentDateTime();
        QString filename_gif = time.toString("yyyyMMddhhmmss") + ".gif";
        QString filepath_gif = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + filename_gif;
        GifBegin(&GW, filepath_gif.toUtf8().data(), spinBox_width->value(), spinBox_height->value(), spinBox_delay->value());
        for(int i=0; i<ui->listWidget->count(); i++){
            ui->statusBar->showMessage("写入第 " + QString::number(i+1) + " 帧......");
            QPixmap pixmap(ui->listWidget->item(i)->toolTip());
            GifWriteFrame(&GW, pixmap.toImage().bits(), spinBox_width->value(), spinBox_height->value(), spinBox_delay->value());
        }
        GifEnd(&GW);
        movie->setFileName(filepath_gif);
        int w = ui->label->width();
        int h = ui->label->width() * spinBox_height->value() / spinBox_width->value();
        movie->setScaledSize(QSize(w, h));
        qDebug() << w << h;
        movie->start();
        ui->mainToolBar->setEnabled(true);
        ui->statusBar->showMessage("写入完成，文件大小 "+ BS(QFileInfo(filepath_gif).size()));
    }
}

void MainWindow::on_actionDelete_triggered()
{
    QList<QListWidgetItem*> list = ui->listWidget->selectedItems();
    for(int i=0; i<list.count(); i++){
        ui->listWidget->removeItemWidget(list.at(i));
        delete list.at(i);
    }
}

void MainWindow::on_actionSort_triggered()
{
    if(ui->actionSort->isChecked()){
        ui->listWidget->setViewMode(QListView::ListMode);
    }else{
        ui->listWidget->setViewMode(QListView::IconMode);
    }
}

QString MainWindow::BS(long b)
{
    QString s = "";
    if(b > 999999999){
        s = QString::number(b/(1024*1024*1024.0), 'f', 2) + "GB";
    }else{
        if(b > 999999){
            s = QString::number(b/(1024*1024.0), 'f', 2) + "MB";
        }else{
            if(b > 999){
                s = QString::number(b/(1024.0), 'f',2) + "KB";
            }else{
                s = QString::number(b) + "B";
            }
        }
    }
    return s;
}