ZooSolver
=========

ZooKeeperを自動プレイ

# インストール
Qtを使っているので、Qtをコンパイル出来る環境(Qt Creatorとか楽？)。

# 使い方
  - ブラウザでZooKeeperのゲーム画面を開く（http://pc.kiteretsu.jp/games/zka/zka.htm）。
  - ZooSolverを起動する(ZooKeeperのゲーム画面と他のウィンドウが被らないようにする)。
  - ゲームを開始したらすぐにZooSolverの操作メニューにあるスタートを押す。

# 調整方法
ウェイトを置かなすぎると、ゲーム進行に問題があるようなので、適宜調整する。
調整する部分はQThread::msleepの呼び出し箇所。ミリ秒単位でウェイト調整可能。
