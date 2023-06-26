
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <SoftwareSerial.h>

#include <RemoteXY.h>
#include <Wire.h>
#include "RTClib.h"

#define alarmPin 32

const int alarm[]={12,34,0};
const int waitTime =4000;


RTC_DS1307 rtc;

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial2
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "DanaWifi"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 52 bytes
  { 255,1,0,0,0,45,0,16,223,1,2,0,20,39,22,11,218,26,31,31,
  79,78,0,79,70,70,0,129,0,7,11,49,6,6,67,108,101,97,110,32,
  115,111,108,97,114,32,112,97,110,101,108,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_SWITCH_1 32

int l298n_ena=2; //driver for motors
int l298n_in1=4; //driver
int l298n_in2=5; //driver
int l298n_in3=6; //driver
int l298n_in4=7; //driver
int l298n_enb=3; //driver
int led=32;
int trig=22;
int echo=23;
int value =0;
long Time;
int flag=0;
boolean move_status = false; 
double fnc_ultrasonic_distance(int _t, int _e){
 digitalWrite(_t,LOW);
delayMicroseconds(2);
digitalWrite(_t,HIGH);
delayMicroseconds(10);
digitalWrite(_t,LOW);
Time=pulseIn(_e,HIGH); 
value =Time * 0.034 /2 ; 
return value;
}
void setup() {
  //RTC
   pinMode(alarmPin,OUTPUT);//set pin 2 as output
 
  while (!Serial); // for Leonardo/Micro/Zero

  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");

  }
  //RTC ///////////////////////////////////////////////////////

  
    RemoteXY_Init (); 
  
  pinMode (PIN_SWITCH_1, OUTPUT);
  
  // TODO you setup code
    pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(30, INPUT);
  pinMode(29, OUTPUT);
  pinMode(28, INPUT);
  pinMode(23, INPUT);
  pinMode(22, OUTPUT);
  pinMode(39, OUTPUT);
  pinMode(38, INPUT);
   pinMode(53, OUTPUT);
  pinMode(52, INPUT);
  pinMode(32, OUTPUT);

  pinMode(8,OUTPUT);
pinMode(9,OUTPUT);
pinMode(10,OUTPUT);
pinMode(11,OUTPUT);
pinMode(12,OUTPUT);
pinMode(13,OUTPUT);
}

void goForward() {


  digitalWrite(l298n_in3,HIGH);
  digitalWrite(l298n_in4,LOW);
  analogWrite(l298n_ena,(100));
  digitalWrite(l298n_in1,HIGH);
  digitalWrite(l298n_in2,LOW);
  analogWrite(l298n_enb,(100));
}

void goBackward() {
  digitalWrite(l298n_in1,LOW);
  digitalWrite(l298n_in2,HIGH);
  analogWrite(l298n_ena,(50));
  digitalWrite(l298n_in3,LOW);
  digitalWrite(l298n_in4,HIGH);
  analogWrite(l298n_enb,(50));
}

void goLeft() {
  digitalWrite(l298n_in1,HIGH); //  خلفي يمين 
  digitalWrite(l298n_in2,LOW);  //امامي يسار
  analogWrite(l298n_ena,(130));
  digitalWrite(l298n_in3,LOW); // خلفي يسار 
  digitalWrite(l298n_in4,HIGH); 
  analogWrite(l298n_enb,(130));
}

void goSoftLeft() {
  digitalWrite(l298n_in1,HIGH);
  digitalWrite(l298n_in2,LOW);
  analogWrite(l298n_ena,(100));
  digitalWrite(l298n_in3,LOW);
  digitalWrite(l298n_in4,HIGH);
  analogWrite(l298n_enb,(0));
}

void goRight() {
  digitalWrite(l298n_in1,LOW);
  digitalWrite(l298n_in2,HIGH);
  analogWrite(l298n_ena,(130));
  digitalWrite(l298n_in3,HIGH);
  digitalWrite(l298n_in4,LOW);
  analogWrite(l298n_enb,(130));
}

void goSoftRight() {
  digitalWrite(l298n_in1,LOW);
  digitalWrite(l298n_in2,HIGH);
  analogWrite(l298n_ena,(0));
  digitalWrite(l298n_in3,HIGH);
  digitalWrite(l298n_in4,LOW);
  analogWrite(l298n_enb,(100));
}

void stopMove() {
  digitalWrite(l298n_in1,LOW);
  digitalWrite(l298n_in2,LOW);
  analogWrite(l298n_ena,(0));
  digitalWrite(l298n_in3,LOW);
  digitalWrite(l298n_in4,LOW);
  analogWrite(l298n_enb,(0));
}
void brush_pump(){
    analogWrite(12, 200);
    analogWrite(13, 200);
digitalWrite(9,HIGH);
digitalWrite(8,LOW);

}
void stop_brush(){
    analogWrite(12,0);
    analogWrite(13, 0);
digitalWrite(9,LOW);
digitalWrite(8,LOW);
digitalWrite(10,LOW);
digitalWrite(11,LOW);
}



void loop() {

   RemoteXY_Handler ();


    DateTime robojax = rtc.now();
    Serial.print("Alarm: ");
    for(int i=0;i<3;i++){
      Serial.print(alarm[i]);
      if(i !=2){
        Serial.print("-");
      }
    }
    Serial.println();
    Serial.print("Time:  ");   
    Serial.print(robojax.hour(), DEC);
    Serial.print("-");
    Serial.print(robojax.minute(), DEC);
    Serial.print("-");
    Serial.print(robojax.second(), DEC);
    Serial.println();
   if(checkAlarm(robojax) || (RemoteXY.switch_1==1)){
    flag=1;
    }

digitalWrite(32,HIGH);
Serial.print("hhhh");
Serial.print(fnc_ultrasonic_distance(22,23));
   if ((fnc_ultrasonic_distance(22,23) <= 5)){

    stopMove();
    stop_brush();
    digitalWrite(32,HIGH);
    
    }
    else {
if (((fnc_ultrasonic_distance(31,30) <= 15) && (fnc_ultrasonic_distance(29,28) <= 15))) {

  if ((fnc_ultrasonic_distance(23,22) <= 15) && (fnc_ultrasonic_distance(39,38) <= 15)) {
      
    goForward();
    brush_pump();
    brush_pump();
       
  } else {

      if ((fnc_ultrasonic_distance(23,22) >= 15)) {
    goSoftRight();
      }

      if((fnc_ultrasonic_distance(53,52) >= 15)) {
          
                goSoftLeft();
      }
   
  }

} else {

digitalWrite(32,LOW);
  stopMove();
  delay(2000);
  
  move_status = !move_status;
brush_pump();
  if(move_status) {
 goBackward(); 
 delay(2000);
    goRight();
    delay(4350);
    stopMove();
  goForward();
  delay(2000);

  goRight();
  delay(4350);
  stopMove();
  
  }
      

else{
 goBackward(); 
 delay(2000);
    goLeft();
    delay(4350);
    stopMove();
  goForward();
  delay(2000);

  goLeft();
  delay(4350);
  stopMove();

}

}

}
}
boolean checkAlarm(DateTime timeNow){

  if(

   alarm[0] ==timeNow.hour()
   &&
   alarm[1] ==timeNow.minute()
   &&
   alarm[2] ==timeNow.second()
   ){
      return true;
   }else{
      return false;
     
   }
  
}
                                                                                  
