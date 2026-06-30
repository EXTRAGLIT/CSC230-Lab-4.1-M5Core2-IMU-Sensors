/*
  Lab 4.1 - M5Core2 IMU Sensors
  Part 2:
    - Displays X, Y, Z acceleration
    - Tracks maximum X, Y, Z values
    - Tracks maximum acceleration vector magnitude
    - Left button resets maximum values

  Part 3:
    - Detects a free fall (drop about 10 cm or more)
    - Blinks screen red and white 5 times
*/

#include <M5Core2.h>
#include <math.h>

float accX, accY, accZ;

// Maximum values
float maxX = 0;
float maxY = 0;
float maxZ = 0;
float maxVector = 0;

void setup() {
  M5.begin();

  M5.IMU.Init();

  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void loop() {

  M5.update();

  // Read accelerometer
  M5.IMU.getAccelData(&accX, &accY, &accZ);

  // Update max values
  if (fabs(accX) > maxX)
    maxX = fabs(accX);

  if (fabs(accY) > maxY)
    maxY = fabs(accY);

  if (fabs(accZ) > maxZ)
    maxZ = fabs(accZ);

  // Calculate total acceleration vector
  float Xsq = accX * accX;
  float Ysq = accY * accY;
  float Zsq = accZ * accZ;

  float vectorMag = sqrt(Xsq + Ysq + Zsq);

  if (vectorMag > maxVector)
    maxVector = vectorMag;

  // Reset max values with left button
  if (M5.BtnA.wasPressed()) {
    maxX = 0;
    maxY = 0;
    maxZ = 0;
    maxVector = 0;
  }

  // Display data
  M5.Lcd.setCursor(0, 0);

  M5.Lcd.printf("Accel X: %.2f g\n", accX);
  M5.Lcd.printf("Accel Y: %.2f g\n", accY);
  M5.Lcd.printf("Accel Z: %.2f g\n\n", accZ);

  M5.Lcd.printf("Max X: %.2f\n", maxX);
  M5.Lcd.printf("Max Y: %.2f\n", maxY);
  M5.Lcd.printf("Max Z: %.2f\n\n", maxZ);

  M5.Lcd.printf("Vector: %.2f\n", vectorMag);
  M5.Lcd.printf("Max Vector: %.2f\n", maxVector);

  M5.Lcd.println();
  M5.Lcd.println("Btn A = Reset Max");

  // Detect free fall
  // Normal gravity is about 1.0g
  // During a drop the magnitude approaches 0g
  if (vectorMag < 0.30) {

    for (int i = 0; i < 5; i++) {

      M5.Lcd.fillScreen(RED);
      delay(500);

      M5.Lcd.fillScreen(WHITE);
      delay(500);
    }

    M5.Lcd.fillScreen(BLACK);
  }

  delay(100);
}