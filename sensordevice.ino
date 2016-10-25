#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>

rgb_lcd lcd;

const int light = A0; //Initialize light sensor
const int sound = A1; //Initialize sound sensor
const int temp = A2;  //Initialize temp sensor

double initAvgLight = 0;
double initAvgSound = 0;
double initAvgTemp = 0;

double rollingAvgLight;
double rollingAvgSound;
double rollingAvgTemp;

const int buttonPin = 2;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

double avg(double a, double b){
  return 0.9*a + 0.1*b;
}

double avgT(double a, double b){
  return 0.9999*a + 0.0001*b;
}

double analogReadVoltage(int source){
  return analogRead(source)*5.0/1023.0;
}
double temperatureC(int source){
  double a = analogRead(source);
  return 1.0/(log10((1023.0-a+1)/a)/4275.0 + 1/298.15)-273.15;
}

double dB(int source){
  double ref = 1.767767/362.0;
  double v = analogReadVoltage(source) + 1.767767/362.0;
  return 20*log10(v/ref);
}

double lux(int source){
  double x = analogReadVoltage(source);
  if (x == 5){
    return 100.0;
  }
  if (x < 5){
    return 0.0029*pow(x,6.4939);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  lcd.begin(16, 2);

  lcd.setRGB(0,0,0);

  pinMode(buttonPin, INPUT);
  
  for(int i = 0; i < 100; i++){
    initAvgLight += lux(light);
    initAvgSound += dB(sound);
    initAvgTemp += temperatureC(temp);
    delay(100);
  }

  double rollingAvgLight = initAvgLight/100;
  double rollingAvgSound = initAvgSound/100;
  double rollingAvgTemp = initAvgTemp/100;
}

double max = 0;

void loop(){
  int i = 1;
  while (i > 0){
    double currentLight = lux(light);
    if (currentLight == 100.0){
      double currentSound = dB(sound);
      rollingAvgSound = avg(rollingAvgSound, currentSound);
      double currentTemp = temperatureC(temp);
      rollingAvgTemp = avgT(rollingAvgTemp, currentTemp);
      lcd.setRGB(255,0,0);
      lcd.clear();
      lcd.print("Light Level:");
      lcd.setCursor(0,1);
      lcd.print("> 100 lux");
      delay(50);
      buttonState1 = digitalRead(buttonPin);
      delay(50);
      buttonState2 = digitalRead(buttonPin);
      delay(50);
      buttonState3 = digitalRead(buttonPin);
      delay(50);
      if (buttonState1 == HIGH || buttonState2 == HIGH || buttonState3 == HIGH){
        i = 0;
      }
    }
    else {
      rollingAvgLight = avg(rollingAvgLight, currentLight);
      double currentSound = dB(sound);
      rollingAvgSound = avg(rollingAvgSound, currentSound);
      double currentTemp = temperatureC(temp);
      rollingAvgTemp = avgT(rollingAvgTemp, currentTemp);
      delay(50);
      double l = (255/1.62)*log10(abs(currentLight - rollingAvgLight)+ 1);
      double m = (255/1.62)*abs((log10(abs(currentLight - rollingAvgLight)+ 1)-1.62));
      lcd.setRGB((int) l, (int) m, 0);
      lcd.clear();
      lcd.print("Light Level:");
      lcd.setCursor(0,1);
      lcd.print(currentLight);
      lcd.print(" lux");
      delay(50);
      buttonState1 = digitalRead(buttonPin);
      delay(50);
      buttonState2 = digitalRead(buttonPin);
      delay(50);
      buttonState3 = digitalRead(buttonPin);
      delay(50);
      if (buttonState1 == HIGH || buttonState2 == HIGH || buttonState3 == HIGH){
        i = 0;
      }
    }
  }
  i = 1;
  while (i > 0){
    double currentLight = lux(light);
    rollingAvgLight = avg(rollingAvgLight, currentLight);
    double currentSound = dB(sound);
    rollingAvgSound = avg(rollingAvgSound, currentSound);
    double currentTemp = temperatureC(temp);
    rollingAvgTemp = avgT(rollingAvgTemp, currentTemp);
    delay(50);
    double l = (255/1.42)*log10(abs(currentSound - rollingAvgSound)+ 1);
    double m = (255/1.42)*abs(log10(abs(currentSound - rollingAvgSound)+1)-1.42);
    lcd.setRGB((int) l, (int) m, 0);
    lcd.clear();
    lcd.print("Sound Level:");
    lcd.setCursor(0,1);
    lcd.print(currentSound);
    lcd.print(" dB");
    delay(50);
    buttonState1 = digitalRead(buttonPin);
    delay(50);
    buttonState2 = digitalRead(buttonPin);
    delay(50);
    buttonState3 = digitalRead(buttonPin);
    delay(50);
    if (buttonState1 == HIGH || buttonState2 == HIGH || buttonState3 == HIGH){
      i = 0;
    }
  }
  i = 1;
  while (i > 0){
    double currentLight = lux(light);
    rollingAvgLight = avg(rollingAvgLight, currentLight);
    double currentSound = dB(sound);
    rollingAvgSound = avg(rollingAvgSound, currentSound);
    double currentTemp = temperatureC(temp);
    rollingAvgTemp = avgT(rollingAvgTemp, currentTemp);
    delay(50);
    double l = (255/1.8)*log10(abs(currentTemp - rollingAvgTemp)+ 1);
    double m = (255/1.8)*abs(log10(abs(currentTemp - rollingAvgTemp)+1)-1.8);
    lcd.setRGB((int) l, (int) m, 0);
    lcd.clear();
    lcd.print("Temperature:");
    lcd.setCursor(0,1);
    lcd.print(currentTemp);
    lcd.print(" Celcius");
    delay(50);
    buttonState1 = digitalRead(buttonPin);
    delay(50);
    buttonState2 = digitalRead(buttonPin);
    delay(50);
    buttonState3 = digitalRead(buttonPin);
    delay(50);
    if (buttonState1 == HIGH || buttonState2 == HIGH || buttonState3 == HIGH){
      i = 0;
    }
  }
}

