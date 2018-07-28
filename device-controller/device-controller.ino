char serialBuf[32];
String serialCommand = "";
unsigned long debounceDelay = 100;   
const int maxLedState = 2; 
unsigned long lastLedToggleMillis = 0;   
int ledToggle = 200;
unsigned long nowMillis = 0;
int ledOn = LOW;

const int redLed = 10;
const int redButton = 8;
const int redIntensityPin = A0;
int redLedIntensity   = 255;
int redLedState = maxLedState;
int lastRedLedState = 0;
int lastRedButtonReading = LOW;   
int lastRedLedIntensity = 255;
unsigned long lastRedDebounceTime = 0;  

const int greenLed = 9;
const int greenButton = 7;
const int greenIntensityPin = A1;
int greenLedIntensity = 255;
int greenLedState = maxLedState;
int lastGreenLedState = 0;
int lastGreenButtonReading = LOW;   
int lastGreenLedIntensity = 255;
unsigned long lastGreenDebounceTime = 0;  

void setup() 
{
  pinMode(redLed, OUTPUT); 
  pinMode(greenLed, OUTPUT); 
  pinMode(redButton, INPUT);
  pinMode(greenButton, INPUT);
  pinMode(redIntensityPin, INPUT);
  pinMode(greenIntensityPin, INPUT);

  Serial.begin(9600);
}

void loop() 
{
  readSerial();
  if (serialCommand.length() > 0)
  {
    Serial.println(serialCommand);
    if (serialCommand.indexOf("redLedState") >= 0) redLedState = newIntSetting("redLedState", serialCommand);
    if (serialCommand.indexOf("greenLedState") >= 0) greenLedState = newIntSetting("greenLedState", serialCommand);
    if (serialCommand.indexOf("ledToggle") >= 0) ledToggle = newIntSetting("ledToggle", serialCommand);
  }
  buttonDebouncer(&lastGreenDebounceTime, &lastGreenButtonReading, &greenLedState, greenButton);
  buttonDebouncer(&lastRedDebounceTime, &lastRedButtonReading, &redLedState, redButton);
  if (redLedState != lastRedLedState)
  {
    if (redLedState == 0) analogWrite(redLed, 0);
    if (redLedState == 2) analogWrite(redLed, redLedIntensity);
    lastRedLedState = redLedState;
  }
  if (greenLedState != lastGreenLedState)
  {
    if (greenLedState == 0) analogWrite(greenLed, 0);
    if (greenLedState == 2) analogWrite(greenLed, greenLedIntensity);
    lastGreenLedState = greenLedState;
  }
  nowMillis = millis();
  if ((nowMillis - lastLedToggleMillis) > ((unsigned long) ledToggle))
  {
    lastLedToggleMillis = nowMillis;
    if (ledOn == LOW)
    {
      ledOn = HIGH;
    }
    else
    {
      ledOn = LOW;
    }
    if (redLedState == 1) analogWrite(redLed, redLedIntensity * ledOn);
    if (greenLedState == 1) analogWrite(greenLed, greenLedIntensity * ledOn);
  }
  redLedIntensity = analogRead(redIntensityPin) / 16;
  redLedIntensity = redLedIntensity * 4;
  if (redLedIntensity != lastRedLedIntensity)
  {
    lastRedLedIntensity = redLedIntensity;
    if (redLedState == 2) analogWrite(redLed, redLedIntensity);
  }
  greenLedIntensity = analogRead(greenIntensityPin) / 16;
  greenLedIntensity = greenLedIntensity * 4;
  if (greenLedIntensity != lastGreenLedIntensity)
  {
    lastRedLedIntensity = greenLedIntensity;
    if (greenLedState == 2) analogWrite(greenLed, greenLedIntensity);
  }
  
}
void buttonDebouncer(unsigned long* lastDebounceTime, int* lastButtonReading, int* ledState, int buttonPin)
{
  if ((millis() - *lastDebounceTime) > debounceDelay)
  {
    int reading = digitalRead(buttonPin);
    if ((reading == HIGH) && (*lastButtonReading == LOW))
    {
      *lastDebounceTime = millis();
      *ledState = *ledState + 1;
      if (*ledState > maxLedState) *ledState = 0;
    }
    *lastButtonReading = reading;
  }
}
void readSerial()
{
  serialCommand = "";
  while(Serial.available() > 0)
  {
    
    for (int ii = 0; ii < sizeof(serialBuf); ++ii) serialBuf[ii] = 0;
    Serial.readBytesUntil('!', serialBuf, 31);
    serialCommand =  String(serialBuf);
  }
}
int newIntSetting(String setParse, String input)
{
  char intbuf[32];
  String newSettingString;
  newSettingString = input.substring(input.lastIndexOf(setParse) + setParse.length() + 1,input.length());
  newSettingString.toCharArray(intbuf, sizeof(intbuf));
  return atoi(intbuf);
}
float newFloatSetting(String setParse, String input)
{
  char intbuf[32];
  String newSettingString;
  newSettingString = input.substring(input.lastIndexOf(setParse) + setParse.length() + 1,input.length());
  newSettingString.toCharArray(intbuf, sizeof(intbuf));
  return atof(intbuf);
}

