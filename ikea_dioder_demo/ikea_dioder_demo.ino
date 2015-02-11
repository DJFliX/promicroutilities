byte r = 0;
byte g = 0;
byte b = 0;
byte rCurrent = 0;
byte gCurrent = 0;
byte bCurrent = 0;

#define PIN_G 3
#define PIN_B 5
#define PIN_R 6

#define FADE_FAST 1
#define FADE_MEDIUM 3
#define FADE_SLOW 5
#define LONG_PAUSE 500
#define MEDIUM_PAUSE 300
#define SHORT_PAUSE 150

#define PAUSE LONG_PAUSE
#define FADE FADE_SLOW
bool fadeTo(byte nr, byte ng, byte nb, byte delayms){
  bool isFinished = false;
  r = nr;
  g = ng;
  b = nb;
  
  while (!isFinished){
   if(rCurrent != r)  {
     if(rCurrent < r) rCurrent++;
     else rCurrent--;
     analogWrite(PIN_R, rCurrent);
   }
   if(gCurrent != g) {
      if(gCurrent < g) gCurrent++;
      else gCurrent--; 
      analogWrite(PIN_G, gCurrent);
   }
   if(bCurrent != b) {
     if(bCurrent < b) bCurrent++;
     else bCurrent--;
     analogWrite(PIN_B, bCurrent);
   }
   if(rCurrent == r && bCurrent == b && gCurrent == g) isFinished = true;
   delay(delayms);
  }
}

void setup(){
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
}

void rainbow(){
  fadeTo(255, 0, 0, FADE);
  delay(PAUSE);
  fadeTo(255, 127, 0, FADE);
  delay(PAUSE);
  fadeTo(255, 255, 0, FADE);
  delay(PAUSE);
  fadeTo(127, 0, 0, FADE);
  delay(PAUSE);
  fadeTo(0, 255, 0, FADE);
  delay(PAUSE);
  fadeTo(0, 255, 0, FADE);
  delay(PAUSE);
  fadeTo(0, 255, 127, FADE);
  delay(PAUSE);
  fadeTo(0, 255, 255, FADE);
  delay(PAUSE);
  fadeTo(0, 127, 255, FADE);
  delay(PAUSE);
  fadeTo(0, 0, 255, FADE);
  delay(PAUSE);
  fadeTo(127, 0, 255, FADE);
  delay(PAUSE);
  fadeTo(255, 0, 255, FADE);
  delay(PAUSE);
  fadeTo(255, 0, 127, FADE);
  delay(PAUSE);
  fadeTo(0,0,0,FADE);
  delay(PAUSE);
}

void burstColor(byte nr, byte ng, byte nb, int duration) {
   analogWrite(PIN_R, nr);
   analogWrite(PIN_G, ng);
   analogWrite(PIN_B, nb);
   delay(duration);
   analogWrite(PIN_R, rCurrent);
   analogWrite(PIN_G, gCurrent);
   analogWrite(PIN_B, bCurrent);   
}

void burstMultiple(byte nr, char ng, byte nb, int duration, char times){
  int i;
   for(i = 0; i < times; i++){
     burstColor(nr, ng, nb, duration);
     delay(duration);
   } 
}

void fadeWhite(){
  fadeTo(0,0,0,1);
  fadeTo(255, 255, 255, 20);
  fadeTo(0,0,0,20);
}

void loop(){
  //rainbow();
  fadeWhite();
  /*burstMultiple(255, 0, 0, 50, 10);
  burstMultiple(0, 255, 0, 50, 10);
  burstMultiple(0, 0, 255, 50, 10);
  burstMultiple(255, 255, 255, 50, 10);
  burstMultiple(255, 0, 0, 50, 10);
  burstMultiple(0, 255, 0, 50, 10);
  burstMultiple(0, 0, 255, 50, 10);
  burstMultiple(16, 16, 16, 50, 10);
  burstMultiple(48, 48, 48, 50, 10);
  burstMultiple(128, 128, 128, 50, 10);
  burstMultiple(192, 192, 192, 50, 10);
  burstMultiple(255, 255, 255, 50, 10);*/
}


