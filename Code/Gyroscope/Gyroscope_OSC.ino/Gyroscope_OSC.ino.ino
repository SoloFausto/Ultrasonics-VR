#include <OSCMessage.h>
#include <OSCBoards.h>

#include "FastIMU.h"
#include "Madgwick.h"

// This .ino sketch is adapted largely from examples from the FastIMU and OSC libraries that are available for arduino. I'm using the arduino Uno and a MPU 6050 for the Gyroscope

#define IMU_ADDRESS 0x68  
//#define PERFORM_CALIBRATION //Comment to disable startup calibration
MPU6050 IMU;               



calData calib = { 0 };  //Calibration data
AccelData IMUAccel;    //Sensor data
GyroData IMUGyro;
Madgwick filter;

#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );
#else
#include <SLIPEncodedSerial.h>
 SLIPEncodedSerial SLIPSerial(Serial); // Change to Serial1 or Serial2 etc. for boards with multiple serial ports that don’t have Serial
#endif

 int index = 0;
 int status = 1;
 float timeoffset = 0;
 float posx = 0;
 float posy = 0;
 float posz = 0.3;
void setup() {
  SLIPSerial.begin(9600);

  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0) {
    
    while (true) {
      ;
    }
  }

#ifdef PERFORM_CALIBRATION
  delay(5000);
  IMU.calibrateAccelGyro(&calib);
  delay(5000);
  IMU.init(calib, IMU_ADDRESS);

  filter.begin(0.2f);
#endif
}

void loop() {
  IMU.update();
  IMU.getAccel(&IMUAccel);
  IMU.getGyro(&IMUGyro);
  filter.updateIMU(IMUGyro.gyroX, IMUGyro.gyroY, IMUGyro.gyroZ, IMUAccel.accelX, IMUAccel.accelY, IMUAccel.accelZ);
  float qw = filter.getQuatW();
  float qx = filter.getQuatX();
  float qy = filter.getQuatY();
  float qz = filter.getQuatZ();
  OSCMessage msg("/VMT/Room/Unity");
  msg.add((int)index);
  msg.add((int)status);
  msg.add((float)timeoffset);
  msg.add((float)posx);
  msg.add((float)posy);
  msg.add((float)posz);
  msg.add((float)qx);
  msg.add((float)qy);
  msg.add((float)qz);
  msg.add((float)qw);
  SLIPSerial.beginPacket();  
    msg.send(SLIPSerial); 
  SLIPSerial.endPacket();
  delay(50);
}
