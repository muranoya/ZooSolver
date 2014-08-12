#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , operation(new QMenu(tr("操作"), this))
    , operation_start(new QAction(tr("スタート"), this))
    , operation_stop(new QAction(tr("ストップ"), this))
    , view(new QGraphicsView(this))
    , scene(new QGraphicsScene())
    , pixmapitem(new QGraphicsPixmapItem())
{
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle(tr("ZooSolver"));
    setFixedSize(300, 300);

    setCentralWidget(view);
    view->setScene(scene);
    scene->addItem(pixmapitem);

    operation_stop->setEnabled(false);

    operation->addAction(operation_start);
    operation->addAction(operation_stop);
    menuBar()->addMenu(operation);

    connect(operation_start, SIGNAL(triggered()), this, SLOT(menu_operation_start_Clicked()));
    connect(operation_stop, SIGNAL(triggered()), this, SLOT(menu_operation_stop_Clicked()));
    connect(&thread, SIGNAL(changeImage(QImage)), this, SLOT(changeImage(QImage)), Qt::BlockingQueuedConnection);
}

MainWindow::~MainWindow()
{
    delete pixmapitem;
    delete scene;
    delete view;
    delete operation_stop;
    delete operation_start;
    delete operation;
}

void
MainWindow::menu_operation_start_Clicked()
{
    operation_start->setEnabled(false);
    operation_stop->setEnabled(true);

    thread.setThreadStop(false);
    thread.start();
}

void
MainWindow::menu_operation_stop_Clicked()
{
    operation_start->setEnabled(true);
    operation_stop->setEnabled(false);

    thread.setThreadStop(true);
    pixmapitem->setPixmap(QPixmap());
}

void
MainWindow::changeImage(QImage img)
{
    if (thread.getThreadStop())
    {
        pixmapitem->setPixmap(QPixmap());
    }
    else
    {
        pixmapitem->setPixmap(QPixmap::fromImage(img));
    }
}
