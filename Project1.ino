#include <LiquidCrystal.h>
int rs = 7;
int en = 8;
int d4 = 9;
int d5 = 10;
int d6 = 11;
int d7 = 12;
int highscore = 0;
int myscore;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int pinup = 4;
int pinright = 3;
int pindown = 2;
int pinleft = 5;
int Xjoy;
int Yjoy;
int i;
int stage = 0;
int sequence[50];
int pins[] = {pinup, pinright, pindown, pinleft};
int pinNum = 4;
int correctpin = 0;
int C = 0;
int lastinput;
bool wait = true;
bool joystick = true;
bool resetflag = false;
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Your Score: 0");
  lcd.setCursor(0, 0);
  lcd.print("High Score: 0");
  delay(2000);
  pinMode(pinup, OUTPUT);
  pinMode(pinright, OUTPUT);
  pinMode(pindown, OUTPUT);
  pinMode(pinleft, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  Serial.begin(9600);
  lose();
} void play() {
  for (i = 0; i < stage; i = i + 1) {
    Serial.print("Seq: ");
    Serial.print(i);
    Serial.print("Pin: ");
    Serial.println(sequence[i]);
    digitalWrite(sequence[i], HIGH);
    delay(500);
    digitalWrite(sequence[i], LOW);
    delay(250);
    lcd.setCursor(0, 1);
    myscore = stage  ;
    lcd.print("Your Score: ");
    lcd.setCursor(12, 1);
    lcd.print(myscore);
    if (myscore > highscore) {
      highscore = stage;
    }
    lcd.setCursor(0, 0);
    lcd.print("High Score: " + (String)(highscore));
  }
}
void reset() {
  wait = true;
  joystick = true;
  C = 0;
  stage = 0;
  correctpin = 0;
  lastinput = 0;
  resetflag = false;
}
void lose() {
  digitalWrite(pinup, HIGH);
  digitalWrite(pindown, HIGH);
  digitalWrite(pinleft, HIGH);
  digitalWrite(pinright, HIGH);
  delay(350);
  digitalWrite(pinup, LOW);
  digitalWrite(pindown, LOW);
  digitalWrite(pinleft, LOW);
  digitalWrite(pinright, LOW);
  delay(200);  digitalWrite(pinup, HIGH);
  digitalWrite(pindown, HIGH);
  digitalWrite(pinleft, HIGH);
  digitalWrite(pinright, HIGH);
  delay(350);
  digitalWrite(pinup, LOW);
  digitalWrite(pindown, LOW);
  digitalWrite(pinleft, LOW);
  digitalWrite(pinright, LOW);
  delay(500);
}
void loop() {
  // put your main code here, to run repeatedly:
  Xjoy = analogRead(A1);
  Yjoy = analogRead(A2);
  if (Yjoy < 10 && Xjoy > 12 && Xjoy < 1018) {
    digitalWrite(pinup, HIGH);
  } else {
    digitalWrite(pinup, LOW);
  }
  if (Yjoy > 1018 && Xjoy > 12 && Xjoy < 1018) {
    digitalWrite(pindown, HIGH);
  } else {
    digitalWrite(pindown, LOW);
  }
  if (Xjoy < 10) {
    digitalWrite(pinleft, HIGH);
  } else {
    digitalWrite(pinleft, LOW);
  }
  if (Xjoy > 1018) {
    digitalWrite(pinright, HIGH);
  } else {
    digitalWrite(pinright, LOW);
  }
  if (wait) {
    randomSeed(analogRead(A0));
    sequence[stage] = pins[random(0, pinNum)];
    stage = stage + 1;
    play();
    wait = false;
  } else {
    if (joystick) {
      correctpin = sequence[C];
      Serial.print("Expected: ");
      Serial.println(correctpin);
      for (i = 0; i < pinNum; i = i + 1) {
        if (pins[i] == correctpin)
          continue;
        if (digitalRead(pins[i]) == HIGH) {
          lastinput = pins[i];
          resetflag = true;
          joystick = false;
          Serial.print("Read: ");
          Serial.println(lastinput);
        }
      }
    }
    if (digitalRead(correctpin) == HIGH && joystick) {
      C = C + 1;
      lastinput = correctpin;
      joystick = false;
    } else {
      if (digitalRead(lastinput) == LOW && !joystick) {
        joystick = true;
        delay(100);
        if (resetflag) {
          lose();
          reset();
        } else {
          if (C == stage) {
            wait = true;
            C = 0;
            delay(1000);
          }
        }
      }
    }
  }
}
