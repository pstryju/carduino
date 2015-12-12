//#define DEBUG

#define R_OFF 36
#define R_SRC 35
#define R_ATT 22
#define R_DISPLAY 23
#define R_UP 24
#define R_DOWN 25
#define R_VOL_UP 26
#define R_VOL_DOWN 27
#define R_SEL 28
#define R_MODE 29
#define T_ENABLE 30
#define T_PLUS 31
#define T_MINUS 32
#define T_RESUME 33
#define K_LEFT A0
#define K_RIGHT A1
#define RADIO_PIN 39
#define SOCKET_PIN 37
#define KEY_STATUS 41
#define MILLISFORMINUTE 60000
#define RADIO_ENABLE_TIME 15

byte currentMode = 0;
int leftRead, rightRead;
unsigned long timeMain = 0, timeWindows = 0, timeTPlus = 0, timeTMinus = 0, timeTRes = 0, radioEnableTime = 0;
bool cruiseState = false, radioStatus = false, keyStatus = false, enabledManually = false;

void setup() {
#ifdef DEBUG
Serial.begin(9600);
#endif
pinMode(R_OFF,OUTPUT);
pinMode(R_SRC,OUTPUT);
pinMode(R_ATT,OUTPUT);
pinMode(R_DISPLAY,OUTPUT);
pinMode(R_UP,OUTPUT);
pinMode(R_DOWN,OUTPUT);
pinMode(R_VOL_UP,OUTPUT);
pinMode(R_VOL_DOWN,OUTPUT);
pinMode(R_SEL,OUTPUT);
pinMode(R_MODE,OUTPUT);
pinMode(T_ENABLE,OUTPUT);
pinMode(T_PLUS,OUTPUT);
pinMode(T_MINUS,OUTPUT);
pinMode(T_RESUME,OUTPUT);
pinMode(RADIO_PIN,OUTPUT);
pinMode(K_LEFT, INPUT_PULLUP);
pinMode(K_RIGHT, INPUT_PULLUP);
}

void loop() {

  void readValuesFromInputs();
  void checkRadioEnableLogic();

  if(pressedButtonVolUp()) {
    switch(currentMode) {
      case 0: 
        digitalWrite(R_VOL_UP,HIGH);
        break;
      case 1:
        if((millis() - timeTPlus) > 100) {
          timeTPlus = millis();
          digitalWrite(T_PLUS,HIGH);
        }
        break;
    }
  }
  else {
    digitalWrite(R_VOL_UP,LOW);
    digitalWrite(T_PLUS,LOW);
  }

  if(pressedButtonVolDown()) {
    switch(currentMode) {
      case 0: 
        digitalWrite(R_VOL_DOWN,HIGH);
        break;
      case 1:
        if((millis() - timeTMinus) > 100) {
          timeTMinus = millis();
          digitalWrite(T_MINUS,HIGH);
        }
        break;
    }
  }
  else {
    digitalWrite(R_VOL_DOWN,LOW);
    digitalWrite(T_MINUS,LOW);
  }

  if(pressedButtonMute()) {
    digitalWrite(R_ATT,HIGH);
  }
  else {
    digitalWrite(R_ATT,LOW);
  }

  if(pressedButtonWindows()) {
    switch(currentMode) {
      case 0: 
        //digitalWrite(R_OFF,HIGH);
        break;
      case 1:
        if((millis() - timeWindows) > 1000) {
          timeWindows = millis();
          cruiseState = !cruiseState;
          digitalWrite(T_ENABLE,cruiseState);
        }
        break;
     }
  }
  else {
    digitalWrite(R_OFF,LOW);
  }

  if(pressedButtonUp()) {
    digitalWrite(R_UP,HIGH);
  }
  else {
    digitalWrite(R_UP,LOW);
  }

  if(pressedButtonDown()) {
    digitalWrite(R_DOWN,HIGH);
  }
  else {
    digitalWrite(R_DOWN,LOW);
  }

  if(pressedButtonSrc()) {
    switch(currentMode) {
      case 0: 
        digitalWrite(R_SRC,HIGH);
        break;
      case 1:
        if((millis() - timeTRes) > 1000) {
          timeTRes = millis();
          digitalWrite(T_RESUME,HIGH);
        }
        break;
    }
  }
  else {
    digitalWrite(R_SRC,LOW);
    digitalWrite(T_RESUME,LOW);
  }

  if(pressedButtonMain()) {
    if((millis() - timeMain) > 1000) {
      timeMain = millis();
      currentMode = ++currentMode % 2;
    }
  }
  
delay(50);
}

void readValuesFromButtons() {
  leftRead = analogRead(K_LEFT);
  rightRead = analogRead(K_RIGHT);
  
  if(digitalRead(KEY_STATUS) == HIGH) {
    keyStatus = true;
  }
  else {
    keyStatus = false;
  }
  
  #ifdef DEBUG
  void showDebugValues();
  #endif
}

void showDebugValues() {
  Serial.print(leftRead);
  Serial.print(" ");
  Serial.print(rightRead);
  Serial.print(" ");
  Serial.println(keyStatus);
}

void checkRadioEnableLogic() {
  if(keyStatus == true) radioEnable();
  if(keyStatus == false && radioStatus == true && enabledManually == false) radioDisable();
  if(keyStatus == false && radioStatus == false && pressedButtonWindows()) radioEnableManually();
  if(keyStatus == false && radioStatus == true && pressedButtonWindows()) radioDisable();
  if(keyStatus == false && radioStatus == true && (millis() - radioEnableTime) > (RADIO_ENABLE_TIME * MILLISFORMINUTE)) radioDisable();
}


bool pressedButtonVolUp() {
if(leftRead >= 25 && leftRead <= 45) return 1;
else return 0;
}

bool pressedButtonVolDown() {
if(leftRead >= 10 && leftRead <= 25) return 1;
else return 0;
}

bool pressedButtonMute() {
if(leftRead >= 45 && leftRead <= 80) return 1;
else return 0;
}

bool pressedButtonWindows() {
  if(leftRead >= 100 && leftRead <= 140) {
    if((millis() - timeWindows) > 1000) {
      timeWindows = millis();
      return 1; //Windows
    }
  }
  return 0;
}

bool pressedButtonUp() {
if(rightRead >= 24 && rightRead <= 45) return 1;
else return 0;
}

bool pressedButtonDown() {
if(rightRead >= 10 && rightRead <= 23) return 1;
else return 0;
}

bool pressedButtonSrc() {
if(rightRead >= 45 && rightRead <= 75) return 1;
else return 0;
}

bool pressedButtonMain() {
if(rightRead >= 100 && rightRead <= 140) return 1;
else return 0;
}

void radioEnableManually() {
  digitalWrite(RADIO_PIN, HIGH);
  digitalWrite(SOCKET_PIN, HIGH);
  radioEnableTime = millis();
  radioStatus = true;
  enabledManually = true;
}

void radioEnable() {
  digitalWrite(RADIO_PIN, HIGH);
  digitalWrite(SOCKET_PIN, HIGH);
  radioStatus = true;
  enabledManually = false;
}


void radioDisable() {
  digitalWrite(RADIO_PIN, LOW);
  digitalWrite(SOCKET_PIN, LOW);
  radioStatus = false;
}

  
