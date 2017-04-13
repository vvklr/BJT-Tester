#include <UTFT.h>
extern uint8_t SmallFont[];
UTFT myGLCD(ILI9341_16,38,39,40,41);
//float Iy[];
//Iy[40]={0.0,0.11,0.23,0.36,0.48,0.6,0.73,0.85,0.97,1.09,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8,4.8};

void screen_init()
{
  myGLCD.clrScr();
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 399, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 226, 399, 239);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("     ********** BJT TESTER**********      ", CENTER, 1);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255,255,0);
  myGLCD.print("     By:Vishal Raut| Guide:Prof J.John     ", CENTER, 227);
}
void IV_graph(){
  myGLCD.setColor(0, 0, 255);
  myGLCD.drawLine(10, 215, 318, 215);
  myGLCD.drawLine(10, 15 , 10, 215);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print(" 0      1       2       3       4 (Vce)5", CENTER, 217);
  myGLCD.print("Ic",LEFT,17);
  myGLCD.print("9",LEFT,30);
  myGLCD.print("7",LEFT,70);
  myGLCD.print("mA",LEFT,90);
  myGLCD.print("5",LEFT,110);
  myGLCD.print("3",LEFT,150);
  myGLCD.print("1",LEFT,190);
  myGLCD.setColor(VGA_LIME);
  myGLCD.fillRoundRect(230,15,319,120);
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(VGA_LIME);
}
//void printI(){
//  for(long int k = 0; k < 40; k++){
//  Serial.println(Iy[k]);
//  }
//}
