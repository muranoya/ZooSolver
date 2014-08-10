ZooSolver
=========

ZooKeeperを自動プレイ

# インストール
Qtを使っているので、Qtをコンパイル出来る環境(Qt Creatorとか楽？)。

マウスの操作にcliclickを使っているので、そのインストール。
完全パスはプログラム中のclickprog変数を適宜設定してコンパイルしてください。

Homebrewでcliclickをインストールする場合は
  $ brew install cliclick
でインストールできます。

# 使い方
  - ブラウザでZooKeeperのゲーム画面を開く（http://pc.kiteretsu.jp/games/zka/zka.htm）。
  - ZooSolverを起動する(ZooKeeperのゲーム画面と他のウィンドウが被らないようにする)。
  - ゲームを開始したらすぐにZooSolverのスタートボタンを押す。
  - 100回操作が終われば、自動操作が終了するので、まだ続行する場合はZooSolverのスタートボタンをまた押す。

# 調整方法
ウェイトを置かなすぎると、ゲーム進行に問題があるようなので、適宜調整する。
調整する部分はQThread::msleepの呼び出し箇所。ミリ秒単位でウェイト調整可能。
