#include <SoftwareSerial.h>

#include <avr/pgmspace.h>
#include <Wire.h>
//---------------FONT + GRAPHIC-----------------------------//
#include "data.h"
//==========================================================//

// OLED I2C bus address
#define OLED_address  0x3c

#define SOFTSERIAL_RX 10
#define SOFTSERIAL_TX 5
#define BUFFLENGTH 16

SoftwareSerial softSerial (SOFTSERIAL_RX, SOFTSERIAL_TX);
long lastNode = 0;
long lastCozir = 0;
long lastNodeUpd = 0;
uint8_t lastNodePos = 0;
long lastCozirUpd = 0;
uint8_t lastCozirPos = 0;
char cozir[BUFFLENGTH];
char node[BUFFLENGTH];
uint8_t countr;

void setup() {
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  Serial.begin(57600);
  Serial1.begin(9600);
  softSerial.begin(9600);
  
  Wire.begin();
  init_OLED();
  reset_display();
  sendStrXY("Serial Debugger ",0,0,true);
  //sendStrXY("Last:",1,0,false);
  sendStrXY("CoZIR => Node   ",4,0,true);
  sendStrXY("Node  => CoZIR  ",2,0,true);
  for(countr = 0; countr < 16; countr++){
       cozir[countr] = ' ';
       node[countr]  = ' ';
  }
  Serial.println("DERP");
}
/*  sendStrXY(" QwikSense Diag ",0,0,true); 
  sendStrXY("    CO2 DIAG    ",1,0,false);
  sendStrXY("Sensor: DETECT  ",3,0,false);
  sendStrXY("Value:  N/A  ppm",4,0,false);  */

void loop() {
  countr = 0;
  char c = 0;
  if(softSerial.available()){
    lastCozir = millis();
    Serial.print("C=>N  ");
   while(softSerial.available()){
     c = softSerial.read();
     Serial.print(c, HEX);
     Serial.print(" ");
     switch(c){
      case '\r':
       c = 'r';
       break;
      case '\n':
       c = 'n';
       break;
      case '\0':
       c = 'N';
       break;
     }
     cozir[countr] = c;
     countr++;
     if(countr == 16) countr = 0;
   } 
   while (countr != 0 && countr < 16) {
       cozir[countr] = ' ';
       countr++;
   }
   Serial.print("\r\n");
   lastCozirPos = 0;
   //Serial.print("Delta reaction time: ");
   Serial.println(lastCozir - lastNode);
   
  }
    
  if(lastCozirPos < 16){
    sendCharXY(cozir[lastCozirPos],5,lastCozirPos);
    lastCozirPos++;
  }
  
  countr = 0;
  if(Serial1.available()){
    lastNode = millis();
   Serial.print("N=>C  ");
   delay(20);
   while(Serial1.available()){
     c = Serial1.read();
     Serial.print(c, HEX);
     Serial.print(" ");
     switch(c){
      case '\r':
       c = 'r';
       break;
      case '\n':
       c = 'n';
       break;
      case '\0':
       c = 'N';
       break;
     }
     node[countr] = c;
     countr++;
     if(countr == 16) countr = 0;
   }
   while (countr != 0 && countr < 16) {
       //node[countr] = ' ';
       countr++;
   }
   Serial.print("\r\n"); 
   lastNodePos = 0;
  }
  
  if(lastNodePos < 16){
    sendCharXY(node[lastNodePos],3,lastNodePos);
    lastNodePos++;
  }
}

static void printCharTimes(char c, uint8_t times){
  for(int i = times; i > 0; i--) SendChar(c); 
}

//==========================================================//
// Resets display depending on the actual mode.
static void reset_display(void)
{
  clear_display();
}

//==========================================================//
// Turns display on.
void displayOn(void)
{
    sendcommand(0xaf);        //display on
}

//==========================================================//
// Turns display off.
void displayOff(void)
{
  sendcommand(0xae);		//display off
}

//==========================================================//
// Clears the display by sendind 0 to all the screen map.
static void clear_display(void)
{
  displayOff();
  unsigned char i,k;
  for(k=0;k<8;k++)
  {	
    setXY(k,0);    
    {
      for(i=0;i<128;i++)     //clear all COL
      {
        SendChar(0);         //clear all COL
      }
    }
  }
  displayOn();
}

static void showSplash(){
  //displayOff();
  displayOn();
  for(int i=0;i<128*8;i++)     // show 128* 64 Logo
  {
    SendChar(pgm_read_byte(qslogo+i));
  }
  displayOn();
}

//==========================================================//
static void printBigTime(char *string)
{
  int Y;
  int lon = strlen(string);
  if(lon == 3) {
    Y = 0;
  } else if (lon == 2) {
    Y = 3;
  } else if (lon == 1) {
    Y = 6;
  }
  
  int X = 2;
  while(*string)
  {
    printBigNumber(*string, X, Y);
    
    Y+=3;
    X=2;
    setXY(X,Y);
    *string++;
  }
}


//==========================================================//
// Prints a display big number (96 bytes) in coordinates X Y,
// being multiples of 8. This means we have 16 COLS (0-15) 
// and 8 ROWS (0-7).
static void printBigNumber(char string, uint8_t X, uint8_t Y)
{    
  setXY(X,Y);
  int salto=0;
  for(int i=0;i<96;i++)
  {
    if(string == ' ') {
      SendChar(0);
    } else 
      SendChar(pgm_read_byte(bigNumbers[string-0x30]+i));
   
    if(salto == 23) {
      salto = 0;
      X++;
      setXY(X,Y);
    } else {
      salto++;
    }
  }
}

//==========================================================//
// Actually this sends a byte, not a char to draw in the display. 
// Display's chars uses 8 byte font the small ones and 96 bytes
// for the big number font.
static void SendChar(unsigned char data) 
{ 
  Wire.beginTransmission(OLED_address); // begin transmitting
  Wire.write(0x40);//data mode
  Wire.write(data);
  Wire.endTransmission();    // stop transmitting
}

//==========================================================//
// Prints a display char (not just a byte) in coordinates X Y,
// being multiples of 8. This means we have 16 COLS (0-15) 
// and 8 ROWS (0-7).
static void sendCharXY(unsigned char data, uint8_t X, uint8_t Y)
{
  setXY(X, Y);
  Wire.beginTransmission(OLED_address); // begin transmitting
  Wire.write(0x40);//data mode
  
  for(int i=0;i<8;i++)
    Wire.write(pgm_read_byte(myFont[data-0x20]+i));
    
  Wire.endTransmission();    // stop transmitting
}

//==========================================================//
// Used to send commands to the display.
static void sendcommand(unsigned char com)
{
  Wire.beginTransmission(OLED_address);     //begin transmitting
  Wire.write(0x80);                          //command mode
  Wire.write(com);
  Wire.endTransmission();                    // stop transmitting
 
}

//==========================================================//
// Set the cursor position in a 16 COL * 8 ROW map.
static void setXY(unsigned char row, unsigned char col)
{
  sendcommand(0xb0+row);                //set page address
  sendcommand(0x00+(8*col&0x0f));       //set low col address
  sendcommand(0x10+((8*col>>4)&0x0f));  //set high col address
}


//==========================================================//
// Prints a string regardless the cursor position.
static void sendStr(unsigned char *string, uint8_t invert)
{
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      byte b = pgm_read_byte(myFont[*string-0x20]+i);
      SendChar((invert == 0) ? b : ~b);
    }
    *string++;
  }
}

static void fillProgressBar(uint8_t percentage){
  setXY(6, 1);
  SendChar(0x00);
  SendChar(0x00);
  SendChar(0x00);
  SendChar(0x00);
  SendChar(0x7C);
  
  for(int counter = 0; counter < 100; counter++){
    if(counter <= percentage) SendChar((counter % 2 == 1) ? 0xAA : 0xD6);
    else SendChar(0x82);
  }
  
  SendChar(0x7C);
  SendChar(0x00);
  SendChar(0x00);
  SendChar(0x00);
  SendChar(0x00);
}

//==========================================================//
// Prints a string in coordinates X Y, being multiples of 8.
// This means we have 16 COLS (0-15) and 8 ROWS (0-7).
static void sendStrXY( char *string, uint8_t X, uint8_t Y, uint8_t invert)
{
  setXY(X,Y);
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      byte b = pgm_read_byte(myFont[*string-0x20]+i);
      SendChar((invert == 0) ? b : ~b);
    }
    *string++;
  }
}



//==========================================================//
// Inits oled and draws logo at startup
static void init_OLED(void)
{
  sendcommand(0xae);		//display off
  sendcommand(0xa6);            //Set Normal Display (default)
    // Adafruit Init sequence for 128x64 OLED module
    sendcommand(0xAE);             //DISPLAYOFF
    sendcommand(0xD5);            //SETDISPLAYCLOCKDIV
    sendcommand(0x80);            // the suggested ratio 0x80
    sendcommand(0xA8);            //SSD1306_SETMULTIPLEX
    sendcommand(0x3F);
    sendcommand(0xD3);            //SETDISPLAYOFFSET
    sendcommand(0x0);             //no offset
    sendcommand(0x40 | 0x0);      //SETSTARTLINE
    sendcommand(0x8D);            //CHARGEPUMP
    sendcommand(0x14);
    sendcommand(0x20);             //MEMORYMODE
    sendcommand(0x00);             //0x0 act like ks0108
    
    sendcommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
    //sendcommand(0xA0);
    
    sendcommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
    //sendcommand(0xC0);
    
    sendcommand(0xDA);            //0xDA
    sendcommand(0x12);           //COMSCANDEC
    sendcommand(0x81);           //SETCONTRAS
    sendcommand(0xCF);           //
    sendcommand(0xd9);          //SETPRECHARGE 
    sendcommand(0xF1); 
    sendcommand(0xDB);        //SETVCOMDETECT                
    sendcommand(0x40); 
    sendcommand(0xA4);        //DISPLAYALLON_RESUME        
    sendcommand(0xA6);        //NORMALDISPLAY             

  clear_display();
  sendcommand(0x2e);            // stop scroll
  //----------------------------REVERSE comments----------------------------//
  //  sendcommand(0xa0);		//seg re-map 0->127(default)
  //  sendcommand(0xa1);		//seg re-map 127->0
  //  sendcommand(0xc8);
  //  delay(1000);
  //----------------------------REVERSE comments----------------------------//
  // sendcommand(0xa7);  //Set Inverse Display  
  // sendcommand(0xae);		//display off
  sendcommand(0x20);            //Set Memory Addressing Mode
  sendcommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
  //  sendcommand(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)  
  
   setXY(0,0);
  
  sendcommand(0xaf);		//display on 
}
