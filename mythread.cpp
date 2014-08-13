#include "mythread.h"

MyThread::MyThread(QObject *parent)
    : QThread(parent)
{
    const int opaque = 200;
    colors[0] = QColor(127, 255, 212, opaque); // aquamarine
    colors[1] = QColor(96, 144, 239, opaque);  // cornflowerblue
    colors[2] = QColor(0, 100, 0, opaque);     // darkgreen
    colors[3] = QColor(255, 140, 0, opaque);   // darkorange
    colors[4] = QColor(143, 188, 143, opaque); // darkseagreen
    colors[5] = QColor(0, 191, 255, opaque);   // deepskyblue
    colors[6] = QColor(255, 215, 0, opaque);   // gold
    colors[7] = QColor(173, 255, 47, opaque);  // greenyellow
    colors[8] = QColor(255, 105, 180, opaque); // hotpink
    colors[9] = QColor(255, 99, 71, opaque);   // tomato
}

void
MyThread::setThreadStop(bool flag) { thread_stop = flag; }

bool
MyThread::getThreadStop() { return thread_stop; }

void
MyThread::run()
{
    forever
    {
        color_i = 0;
        if (thread_stop) break;
        if (analyzeScreen())
        {
            analyzeBoard();
            search();
            emit changeImage(screen);
        }
        QThread::msleep(33);
    }
}

QImage
MyThread::shotScreen(const int x, const int y, const int w, const int h)
{
    return QPixmap::grabWindow(QApplication::desktop()->winId(), x, y, w, h)
            .toImage().convertToFormat(QImage::Format_ARGB32);
}

bool
MyThread::analyzeScreen()
{
    QImage img = shotScreen();

    // ゲーム画面を覆う300px X 300px、 太さ5pxのラインを探す
    const int sw = img.width();
    const int sh = img.height();
    const QRgb *rgb = (QRgb*)img.bits();
    const QRgb tcolor = qRgb(84, 68, 41);
    int count = 0;
    int y = 0, x = 0;

    for (; y < sh; ++y)
    {
        for (; x < sw; ++x)
        {
            if (*(rgb+x) == tcolor)
            {
                count++;
                if (count % wh == 0)
                {
                    if (count == wh * 5)
                    {
                        game_rect = QRect(x-(wh-1)+FRAME, y-4+FRAME,
                                          wh-FRAME*2, wh-FRAME*2);
                        return true;
                    }
                    else
                    {
                        x -= (wh-1);
                        goto Lcont;
                    }
                }
            }
            else
            {
                count = 0;
            }
        }
        x = 0;
Lcont:
        rgb += sw;
    }

    return false;
}

void
MyThread::analyzeBoard()
{
    screen = shotScreen(game_rect.x(), game_rect.y(),
                        game_rect.width(), game_rect.height());

    const QRgb *rgb = (QRgb*)screen.bits();
    const int sw = screen.width();

    QRgb index[8];
    index[Gorilla]      = qRgb(233, 59,  33);
    index[Lion]         = qRgb(238, 156, 5);
    index[Panda]        = qRgb(255, 255, 255);
    index[Elephant]     = qRgb(141, 207, 244);
    index[Giraffe]      = qRgb(206, 149, 5);
    index[Hippopotamus] = qRgb(199, 125, 174);
    index[Crocodile]    = qRgb(57,  125, 5);
    index[Rabbit]       = qRgb(255, 187, 204);

    for (int x = 0; x < BOARDSIZE; ++x)
    {
        for (int y = 0; y < BOARDSIZE; ++y)
        {
            board[x][y] = Special;
        }
    }

    for (int y = 0; y < BOARDSIZE; ++y)
    {
        for (int x = 0; x < BOARDSIZE; ++x)
        {
            const QRgb *t = rgb
                    + (PADDING + y*(ICON+MARGIN))*sw
                    +  PADDING + x*(ICON+MARGIN);
            for (int c = 0; c < 8; ++c)
            {
                int sum = 0;
                const int r = qRed(index[c]);
                const int g = qGreen(index[c]);
                const int b = qBlue(index[c]);
                for (int cy = 0; cy < ICON; ++cy)
                {
                    for (int cx = 0; cx < ICON; ++cx)
                    {
                        const QRgb temp(*(t+cx+cy*sw));
                        if (r == qRed(temp) && g == qGreen(temp) &&
                            abs(b-qBlue(temp)) < 10) ++sum;
                    }
                }
                if (sum > 50)
                {
                    board[x][y] = (Animal)c;
                    break;
                }
            }
        }
    }
}

void
MyThread::search()
{
    for (int x = 0; x < BOARDSIZE-1; ++x)
    {
        for (int y = 0; y < BOARDSIZE-1; ++y)
        {
            if (checkSwapAnimal(x, y, x+1, y)) drawAnswer(x, y, x+1, y);
            if (checkSwapAnimal(x, y, x, y+1)) drawAnswer(x, y, x, y+1);
        }
    }

    // 右端
    for (int x = BOARDSIZE-1, y = 0; y < BOARDSIZE-1; ++y)
    {
        if (checkSwapAnimal(x, y, x, y+1)) drawAnswer(x, y, x, y+1);
    }

    // 下端
    for (int x = 0, y = BOARDSIZE-1; x < BOARDSIZE-1; ++x)
    {
        if (checkSwapAnimal(x, y, x+1, y)) drawAnswer(x, y, x+1, y);
    }
}

bool
MyThread::checkSwapAnimal(const int x1, const int y1, const int x2, const int y2)
{
    Animal temp = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;

    bool result = false;
    for (int x = 0; x < BOARDSIZE-2; ++x)
    {
        if (board[x][y1] == board[x+1][y1] && board[x+1][y1] == board[x+2][y1])
        { result = true; goto Lend; }
    }
    if (y1 != y2)
    {
        for (int x = 0; x < BOARDSIZE-2; ++x)
        {
            if (board[x][y2] == board[x+1][y2] && board[x+1][y2] == board[x+2][y2])
            { result = true; goto Lend; }
        }
    }

    for (int y = 0; y < BOARDSIZE-2; ++y)
    {
        if (board[x1][y] == board[x1][y+1] && board[x1][y+1] == board[x1][y+2])
        { result = true; goto Lend; }
    }
    if (x1 != x2)
    {
        for (int y = 0; y < BOARDSIZE-2; ++y)
        {
            if (board[x2][y] == board[x2][y+1] && board[x2][y+1] == board[x2][y+2])
            { result = true; goto Lend; }
        }
    }

Lend:
    temp = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;

    return result;
}

void
MyThread::drawAnswer(const int x1, const int y1, const int x2, const int y2)
{
    int x_1, y_1, x_2, y_2;

    x_1 = x_2 = y_1 = y_2 = PADDING;

    x_1 += x1*(MARGIN+ICON);
    x_2 = (x1 == x2) ? x_1 : x_2 + x2*(MARGIN+ICON);

    y_1 += y1*(MARGIN+ICON);
    y_2 = (y1 == y2) ? y_1 : y_2 + y2*(MARGIN+ICON);

    QPainter painter(&screen);
    painter.fillRect(x_1, y_1, ICON, ICON, colors[color_i]);
    painter.fillRect(x_2, y_2, ICON, ICON, colors[color_i]);

    color_i++;
    if (color_i == 10) color_i = 0;
}
