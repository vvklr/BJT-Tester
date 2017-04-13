#include <SPI.h>
#include <UTFT.h>
#include "vishal.h"
const int SSBase = 48;
const int SSCollector = 49;

const int VCE_Pin = 0;
const int VCC_Pin = 2;
const int VBE_Pin = 1;
const int VBB_Pin = 3;

int vceValue = 0;
int vccValue = 0;
int vbeValue = 0;
int vbbValue = 0;
int vstepbase = 800;
int vstepcollector = 100;

float vce = 0;
float vbb = 0;
float vcc = 0;
float vbe = 0;

float Rcollector = 100.0; //Collector resistance (ohm)
float Rbase = 80000.0; //Base resistance (ohm)
float Icollector = 0; //Collector current
float Ibase = 0; // Collector base

word outputValue = 0; // a word is a 16-bit number
byte data = 0;

float currx, curry, prevx, prevy = 0;
//float I_y[];
//I_y[]={0.0,0.11,0.23,0.36,0.48,0.6,0.73,0.85,0.97,1.09,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8};


void setup() {
  pinMode(SSBase, OUTPUT);
  pinMode(SSCollector, OUTPUT);
  SPI.begin();                                                                        //Initialize the SPI Bus
  SPI.setBitOrder(MSBFIRST);                                                          //Transfer MSB first according to the MP4921 datasheet
  //Set initial values for VBB and VCC (avoids some problems
  // with the first values to be read)
  dacTransfer(SSBase, 1);
  dacTransfer(SSCollector, 1);
  Serial.begin(9600); //Initialize serial
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
}
void loop() {
  int ct = 0;
  screen_init();
  IV_graph();
//      prevx = 10; prevy = 215;
  for (int i = 2000; i <= 2095; i = i + 1000) { // for (int i=800; i <= 4095; i=i+800){
    dacTransfer(SSBase, i);
    currx=10; curry = 215;;
    prevx = 10; prevy = 215;
    Serial.println(i);
    for (int j = 0; j <= 4095; j = j + 100) {
      dacTransfer(SSCollector, j);
      delay(10);
      readValues();
      currx = 10 + vcc * (318.0 - 10.0) / (5.0 - 0.0);
      curry = 215.0 - (Icollector) * (215 - 15.0) / (100.0 - 0.0);
      myGLCD.setColor(252, 232, 83);
      myGLCD.fillCircle(currx , curry , 2 );
      myGLCD.setColor(98, 225, 247);
      myGLCD.drawLine(prevx, prevy, currx, curry);
      
      prevx = currx;
      prevy = curry;
      
    }
  }
  
      
//  for (int i = 4000; i <= 4095; i = i + 8000) { // for (int i=800; i <= 4095; i=i+800){
//    dacTransfer(SSBase, i);
//    Serial.println();
//    Serial.println();
//    Serial.println();
//    Serial.println();
//    Serial.println();
//    Serial.println();
//    Serial.println(i);
//    for (int j = 0; j <= 4095; j = j + 100) {
//      dacTransfer(SSCollector, j);
//      Serial.println(j);
//      delay(10);
//      readValues();
//      currx = 10 + vce * (318.0 - 10.0) / (5.0 - 0.0);
//      curry = 215.0 - Icollector * (215 - 15.0) / (100.0 - 0.0);
//      myGLCD.setColor(252, 232, 83);
//      myGLCD.fillCircle(currx , curry , 2 );
//      myGLCD.setColor(98, 225, 247);
//      myGLCD.drawLine(prevx, prevy, currx, curry);
//      prevx = currx;
//      prevy = curry;
//    }
//  }
  delay(10000000);
}
void dacTransfer(int slaveNumber, int value) {
  // 16-bit value is splitted into two bytes
  // 4 most significant bits correspond to configuration of the DAC
  // 12 least significant bits correspond to 12-bit data
  outputValue = value;
  digitalWrite(slaveNumber, LOW);
  data = highByte(outputValue);
  data = 0b00001111 & data;
  data = 0b00110000 | data;
  SPI.transfer(data);
  data = lowByte(outputValue);
  SPI.transfer(data);
  digitalWrite(slaveNumber, HIGH);
}
//byte upperData = 0x30;  // Set the 4 configuration bits
//  byte lowerData = 0x00;
//  word dataWord; // The 16-bit word to be written
//  dataWord = value;
//  lowerData = lowByte(dataWord);
//  //transfer the 4 most significant bits of data to the upper byte
//  upperData |= highByte(dataWord);
//  //Select the DAC
//  digitalWrite(slaveNumber,LOW);
//  //Transfer the data
//  SPI.transfer(upperData);
//  SPI.transfer(lowerData);
//  //DAC is unselected
//  digitalWrite(slaveNumber,HIGH);
//}
//  byte data;
//  digitalWrite(slaveNumber, LOW);
//  data = highByte(value);
//  data = B00001111 & data;
//  data = B00110000 | data;
//  SPI.transfer (data);
//  data = lowByte(value);
//  SPI.transfer (data);
//  digitalWrite(slaveNumber, HIGH);
//}

//void printI(){
//  float I_y[40];
//  I_y[40]={0.0,0.11,0.23,0.36,0.48,0.6,0.73,0.85,0.97,1.09,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8};
//
//  for(int k = 0; k < 40; k++){
//  Serial.println(I_y[k]);
//  }
//}
void readValues() {
  //Read the values at the analog inputs
  vbbValue = analogRead(VBB_Pin);
  vbeValue = analogRead(VBE_Pin);
  vccValue = analogRead(VCC_Pin);
  vceValue = analogRead(VCE_Pin);
  //Convert to volts
  vce = (float)vceValue * 5./ 1023.;
  vcc = (float)vccValue * 5./ 1023.;
  vbb = (float)vbbValue * 5./ 1023.;
  vbe = (float)vbeValue * 5./ 1023.;
  //Calculate collector and base currents
  Icollector = 10*(vcc-vce)/Rcollector * 1000.;
  
  Ibase = (vbb-vbe)/Rbase * 1000000.;
  float A, B = 0.0;
  B = 100*Icollector/Ibase;
  A = B/(B+1);
  
  myGLCD.setColor(10, 10, 10);
  myGLCD.print("Vbe:" +String(vbe)+"V",RIGHT,20 );
  myGLCD.print("Vce:" +String(vce)+"V",RIGHT,35 );
  myGLCD.print("Ic" + String(Icollector)+"mA" ,RIGHT,50);
  myGLCD.print("Ib:" + String(Ibase)+"uA",RIGHT,65);
  myGLCD.print("Beta:"+String(B),RIGHT,80 );
  myGLCD.print("Alpha:"+String(A),RIGHT,95 );
 

//  currx = 10 + vce * (318.0 - 10.0) / (5.0 - 0.0);
//  curry = 215.0 - Icollector * (215 - 15.0) / (100.0 - 0.0);
//  myGLCD.setColor(252, 232, 83);
//  myGLCD.fillCircle(currx , curry , 2 );
//  myGLCD.setColor(98, 225, 247);
//  myGLCD.drawLine(prevx, prevy, currx, curry);
//  prevx = currx;
//  prevy = curry;
//  

  //Print to serial output
  // VCC, VBB, VBE, IC (mA), IB (uA)
  Serial.print(vcc);
  Serial.print("\t");
  Serial.print(vce);
  Serial.print("\t");
  Serial.print(vbb);
  Serial.print("\t");
  Serial.print(vbe);
  Serial.print("\t");
  Serial.print(Icollector);
  Serial.print("\t");
  Serial.print(Ibase);
  Serial.println();
//  printI();
  delay(100);
}  
