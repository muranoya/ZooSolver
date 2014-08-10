#include "mainwindow.h"

const QString MainWindow::clickprog("/usr/local/bin/cliclick");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , startButton(new QPushButton(tr("スタート")))
    , centralWidget(new QWidget())
    , layout(new QVBoxLayout())
{
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
    layout->addWidget(startButton);

    connect(startButton, SIGNAL(clicked()), this, SLOT(startButton_Clicked()));

    setFixedSize(120, 50);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

MainWindow::~MainWindow()
{
    delete startButton;
    delete layout;
    delete centralWidget;
}

void
MainWindow::startButton_Clicked()
{
    startButton->setEnabled(false);

    analyzeScreen();
    for (int i = 0; i < 100; ++i)
    {
        analyzeBoard();
        search();
        startButton->setText(tr("@:%1").arg(i));
        QThread::msleep(100);
    }

    startButton->setText("スタート");
    startButton->setEnabled(true);
}

void
MainWindow::analyzeScreen()
{
    shotScreen();

    // ゲーム画面を覆う300px X 300px、 太さ5pxのラインを探す
    const int sw = screen_img.width();
    const int sh = screen_img.height();
    const QRgb *rgb = (QRgb*)screen_img.bits();
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

                if (count % 300 == 0)
                {
                    if (count == 300 * 5)
                    {
                        game_rect = QRect(x-299, y-4, 300, 300);
                        return;
                    }
                    else
                    {
                        x -= 299;
                        goto Lcont;;
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
}

void
MainWindow::shotScreen(const int x, const int y, const int w, const int h)
{
    screen_img = QPixmap::grabWindow(QApplication::desktop()->winId(), x, y, w, h).toImage()
            .convertToFormat(QImage::Format_RGB32);
}

void
MainWindow::analyzeBoard()
{
    shotScreen(game_rect.x(), game_rect.y(), game_rect.width(), game_rect.height());

    const QRgb *rgb = (QRgb*)screen_img.bits();

    QRgb index[8];
    index[Gorilla]      = qRgb(233, 59,  33);
    index[Lion]         = qRgb(238, 156, 5);
    index[Panda]        = qRgb(255, 255, 255);
    index[Elephant]     = qRgb(141, 207, 244);
    index[Giraffe]      = qRgb(206, 149, 5);
    index[Hippopotamus] = qRgb(199, 125, 174);
    index[Crocodile]    = qRgb(57,  125, 5);
    index[Rabbit]       = qRgb(255, 187, 204);

    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            board[x][y] = Special;
        }
    }

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            const QRgb *t = rgb
                    + (FRAME+PADDING + y*ICON + y*MARGIN)*wh
                    +  FRAME+PADDING + x*ICON + x*MARGIN;
            for (int c = 0; c < 8; ++c)
            {
                int sum = 0;
                const QRgb test = index[c];
                for (int cy = 0; cy < 32; ++cy)
                {
                    for (int cx = 0; cx < 32; ++cx)
                    {
                        if (qRed(test) == qRed(*(t+cx+cy*wh)) &&
                            qGreen(test) == qGreen(*(t+cx+cy*wh)) &&
                            abs(qBlue(test)-qBlue(*(t+cx+cy*wh))) < 10 )
                        {
                            ++sum;
                        }
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
MainWindow::search()
{
    for (int x = 0; x < 8; ++x)
    {
        for (int y = 0; y < 8; ++y)
        {
            if (board[x][y] == Special)
            {
                clickSpecialIcon(x, y);
                return;
            }
        }
    }

    for (int x = 0; x < 7; ++x)
    {
        for (int y = 0; y < 7; ++y)
        {
            if (checkSwapAnimal(x, y, x+1, y))
            { realSwapAnimal(x, y, x+1, y); return; }
            if (checkSwapAnimal(x, y, x, y+1))
            { realSwapAnimal(x, y, x, y+1); return; }
        }
    }

    // 右端
    for (int x = 7, y = 0; y < 7; ++y)
    {
        if (checkSwapAnimal(x, y, x, y+1))
        { realSwapAnimal(x, y, x, y+1); return; }
    }

    // 下端
    for (int x = 0, y = 7; x < 7; ++x)
    {
        if (checkSwapAnimal(x, y, x+1, y))
        { realSwapAnimal(x, y, x+1, y); return; }
    }
}

bool
MainWindow::checkSwapAnimal(const int x1, const int y1, const int x2, const int y2)
{
    Animal temp = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;

    bool result = false;
    for (int x = 0; x < 6; ++x)
    {
        if (board[x][y1] == board[x+1][y1] && board[x+1][y1] == board[x+2][y1])
        { result = true; goto Lend; }
    }
    if (y1 != y2)
    {
        for (int x = 0; x < 6; ++x)
        {
            if (board[x][y2] == board[x+1][y2] && board[x+1][y2] == board[x+2][y2])
            { result = true; goto Lend; }
        }
    }

    for (int y = 0; y < 6; ++y)
    {
        if (board[x1][y] == board[x1][y+1] && board[x1][y+1] == board[x1][y+2])
        { result = true; goto Lend; }
    }
    if (x1 != x2)
    {
        for (int y = 0; y < 6; ++y)
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
MainWindow::realSwapAnimal(const int x1, const int y1, const int x2, const int y2)
{
    int x_1, y_1, x_2, y_2;

    x_1 = x_2 = game_rect.x() + FRAME + PADDING;
    y_1 = y_2 = game_rect.top() + FRAME + PADDING;

    x_1 += x1*(MARGIN+ICON) + ICON/2;
    x_2 = (x1 == x2) ? x_1 : x_2 + x2*(MARGIN+ICON) + ICON/2;

    y_1 += y1*(MARGIN+ICON) + ICON/2;
    y_2 = (y1 == y2) ? y_1 : y_2 + y2*(MARGIN+ICON) + ICON/2;

    // ブラウザのウィンドウがアクティブになっていない場合を考慮して2度クリックする
    QProcess proc;
    proc.start(tr("%1 c:%2,%3").arg(clickprog).arg(x_1).arg(y_1));
    proc.waitForFinished(3000);

    QThread::msleep(100);

    proc.start(tr("%1 c:%2,%3").arg(clickprog).arg(x_1).arg(y_1));
    proc.waitForFinished(3000);

    QThread::msleep(100);

    proc.start(tr("%1 c:%2,%3").arg(clickprog).arg(x_2).arg(y_2));
    proc.waitForFinished(3000);
}

void
MainWindow::clickSpecialIcon(const int x, const int y)
{
    QProcess proc;
    proc.start(tr("%1 c:%2,%3").arg(clickprog)
               .arg(game_rect.x()+FRAME+PADDING+x*(MARGIN+ICON)+ICON/2)
               .arg(game_rect.y()+FRAME+PADDING+y*(MARGIN+ICON)+ICON/2));
    proc.waitForFinished(3000);
}

void
MainWindow::printBoard()
{
    for (int i = 0; i < 8; ++i)
    {
        qDebug() << board[0][i] << ":" << board[1][i] << ":"
                 << board[2][i] << ":" << board[3][i] << ":"
                 << board[4][i] << ":" << board[5][i] << ":"
                 << board[6][i] << ":" << board[7][i];
    }
}
