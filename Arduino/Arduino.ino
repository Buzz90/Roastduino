#include "max6675.h"
#include "SoftwareSerial.h"

SoftwareSerial lcd(8,9);
String getNex, preNex, nextionData;

//Start of TempSensor Init...
int beanDO = 4;
int beanCS = 5;
int beanCLK = 6;

int drumDO = 12;
int drumCS = 11;
int drumCLK = 10;
//End of TempSensor Init...

int drum = 2;
int beanFan = 3;
int drumFan = 7;

int beanTemp;
int drumTemp;

int c;
int e;
int progBar=0;

// For changing color in the Nextion
int redClr = 47104;
int blkClr = 12610;

int maxRange = 150; // For converting temperature...
float beanBar;
float drumBar;

MAX6675 beantemp(beanCLK, beanCS, beanDO);
MAX6675 drumtemp(drumCLK, drumCS, drumDO);

void setup() {
  Serial.begin(9600);
  lcd.begin(9600);
  pinMode(drum, OUTPUT);
  pinMode(beanFan, OUTPUT);
  pinMode(drumFan, OUTPUT);
}

void loop() {
  updateTemp();
  checkNextionCmd();
}

void checkNextionCmd() {
   //if (lcd.available()) {
    // Read the received data as a string
   // String data = lcd.readStringUntil('\n');
   // Serial.println(data);
  //}
  if (lcd.available()) {
    String data = lcd.readStringUntil('\n'); // Read data until newline

    //Airflow Control
    if (data.indexOf("AirflowOn") != -1) {
      // Perform actions relevant to the specific button touch
      digitalWrite(drumFan, LOW);
      Serial.println("Airflow Turned On!");}
    if (data.indexOf("AirflowOff") != -1) {
      // Perform actions relevant to the specific button touch
      digitalWrite(drumFan, HIGH);
      Serial.println("Airflow Turned Off!");}

    //Motor Control
    if (data.indexOf("MotorOn") != -1) {
      // Perform actions relevant to the specific button touch
      digitalWrite(drum, LOW);
      Serial.println("Motor Turned On!");}
    if (data.indexOf("MotorOff") != -1) {
      // Perform actions relevant to the specific button touch
      digitalWrite(drum, HIGH);
      Serial.println("Motor Turned Off!");}

    //Cooler Control
    if (data.indexOf("CoolerOn") != -1) {
      // Perform actions relevant to the specific button touch
      digitalWrite(beanFan, LOW);
      Serial.println("Cooler Turned On!");}
    if (data.indexOf("CoolerOff") != -1) {
      // Perform actions relevant to the specific button touch
      digitalWrite(beanFan, HIGH);
      Serial.println("Cooler Turned Off!");
    } else {
      // Ignore other commands or data
    }
  }
}

void updateTemp() {
   // basic readout test, just print the current temp
  beanTemp = beantemp.readCelsius();
  drumTemp = drumtemp.readCelsius();
  if(beanTemp > 200)
  {
    txtCMD("beanLabel.pco=53800");
    valCMD("beanLabel.val=", beanTemp);
    valCMD("page0.beanGlobal.val=", beanTemp);
  }
  if(beanTemp <= 200)
  {
    txtCMD("beanLabel.pco=8484");
    valCMD("beanLabel.val=", beanTemp);
    valCMD("page0.beanGlobal.val=", beanTemp);
  }
   if(drumTemp > 200)
  {
    txtCMD("drumLabel.pco=53800");
    valCMD("drumLabel.val=", drumTemp);
    valCMD("page0.drumGlobal.val=", drumTemp);
  }
  if(drumTemp <= 200)
  {
    txtCMD("drumLabel.pco=8484");
    valCMD("drumLabel.val=", drumTemp);
    valCMD("page0.drumGlobal.val=", drumTemp);
  }
    // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
    delay(250);
}

void valCMD(String cmd, int val) {
  lcd.print(cmd);
  lcd.print(val);
  lcd.write(0xff);
  lcd.write(0xff);
  lcd.write(0xff);
}

void txtCMD(String cmd) {
  lcd.print(cmd);
  lcd.write(0xff);
  lcd.write(0xff);
  lcd.write(0xff);
}

void error(){

}

float convertValue(int maxRange) {
  float progressValue = (float)maxRange / 250.0 * 100.0;
  return progressValue;
}