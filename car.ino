#include <MsTimer2.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <Wire.h>

LiquidCrystal lcd(8,9, 4, 5, 6, 7);

Servo myservo; // 建立Servo物件，控制伺服馬達

#define TRIG_PIN    15          // trigger Pin
#define ECHO_PIN    14          // Echo Pin
#define MAX_DISTANCE 500        // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);    // 設定 NewPing 物件，並給與最遠測試距離

SoftwareSerial I2CBT(16,17); //定義Arduino PIN10及PIN11分別為RX及TX腳位
//請注意Arduino的TX要接藍牙模組的RX, 反之亦然
  byte cmmd[20];
  int insize;
  int j = 0;
void setup() {
   MsTimer2::set(50,Distance);
     MsTimer2::start();
    lcd.begin(16, 2); // start the library
    lcd.setCursor(0,0);
    lcd.print("the distance:");
    lcd.setCursor(4,1);
    lcd.print("cm");
  
  Serial.begin(9600);
  myservo.attach(19);
  myservo.write(0);
  I2CBT.begin(9600);//bluetooth baud rate 
  //=================左馬達=================
  //====轉動方向=====
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  //====轉動速度=====
  pinMode(10,OUTPUT);
  //=============左馬達設定結束============
  //=================右馬達=================
  //====轉動方向=====
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  //====轉動速度=====
  pinMode(13,OUTPUT);
  //=============右馬達設定結束============ 
}
void Forward()
{
  //==========左馬達向前轉=================
  digitalWrite(2,HIGH);
  digitalWrite(3,LOW);
  //=====左馬達速度(150可微調130~255)======
  analogWrite(10,255);
  //----------左馬達動作指派結束----------
  
  //==========右馬達向前轉=================
  digitalWrite(11,HIGH);
  digitalWrite(12,LOW);
  //=====右馬達速度(130可微調130~255)======
  analogWrite(13,255);
  //----------右馬達動作指派結束----------

  //========向前無旋轉角度不用給延遲======
}
void TurnLeft()
{
  //==========左馬達向後轉=================
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);
  //=====左馬達速度(150可微調130~255)======
  analogWrite(10,255);
  //----------左馬達動作指派結束----------
  
  //==========右馬達向前轉=================
  digitalWrite(11,HIGH);
  digitalWrite(12,LOW);
  //=====右馬達速度(150可微調130~255)======
  analogWrite(13,255);
  //----------右馬達動作指派結束----------
  
}

void TurnRight()
{
  //==========左馬達向前轉=================
  digitalWrite(2,HIGH);
  digitalWrite(3,LOW);
  //=====左馬達速度(150可微調130~255)======
  analogWrite(10,255);
  //----------左馬達動作指派結束----------
  
  //==========右馬達向後轉=================
  digitalWrite(11,LOW);
  digitalWrite(12,HIGH);
  //=====右馬達速度(150可微調130~255)======
  analogWrite(13,255);
  //----------右馬達動作指派結束----------
  
}
void TurnBack()
{
  //==========左馬達向前轉=================
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);
  //=====左馬達速度(150可微調130~255)======
  analogWrite(10,255);
  //----------左馬達動作指派結束----------
  
  //==========右馬達向後轉=================
  digitalWrite(11,LOW);
  digitalWrite(12,HIGH);
  //=====右馬達速度(150可微調130~255)======
  analogWrite(13,255);
  //----------右馬達動作指派結束----------
  
}
void TurnStop(){
  //==========左馬達stop=================
   digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  //=====左馬達速度(150可微調130~255)======
  analogWrite(10,255);
  //----------左馬達動作指派結束----------
  
  //==========右馬達stop=================
  digitalWrite(11,LOW);
  digitalWrite(12,LOW);
  //=====右馬達速度(150可微調130~255)======
  analogWrite(13,255);
  //----------右馬達動作指派結束----------
  
}
void Around(){
  while(cmmd[0]!=100&&cmmd[0]!=102&&j!=180){
     if ((insize=(I2CBT.available()))>0){
      for (int i=0; i<insize; i++){
        Serial.print(cmmd[i]=char(I2CBT.read()));
        Serial.print("\n"); 
      }
    }
    j+=5;
    myservo.write(j);
    delay(20);
  }
  while(cmmd[0]!=100&&cmmd[0]!=102&&j!=0){
     if ((insize=(I2CBT.available()))>0){
      for (int i=0; i<insize; i++){
        Serial.print(cmmd[i]=char(I2CBT.read()));
        Serial.print("\n"); 
      }
    }
    j-=5; 
    myservo.write(j);
    delay(20);

  }
}
void Distance(){
  lcd.setCursor(0,1);
  lcd.print("    ");
  unsigned int uS = sonar.ping();   // 送出 ping，並取得微秒 microseconds(uS) 時間
  Serial.print("Ping: ");
  Serial.print(sonar.convert_cm(uS));    // 換算時間為公分，如顯示 0，表示超出距離範圍
  Serial.println("cm");
   if(sonar.convert_cm(uS)>5&&sonar.convert_cm(uS)<501)
   {
    I2CBT.write(sonar.convert_cm(uS));
    if(sonar.convert_cm(uS)<=15)
    {
      TurnStop();
      
    }
   lcd.setCursor(0,1);
   lcd.print(sonar.convert_cm(uS));
   }
}

void loop() {

  while(1){
     if ((insize=(I2CBT.available()))>0){
      for (int i=0; i<insize; i++){
        Serial.print(cmmd[i]=char(I2CBT.read()));
        Serial.print("\n"); 
      }
    }
    
    //根據所收到訊號執行對應動作  
    switch (cmmd[0]) {
    case 96:      
      Forward();
      Around();
      break;  

    case 97:
      TurnLeft();
      Around();
      break;
    case 98://'b'
      TurnRight();
      Around();
      break;
    case 99:
      TurnBack();
      Around();
      break;
    case 100:
      TurnStop();
      break;
    case 101:
     Around();
     break;
     case 102:
     myservo.write(j); 
     break;   
      
    } //Switch

  } //while


}
