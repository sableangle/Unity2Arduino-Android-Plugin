#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>
/*/Acc
MODULE`                Arduino
GND                    GND
VCC                    +3.3V
CS                     +3.3V*
INT1                   N/A
INT2                   N/A
SD0                    N/A
SDA                    A4*
SCL                    A5*
*/
/* Alternate I2C address of the module */
#define I2C_Add 0x53
/* ADXL345 register addresses */
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define X_Axis 0x32
#define Y_Axis 0x34
#define Z_Axis 0x36
/* Accelerometer range modes */
#define RANGE_2g 0
#define RANGE_4g 1
#define RANGE_8g 2
#define RANGE_16g 3


// 儲存加速度數值用的變數
// 基準，用來計算 error 以及控制用
float standardAcceData[3];

//目前的加速度
float currentAcceData[3];

//Motor
// 有提供 pwm 的腳位
// 支援 PWM 的 pin 腳
int motor_pin[4] = {
  3, 5, 9, 10};

Servo motor[4];
// 四個油門（出力）
int throttle[4] = {
  20, 20 ,20 ,20};

// for PID
int errorX;
int errorY;
double tFix[4];
float Kp;


//Bluetooth
// the maximum received command length from an Android system (over the bluetooth)
#define MAX_BTCMDLEN 128
char str[MAX_BTCMDLEN];
String mStr[20];
// 建立一個軟體模擬的序列埠; 不要接反了!
// HC-06    Arduino
// TX       RX/Pin7
// RX       TX/Pin8
SoftwareSerial BTSerial(7,8); // Arduino RX/TX

//PWM
//3 5 6 9 10 11




byte cmd[MAX_BTCMDLEN]; // received 128 bytes from an Android system
int len = 0; // received command length

void setup() {
    /* Initialise the I2C bus */
    Wire.begin(); 
    Serial.begin(9600);   // Arduino起始鮑率：9600
    BTSerial.begin(9600); // HC-06 出廠的鮑率：每個藍牙晶片的鮑率都不太一樣，請務必確認

    Kp = 15;

    int c;
    // 初始化修正輸出量
    for(c=0; c<4; c++){
      tFix[c] = 0;
    }
    
    /* Initialise the ADXL345 */ 
    Init_ADXL345(RANGE_2g);


    // 初始化油門輸出量，馬達對應到的腳位
    for(c=0; c<4; c++){
      throttle[c] = 15;
      motor[c].attach(motor_pin[c]);
    }
    // 讓他在低檔位大約 5 秒鐘
    int d;
    for(d=0; d<500; d++){
      sendSignalToMotor();
      delay(10);
    }
    initSensorValue() ;

    
}

void loop() {
    int c;
    //BluetoothLisent();
    BluetoothLisent3();

    if(mStr[0] == "f"){
      
      int Mspeed = mStr[1].toInt()*10;
      Kp = mStr[2].toInt()*0.005;
      SetPower(Mspeed+50);
      GetAllAxis(); 
      calculatePID();
      sendSignalToMotor();
      Serial.println(Kp);
      Serial.println(Mspeed);
    }
    if(mStr[0] == "b"){
      SetPower(15);
    }
}

void SetPower(int mainPower){

    for(int c=0; c<4; c++){
      throttle[c] = mainPower;
      motor[c].attach(motor_pin[c]);
    }
}

void calculatePID() {
  int c;

  // 初始化 修正輸出量
  for(c=0; c<4; c++){
    tFix[c] = 0;
  }

  // calculate error
  errorX = currentAcceData[0] - standardAcceData[0];
  errorY = currentAcceData[1] - standardAcceData[1];

  // P control (X axis)
  tFix[0] -= Kp * errorX;
  tFix[1] += Kp * errorX;
  tFix[2] += Kp * errorX;
  tFix[3] -= Kp * errorX;

  // P control (Y axis)
  tFix[0] += Kp * errorY;
  tFix[1] += Kp * errorY;
  tFix[2] -= Kp * errorY;
  tFix[3] -= Kp * errorY;

}

void initSensorValue() {
  // init acceleration data
  GetAllAxis();
  int c;
  for(c=0; c<3; c++){
    standardAcceData[c] = currentAcceData[c];
  }
  
}

void GetAllAxis(){
    currentAcceData[0] = Read_Axis(X_Axis);
    currentAcceData[1] = Read_Axis(Y_Axis);
    currentAcceData[2] = Read_Axis(Z_Axis);
    //Serial.println("X:" );
    //Serial.println(currentAcceData[0] );
    //Serial.println("Y:" );
    //Serial.println(currentAcceData[1] );
   // Serial.println("Z:" );
    //Serial.println(currentAcceData[2] );
}

void sendSignalToMotor() {
  int c;
  for(c=0; c<4; c++) {
    int fix = (int)tFix[c];
    motor[c].write(throttle[c] + fix);
  }
}

void BluetoothLisent3(){
   if (BTSerial.available() >0) {      // 藍芽模組 回傳 的 訊息 給電腦
      String s =  "";
      char c;
      int i = 0;
      
      for(int x=0;x<20;x++){
        mStr[x] = "";
      }
      
      while((c = BTSerial.read()) !='\n'){
          if(c != ','){
            s += c;
          }
          else{
           mStr[i] = s;
           s="";
           i++;
          }
      }      
        //strcpy(str, s.c_str());
        //Serial.println(mStr[1]);
        //Serial.println(str);  
    }
    

    delay(10);
}

void BluetoothLisent2(){
   if (BTSerial.available() >0) {      // 藍芽模組 回傳 的 訊息 給電腦
      String s =  "";
      char c;
      while((c = BTSerial.read()) !='\n'){
          s += c;
      }      
        strcpy(str, s.c_str());
        //Serial.println(s);
        //Serial.println(str);  
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
        
        //Serial.println(str);
        cmd[0] = '\0';
    }
    len = 0;

    delay(5);
}



/* Read one of the 3 axis via the I2C interface */
int Read_Axis(byte axis)
{
  int Data;
   
  Wire.beginTransmission(I2C_Add);
  Wire.write(axis);
  Wire.endTransmission();
 
  Wire.beginTransmission(I2C_Add);
  Wire.requestFrom(I2C_Add, 2);
 
  /* If data is available then read it (2 bytes) */
  if(Wire.available())     
  {
    Data = (int)Wire.read();
    Data = Data  | (Wire.read() << 8);
  }else
  {
    Data = 0;
  }
   
  Wire.endTransmission(); 
  return Data;
}

/* Initialise the ADXL345 */
void Init_ADXL345(byte range)
{
  Wire.beginTransmission(I2C_Add);
 
  /* Set the sensitivity of the module */
  Wire.write(DATA_FORMAT);
  Wire.write(range);
  Wire.endTransmission();
 
  /* Put the module into measurement mode to start taking measurements */
  Wire.beginTransmission(I2C_Add);
  Wire.write(POWER_CTL);
  Wire.write(0x08);
 
  Wire.endTransmission();
}
