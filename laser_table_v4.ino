//**************************************************
//  Name : Laser Table                                
//  Author : Ideanator
//  Date : 16 June, 2014
//  Modified : 30 July, 2014    
//  Version : 0.4                                             
//  Notes : Control code for the laser tables
//**************************************************

//Pin connected to ST_CP (latch) of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of (clock) 74HC595
int clockPin = 12;
//Pin connected to DS (data) of 74HC595
int dataPin = 11;

//holders for infromation you're going to pass to shifting function
byte data;
byte dataA;
byte dataB;
byte dataArray[23];

//===================================================================

//button detection
const int button = 4;
//laser control
int laser = 3;
//player A laser reciever
int playerA = A1;
//player B laser reciever
int playerB = A0;

//minimum light on target for reciever to trigger
//0-255?
int thresholdA = 600;  //515
int thresholdB = 600;  //500 
//timer vars
unsigned long t1;
unsigned long t2;
//win/lose length
//int wintime = 5000;



void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);

  //set up other pins
  pinMode(button,  INPUT_PULLUP);
  pinMode(laser,   OUTPUT);
  pinMode(playerA, INPUT);
  pinMode(playerB, INPUT); 

  Serial.begin(9600);

//===================================================================

  //Arduino doesn't seem to have a way to write binary straight into the code
  //so these values are in HEX.  Decimal would have been fine, too.

  dataArray[1]	=	0x00;	//00000000
  dataArray[2]	=	0x80;	//10000000
  dataArray[3]	=	0xC0;	//11000000
  dataArray[4]	=	0xE0;	//11100000
  dataArray[5]	=	0xF0;	//11110000
  dataArray[6]	=	0xF8;	//11111000
  dataArray[7]	=	0xFC;	//11111100
  dataArray[8]	=	0xFE;	//11111110
  dataArray[9]	=	0xFF;	//11111111

  dataArray[10]	=	0x00;	//00000000
  dataArray[11]	=	0x01;	//00000001
  dataArray[12]	=	0x03;	//00000011
  dataArray[13]	=	0x07;	//00000111
  dataArray[14]	=	0x0F;	//00001111
  dataArray[15]	=	0x1F;	//00011111
  dataArray[16]	=	0x3F;	//00111111
  dataArray[17]	=	0x7F;	//01111111
  dataArray[18]	=	0xFF;	//11111111

  dataArray[19]	=	0x55;	//01010101
  dataArray[20]	=	0xAA;	//10101010
  dataArray[21]	=	0xFF;	//11111111    
  dataArray[22]	=	0x00;	//00000000


    //function that blinks all the LEDs
  //gets passed the number of blinks and the pause time
  blinkAll_2Bytes(2,500); 
}

//===================================================================

void loop() {
  //wait for button press
  if(digitalRead(button) == LOW){

//===================================================================

    //turn on led runways 
    for (int j = 1; j < 10; j++) {
      dataA = dataArray[j];
      int k = j+9;
      dataB = dataArray[k];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, dataA);   //plyA
      shiftOut(dataPin, clockPin, dataB);   //plyB
      digitalWrite(latchPin, 1);
      delay(100);
    }

    //laser on; transistor
    digitalWrite(laser, HIGH);

//===================================================================

    for(int x=0; x<=3000; x++){
//========== if player A wins ===============
      if(analogRead(playerA) > thresholdA){ 
        playerAwin(); 
        goto next; 
      }
//===========================================

//========== if player B wins ===============
      else if(analogRead(playerB) > thresholdB){ 
        playerBwin(); 
        goto next; 
      }
      delay(20);
    }
//===========================================

//============== if nobody wins =============
//nobody:
    for (int j = 9; j >=0; j--) {
      dataA = dataArray[j];
      int k = j+9;
      dataB = dataArray[k];
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, dataA);   //plyA
      shiftOut(dataPin, clockPin, dataB);   //plyB
      digitalWrite(latchPin, 1);
      int o = map(j, 9, 0, 250, 100);

      for (int m = 0; m < 12; m++) {
        digitalWrite(laser, LOW);
        delay(10*j);
        analogWrite(laser, o);
        for(int n = 0; n < 10*j; n++){
          if(analogRead(playerA) > thresholdA){ 
            playerAwin(); 
            goto next; 
          }
          else if(analogRead(playerB) > thresholdB){ 
            playerBwin(); 
            goto next; 
          }
        }
      }
    }
//===========================================

next:    
    next2();
  }
}

//===================================================================

void playerAwin() {
  int l = 10;
  int p = 18;
  for(l; l > 0; l--){
    p--;
    if(p<=10){ 
      p=00; 
    }
    //animation frame 1
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, dataArray[19]);   //plyA
    shiftOut(dataPin, clockPin, dataArray[p]);   //plyB
    digitalWrite(latchPin, 1);
    digitalWrite(laser, HIGH);
    delay(100);        

    //animation frame 2
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, dataArray[20]);   //plyA
    shiftOut(dataPin, clockPin, dataArray[p]);   //plyB
    digitalWrite(latchPin, 1);
    digitalWrite(laser, LOW);
    delay(100);       

    //        next1();
  }
  next2();
}

//===================================================================

void playerBwin() {
  int l = 10;
  int p = 10;
  for(l; l > 0; l--){
    p--;
    if(p<=0){ 
      p=0; 
    }
    //animation frame 1
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, dataArray[p]);   //plyA
    shiftOut(dataPin, clockPin, dataArray[19]);   //plyB
    digitalWrite(latchPin, 1);
    digitalWrite(laser, HIGH);
    delay(200);

    //animation frame 2
    digitalWrite(latchPin, 0); 
    shiftOut(dataPin, clockPin, dataArray[p]);   //plyA
    shiftOut(dataPin, clockPin, dataArray[20]);   //plyB
    digitalWrite(latchPin, 1);
    digitalWrite(laser, LOW);        
    delay(200);

    //        next1();
  }
  next2();
}

//===================================================================

void next1() {
  //clear runway
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, dataArray[1]);
  shiftOut(dataPin, clockPin, dataArray[1]);
  digitalWrite(latchPin, 1);
  digitalWrite(laser, LOW);
}

//===================================================================

void next2() {
  //clear runway
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, dataArray[1]);
  shiftOut(dataPin, clockPin, dataArray[1]);
  digitalWrite(latchPin, 1);
  digitalWrite(laser, LOW);
  //cooldown time
  delay(5000); 
}

//===================================================================

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else { 
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}


//blinks the whole register based on the number of times you want to
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}

