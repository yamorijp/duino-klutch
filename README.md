# duino-klutch

<p align="center">
<img alt="device" src="https://raw.githubusercontent.com/yamorijp/duino-klutch/master/device.jpg" width="600">
</p>

esp8266搭載ボード(NodeMCUV2)へ接続したセンサーやモジュールをWebAPIを使用して操作するためのArduinoスケッチプログラムです。

* GETリクエストを通じて接続モジュールの操作を提供するHTTPサーバー機能
* 接続センサーデータ受信用のWebSocketサーバー機能

また、これらの機能を使用したWebブラウザ上で動作するクライアントプログラムも提供しています。

[duino-klutch-client](https://github.com/yamorijp/duino-klutch-client)


## 使用モジュール

* esp8266搭載ボード (NodeMcuV2,V3)
* 8x8 Matrix LEDモジュール (MAX7219)
* 4桁7セグメントディスプレイモジュール (TM1637)
* 湿温度センサーモジュール (DHT11)
* CdSセンサーモジュール
* マイクロ波レーダーセンサースイッチモジュール (RCWL-0516)
* リレースイッチモジュール
* IR送信モジュール
* RGB LEDモジュール


## ワイヤリング

<p align="center">
<img alt="配線図" src="https://raw.githubusercontent.com/yamorijp/duino-klutch/master/wiring.png" width="600" />
</p>

デバイス                           | 使用ピン                 | 備考
---------------------------------|------------------------|---
8x8 Matrix LEDモジュール            | CS=D4, CLK=D5, DIN=D7 |   
4桁7セグメントディスプレイモジュール       | CLK=D2, DIO=D3        |   
湿温度センサーモジュール               | OUT=D1                |  
CdSセンサーモジュール                 | AO=A0                 |   
マイクロ波レーダーセンサースイッチモジュール | OUT=D6                |   
リレースイッチモジュール                | S=D0                  |   
IR送信モジュール                    | S=D8                  |   
RGB LEDモジュール                   | (R=D1, G=D2, B=D3)    | 初期構成では未使用


## スケッチのビルドとアップロード

[platformio](https://platformio.org/) を使用。

```
$ pio run --target uploadfs
$ pio run --target upload
```


## WiFi設定

初回（WiFi設定が機器に保存されていない場合）のみAPモードで起動します。
WiFi接続が可能なスマートフォンやPCから以下のアクセスポイントに接続してください。

```
アクセスポイント名: duino-k
パスワード: 8086
```


## WebAPIへのアクセス

ポート80番でhttpサーバーが起動するので、特定のURLに対してGETリクエストを送ることで各種操作を行えます。
また、mDNSが使用可能な環境ではホスト名`duino-k.local`が使用できます。

例えば`curl`コマンドで温度と湿度を取得するには以下のようなコマンドを実行します。
```bash
$ curl --request GET --url http://duino-k.local/sensor/ht
{"status":"success","data":{"temperature":27,"humidity":53}}
```

また、ポート81番ではWebSocketによる状態変化の受信が可能です。

使用可能なAPIは[APIドキュメント](data/curl/index.html)を参照してください。
APIドキュメントはブラウザでルートにアクセスすることでも表示できます。


## セキュリティ

当プログラムにセキュリティ機能はありません。  
プライベートネットワーク外からアクセスする場合はVPN等を使用し、
直接インターネットに公開することは避けてください。


## 動作テスト

[mocha](https://mochajs.org/) test framework を使用。
test/srcディレクトリ内のテストコードをmochaで実行する。

```
# 依存パッケージを追加
$ yarn

# テスト実行
$ yarn test
```
