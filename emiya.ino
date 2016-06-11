#include <SHT1x.h>

#define DataPin 14
#define ClockPin 15

SHT1x sht1x(DataPin, ClockPin);

float temp;
float humid;
int photo;
int water;
int soil;
int motorIN1 = 11;
int motorIN2 = 10;

void setup() {
  Serial.begin(115200);
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
}


void ReadValue() {
  temp = sht1x.readTemperatureC() - 5;
  humid = sht1x.readHumidity();
  photo = analogRead(0);
  water = digitalRead(2);
  soil = analogRead(2);
}

//펌프 실행 함수
void pump(){
    digitalWrite(motorIN1, HIGH);
    digitalWrite(motorIN2, LOW);
    delay(3000);
    digitalWrite(motorIN1, LOW);
    digitalWrite(motorIN2, LOW);
}

void loop() {
  float sum_t = 0;
  float sum_h = 0;
  float sum_pd = 0;
  float sum_soil = 0;

  //센서 평균값 계산
  for (int i = 0 ; i < 3 ; i++) {
    ReadValue();
    sum_t += temp;
    sum_h += humid;
    sum_pd += (float)photo;
    sum_soil += (float)soil;
  }
  
                                                          
  Serial.print(sum_t / 3); Serial.print("  ");
  Serial.print(sum_h / 3); Serial.print("  ");
  Serial.print(sum_pd / 3); Serial.print("  ");
  Serial.print(water); Serial.print("  ");  //water값이 1일경우 물부족
  Serial.println(sum_soil / 3);              //값이 낮을수록 수분이 많음

}
