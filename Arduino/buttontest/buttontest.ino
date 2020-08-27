/*
Five State Switch --  By: Philip Gonzalez
*/
int switchPin = 12;
int ledPin =  5;      
int ledState = LOW; 
int ledDelay = 0;
unsigned long previousMillis = 0;                   
int switchState = 0;  // tracks button presses
boolean buttonState = LOW; // tracks button state with a debouncer


void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);  
}

void loop()
{
  unsigned long currentMillis = millis();  
  if(currentMillis - previousMillis > ledDelay)  // blink without delay function 
  {
    previousMillis = currentMillis;   
    if (ledState == LOW)  
    {
    ledState = HIGH;
    }
    else
    {
    ledState = LOW;
    }
  }
  if (digitalRead(switchPin) == HIGH) // debouncer
  {
    delay(75);
    buttonState = digitalRead(switchPin);
  }
  if (buttonState == HIGH && switchState == 0) // if the button is pressed change the switch to the first sate
  {
    switchState = 1;
    buttonState = LOW;
  }
  if (buttonState == HIGH && switchState == 1) // if the button is pressed again change the switch to the second sate 
  {
   switchState = 2;
   buttonState = LOW;
  }
  if (buttonState == HIGH && switchState == 2) // if the button is pressed again change the switch to the third sate 
  {
   switchState = 3;
   buttonState = LOW;
  }
  if (buttonState == HIGH && switchState == 3) // if the button is pressed again change the switch to the fourth sate 
  {
   switchState = 4;
   buttonState = LOW;
  }
  if (buttonState == HIGH && switchState == 4) // if the button is pressed a last time bring the switch to the null state  
  {
   switchState = 0;
   buttonState = LOW;
  }
  if (switchState == 0)  // if the switch is in the null state, the LED is off
  {
    digitalWrite(ledPin, LOW);
  }
  if (switchState == 1)  // if the switch is in the first state, the LED is on
  {
    digitalWrite(ledPin, HIGH);
  }
  if (switchState == 2)  // if the switch is in the second state, the LED blinks every half second
  {
    ledDelay = 500;
    digitalWrite(ledPin, ledState);
  }
  if (switchState == 3)  // if the switch is in the third state, the LED blinks every quarter second
  {
    ledDelay = 250;
    digitalWrite(ledPin, ledState);
  }
  if (switchState == 4)  // if the switch is in the second state, the LED blinks every eighth of a second
  {
    ledDelay = 125;
    digitalWrite(ledPin, ledState);
  }
}
