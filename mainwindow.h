#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QDesktopWidget>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QProcess>
#include <QThread>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void startButton_Clicked();

private:
    QPushButton *startButton;
    QWidget *centralWidget;
    QVBoxLayout *layout;

    enum Animal
    {
        Gorilla      = 0, // ゴリラ
        Lion         = 1, // ライオン
        Panda        = 2, // パンダ
        Elephant     = 3, // 象
        Giraffe      = 4, // キリン
        Hippopotamus = 5, // カバ
        Crocodile    = 6, // ワニ
        Rabbit       = 7, // ウサギ
        Special      = 8, // スペシャルマス
    };

    QImage screen_img;
    QRect game_rect;
    Animal board[8][8];

    static const int FRAME = 5;    // 枠の太さ
    static const int PADDING = 10; // 枠からアイコンまでの距離
    static const int MARGIN = 2;   // アイコンの間隔
    static const int ICON = 32;    // 動物アイコンの一辺のサイズ
    static const int wh = 300;     // 枠を含めたゲーム画面の縦(横)サイズ
    static const QString clickprog;

    void analyzeScreen();
    void shotScreen(const int x = 0, const int y = 0, const int w = -1, const int h = -1);
    void analyzeBoard();
    void search();
    bool checkSwapAnimal(const int x1, const int y1, const int x2, const int y2);
    void realSwapAnimal(const int x1, const int y1, const int x2, const int y2);
    void clickSpecialIcon(const int x, const int y);
    void printBoard();
};

#endif // MAINWINDOW_H
