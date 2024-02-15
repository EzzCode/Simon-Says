#include <LiquidCrystal.h>

// LCD pins
int rs = 7;
int en = 8;
int d4 = 9;
int d5 = 10;
int d6 = 11;
int d7 = 12;

// Game variables
int highscore = 0;
int myscore;

// LCD object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Button pins
int pinup = 4;
int pinright = 3;
int pindown = 2;
int pinleft = 5;

// Joystick values
int Xjoy;
int Yjoy;

// Loop variables
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
  // LCD initialization
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Your Score: 0");
  lcd.setCursor(0, 0);
  lcd.print("High Score: 0");
  delay(2000);

  // Set pin modes
  pinMode(pinup, OUTPUT);
  pinMode(pinright, OUTPUT);
  pinMode(pindown, OUTPUT);
  pinMode(pinleft, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

  // Initialize serial communication
  Serial.begin(9600);

  // Call lose function
  lose();
}

void play() {
  // Display and play the generated sequence
  for (i = 0; i < stage; i = i + 1) {
    // Display sequence on Serial monitor
    Serial.print("Seq: ");
    Serial.print(i);
    Serial.print("Pin: ");
    Serial.println(sequence[i]);

    // Light up LED corresponding to the current sequence element
    digitalWrite(sequence[i], HIGH);
    delay(500);
    digitalWrite(sequence[i], LOW);
    delay(250);

    // Update LCD with current score
    lcd.setCursor(0, 1);
    myscore = stage;
    lcd.print("Your Score: ");
    lcd.setCursor(12, 1);
    lcd.print(myscore);

    // Update high score on LCD
    if (myscore > highscore) {
      highscore = stage;
    }
    lcd.setCursor(0, 0);
    lcd.print("High Score: " + (String)(highscore));
  }
}

void reset() {
  // Reset game variables
  wait = true;
  joystick = true;
  C = 0;
  stage = 0;
  correctpin = 0;
  lastinput = 0;
  resetflag = false;
}

void lose() {
  // Blink LEDs to indicate game loss
  digitalWrite(pinup, HIGH);
  digitalWrite(pindown, HIGH);
  digitalWrite(pinleft, HIGH);
  digitalWrite(pinright, HIGH);
  delay(350);
  digitalWrite(pinup, LOW);
  digitalWrite(pindown, LOW);
  digitalWrite(pinleft, LOW);
  digitalWrite(pinright, LOW);
  delay(200);
  digitalWrite(pinup, HIGH);
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
  // Read joystick values
  Xjoy = analogRead(A1);
  Yjoy = analogRead(A2);

  // Control button outputs based on joystick values
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
    // Generate random sequence and play
    randomSeed(analogRead(A0));
    sequence[stage] = pins[random(0, pinNum)];
    stage = stage + 1;
    play();
    wait = false;
  } else {
    if (joystick) {
      // Check if the joystick is moved in any direction
      correctpin = sequence[C];
      Serial.print("Expected: ");
      Serial.println(correctpin);
      for (i = 0; i < pinNum; i = i + 1) {
        // Check which button is pressed
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
      // If correct button is pressed, move to the next stage
      C = C + 1;
      lastinput = correctpin;
      joystick = false;
    } else {
      if (digitalRead(lastinput) == LOW && !joystick) {
        // If the last input is released, reset or advance the game
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

