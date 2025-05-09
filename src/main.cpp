#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include "M5Unified.h" // M5Stack用ライブラリ

// BNO055のインスタンスを作成
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  M5.begin(); // M5Stackの初期化
  Wire.begin(); // I2C通信の初期化

  M5.Display.setTextSize(2); // テキストサイズを設定
  M5.Display.println("Initializing...");

  if (!bno.begin()) {
    M5.Display.println("BNO055 not detected!");
    while (1); // センサーが見つからない場合は停止
  }

  M5.Display.println("BNO055 detected!");
  bno.setExtCrystalUse(true); // 外部クリスタルを使用
  delay(1000); // 初期化メッセージを表示するための遅延
}

void loop() {
  M5.update(); // M5Stackの状態を更新

  // クォータニオンデータを取得
  imu::Quaternion quat = bno.getQuat();

  // ディスプレイにクォータニオンを表示
  M5.Display.clear(); // ディスプレイをクリア
  M5.Display.setCursor(0, 0); // カーソルを左上に設定
  M5.Display.println("Quaternion:");
  M5.Display.printf("W: %.2f\n", quat.w());
  M5.Display.printf("X: %.2f\n", quat.x());
  M5.Display.printf("Y: %.2f\n", quat.y());
  M5.Display.printf("Z: %.2f\n", quat.z());

  delay(1000); // 1秒ごとに更新
}