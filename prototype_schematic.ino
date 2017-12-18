// Declaring pin variables
int potPin = A0;
int ledPin[3] = {3, 4, 5};
int switchPin = 2;
int piezoPin = 9;
// Declaring initial component values (to detect changes in state)
int potVal;
int switchState_old = 0;
int switchState_new = 0;
// User determined values
int setLEDNumber;
int timePerLED = 1; // Default time per LED: 1 second/LED

void setup()
{
  Serial.begin(9600);
  pinMode(potPin, INPUT);
  pinMode(switchPin, INPUT);
  pinMode(ledPin[0], OUTPUT);
  pinMode(ledPin[1], OUTPUT);
  pinMode(ledPin[2], OUTPUT);
  pinMode(piezoPin, OUTPUT);
}

void loop()
{
  switchState_new = digitalRead(switchPin);
  // potMin = 528, potMax = 1023
  potVal = map(analogRead(potPin), 528, 1023, 0, 3);

  // Setting LED indicators (pretty crude method -- optimize later)
  // turns off all LEDs
  for (int i = 0; i < sizeof(ledPin) / sizeof(int); i++)
  {
    digitalWrite(ledPin[i], LOW);
  }
  // turns on LEDs corresponding to potentiometer position
  for (int i = 0; i < potVal; i ++)
  {
    digitalWrite(ledPin[i], HIGH);
  }

  // Pomodoro START function
  // Executes timer function when switch is pressed
  if (switchState_new != switchState_old)
  {
    if (switchState_new == HIGH)
    {
      if (potVal == 0) // Learn exception handling to replace this!
      {
        Serial.println("No duration selected!");
      }
      else
      {
        setLEDNumber = potVal;
        Serial.print("Timer started for ");
        Serial.print(timePerLED * setLEDNumber);
        Serial.println(" seconds.");
        for (int i = setLEDNumber - 1; i > -1; i = i - 1)
        {
          delay(1000 * timePerLED);
          digitalWrite(ledPin[i], LOW);
          if (i == 0)
          {
            Serial.println("Time's up!");
            tone(piezoPin, 440);
            delay(1000);
            noTone(piezoPin);
          }
        }
      }
    }
    delay(50); // For bounces
  }

  // Calibration of time indicated per LED
  // Should simply display current setting with option to input change
  // on LCD
  while (Serial.available() > 0)
  {
    timePerLED = Serial.parseInt();
    Serial.print("You have calibrated the timer for ");
    Serial.print(timePerLED);
    Serial.print(" seconds per LED.\n");
  }

}
