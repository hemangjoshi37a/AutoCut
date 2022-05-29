//#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

Servo myservo;  // create servo object to control a servo
int servoPos = 0;  
const int buttonUp = 4;      // UP BUTTON
const int buttonOK = 5;      // OK BUTTON
const int buttonDown = 6;    // DOWN BUTTON 
const int buttonSet = 7;     // SET BUTTON

const int SDA_pin =  A4;    // SDA
const int SCL_pin =  A5;     // SCL

const int stepPin =  2;    // STEPPER STEP PIN
const int dirPin =  3;     // STEPPER DIR PIN` 
const int servoPin =  A1;   // SERVO PIN
const float stepperWheelDiameter = 5.4; // STEPPER WHEEL DIAMETER
const int stepsPerRevolution = 200;
const float cmPerStep = 0.085;
// 17 CM for one rev
// 0.085 cm per step

int qtycounter = 1;
int cmcounter = 1;
bool isLastModeQty = HIGH;
bool lastSetButtonPosition = LOW;
bool lastOKButtonPosition = LOW;

//LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
  Serial.begin(9600);
  
  
//  lcd.begin(16, 2);

    //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight();

  
  lcd.setCursor(0, 0);
  lcd.print("   = Nexleen =");
//  pinMode(ledPin, OUTPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonOK, INPUT);
  pinMode(A2, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);
  myservo.attach(A1);
  myservo.write(0);        
}

void loop() {

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
 
  lcd.setCursor(0, 1);
  lcd.print("Qty:");
//  lcd.print("   ");  
  if(isLastModeQty == HIGH){
    lcd.print("   ");  
    delay(5);
  }
  lcd.setCursor(4, 1);
  lcd.print(qtycounter);
  lcd.setCursor(8, 1);
  lcd.print("CM:");
//  lcd.print("   ");  
  if(isLastModeQty == LOW){
    lcd.print("   ");  
    delay(5);
  }
  lcd.setCursor(11, 1); 

  lcd.print(cmcounter);
  delay(10);

  bool isSetButtonPressed = digitalRead(buttonSet);
  if (isSetButtonPressed == HIGH and isSetButtonPressed!= lastSetButtonPosition) {
    isLastModeQty = !isLastModeQty;
    delay(10);
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
      for (servoPos = 0; servoPos <= 130; servoPos += 1) {
        // in steps of 1 degree
        myservo.write(servoPos);             
        delay(6);                     
        }  
      for (servoPos = 130; servoPos >= 0; servoPos -= 1) { 
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
