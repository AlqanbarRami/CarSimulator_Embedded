#include <Arduino.h>
#include <Servo.h>

//Servo Höger och Vänster 
 int x;  // Pin A3
 int y;  //Pin A2

 //  Tuta button PD6
 int button = 6  ; 
 // för att läsa tuta button ( button2= digitalRead(button) ) 
 int button2;
// Öppna dörren button PD5
 int buttonLeft = 5 ; 
 //starta motorn button PD4
 int buttonRight = 4;
 //Led dörren PD3
 int ledD = 3;
 //led motorn PD2
 int ledM = 2; 
 // Alarm led ifall någon startar motorn innan dörröppning
 int ledAlarm = 19;
 // value för map funktion ( Servo)
 int value; 
//Obj för servo 
Servo servo1;
//Servo pin PB12
int servoPin = 12;
//Buzzer Pin PD7
int peebPin = 7;

//L293D motor driver 
int IN3 = 11, IN4 = 10;
//Enable3,4
int enable34 = 9;
//HC-SR04
int trig = 13;
int echo = 8;
long duration;
int distance;

// bilen kör fram  "Clockwise"
void Forward()   
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

//bilen kör bakåt "Anticlockwise"
void Back()  
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//stanna motorn
void Stop()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

//när man startar motorn kommer det här ljudet med blinkande led
void PeebStartMotor(){
  for(int i=0; i<10; i++){
    PORTD=PORTD ^ (1<<PD2);
     digitalWrite(peebPin,HIGH);
     delay(100);
     digitalWrite(peebPin,LOW);
     delay(100);

  }
}

//när man öppnar dörren kommer det här ljudet med blinkande led
void PeebOpenDoor(){
  for(int i=0; i<4; i++){
    PORTD=PORTD ^ (1<<PD3);
     digitalWrite(peebPin,HIGH);
     delay(200);
     digitalWrite(peebPin,LOW);
     delay(200);

  }
}

//avstånd 55cm från sensor HC-SR04 "ljud"
void Distance_55(){
  for(int i=0; i<4; i++){
    digitalWrite(ledAlarm,HIGH);
     digitalWrite(peebPin,HIGH);
     delay(500);
     digitalWrite(ledAlarm,LOW);
     digitalWrite(peebPin,LOW);
     delay(500);

  }
}

//avstånd 40cm från sensor HC-SR04 "ljud"
void Distance_40(){
  for(int i=0; i<4; i++){
    digitalWrite(ledAlarm,HIGH);
     digitalWrite(peebPin,HIGH);
     delay(300);
     digitalWrite(ledAlarm,LOW);
     digitalWrite(peebPin,LOW);
     delay(300);

  }
}

//avstånd 20cm från sensor HC-SR04 "ljud"
void Distance_20(){
  for(int i=0; i<4; i++){
    digitalWrite(ledAlarm,HIGH);
     digitalWrite(peebPin,HIGH);
     delay(150);
     digitalWrite(ledAlarm,LOW);
     digitalWrite(peebPin,LOW);
     delay(150);
  }
  }

//avstånd 10cm från sensor HC-SR04 "ljud"
void Distance_10(){
  for(int i=0; i<4; i++){
    digitalWrite(ledAlarm,HIGH);
     digitalWrite(peebPin,HIGH);
     delay(20);
     digitalWrite(ledAlarm,LOW);
     digitalWrite(peebPin,LOW);
     delay(20);
  }
  }

//ifall någon startar motorn innan dörröppning "alarm"
  void StoleAlarm(){
      for(int i=0; i<15; i++){
    digitalWrite(ledAlarm,HIGH);
     digitalWrite(peebPin,HIGH);
     delay(200);
     digitalWrite(ledAlarm,LOW);
     digitalWrite(peebPin,LOW);
     delay(200);

  }
  }


void setup()
{
  pinMode(buttonLeft,INPUT); // dörröppning button som input
  pinMode(buttonRight,INPUT); // motor button som input
  pinMode(ledD,OUTPUT);      // dörr led som output
  pinMode(ledM,OUTPUT);     // motor led som output
   pinMode(ledAlarm,OUTPUT);  // alarm led som output
  pinMode(button,INPUT);     // tuta button som input
  digitalWrite(button,HIGH);  // tuta button high
  pinMode(peebPin, OUTPUT);  // buzzer pin
  servo1.attach(servoPin);   // servo obj
  pinMode(trig, OUTPUT); // trig som Output
  pinMode(echo, INPUT); // echo som Input
  Serial.begin(9600); // jag behövde den för att kontrollera några saker :)
  pinMode(IN3, OUTPUT);   //IN3 som output
  pinMode(IN4, OUTPUT);   //IN4 som output
  pinMode(enable34, OUTPUT); //Enable3,4 som output
  digitalWrite(enable34, HIGH); // Enable3,4 high
}



void loop()
{

//när man startar motorn innan dörröppning kommer alarm
if( PIND  & (1<<PD4) ){
StoleAlarm();
}

// dörrens led och ljud
else if( PIND  & (1<<PD5) ){
  digitalWrite(ledD,HIGH);
  PeebOpenDoor();

while (1)
{
// när man startar motorn kommer andra funktioner som Servo,joystick,HC-SR04 och tuta fungera
if( PIND  & (1<<PD4) ){
  digitalWrite(ledM,HIGH);
  PeebStartMotor();
  while (1)
  {
  // tuta button
int buttonPress=0;
button2= digitalRead(button);
if((button2==LOW) && buttonPress==0){
 digitalWrite(peebPin,HIGH);
}
buttonPress=1;
if((button2==HIGH) && buttonPress==1){
  digitalWrite(peebPin,LOW);
}

  y = analogRead(A2);     //läsa analog A2 om det är mer än 850 kör bakåt och då startar HC-SR04 
  if(y>850){  
  Back();
  digitalWrite(trig, LOW);
  delay(2);
//  trig som HIGH  10 ms
digitalWrite(trig, HIGH);
delay(10);
digitalWrite(trig, LOW);
duration = pulseIn(echo, HIGH);
// räknar  avstånd
distance= duration*0.034/2;

if(distance<40 && distance > 25){
  
  Distance_40();
}

if(distance<55 && distance>40){
  
  Distance_55();
}

if(distance<40 && distance>20){
  
  Distance_40();
}

if(distance<20 && distance>10){
  
  Distance_20();
}
if(distance<10 && distance>1){
  
  Distance_10();
}

  }
  //om det är mindre än 500 kör fram men då startar inte HC-SR04 
  else if(y<500)
  {
     Forward();
  }
  else
  {
    Stop();
  }

//läsa servo pin A3
 x = analogRead(A3);     
  value = map(x, 0, 1000, 0, 180);     
  servo1.write(value);                 
  delay(20);


  }
    
}
}
}
}



