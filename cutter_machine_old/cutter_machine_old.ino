#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int servoPos = 130;  
const int buttonUp = 11;     // UP BUTTON
const int buttonOK = A3;     // OK BUTTON
const int buttonDown = 9;    // DOWN BUTTON
const int buttonSet = 8;     // SET BUTTON

const int stepPin =  13;    // STEPPER STEP PIN
const int dirPin =  12;      // STEPPER DIR PIN` 
const int servoPin =  A1;   // SERVO PIN
const float stepperWheelDiameter = 5.4; // STEPPER WHEEL DIAMETER
const int stepsPerRevolution = 200;
const float cmPerStep = 0.085;
// 17 CM for one rev
// 0.085 cm per step

int blinkcounter=0;
int qtycounter = 1;
int cmcounter = 1;
bool isLastModeQty = HIGH;
bool lastSetButtonPosition = LOW;
bool lastOKButtonPosition = LOW;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("= Ramdev Light =");
//  pinMode(ledPin, OUTPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonOK, INPUT);
  pinMode(A2, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);
  myservo.attach(A1);
  myservo.write(servoPos);        
}

void loop() {
  blinkcounter+=1;
  if(blinkcounter==20){
    blinkcounter=0;
    }
  bool isbuttonUp = digitalRead(buttonUp);
  bool isbuttonDown = digitalRead(buttonDown);

  if (isbuttonUp == HIGH and isLastModeQty == HIGH) {
    qtycounter += 1;
    delay(50);
    }
  if (isbuttonDown == HIGH and qtycounter>1  and isLastModeQty == HIGH) {
    qtycounter -= 1;
    delay(50);
    }
  if (isbuttonUp == HIGH and isLastModeQty == LOW) {
    cmcounter += 1;
    delay(50);
    }
  if (isbuttonDown == HIGH and cmcounter>1  and isLastModeQty == LOW) {
    cmcounter -= 1;
    delay(50);
    }

  Serial.print(blinkcounter);
  lcd.setCursor(0, 1);
  lcd.print("Qty:");
  if(isLastModeQty == HIGH and blinkcounter>10){ 
    lcd.print("   ");  
  }
  else{
    lcd.setCursor(4, 1);
    lcd.print(qtycounter);    
  }

  lcd.setCursor(8, 1);
  lcd.print("CM:");
  if(isLastModeQty == LOW and blinkcounter>10){ 
    lcd.print("   ");  
  }else{
    lcd.setCursor(11, 1); 
    lcd.print(cmcounter);    
  }

  bool isSetButtonPressed = digitalRead(buttonSet);
  if (isSetButtonPressed == HIGH and isSetButtonPressed!= lastSetButtonPosition) {
    isLastModeQty = !isLastModeQty;
    delay(15);
    }
  lastSetButtonPosition = isSetButtonPressed;
  
  bool isOKButtonPressed = digitalRead(buttonOK);
  delay(10);
  Serial.print("\n presed ok");
  Serial.print(isOKButtonPressed);
  
  if(isOKButtonPressed == HIGH and lastOKButtonPosition != isOKButtonPressed) {
    lastOKButtonPosition = isOKButtonPressed;
    delay(10);
    Serial.print("\n inside presed ok");

    for (int hj = 0;hj<qtycounter;hj++){
      int requiredStepsForGivenLen = int(cmcounter * (200/3));
      for(int x = 0; x < requiredStepsForGivenLen; x++){
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(700);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(700);
      }

      lcd.setCursor(0, 1);
      lcd.print("                         ");
      lcd.setCursor(0, 1);
      lcd.print("Done : ");
      lcd.setCursor(8, 1);
      lcd.print(hj+1);
      delay(100);
      // CUT WIRE BY SERVO
      for (servoPos = 130; servoPos >= 0; servoPos -= 1) {
        // in steps of 1 degree
        myservo.write(servoPos);             
        delay(6);                     
        }  
      for (servoPos = 0; servoPos <= 130; servoPos += 1) { 
        myservo.write(servoPos);             
        delay(6);                      
        }
      delay(100);
      }
      lcd.setCursor(0, 1);
      lcd.print("                         ");
    }
  lastOKButtonPosition = isOKButtonPressed;

}
