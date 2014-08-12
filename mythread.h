#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QApplication>
#include <QDesktopWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);

    void setThreadStop(bool flag);
    bool getThreadStop();

signals:
    void changeImage(QImage img);

protected:
    virtual void run();

private:
    static const int FRAME = 5;     // 枠の太さ
    static const int PADDING = 10;  // 枠からアイコンまでの距離
    static const int MARGIN = 2;    // アイコンの間隔
    static const int ICON = 32;     // 動物アイコンの一辺のサイズ
    static const int wh = 300;      // 枠を含めたゲーム画面の縦(横)サイズ
    static const int BOARDSIZE = 8; // ゲーム版のサイズ

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

    QImage screen;
    QRect game_rect;
    Animal board[BOARDSIZE][BOARDSIZE];
    QColor colors[10];
    int color_i;
    bool thread_stop;

    QImage shotScreen(const int x = 0, const int y = 0, const int w = -1, const int h = -1);
    void analyzeScreen();
    void analyzeBoard();
    void search();
    bool checkSwapAnimal(const int x1, const int y1, const int x2, const int y2);
    void drawAnswer(const int x1, const int y1, const int x2, const int y2);
};

#endif // MYTHREAD_H
