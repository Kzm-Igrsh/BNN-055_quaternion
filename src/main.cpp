#include <M5Unified.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <math.h>

// BNO055のインスタンスを作成
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// 3D座標を回転する関数
void rotatePoint(float &x, float &y, float &z, imu::Quaternion quat) {
  float qw = quat.w(), qx = quat.x(), qy = quat.y(), qz = quat.z();
  float nx = (1 - 2 * (qy * qy + qz * qz)) * x + (2 * (qx * qy - qw * qz)) * y + (2 * (qx * qz + qw * qy)) * z;
  float ny = (2 * (qx * qy + qw * qz)) * x + (1 - 2 * (qx * qx + qz * qz)) * y + (2 * (qy * qz - qw * qx)) * z;
  float nz = (2 * (qx * qz - qw * qy)) * x + (2 * (qy * qz + qw * qx)) * y + (1 - 2 * (qx * qx + qy * qy)) * z;
  x = nx;
  y = ny;
  z = nz;
}

// 立方体の頂点
float cubeVertices[8][3] = {
    {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
    {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}};

void drawCube(imu::Quaternion quat) {
  int centerX = 160, centerY = 120; // ディスプレイの中心
  float scale = 50;                 // スケール

  // 頂点を回転して描画
  for (int i = 0; i < 8; i++) {
    float x = cubeVertices[i][0], y = cubeVertices[i][1], z = cubeVertices[i][2];
    rotatePoint(x, y, z, quat);
    int screenX = centerX + x * scale;
    int screenY = centerY - y * scale;
    M5.Display.fillCircle(screenX, screenY, 2, TFT_WHITE);
  }
}

void setup() {
  M5.begin();
  Wire.begin();
  M5.Display.setTextSize(2);
  M5.Display.println("Initializing...");

  if (!bno.begin()) {
    M5.Display.println("BNO055 not detected!");
    while (1);
  }

  M5.Display.println("BNO055 detected!");
  bno.setExtCrystalUse(true);
  delay(1000);
}

void loop() {
  M5.update();
  imu::Quaternion quat = bno.getQuat();

  M5.Display.clear();
  drawCube(quat); // クォータニオンで回転した立方体を描画

  delay(100);
}