#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include "mythread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void menu_operation_start_Clicked();
    void menu_operation_stop_Clicked();
    void changeImage(QImage img);

private:
    QMenu *operation;
    QAction *operation_start;
    QAction *operation_stop;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapitem;

    MyThread thread;
};

#endif // MAINWINDOW_H
