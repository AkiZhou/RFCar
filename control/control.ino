#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//////////////////////////////////////////////
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ;

//////////////////////////////////////////////
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "11111"; // Pipe any string of size 5
const int freeze = 1;
const int foward = 0;
const int right = 2;
const int left = 3;
int order = 0;
//const int switch1 = 2;
//const int switch2 = 4;
//int stateSwitch1 = 0;
//int stateSwitch2 = 0;

//////////////////////////////////////////////
void setup() 
{
  Wire.begin(); // initiate i2c system
  Wire.beginTransmission(MPU_addr); // be sure we talk to our MPU vs some other device
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true); // done talking over to MPU device, for the moment
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN); // Power Amplifier Level maybe increase parameter to increase range
  Serial.begin(9600);
}

//////////////////////////////////////////////
void loop() 
{
  Wire.beginTransmission(MPU_addr); // get ready to talk to MPU again
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false); // done talking to MPU for the time being
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  // all the fancy <<8| stuff is to bit shift the first 8 bits to
  // the left & combine it with the next 8 bits to form 16 bits
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  // the above lines have gathered Accellerometer values for X, Y, Z

  //Serial.print("AcX = "); Serial.print(AcX); // share accellerometer values over debug channel 
  //Serial.print(" | AcY = "); Serial.print(AcY);
  //Serial.print(" | AcZ = "); Serial.print(AcZ);

//////////////////////////////////////////////
  radio.stopListening(); // Set the unit as a transmitter
  // Send order signals
  if((-16000<AcX && AcX<-15000) && (-1000<AcY && AcY<1000) && (-3500<AcZ && AcZ<-1500))
  {
    radio.write(&foward, sizeof(foward)); // 
    Serial.print("0");
  }
  else if((500<AcX && AcX<2500) && (15500<AcY && AcY<17500) && (-1500<AcZ && AcZ<500))
  {
    radio.write(&right, sizeof(right)); // 
    Serial.print("2");
  }
  else if((0<AcX && AcX<2000) && (-17500<AcY && AcY<-15500) && (-1500<AcZ && AcZ<500))
  {
    radio.write(&left, sizeof(left)); // 
    Serial.print("3");
  }
  else
  {
    radio.write(&freeze, sizeof(freeze)); // 
    Serial.print("1");
  }
  
  delay(10);
  /*
  delay(1000);
  Serial.print(stateSwitch1);
  Serial.print(stateSwitch2);
  
  // Read state of switches
  stateSwitch1 = digitalRead(switch1);
  stateSwitch2 = digitalRead(switch2);
  
  // Send order signals
  if(stateSwitch1 == 0 && stateSwitch2 == 0)
  {
    radio.write(&stopThere, sizeof(stopThere)); // order = 0
  }
  else if(stateSwitch1 == 0 && stateSwitch2 == 1)
  {
    radio.write(&goRight, sizeof(goRight)); // order = 2
  }
  else if(stateSwitch1 == 1 && stateSwitch2 == 0)
  {
    radio.write(&goLeft, sizeof(goLeft)); // order = 3
  }
  else
  {
    radio.write(&goStraight, sizeof(goStraight)); // order = 1
  }
  */
}
