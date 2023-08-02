# connect()のタイムアウト

connect()関数で接続できなかった場合はエラーが返る。

接続できない理由
- 指定したホストが存在しない
- ホストは存在しているが接続ポートをlisten()していない
  (サーバーが起動していない、ファイアウォールでrejectしているなど)

以下、connect()はAlmaLinux 9に対して行っている場合のログ。

## 指定したホストが存在していない場合

[指定したホストが存在していない場合のログ](not-exist-host/not-exist-host.log)
コマンド引数で指定したIPアドレス、あるいはホスト名から
ARPでEthernet MACアドレスの取得を試みている。ホストが
存在せず、返答がないので3度ARPを試したあと、connect()は
No route to host
エラーで終了している。

connect()は接続を試みたあと、3秒後にエラーで終了している。

## 指定したホストが存在しているが指定したポートでlisten()していない場合

指定したホストが存在しているが指定したポートでlisten()していない場合の
ログ。

[サーバー側でfirewalldを起動していない場合のログ](connection-refused-no-firewall/connection-refused.log)

[サーバー側でfirewalldを起動している場合のログ](connection-refused-firewall/connection-refused-firewall.log)

いずれの場合も接続が失敗したのがが(3秒かかるとかではなく)すぐわかる。

### connect()が失敗したあとのソケットファイルディスクリプタ

UNIX Network Programming 3rd edition (Stevens, Fenner, Rudoff)
Section 4.3 (p. 101)

> If connect fails, the socket is no longer usable and must be
> closed.  We cannot call connect again on the socket.

と書かれている。close(sockfd)でソケットを閉じる。
