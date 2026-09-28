// #define WIFI_SSID "Patcher_WIFI_2.4G"
// #define WIFI_PASSWORD "0825408633"
// #define REFERENCE_URL "https://arduino-wifi-001-default-rtdb.asia-southeast1.firebasedatabase.app"
// #define AUTH_TOKEN "KLosZ4m1nsPm4YSQ7R3nbrfHl3ncEExpRY8beADO"
#include "secrets.h"
/////////// clock
#include "RTClib.h"  // https://github.com/adafruit/RTClib
RTC_DS3231 RTC;      // Setup an instance of DS3231  naming it RTC


#include "Firebase.h"
#include <ArduinoJson.h>

/* Use the following instance for Locked Mode (With Authentication) */
Firebase fb(REFERENCE_URL, AUTH_TOKEN);


////////////// GPS
//#include <TinyGPS++.h>  // install  https://github.com/mikalhart/TinyGPSPlus
//#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>

//TinyGPSPlus gps;
//SoftwareSerial gpsSerial(8, 9); // RX, TX

String BasePathSetiing1 = "Example_1";
String BasePathSetiing2 = "Example_2";


char setting_1 = 'A';
char setting_2 = 'B';

char calculatePlan_1 = 'B';
char calculatePlan_2 = 'A';
int totalRoundDayPlan_1 = 3;
int totalRoundDayPlan_2 = 3;
int totalValvePlan_1 = 7;
int totalValvePlan_2 = 0;
float irrigationPlan_1 = 0;
float irrigationPlan_2 = 0;
int currentValvePlan_1 = 1;
int currentValvePlan_2 = 1;


int currentValve = 1;
int totalValve = 7;
boolean statusValve = false;

float etWeek[7] = { 0, 0, 0, 0, 0, 0, 0 };
double rainWeek[7] = { 0, 0, 0, 0, 0, 0, 0 };
double kcWeek_1[7] = { 0, 0, 0, 0, 0, 0, 0 };
double kcWeek_2[7] = { 0, 0, 0, 0, 0, 0, 0 };


long timeStopPlan_1 = 0;
long timeStopPlan_2 = 0;

bool activeValvePlan_1 = false;
bool activeValvePlan_2 = false;
long currentTime = 0;

/// config
int setting_1_count = 0;
int setting_2_count = 0;
int idxCountDay = 0;
float maxTemp = 0;
float minTemp = 10000;
int area = 1;
int pumpRate;
int dap_day = 0, dap_month = 0, dap_year = 0;
int time_h = 0, time_m = 0, time_s = 0;

// current time
int currentDay = 0;
int currentMonth = 0;
int currentYear = 0;
int currentHour = 0;
int currentMinute = 0;
int currentSecond = 0;

//
//int gpsDay=0;
//int gpsMonth =0;
//int gpsYear=0;
//int gpsHour=0;
//int gpsMinute=0;
//int gpsSecond=0;
const int SD_CS = 10;
unsigned long lastWrite = 0;

////////////// lcd
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27, 20, 4);
// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display





////////// ค่าอุณหภูมิ T/h
#include "DHT.h"
#define TH_in_pin 5  // ขา 5 ต่อ T/H in
#define TH_ex_pin 4  // ขา 4 ต่อ T/H ex

#define DHTTYPE DHT22  // DHT 22  (AM2302)

DHT TH_in(TH_in_pin, DHTTYPE);
DHT TH_ex(TH_ex_pin, DHTTYPE);

float H_IN = 0;
float T_IN = 0;
float H_EX = 0;
float T_EX = 0;


///// TM1637 แสดงเลขวาล์ว 4 หลัก

#include <TM1637Display.h>
/*
ใช้ Library ของคุณ Avishay Orpaz ซึ่งสามารถเพิ่มได้จาก 
Arduino IDE -> Tools -> Manage Libraries -> พิมพ์ว่า TM1637 
 เลือก TM1637 by Avishay Orpazt
*/

// Module connection pins (Digital Pins)
#define CLK 6
#define DIO 7

TM1637Display DisplayValve(CLK, DIO);




///////// ปริมาณน้ำฝน

int interruptPin = 2;  ///  ขา 2
float rainDay = 0;
int RainState = 0;
volatile int rainCount = 0;
volatile bool newRainDetected = false;

//// สถานะต่าง ๆ
boolean FanState = 0;          ///  สถานะพัดลม  : 0  หยุดการทำงาน ,1 กำลังทำงาน
boolean ErrorState = 0;        ///  สถานะไฟแจ้งเตือน :  0  หยุดการทำงาน ,1 กำลังทำงาน
boolean PumpState = 0;         /// สถานะปั๊ม : 0  หยุดการทำงาน ,1 กำลังทำงาน
boolean RelayOptionState = 0;  /// สถานะRelayOption : 0  หยุดการทำงาน ,1 กำลังทำงาน


////////  analog sensor
int VoltageBattPin = A0;   // ต่อแบ่งแรงดัน 1023 = 15V
int VoltageBattValue = 0;  // variable to store the value coming from the sensor
float VoltageBatt = 0.0;   // แสดงแรงดันแบตเตอรี่ 0-15V

const int ModeSwitch1 = A1;  // FM1  ต่อ A1
const int ModeSwitch2 = A2;  // FM2 Analog input pin that the potentiometer is attached to
int Mode1 = 1;               /// mode switch
int Mode2 = 1;               /// mode switch

///  Flow Sensor  เซ็นเซอร์น้ำไหล
int FlowSensor_pin = A3;
boolean FlowState = 0;  /// 1 = น้ำไหล ,0 = น้ำไม่ไหล



/***********  พอร์ทเสิรม  sensor port option ยัีงไม่ต่อใช้งาน ***************  
int SS_OptionPin1 = A1;  // 1023 = 5V 
int SS_OptionPin2 = A2;  // 1023 = 5V 
int SS_OptionPin3 = A3;  // 1023 = 5V 
*************************************************/

///////////////// คุม relay
#include <PCF8574.h>       ///  ติดตั้ง PCF8574 library by Rob Tillaart   https://github.com/RobTillaart/PCF8574
PCF8574 Relay8ch_1(0x20);  // Relay 8 ch #1    ***** PCF8574T คุมวาล์ว 1-7  + pump
PCF8574 Relay8ch_2(0x21);  // Relay 8 ch #2    ***** PCF8574T Option
PCF8574 Relay4ch(0x22);    // Relay 4 ch       ***** PCF8574T  คุมพัดลมไฟแสดงผล

/*
Relay 8 ch #1
P0 VALVE  2
P1 VALVE  1
P2 pump   Control C NO NC 
P3 VALVE  3
P4 VALVE  7
P5 VALVE  6
P6 VALVE  5
P7 VALVE  1
     
*/
/*
Relay 8 ch #2
P7        P7       
P6        P6
P5        P5
P4        P4
P3        P3
P2        P2
P1        P1
P0        P0
*/

/*
Relay 4 ch 
P0        Pump Lamp  (PL)
P1        Flow Lamp   (FL)
P2        Error Lamp  (AL)  
P3        FAN 


//********  option *****
P4        ขั้วต่อภายนอก
P5        ขั้วต่อภายนอก
P6        ขั้วต่อภายนอก
P7        ขั้วต่อภายนอก

*/


boolean PUMP_State = 0;
boolean V1_State = 0;
boolean V2_State = 0;
boolean V3_State = 0;
boolean V4_State = 0;
boolean V5_State = 0;
boolean V6_State = 0;
boolean V7_State = 0;


int NumberValveRun = 0;  /// หมายเลขวาล์วที่กำไลังทำงาน  0-7   : 0 ไม่มีตัวไหนทำงาน

/////////////



///////////////////////////////////////////////////// ฟังชั่น ///////////////////////////////////////////////////

void DisplayValveNumber(int v)  //// แสดงเลขวาล์ว ออกจอ 4 หลัก
{
  DisplayValve.showNumberDec(NumberValveRun, false, 2, 1);  // Expect: __4_
}




void PumpControl(boolean State) {

  if (State == 1)  /// on
  {
    Relay8ch_1.write(2, LOW);  /// LOW = on
    Relay4ch.write(0, LOW);    /// LOW = on
    PumpState = 1;             // สถานะกำลังทำงาน
  } else                       // off
  {
    Relay8ch_1.write(2, HIGH);  /// HIGH = off
    Relay4ch.write(0, HIGH);    /// HIGH = off
    PumpState = 0;              // สถานะหยุดการทำงาน
  }
}


//////////////// control valve 1-14




void ValveControl(int Valve, boolean State) {

  //////////// V1
  if (Valve == 1) {
    if (State == 1)  /// on
    {
      Relay8ch_1.write(1, LOW);
      V1_State = 1;  /// LOW = on
      NumberValveRun = 1;
    } else  // off
    {
      Relay8ch_1.write(1, HIGH);
      V1_State = 0;  /// HIGH = off
      NumberValveRun = 0;
    }

  }

  ///////////// V2
  else if (Valve == 2) {
    if (State == 1)  /// on
    {
      Relay8ch_1.write(0, LOW);
      V2_State = 1;  ///  LOW = on
      NumberValveRun = 2;
    } else  // off
    {
      Relay8ch_1.write(0, HIGH);
      V2_State = 0;  /// HIGH = off
      NumberValveRun = 0;
    }

  }

  ///////////// V3
  else if (Valve == 3) {
    if (State == 1)  /// on
    {
      Relay8ch_1.write(3, LOW);
      V3_State = 1;  /// LOW = on
      NumberValveRun = 3;
    } else  /// off
    {
      Relay8ch_1.write(3, HIGH);
      V3_State = 0;  /// HIGH = off
      NumberValveRun = 0;
    }

  }

  ///////////// V4
  else if (Valve == 4) {
    if (State == 1)  /// on
    {
      Relay8ch_1.write(7, LOW);
      V4_State = 1;  /// LOW = on
      NumberValveRun = 4;
    } else  /// off
    {
      Relay8ch_1.write(7, HIGH);
      V4_State = 0;  /// HIGH = off
      NumberValveRun = 0;
    }

  }

  ///////////// V5
  else if (Valve == 5) {
    if (State == 1)  /// on
    {
      Relay8ch_1.write(6, LOW);
      V5_State = 1;  /// LOW = on
      NumberValveRun = 5;
    } else  /// off
    {
      Relay8ch_1.write(6, HIGH);
      V5_State = 0;  /// HIGH = off
      NumberValveRun = 0;
    }

  }


  ///////////// V6
  else if (Valve == 6) {
    if (State == 1)  /// on
    {
      Relay8ch_1.write(5, LOW);
      V6_State = 1;  /// LOW = on
      NumberValveRun = 6;
    } else  /// off
    {
      Relay8ch_1.write(5, HIGH);
      V6_State = 0;  /// HIGH = off
      NumberValveRun = 0;
    }

  }

  ///////////// V7
  else if (Valve == 7) {
    if (State == 1)  /// on
    {
      Relay8ch_1.write(4, LOW);
      V7_State = 1;  /// LOW = on
      NumberValveRun = 7;
    } else  /// off
    {
      Relay8ch_1.write(4, HIGH);
      V7_State = 0;  /// HIGH = off
      NumberValveRun = 0;
    }
  }



  DisplayValveNumber(NumberValveRun);  /// แสดงหมายเลขวาล์ว
}




/*
P0 pump lamp  (PL) 
P1 Flow lamp  (FL) 
P2 Error Lamp (AL)
P3 FAN

*/




void FanControl(boolean State)  // State  : 0 = off , 1 = on
{
  if (State == 1)  /// on
  {
    Relay4ch.write(3, LOW);  /// relay 4 ch  IN1 :   Fan    : LOW = on
    FanState = 1;            // สถานะพัดลม  : 0  หยุดการทำงาน ,1 กำลังทำงาน
  } else {
    Relay4ch.write(3, HIGH);  /// relay 4 ch IN1 :   Fan    : HIGH = off
    FanState = 0;             // สถานะพัดลม  : 0  หยุดการทำงาน ,1 กำลังทำงาน
  }
}



void ErrorLampControl(boolean State)  // State  : 0 = off , 1 = on
{
  if (State == 1)  /// on
  {
    Relay4ch.write(2, LOW);  ///  LOW = on
    ErrorState = 1;          ///  สถานะไฟแจ้งเตือน :  0  หยุดการทำงาน ,1 กำลังทำงาน

  } else {
    Relay4ch.write(2, HIGH);  ///  HIGH = off

    ErrorState = 0;  ///  สถานะไฟแจ้งเตือน :  0  หยุดการทำงาน ,1 กำลังทำงาน
  }
}

void PumpLampControl(boolean State)  // State  : 0 = off , 1 = on
{
  if (State == 1)  /// on
  {
    Relay4ch.write(0, LOW);  /// LOW = on
                             //// PumpState สถานะ อยู่ที่ pumpcontrol แล้ว

  } else {
    Relay4ch.write(0, HIGH);  /// HIGH = off
                              //// PumpState สถานะ อยู่ที่ pumpcontrol แล้ว
  }
}


void FlowLampControl(boolean State)  // State  : 0 = off , 1 = on
{

  if (State == 1)  /// on
  {
    Relay4ch.write(1, LOW);  /// LOW = on
                             //// สถานะ อยู่ที่ pumpcontrol แล้ว
  } else {
    Relay4ch.write(1, HIGH);  /// HIGH = off
  }
}


/*****************  ฟังชั่นเสริม สำฟรับพอร์ท pcf8574 ที่ไปคุมรีเลย์ 4 ตัว แล้วยังเหลือพอร์ทอีก 4 ช่อง    


void PcfOptionP4Control(boolean State) // State  : 0 = off , 1 = on
{

  if(State==1) /// on
  {
    Relay4ch.digitalWrite(P4, LOW);   ///  P4 :    : LOW = on 
  }  
  else
  {
    Relay4ch.digitalWrite(P4,HIGH);   ///  P4 :    : HIGH = off 
  }  
 
}

void PcfOptionP5Control(boolean State) // State  : 0 = off , 1 = on
{

  if(State==1) /// on
  {
    Relay4ch.digitalWrite(P5, LOW);   ///  P5 :    : LOW = on 
  }  
  else
  {
    Relay4ch.digitalWrite(P5,HIGH);   ///  P5 :    : HIGH = off 
  }  
 
}

void PcfOptionP6Control(boolean State) // State  : 0 = off , 1 = on
{

  if(State==1) /// on
  {
    Relay4ch.digitalWrite(P6, LOW);   ///  P6 :    : LOW = on 
  }  
  else
  {
    Relay4ch.digitalWrite(P6,HIGH);   ///  P6 :    : HIGH = off 
  }  
 
}

void PcfOptionP7Control(boolean State) // State  : 0 = off , 1 = on
{

  if(State==1) /// on
  {
    Relay4ch.digitalWrite(P7, LOW);   ///  P7 :    : LOW = on 
  }  
  else
  {
    Relay4ch.digitalWrite(P7,HIGH);   ///  P7 :    : HIGH = off 
  }  
 
}



*/






void CountRain() {
  rainCount += 1;
  newRainDetected = true;
}
void readVoltageBatt() {

  VoltageBattValue = analogRead(VoltageBattPin);
  VoltageBatt = VoltageBattValue * 0.014765;  /// ....1023 = 15V. *0.014  ค่าปรับแต่ง 0.014765
}





void ShowSerial() {
  DateTime now = RTC.now();
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year() + 543, DEC);  /// +543 สำหรับ พ.ศ.
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);


  // ค่า T/H นอกใน
  Serial.print("EX H = ");
  Serial.print(H_EX, 1);
  Serial.print("%\t");
  Serial.print("T = ");
  Serial.print(T_EX, 1);
  Serial.println(" C");
  Serial.print("IN H = ");
  Serial.print(H_IN, 1);
  Serial.print("%\t");
  Serial.print("T = ");
  Serial.print(T_IN, 1);
  Serial.println(" C");


  // ค่าน้ำฝน
  //Serial.print(",");
  Serial.print(rainCount);  //// RainCount


  // สถานะวาล์ว ที่กำลังทำงาน
  Serial.print(",");
  Serial.print(NumberValveRun);

  //
  // สถานะปั๊ม
  Serial.print(",");
  Serial.print(PumpState);

  // สถานะพัดลม
  Serial.print(",");
  Serial.print(FanState);

  // สถานะไฟ ara=0-796845
  Serial.print(",");
  Serial.print(ErrorState);


  // ค่า flow sensor
  Serial.print(",");
  Serial.print(FlowState);  /// 1 น้ำไหล ,0 น้ำไม่ไหล

  // ค่าแรงดันแบตเตอรี่
  Serial.print(",");

  Serial.print(VoltageBatt, 2);
  Serial.print("V,");


  // โหมด
  Serial.print(Mode1);
  Serial.print(",");
  Serial.println(Mode2);

  Serial.print("idxCountDay::");
  Serial.println(idxCountDay);
}

void ShowLCD() {
  //              char dateStr[21];
  //      sprintf(dateStr, "%02d/%02d/%04d",
  //              gps.date.day(),
  //              gps.date.month(),
  //              gps.date.year()+543);
  //      lcd.setCursor(0,0);
  //      lcd.print(dateStr);
  //
  //        char timeStr[21];
  //      int hour = gps.time.hour() + 7; // ปรับเป็นเวลาไทย (GMT+7)
  //      if (hour >= 24) hour -= 24;
  //      sprintf(timeStr, "%02d:%02d:%02d",
  //              hour,
  //              gps.time.minute(),
  //              gps.time.second());
  //
  //      lcd.setCursor(12,0);
  //      lcd.print(timeStr);


  lcd.setCursor(0, 0);
  if (currentDay < 10) {
    lcd.print(F("0"));
    lcd.print(currentDay);
  } else {
    lcd.print(currentDay);
  }
  lcd.print(F("/"));

  if (currentMonth < 10) {
    lcd.print(F("0"));
    lcd.print(currentMonth);
  } else {
    lcd.print(currentMonth);
  }
  lcd.print(F("/"));

  if (currentYear < 10) {
    lcd.print(F("0"));
    lcd.print(currentYear);
  } else {
    lcd.print(currentYear);
  }

  lcd.setCursor(12, 0);
  //lcd.print(now.min, DEC);
  if (currentHour < 10) {
    lcd.print(F("0"));
    lcd.print(currentHour);
  } else {
    lcd.print(currentHour);
  }
  lcd.print(F(":"));

  if (currentMinute < 10) {
    lcd.print(F("0"));
    lcd.print(currentMinute);
  } else {
    lcd.print(currentMinute);
  }
  lcd.print(F(":"));


  //lcd.print(now.sec, DEC);
  if (currentSecond < 10) {
    lcd.print(F("0"));
    lcd.print(currentSecond);
  } else {
    lcd.print(currentSecond);
  }


  //////////// TH
  lcd.setCursor(0, 1);
  lcd.print(T_EX);
  lcd.print("C");
  lcd.print(" ");
  lcd.print(H_EX);
  lcd.print("%");
  lcd.print(" ");
  lcd.print(T_IN);
  lcd.print("C");


  //////////// rain
  lcd.setCursor(12, 2);
  lcd.print("      ");  //// clear
  lcd.setCursor(12, 2);
  lcd.print(rainCount * 0.5);
  lcd.setCursor(18, 2);
  lcd.print("mL");


  ///////////mode swith
  lcd.setCursor(0, 2);
  lcd.print("    ");  //// clear
  lcd.setCursor(0, 2);
  lcd.print(Mode1);
  lcd.setCursor(2, 2);
  lcd.print("    ");  //// clear
  lcd.setCursor(2, 2);
  lcd.print(Mode2);


  //////////// status

  // lcd.setCursor(0,3); lcd.print("idx "); lcd.print(idxCountDay);       // สถานะปั๊ม         1 ทำงาน  0 หยุด
  // lcd.setCursor(1,3); lcd.print(FanState);        //  สถานะพัดลม      1 ทำงาน  0 หยุด
  // lcd.setCursor(2,3); lcd.print(ErrorState);      // สถานะไฟ Error    1 แจ้งเตือน 0 ดับ
  // lcd.setCursor(3,3); lcd.print(FlowState);       // ค่า flow sensor   1 น้ำไหล  0 น้ำไม่ไหล
  // lcd.setCursor(5,3); lcd.print(NumberValveRun);    // สถานะวาล์ว ที่กำลังทำงาน

  lcd.setCursor(8, 3);
  lcd.print(VoltageBatt, 1);  // ค่าแรงดันแบตเตอรี่
  lcd.setCursor(13, 3);
  lcd.print("Process");
}



int limitTemp(int temp) {
  if (temp >= 45) {
    return 45;
  } else if (temp <= 20) {
    return 20;
  } else {
    return temp;
  }
}


void readTH() {
  //// ตัวภายใน
  T_IN = TH_in.readTemperature();
  H_IN = TH_in.readHumidity();

  //// ตัวภายใน
  T_EX = TH_ex.readTemperature();
  H_EX = TH_ex.readHumidity();

  ////  อ่านเฉพาะอุณหภูมิภายใน
  if (T_IN < minTemp) {
    minTemp = limitTemp(T_IN);
  }
  if (T_IN > maxTemp) {
    maxTemp = limitTemp(T_IN);
  }

  // อ่านเฉพาะอุณหภูมิภายนอก
  //  if (T_EX < minTemp) {
  //    minTemp = limitTemp(T_EX );
  //  }
  //  if (T_EX > maxTemp  ) {
  //    maxTemp = limitTemp (T_EX);
  //  }
  //-----------
}




void DemoTestIO() {

  //// pump
  lcd.setCursor(0, 1);
  lcd.print("PUMP on");
  PumpControl(1);  // PUMP on
  delay(1000);

  lcd.setCursor(0, 1);
  lcd.print("PUMP off");
  PumpControl(0);  // PUMP off
  delay(1000);

  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
                                      /// V1
  lcd.setCursor(0, 1);
  lcd.print("Valve 1 on");
  ValveControl(1, 1);  /// Valve on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Valve 1 off");
  ValveControl(1, 0);  /// Valve off
  delay(1000);

  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
                                      /// V2
  lcd.setCursor(0, 1);
  lcd.print("Valve 2 on");
  ValveControl(2, 1);  /// Valve on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Valve 2 off");
  ValveControl(2, 0);  /// Valve off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
                                      /// V3
  lcd.setCursor(0, 1);
  lcd.print("Valve 3 on");
  ValveControl(3, 1);  /// Valve on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Valve 3 off");
  ValveControl(3, 0);  /// Valve off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
                                      /// V4
  lcd.setCursor(0, 1);
  lcd.print("Valve 4 on");
  ValveControl(4, 1);  /// Valve on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Valve 4 off");
  ValveControl(4, 0);  /// Valve off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  /// V5
  lcd.setCursor(0, 1);
  lcd.print("Valve 5 on");
  ValveControl(5, 1);  /// Valve on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Valve 5 off");
  ValveControl(5, 0);  /// Valve off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
                                      /// V6
  lcd.setCursor(0, 1);
  lcd.print("Valve 6 on");
  ValveControl(6, 1);  /// Valve on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Valve 6 off");
  ValveControl(6, 0);  /// Valve off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
                                      /// V7
  lcd.setCursor(0, 1);
  lcd.print("Valve 7 on");
  ValveControl(7, 1);  /// Valve on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Valve 7 off");
  ValveControl(7, 0);  /// Valve off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data


  //// relay 4 ch

  /// fan
  lcd.setCursor(0, 1);
  lcd.print("Fan on");
  FanControl(1);  /// on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Fan off");
  FanControl(0);  /// off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
                                      /// Error Lamp
  lcd.setCursor(0, 1);
  lcd.print(" Error Lamp on");
  ErrorLampControl(1);  /// on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Error Lamp off");
  ErrorLampControl(0);  /// off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
                                      /// Pump Lamp
  lcd.setCursor(0, 1);
  lcd.print("Pump Lamp on");
  PumpLampControl(1);  /// on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Pump Lamp off");
  PumpLampControl(0);  /// off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data

  /// FlowLamp
  lcd.setCursor(0, 1);
  lcd.print("Flow Lamp on");
  FlowLampControl(1);  /// on
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Flow Lamp off");
  FlowLampControl(0);  /// off
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                    ");  /// clear data
}



void Readflow() {
  //ใช้ขา A3 ในการอ่านค่า   <512 = น้ำไหล , > 512 = น้ำไม่ไหล

  if (analogRead(FlowSensor_pin) > 512)  /// น้ำไม่ไหล
  {
    FlowLampControl(0);  /// ไฟดับ
    FlowState = 0;

  } else  /// น้ำไหล
  {
    FlowLampControl(1);  /// ไฟสว่าง
    FlowState = 1;
  }
}


void raincount_setup() {

  ///////////  rain count
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), CountRain, FALLING);  ///  ของเดิม attachInterrupt(0,CountRain,FALLING); ใช้ไม่ได้กับ UNO R4

  // เลข 0 คือ Interrupt หมายเลข 0 จะอยู่ที่ ขา 2 , เลข 1 คือ Interrupt หมายเลข 1 จะอยู่ที่ ขา 3

  // CountRain  คือ ชื่อฟังชันที่จะให้ไปทำ
  /*


    LOW to trigger the interrupt whenever the pin is low,

    CHANGE to trigger the interrupt whenever the pin changes value

    RISING to trigger when the pin goes from low to high,

    FALLING for when the pin goes from high to low.

The Due, Zero and MKR1000 boards allow also:

    HIGH to trigger the interrupt whenever the pin is high

*/
}



void Relay_setup() {

  Relay8ch_1.begin();  //
  Relay8ch_2.begin();  //
  Relay4ch.begin();    //
}

void DS3231_setup() {
  Wire.begin();  // Start the I2C
  RTC.begin();   // Init RTC
  // RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));  // กำหนดเวลาจากคอม
  // Serial.print('Time and date set');
}



void ReadModeSwitch1() {
  int sensorValue = 0;  // value read from the pot


  sensorValue = analogRead(ModeSwitch1);

  /// ตัดค่าช่วง
  if (sensorValue >= 0 && sensorValue < 46) { Mode1 = 1; }  // ปกติอ่านได้ 0
  else if (sensorValue >= 47 && sensorValue < 139) {
    Mode1 = 2;
  }                                                                 //  ปกติอ่านได้ 92
  else if (sensorValue >= 140 && sensorValue < 232) { Mode1 = 3; }  //  ปกติอ่านได้ 186
  else if (sensorValue >= 233 && sensorValue < 326) {
    Mode1 = 4;
  }                                                                 //  ปกติอ่านได้ 279
  else if (sensorValue >= 327 && sensorValue < 419) { Mode1 = 5; }  //  ปกติอ่านได้ 372
  else if (sensorValue >= 420 && sensorValue < 513) {
    Mode1 = 6;
  }                                                                 //  ปกติอ่านได้ 466
  else if (sensorValue >= 514 && sensorValue < 606) { Mode1 = 7; }  //  ปกติอ่านได้ 559
  else if (sensorValue >= 607 && sensorValue < 699) {
    Mode1 = 8;
  }  //  ปกติอ่านได้ 652
  else if (sensorValue >= 700 && sensorValue < 793) {
    Mode1 = 9;
  }  //  ปกติอ่านได้ 746
  else if (sensorValue >= 794 && sensorValue < 885) {
    Mode1 = 10;
  }                                                                  //  ปกติอ่านได้ 839
  else if (sensorValue >= 886 && sensorValue < 977) { Mode1 = 11; }  //  ปกติอ่านได้ 931
  else if (sensorValue >= 978 && sensorValue < 1024) {
    Mode1 = 12;
  }  //  ปกติอ่านได้ 1023

  //Serial.print("sensor = ");
  // Serial.print(sensorValue);
  // Serial.print("\t output = ");
}

void ReadModeSwitch2() {
  int sensorValue = 0;  // value read from the pot


  sensorValue = analogRead(ModeSwitch2);

  /// ตัดค่าช่วง
  if (sensorValue >= 0 && sensorValue < 46) {
    Mode2 = 1;
  }  // ปกติอ่านได้ 0
  else if (sensorValue >= 47 && sensorValue < 139) {
    Mode2 = 2;
  }                                                                 //  ปกติอ่านได้ 92
  else if (sensorValue >= 140 && sensorValue < 232) { Mode2 = 3; }  //  ปกติอ่านได้ 186
  else if (sensorValue >= 233 && sensorValue < 326) {
    Mode2 = 4;
  }                                                                 //  ปกติอ่านได้ 279
  else if (sensorValue >= 327 && sensorValue < 419) { Mode2 = 5; }  //  ปกติอ่านได้ 372
  else if (sensorValue >= 420 && sensorValue < 513) {
    Mode2 = 6;
  }                                                                 //  ปกติอ่านได้ 466
  else if (sensorValue >= 514 && sensorValue < 606) { Mode2 = 7; }  //  ปกติอ่านได้ 559
  else if (sensorValue >= 607 && sensorValue < 699) {
    Mode2 = 8;
  }                                                                 //  ปกติอ่านได้ 652
  else if (sensorValue >= 700 && sensorValue < 793) { Mode2 = 9; }  //  ปกติอ่านได้ 746
  else if (sensorValue >= 794 && sensorValue < 885) {
    Mode2 = 10;
  }                                                                  //  ปกติอ่านได้ 839
  else if (sensorValue >= 886 && sensorValue < 977) { Mode2 = 11; }  //  ปกติอ่านได้ 931
  else if (sensorValue >= 978 && sensorValue < 1024) {
    Mode2 = 12;
  }  //  ปกติอ่านได้ 1023
}


// ==== UTILS ====

void parseDate(char *value) {
  char *token;

  token = strtok(value, "/");
  if (token) dap_day = atoi(token);

  token = strtok(NULL, "/");
  if (token) dap_month = atoi(token);

  token = strtok(NULL, "/");
  if (token) dap_year = atoi(token);
}

void parseTime(char *value) {
  char *token;

  token = strtok(value, ":");
  if (token) time_h = atoi(token);

  token = strtok(NULL, ":");
  if (token) time_m = atoi(token);

  token = strtok(NULL, ":");
  if (token) time_s = atoi(token);
}

void trim(char *str) {
  // remove leading spaces
  while (*str == ' ') {
    memmove(str, str + 1, strlen(str));
  }

  // remove trailing spaces + newline
  int len = strlen(str);
  while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\r' || str[len - 1] == '\n')) {
    str[len - 1] = '\0';
    len--;
  }
}

void parseArray(char *value, float *arr, int *count) {
  *count = 0;
  char *token = strtok(value, ",");

  while (token != NULL) {
    trim(token);
    arr[*count] = atoi(token);
    (*count)++;
    token = strtok(NULL, ",");
  }
}

// ==== PARSER ====

void parseLine(char *line) {
  char *key = strtok(line, "=");
  char *value = strtok(NULL, "=");

  if (!key || !value) return;

  trim(key);
  trim(value);

  // ==== MATCH KEYS ====

  if (strcmp(key, "total_valve_plan_1") == 0) {
    totalValvePlan_1 = atoi(value);
  } else if (strcmp(key, "total_valve_plan_2") == 0) {
    totalValvePlan_2 = atoi(value);
  } else if (strcmp(key, "calculate_plan_1") == 0) {

    calculatePlan_1 = value[0];
  } else if (strcmp(key, "calculate_plan_2") == 0) {

    calculatePlan_2 = value[0];
  } else if (strcmp(key, "round_day_plan_1") == 0) {
    totalRoundDayPlan_1 = atoi(value);
  } else if (strcmp(key, "round_day_plan_2") == 0) {
    totalRoundDayPlan_2 = atoi(value);
  } else if (strcmp(key, "setting_1") == 0) {
    setting_1 = value[0];
    //    parseArray(value, setting_1, &setting_1_count);
  } else if (strcmp(key, "setting_2") == 0) {
    setting_2 = value[0];
    //    parseArray(value, setting_2, &setting_2_count);
  } else if (strcmp(key, "area") == 0) {
    area = atol(value);
  } else if (strcmp(key, "pumprate") == 0) {
    pumpRate = atoi(value);
  } else if (strcmp(key, "dap") == 0) {
    parseDate(value);
  } else if (strcmp(key, "time") == 0) {
    parseTime(value);
  }
}

// ==== READ FILE ====

bool readConfigWithRetry(int maxRetry) {
  for (int attempt = 1; attempt <= maxRetry; attempt++) {

    //    Serial.print("Read attempt: ");
    //    Serial.println(attempt);

    // re-init SD every attempt (important!)
    if (!SD.begin(SD_CS)) {
      Serial.println("SD init failed");
      delay(500);
      continue;
    }

    File file = SD.open("config.txt");

    if (!file) {
      Serial.println("Open file failed");
      delay(500);
      continue;
    }

    // ==== READ FILE ====
    char line[100];
    int index = 0;

    while (file.available()) {
      char c = file.read();

      // protect overflow
      if (index >= sizeof(line) - 1) {
        index = 0;  // reset line if overflow
        continue;
      }

      if (c == '\n') {
        line[index] = '\0';
        parseLine(line);
        index = 0;
      } else {
        line[index++] = c;
      }
    }

    // last line
    if (index > 0) {
      line[index] = '\0';
      parseLine(line);
    }

    file.close();

    Serial.println("Read success ✅");
    return true;  // success
  }

  Serial.println("Read failed after retries ❌");
  return false;  // all retries failed
}

void printData() {
  Serial.println("=== RESULT ===");


  Serial.print("Valve A: ");
  Serial.println(totalValvePlan_1);
  Serial.print("Valve B: ");
  Serial.println(totalRoundDayPlan_2);

  Serial.print("round_day_plan_1:");
  Serial.println(totalRoundDayPlan_1);
  Serial.print("round_day_plan_2:");
  Serial.println(totalRoundDayPlan_2);

  Serial.print("calculate_plan_1:");
  Serial.println(calculatePlan_1);
  Serial.print("calculate_plan_2:");
  Serial.println(calculatePlan_2);


  Serial.println("Setting 1:");
  Serial.println(setting_1);

  Serial.println("Setting 2:");
  Serial.println(setting_2);

  Serial.print("Area: ");
  Serial.println(area);
  Serial.print("Pump rate: ");
  Serial.println(pumpRate);
  Serial.print("Date: ");
  Serial.print(dap_day);
  Serial.print("/");
  Serial.print(dap_month);
  Serial.print("/");
  Serial.println(dap_year);
  Serial.print("Time: ");
  Serial.print(time_h);
  Serial.print(":");
  Serial.print(time_m);
  Serial.print(":");
  Serial.println(time_s);
}


void writeFileLog(float etSum, float rainSum, float irrigation, char remark[10]) {
  int idx = idxCountDay % 7;  // 7 refer to week is 7 day
  float maxTempLog = limitTemp(maxTemp);
  float minTempLog = limitTemp(minTemp);
  float avgTempLog = (maxTempLog + minTempLog) / 2;

  File file = SD.open("DATA.TXT", FILE_WRITE);  // เปิดไฟล์ที่ชื่อ Datalog.txt เพื่อเขียนข้อมูล โหมด FILE_WRITE
  if (!file) {
    // ถ้าเปิดไฟลืไม่สำเร็จ ให้แสดง error
    //    BuzzerControl(1);
    Serial.println(F("errOpen Data"));
    return;

  } else {

    file.print(remark);
    file.print(",");
    file.print(currentDay);
    file.print("/");
    file.print(currentMonth);
    file.print("/");
    file.print(currentYear);
    file.print(",");
    file.print(currentHour);
    file.print(":");
    file.print(currentMinute);
    file.print(":");
    file.print(currentSecond);
    file.print(",");
    file.print(maxTempLog);
    file.print(",");
    file.print(minTempLog);
    file.print(",");
    file.print(avgTempLog);
    file.print(",");
    file.print(rainWeek[idx]);
    file.print(",");
    file.print(rainSum);
    file.print(",");
    file.print(etWeek[idx]);
    file.print(",");
    file.print(etSum);
    file.print(",");
    file.println(irrigation);
    file.close();  // ปิดไฟล์
  }
  Serial.println(F("log success"));
}

void updateFireBaseDaily(float etSum, float rainSum, float irrigation, String setting) {
  int idx = idxCountDay % 7;  // 7 refer to week is 7 day
  float maxTempLog = limitTemp(maxTemp);
  float minTempLog = limitTemp(minTemp);
  float avgTempLog = (maxTempLog + minTempLog) / 2;

  // String basePath = setting + "/data/";
  // // dd/mm/yy hh:mm:ss
  // String dateTimeFormat = String(currentDay) + "/" + String(currentMonth) + "/" + String(currentYear) + " " + String(currentHour) + ":" + String(currentMinute) + ":" + String(+currentSecond);
  // String OldPath = basePath + "date/" + String(currentHour) + "/" + String(currentMinute);
  // fb.setFloat(OldPath + "/Irrigation", etSum);
  // fb.setFloat(OldPath + "/sumEt", rainSum);
  // fb.setFloat(OldPath + "/sumRain", irrigation);
  // fb.setFloat(OldPath + "/max", maxTempLog);
  // fb.setFloat(OldPath + "/min", minTempLog);
  // fb.setFloat(OldPath + "/meanDay", avgTempLog);
  // fb.setString(OldPath + "/date", dateTimeFormat);

  String Path = setting + "/" + String(currentYear) + "/" + String(currentMonth) + "/" + String(currentDay);
  fb.setFloat(Path + "/Irrigation", irrigation);
  fb.setFloat(Path + "/sumEt", etSum);
  fb.setFloat(Path + "/sumRain", rainSum);
  fb.setFloat(Path + "/max", maxTempLog);
  fb.setFloat(Path + "/min", minTempLog);
  fb.setFloat(Path + "/meanDay", avgTempLog);
}


void setup() {
  lcd.begin();         // initialize LCD
  Serial.begin(9600);  // Setup Serial connection
                       //   gpsSerial.begin(9600);

  WiFi.disconnect();
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("-");
    // delay(500);
  }

  //
  if (readConfigWithRetry(3)) {
    printData();
  } else {
    Serial.println("SYSTEM HALT or fallback config");
  }
  raincount_setup();  // raincount
  Relay_setup();      // relay


  /////// T/H
  TH_in.begin();
  TH_ex.begin();


  //////////////  TM1637  4 digit display valve
  DisplayValve.setBrightness(6);  /// 6  สว่างมากที่สุด
  DisplayValve.clear();           /// ดับ

  ///
  //////////  ทดสอบระบบ
  lcd.setCursor(7, 0);
  lcd.print("Start");
  lcd.clear();
  DS3231_setup();  //  DS3231 เวลา
}


//void runGPS()
//{
//// -----------------------------
//  // 1) นำข้อมูลจาก GPS -> TinyGPS++
//  // -----------------------------
//  while (gpsSerial.available()) {
//    char c = gpsSerial.read();
//    gps.encode(c);
//  }
//
//  if (gps.time.isValid()) {
//
//      Serial.println("gps valid ");
//      gpsDay= gps.date.day();
//      gpsMonth = gps.date.month();
//      gpsYear= gps.date.year()+543;
//      int hour = gps.time.hour() + 7; // ปรับเป็นเวลาไทย (GMT+7)
//      if (hour >= 24) hour -= 24;
//      gpsHour = hour;
//      gpsMinute= gps.time.minute();
//      gpsSecond= gps.time.second();
//    }
//}

void readTime() {
  DateTime Tnow = RTC.now();  /// ค่าเวลา
  currentDay = Tnow.day();
  currentMonth = Tnow.month();
  currentYear = Tnow.year();
  currentHour = Tnow.hour();
  currentMinute = Tnow.minute();
  currentSecond = Tnow.second();
}


struct TimeStruct {
  int h = 0;
  int m = 0;
  int s = 0;
};

struct TimeStruct addSeconds(TimeStruct t, long addSec) {
  long totalSeconds = t.h * 3600L + t.m * 60L + t.s + addSec;

  // keep within 24 hours
  totalSeconds = totalSeconds % 86400L;
  if (totalSeconds < 0) totalSeconds += 86400L;

  TimeStruct result;
  result.h = totalSeconds / 3600;
  result.m = (totalSeconds % 3600) / 60;
  result.s = totalSeconds % 60;


  return result;
}

long calculateTimeStopValve(float valueTime) {
  TimeStruct t = { currentHour, currentMinute, currentSecond };

  TimeStruct newTime;

  newTime = addSeconds(t, valueTime);
  return newTime.h * 3600 + newTime.m * 60 + newTime.s;
}



float meanDaily(int a) {
  static const float values[12] = { 0.26, 0.26, 0.27, 0.28, 0.29, 0.29, 0.29, 0.28, 0.28, 0.27, 0.26, 0.25 };
  if (a >= 1 && a <= 12) return values[a - 1];
  return 0.0;
}

double calculateET() {
  float P = meanDaily(currentMonth);
  return P * ((0.46 * ((limitTemp(maxTemp) + limitTemp(minTemp)) / 2)) + 8);
}

double calculateKC(char setting, int numberConfig) {

  int DAP = findDifferentDay(dap_day, dap_month, dap_year, currentDay, currentMonth, currentYear);


  return fomulaSetting(DAP, setting, numberConfig);
}

void updateDailyData() {
  int ran = random(10, 15);
  int ran2 = random(1, 5);

  int idx = idxCountDay % 7;  // 7 refer to week is 7 day
  rainDay = rainCount * (0.0161 * 25.40);
  etWeek[idx] = ran;
  rainWeek[idx] = ran2;
  kcWeek_1[idx] = calculateKC(setting_1, Mode1);
  kcWeek_2[idx] = calculateKC(setting_2, Mode2);
  idxCountDay++;

  rainCount = 0;
}




float resultCalculatePlan(int totalValve, float etValue, float rainValue, char plan) {

  return (etValue - rainValue) * area;
}


float getSummary(double data[7], int roundWatering) {
  int i = 0;
  int startI = idxCountDay > roundWatering ? 7 % roundWatering : 0;
  float summary = 0;
  for (i = 0; i < roundWatering; i++) {
    int idx = idxCountDay > roundWatering ? (idxCountDay + startI + i) % 7 : i;
    summary += data[idx];
  }
  return summary;
}


double etSummary = 0;
double rainSummary = 0;
double irrigation = 0;
// fomulaSetting
double calculateIrrigation(double *summaryEtWithKc, int totalValve, int roundWatering, char calculatePlan) {

  int checkRound = idxCountDay % roundWatering;

  Serial.print("idxCountDay ");
  Serial.println(idxCountDay);
  Serial.print("roundWatering ");
  Serial.println(roundWatering);
  Serial.print("checkRound ");
  Serial.println(checkRound);
  // Serial.println("idxCountDay ",idxCountDay);
  // Serial.println("roundWatering ",roundWatering);
  // Serial.println("checkRound ",checkRound);

  if (roundWatering >= 1 && totalValve >= 1) {

    if (checkRound == 0) {

      etSummary = getSummary(summaryEtWithKc, roundWatering);
      rainSummary = getSummary(rainWeek, roundWatering);
      irrigation = resultCalculatePlan(totalValve, etSummary, rainSummary, calculatePlan);
      return irrigation;
    }
  }



  return 0;
}



unsigned long task_1 = 0;
unsigned long task_1_timer = 1000;  // 1 วินนาที

unsigned long task_2 = 0;
unsigned long task_2_timer = 1000 * 60;  // 1 วินนาที


unsigned long task_3 = 0;
unsigned long task_3_timer = 1000;  // 1 วินนาที




/// task 1 general task //
// about senser data
void readSenser(unsigned long currentMillis) {
  readTime();
  readTH();           /// ค่าอุณหภูมิความชื้น ใน นอก
  readVoltageBatt();  // อ่านแรงดันแบตเตอรี่
  Readflow();
  ReadModeSwitch1();
  ReadModeSwitch2();
  ShowLCD();
  //runGPS();
  //DisplayValveNumber(Mode1);  //// แสดงเลข ออกจอ 4 หลัก

  // ShowSerial();
  //delay(1000);
}



bool checkPassDay = false;
long getCurrentTimeLongValue() {
  return currentHour * 3600 + currentMinute * 60 + currentSecond;
}

double calculateETWithKC(double *kc, double *sumList, int totalDay) {
  int i = 0;
  for (i = 0; i <= totalDay; i++) {
    sumList[i] = etWeek[i] * kc[i];
  }
}

// task 2 calculate value
// et rain irrigation
void calculateValueFromSensor(unsigned long currentMillis) {

  if ((currentMillis - task_2 > task_2_timer)) {
    Serial.println("t 2");
    updateDailyData();

    double tempIrrigation = 0;
    double tempSummaryETWithKC[7] = { 0, 0, 0, 0, 0, 0, 0 };


    calculateETWithKC(kcWeek_1, tempSummaryETWithKC, totalValvePlan_1);

    tempIrrigation = calculateIrrigation(tempSummaryETWithKC, totalValvePlan_1, totalRoundDayPlan_1, calculatePlan_1);

    Serial.print("temp Irrigation plan 1:");
    Serial.println(tempIrrigation);
    currentTime = getCurrentTimeLongValue();
    writeFileLog(etSummary, rainSummary, tempIrrigation, "setting_1");
    updateFireBaseDaily(etSummary, rainSummary, tempIrrigation, BasePathSetiing1);
    if (tempIrrigation > 0) {
      irrigationPlan_1 = tempIrrigation;
      Serial.print("irrigationPlan_1 :");
      Serial.println(irrigationPlan_1);

      timeStopPlan_1 = calculateTimeStopValve(irrigationPlan_1);
      Serial.print("currentTime:");
      Serial.println(currentTime);
      Serial.print("time A :");
      Serial.println(timeStopPlan_1 - currentTime);
    }

    

    calculateETWithKC(kcWeek_2, tempSummaryETWithKC, totalValvePlan_2);
    tempIrrigation = calculateIrrigation(tempSummaryETWithKC, totalValvePlan_2, totalRoundDayPlan_2, calculatePlan_2);

    writeFileLog(etSummary, rainSummary, tempIrrigation, "setting_2");
    updateFireBaseDaily(etSummary, rainSummary, tempIrrigation, BasePathSetiing2);
    if (tempIrrigation > 0) {
      irrigationPlan_2 = tempIrrigation;
      timeStopPlan_2 = calculateTimeStopValve(irrigationPlan_2);
      Serial.print("currentTime:");
      Serial.println(currentTime);
      Serial.print("time B :");
      Serial.println(timeStopPlan_2 - currentTime);
    }

    task_2 = currentMillis;
  }
};



bool valveProcess(long currentTime, long timeStop, int currentValve) {


  if (timeStop > currentTime) {
    //             Serial.print("plan "); Serial.print(plan);   Serial.print(" valve on "); Serial.print(currentValve); Serial.print(" time :"); Serial.println(timeStop - currentTime);
    ValveControl(currentValve, 1);
    return true;
  } else {
    ValveControl(currentValve, 0);
    //                Serial.println(": valve off");
    return false;
  }
}




int controlUpdateValvePlan(int currentValve, int totalValve) {

  int valve = (currentValve + 1) % (totalValve + 1);
  return valve > 0 ? valve : 1;
}

float controlUpdateIrrigationPlan(int currentIrrigation, int valve, int totalValve, char plan) {

  if (plan == 'A') {
    return 0;
  }

  if (plan == 'B') {
    if (valve >= totalValve) {
      return 0;
    }
    return currentIrrigation;
  }

  return 0;
}


// task 3 control valve
// start/stop valve pump
void controlValueAndPump(unsigned long currentMillis) {

  currentTime = getCurrentTimeLongValue();

  if (irrigationPlan_1 > 0 || irrigationPlan_2 > 0) {
    PumpControl(1);
  } else {
    PumpControl(0);
  }

  if (irrigationPlan_1 > 0) {

    activeValvePlan_1 = valveProcess(currentTime, timeStopPlan_1, currentValvePlan_1);

    if (!activeValvePlan_1) {
      ValveControl(currentValvePlan_1, 0);
      irrigationPlan_1 = controlUpdateIrrigationPlan(irrigationPlan_1, currentValvePlan_1, totalValvePlan_1, calculatePlan_1);

      timeStopPlan_1 = calculateTimeStopValve(irrigationPlan_1);

      currentValvePlan_1 = controlUpdateValvePlan(currentValvePlan_1, totalValvePlan_1);
    }
  }

  if (irrigationPlan_2 > 0 && irrigationPlan_1 == 0) {

    activeValvePlan_2 = valveProcess(currentTime, timeStopPlan_2, currentValvePlan_2 + totalValvePlan_1);

    if (!activeValvePlan_2) {
      ValveControl(currentValvePlan_2 + totalValvePlan_1, 0);
      irrigationPlan_2 = controlUpdateIrrigationPlan(irrigationPlan_2, currentValvePlan_2, totalValvePlan_2, calculatePlan_2);

      timeStopPlan_2 = calculateTimeStopValve(irrigationPlan_2);

      currentValvePlan_2 = controlUpdateValvePlan(currentValvePlan_2, totalValvePlan_2);
    }
  }
};



unsigned long previousMillis = 0;
const long interval = 1000;

unsigned long previousMillis2 = 0;
const long interval2 = 1000 * 30;


void updateFireBaseCurrentStatus(String setting, bool flowState, int valueQueue) {

  // fb.setString(setting + "/farmCode", setting);

  JsonDocument ValueActiveJson;

  // Add various data types to the JSON document
  ValueActiveJson["FlowState"] = flowState;
  ValueActiveJson["Queue"] = String(valueQueue);
  ValueActiveJson["Value_1"] = V1_State;
  ValueActiveJson["Value_2"] = V2_State;
  ValueActiveJson["Value_3"] = V3_State;
  ValueActiveJson["Value_4"] = V4_State;
  ValueActiveJson["Value_5"] = V5_State;
  ValueActiveJson["Value_6"] = V6_State;
  ValueActiveJson["Value_7"] = V7_State;
  // Create a string to hold the serialized JSON data
  String output;

  // Optional: Shrink the JSON document to fit its contents exactly
  ValueActiveJson.shrinkToFit();
  // Serialize the JSON document to a string
  serializeJson(ValueActiveJson, output);
  String basePathValueActive = setting + "/ValueActive";
  fb.setJson(basePathValueActive, output);

  JsonDocument lastActiveTimeJson;
  String basePathLastActiveTime = setting + "/lastActiveTime";

  lastActiveTimeJson["Battery"] = VoltageBatt;
  lastActiveTimeJson["Date"] = String(currentDay) + "/" + String(currentMonth) + "/" + String(currentYear);
  lastActiveTimeJson["Time"] = String(currentHour) + ":" + String(currentMinute) + ":" + String(currentSecond);
  lastActiveTimeJson.shrinkToFit();
  // Serialize the JSON document to a string
  serializeJson(lastActiveTimeJson, output);
  fb.setJson(basePathLastActiveTime, output);

}

void loop() {



  unsigned long currentMillis = millis();
  unsigned long currentMillis1 = millis();

  if (currentMillis - previousMillis >= interval) {

    readSenser(currentMillis);           // about senser data
    controlValueAndPump(currentMillis);  // start/stop valve pump
    previousMillis = currentMillis;
  }

  calculateValueFromSensor(currentMillis);  // et rain irrigation

  if (currentMillis1 - previousMillis2 >= interval2) {
    Serial.println("UPDATE FIRE BASE");
    updateFireBaseCurrentStatus(BasePathSetiing1, FlowState, currentValvePlan_1);
    updateFireBaseCurrentStatus(BasePathSetiing2, FlowState, currentValvePlan_2 + totalValvePlan_1);
    previousMillis2 = currentMillis1;
  }
}
