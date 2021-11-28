#include "HX711.h"					//use 24bitAD HX711 library

HX711 HX711_CH0(2, 3, 1620); //SCK,DT,GapValue
//SCK pin for arduino and HX711 timer
//DT pin for reading AD data from HX711
//GapValue for weight calibration; if result is greater, increase; if smaller, decrease

int LED = 13;
long Weight = 0; //for measuring current weight
long Initial_Weight = 28; //the initial food weight (gram) the diner gets at checkout

long eating_speed;
int dinning = 1; //flag for being at dinning hall
int interval = 3000;


void setup()
{
  pinMode(LED, OUTPUT);				//set LED as output pin
  digitalWrite(LED, LOW);			//LED off
  Serial.begin(9600);						//set Baud

  HX711_CH0.begin();					//get tare weight
  delay(3000);								//3 seconds for stablization
  HX711_CH0.begin();					//get tare weight again

  digitalWrite(LED, HIGH);		//LED on, ready to test
  delay(5000);
  //vibration motor
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}




unsigned long previousMillis = 0;
unsigned long previousTime = 0;
void loop(){
  Weight = HX711_CH0.Get_Weight();
  unsigned long currentMillis = millis() - 8000; //8000 is the time used in setup
  if (currentMillis - previousMillis >= interval){ //output weight every 3 seconds for test
    previousMillis = currentMillis;
//    Serial.print(Weight);      //print current weight
//    Serial.println(" g");
    if (Weight != 0){
      if (dinning == 1){
        Weight = HX711_CH0.Get_Weight();
        //calculate speed
        if (Weight < Initial_Weight){
//          Serial.print(Weight);             
          eating_speed = (Initial_Weight - Weight) / ((currentMillis - previousTime) / 1000);
//          Serial.print(Initial_Weight);
          Initial_Weight = Weight;
          previousTime = currentMillis;
          if (eating_speed > 0.5/*baseline_speed (g/s)*/){ //eat too fast
            Serial.print(eating_speed);
//            Serial.println(" fast ");
            unsigned long vibration_start = millis();
            while (1){
              digitalWrite(4, HIGH);
              if (millis() - vibration_start > 3000){
                digitalWrite(4, LOW);
                break;
              }
            }
          }
          //finish eating & return the tableware
          if (previousTime > 3000000){  //signal sent when tableware is reaturned
            dinning = 0;
          }
        }
      }
    }
  }
}
