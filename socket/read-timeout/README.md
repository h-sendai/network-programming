# ソケットread()タイムアウト

```
struct timeval timeout = { tv_sec, tv_usec };
setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
```

とすることでtimeoutで指定した時間内にread()するデータが
こなかった場合、read()は-1を返し、errnoにEAGAINがセットされる。

このディレクトリのプログラム:

```
./read-timeout remote-host port timeout_sec
```

たとえばnc -l 1234としてサーバープログラムを起動して
別の端末から

```
./read-timeout 127.0.0.1 1234 1
```

とすると、read()に1秒のタイムアウト値が設定されて
10回read()を試みる（そのあと終了する）。

read()がタイムアウトした場合はその時刻と、前回の
read()のと時間差を表示する。

AlmaLinux9での例:

```
% nc -l 1234
```
として別端末から実行:

```
% ./read-timeout 127.0.0.1 1234 1
0.000006 0.000006 program start
1.024351 1.024345 read timeout
2.048408 1.024057 read timeout
3.072346 1.023938 read timeout
4.096347 1.024001 read timeout
5.120346 1.023999 read timeout
6.144349 1.024003 read timeout
7.168344 1.023995 read timeout
8.192347 1.024003 read timeout
9.216343 1.023996 read timeout
10.240403 1.024060 read timeout
```
たしかに1秒程度でtimeoutしているが、1秒より24ミリ秒長い。

```
% ./read-timeout 127.0.0.1 1234 2
0.000007 0.000007 program start
2.061003 2.060996 read timeout
4.108987 2.047984 read timeout
6.157054 2.048067 read timeout
8.205011 2.047957 read timeout
10.252923 2.047912 read timeout
12.300995 2.048072 read timeout
14.348989 2.047994 read timeout
16.396928 2.047939 read timeout
18.445062 2.048134 read timeout
20.492991 2.047929 read timeout
```

タイムアウトを2秒にしたら2.048秒になった。

```
% ./read-timeout 127.0.0.1 1234 4
0.000004 0.000004 program start
4.023186 4.023182 read timeout
8.055140 4.031954 read timeout
12.087140 4.032000 read timeout
16.119210 4.032070 read timeout
20.151158 4.031948 read timeout
24.183141 4.031983 read timeout
28.215182 4.032041 read timeout
32.247137 4.031955 read timeout
36.279190 4.032053 read timeout
40.311312 4.032122 read timeout
```

4.032秒？

```
% ./read-timeout 127.0.0.1 1234 10
0.000007 0.000007 program start
10.331768 10.331761 read timeout
20.571772 10.240004 read timeout
30.811840 10.240068 read timeout
41.051771 10.239931 read timeout
51.291769 10.239998 read timeout
61.531730 10.239961 read timeout
71.771737 10.240007 read timeout
82.011763 10.240026 read timeout
92.251768 10.240005 read timeout
102.491784 10.240016 read timeout
```

10.24秒？

CentOS 7で実行するとわりと正確な値になる:
```
centos7% ./read-timeout 127.0.0.1 1234 1
0.000015 0.000015 program start
0.999500 0.999485 read timeout
1.999498 0.999998 read timeout
2.999497 0.999999 read timeout
3.999499 1.000002 read timeout
4.999497 0.999998 read timeout
5.999497 1.000000 read timeout
6.999498 1.000001 read timeout
7.999496 0.999998 read timeout
8.999498 1.000002 read timeout
9.999497 0.999999 read timeout
```

```
centos7% ./read-timeout 127.0.0.1 1234 2
0.000015 0.000015 program start
1.999241 1.999226 read timeout
3.999244 2.000003 read timeout
5.999244 2.000000 read timeout
7.999248 2.000004 read timeout
9.999242 1.999994 read timeout
11.999239 1.999997 read timeout
13.999246 2.000007 read timeout
15.999258 2.000012 read timeout
17.999239 1.999981 read timeout
19.999238 1.999999 read timeout
```

```
centos7% ./read-timeout 127.0.0.1 1234 4
0.000014 0.000014 program start
3.999988 3.999974 read timeout
7.999978 3.999990 read timeout
11.999983 4.000005 read timeout
15.999984 4.000001 read timeout
19.999993 4.000009 read timeout
23.999984 3.999991 read timeout
27.999978 3.999994 read timeout
31.999979 4.000001 read timeout
35.999978 3.999999 read timeout
39.999982 4.000004 read timeout
```
