#include <SoftwareSerial.h>
#include <Wire.h>

// the maximum received command length from an Android system (over the bluetooth)
#define MAX_BTCMDLEN 128
char str[MAX_BTCMDLEN];
// 建立一個軟體模擬的序列埠; 不要接反了!
// HC-06    Arduino
// TX       RX/Pin10
// RX       TX/Pin11
SoftwareSerial BTSerial(10,11); // Arduino RX/TX

//PWM
//3 5 6 9 10 11


//A1 2 3.
int A1pin01 = 2;
int A1pin02 = 3; //PWM
//A2
int A2pin01 = 4;
int A2pin02 = 5; //PWM
//A3
int A3pin01 = 7;
int A3pin02 = 6; //PWM 
//A4
int A4pin01 = 8;
int A4pin02 = 9; //PWM

//speed
int speed1 = 0;
int speed2 = 0;
int speed3 = 0;
int speed4 = 0;
int baseSpeed = 0;

int adjSpeed1 = 0;
int adjSpeed2 = 0;
int adjSpeed3 = 0;
int adjSpeed4 = 0;

int controlSpeed1 = 0;
int controlSpeed2 = 0;
int controlSpeed3 = 0;
int controlSpeed4 = 0;

byte cmd[MAX_BTCMDLEN]; // received 128 bytes from an Android system
int len = 0; // received command length

void setup() {
    Serial.begin(9600);   // Arduino起始鮑率：9600
    BTSerial.begin(9600); // HC-06 出廠的鮑率：每個藍牙晶片的鮑率都不太一樣，請務必確認
    pinMode(2,OUTPUT);
    pinMode(3,OUTPUT);
    pinMode(4,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(7,OUTPUT);
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);

    //SET GROUND
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
}

void loop() {
    
    //BluetoothLisent();
    BluetoothLisent2();
    //get string

    if(str[0] != '@'){
      SetSpeed(0,0,0,0);
      //Serial.println("Wrong Data");
    }
    else{
      //@,ALL(0~9),A1,A2,A3,A4,adj1,adj2,adj3,adj4

      //adj 1~9

      
      baseSpeed = ((int)str[1] )*25;

      controlSpeed1 = ((int)str[2]);
      controlSpeed2 = ((int)str[3]);
      controlSpeed3 = ((int)str[4]);
      controlSpeed4 = ((int)str[5]);

      adjSpeed1 = ((int)str[6])*2;
      adjSpeed2 = ((int)str[7])*2;
      adjSpeed3 = ((int)str[8])*2;
      adjSpeed4 = ((int)str[9])*2;
     
      speed1 = baseSpeed + controlSpeed1 + adjSpeed1;
      speed2 = baseSpeed + controlSpeed2 + adjSpeed2;
      speed3 = baseSpeed + controlSpeed3 + adjSpeed3;
      speed4 = baseSpeed + controlSpeed4 + adjSpeed4;

      SetSpeed(speed1,speed2,speed3,speed4);
    }
}

void SetSpeed(int a,int b,int c,int d){
    analogWrite(A1pin02, a);
    analogWrite(A2pin02, b);
    analogWrite(A3pin02, c);
    analogWrite(A4pin02, d);
}
void BluetoothLisent2(){
   if (BTSerial.available() >0) {      // 藍芽模組 回傳 的 訊息 給電腦
      String s =  "";
      char c;
      while((c = BTSerial.read()) !='\n'){
          s += c;
      }      
        strcpy(str, s.c_str());
        Serial.println(s); 
    } 

    delay(10);
}

void BluetoothLisent(){
    //Bluetooth Start 

    int insize, ii;  
    int tick=0;
    while ( tick<MAX_BTCMDLEN ) { // 因為包率同為9600, Android送過來的字元可能被切成數份
        if ( (insize=(BTSerial.available()))>0 ){ // 讀取藍牙訊息
            for ( ii=0; ii<insize; ii++ ){
                cmd[(len++)%MAX_BTCMDLEN]=char(BTSerial.read());
            }
        } else {
            tick++;
        }
    }
    if ( len ) { // 用串列埠顯示從Android手機傳過來的訊息
        
        Serial.println(str);
        cmd[0] = '\0';
    }
    len = 0;

    delay(5);
}

