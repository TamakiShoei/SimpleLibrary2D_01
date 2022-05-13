# SimpleLibrary2D_01

使用言語
C/C++

開発環境
Windows10, VisualStudio2019, x64



補足
・NameSpaceを撤廃した。

・Engineクラスをファサードクラスとして実装した。

・Engine.hの24行目以下をprivateにしたいが、ClearScreenとScreenFlipでGraphicsクラスのメンバを使っていて
　それを渡すためにEngineクラスを使っているため、privateにできない(解決法が思いつかない)

・そのほか気になったところがあれば指摘していただきたいです