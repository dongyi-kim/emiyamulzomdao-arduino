#include <SHT1x.h>
 
#define DataPin 14
#define ClockPin 15
 
SHT1x sht1x(DataPin, ClockPin);
 
#define AVG_CNT 3
#define PUMP_CNT 4

int temp;   //temperature
int humid;  //humidity
int photo;  //
int soil;   //humidity of soil
int water;  //
int motorIN1 = 11;
int motorIN2 = 10;
int pump_timer = 4;
 
void setup() 
{
  Serial.begin(115200);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
}
 
//센서값 read
void ReadValue() {
  temp = sht1x.readTemperatureC() - 5;
  humid = sht1x.readHumidity();
  photo = analogRead(0);
  soil = analogRead(2);
  water = digitalRead(2);
}

void SendValue(){
   Serial.print(photo); Serial.print("  ");
   Serial.print(temp); Serial.print("  ");
   Serial.print(humid); Serial.print("  ");
   Serial.print(soil); Serial.print("  ");
   Serial.print(water);
   Serial.print("\n");
}
 
//펌프 실행 함수
void pump()
{  
    if(pump_timer > 0 )
    {  //if pump timer is on
       pump_timer --;
    }
    
    //Serial.write(pump_timer);
    if(pump_timer == 0)
    {     //pump turn off pump
          digitalWrite(motorIN1, LOW);
          digitalWrite(motorIN2, LOW);
    }else
    {   //turn off
         digitalWrite(motorIN1, HIGH);
         digitalWrite(motorIN2, LOW);
    }
}
 
void loop() 
{
  //if some data is on serial
  while(Serial.available())
  {   
      //read command byte from Serial
      char command  = Serial.read();

      switch(command)
      {
        //sensor data request command 
        case 0x01: 
          ReadValue();
          SendValue();
          break;

        //PUMP on command
        case 0x02:
          pump_timer = PUMP_CNT;  
          break;

        //PUMP off command
        case 0x03:
          pump_timer = 0;

          break;

        default:
          command = 0x00;
          break;
      }

      //re-send received command to ecube
      //if invalid command received, send 0x00
      Serial.write(command);
  }

  //run pump cycle
  pump();

  //delay 1 second
  delay(1000);
}
