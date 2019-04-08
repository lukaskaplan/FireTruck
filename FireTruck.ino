#include "FastLED.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

/********** PIN Configuration **********/
#define PIN_WHITE_BTN     2   // HIGH = pushed
#define PIN_BLUE_BTN      3   // HIGH = pushed
#define PIN_BEACON_SW     4   // LOW = switched
#define PIN_ENGINE_DFP_RX 6   // Engine DFPlayer serial
#define PIN_ENGINE_DFP_TX 7   // Engine DFPlayer serial
#define PIN_BEACON_DFP_RX 8   // Beacon DFPlayer serial
#define PIN_BEACON_DFP_TX 9   // Beacon DFPlayer serial
#define PIN_BEACON_SWLED  10  // Led indication in the beacon switch. Switched by MOSFET module.
#define PIN_BEACON_PWR    11  // Switch on/off power for digital led strip. Switched by RELAY module.
#define PIN_BEACON_DATA   12  // Data pin for digital RGB led strip (WS2812).

int pinUnused[] = {0, 1, 2, 5, 13};                             // Unused digital pins.
int pinUnusedCount = sizeof(pinUnused) / sizeof(pinUnused[0]);  // Count unused pins.

/********** Configuration ***********/
#define BEACON_NUM_LEDS   14  // How many leds are in the digital led strip.
#define ENGINE_VOLUME     30  // Set volume value. From 0 to 30
#define BEACON_VOLUME     30  // Set volume value. From 0 to 30

// Button states
#define PUSSHED       HIGH
#define NOT_PUSSHED   LOW

// Beacon switch states
#define SW_ON      LOW
#define SW_OFF     HIGH

// States for turn ON/OFF Relay or Mosfet
#define ON         HIGH
#define OFF        LOW

/********** Initialization **********/
SoftwareSerial ttyEngine(PIN_ENGINE_DFP_RX, PIN_ENGINE_DFP_TX); // RX, TX
SoftwareSerial ttyBeacon(PIN_BEACON_DFP_RX, PIN_BEACON_DFP_TX); // RX, TX

DFRobotDFPlayerMini dfpEngine;
DFRobotDFPlayerMini dfpBeacon;

CRGB leds[BEACON_NUM_LEDS];

int whiteBtnRead;
int blueBtnRead;
int beaconSwRead;

int whiteBtnState;
int blueBtnState;
int beaconSwState;

int whiteBtnLast = NOT_PUSSHED;
int blueBtnLast  = NOT_PUSSHED;
int beaconSwLast = SW_OFF;

int whiteBtnLastStable  = NOT_PUSSHED;
int blueBtnLastStable   = NOT_PUSSHED;
int beaconSwLastStable = SW_OFF;

int beaconSwLed = OFF;
int beaconPwr   = OFF;

/* Beacon Mode:
    0 = off
    1 = blink without sound
    2 = blink with sound 1 (15s)
    3 = blink with sound 2 (15s)
*/
int beaconMode = 0; // 0 to 3

unsigned long lastWhiteBtnTime  = 0;   // the last time the output pin was toggled
unsigned long lastBlueBtnTime   = 0;    // the last time the output pin was toggled
unsigned long lastBeaconSwTime  = 0;   // the last time the output pin was toggled
unsigned long debounceDelay     = 50;     // The debounce time; Increase if the output flickers.

static unsigned long currentMillis = 0;
int ton, toff, tgap;

int beaconFLState   = 0;  //1=light,0=dark
int beaconFRState   = 0;  //1=light,0=dark
int beaconRLState   = 0;  //1=light,0=dark
int beaconRRState   = 0;  //1=light,0=dark
int beaconFrontState  = 0;  //1=light,0=dark
int beaconRearState   = 0;  //1=light,0=dark

unsigned long beaconRotationMillis = 0;
int beaconRotationI = 0;

unsigned long beaconFLMillis  = 0;
unsigned long beaconFRMillis  = 0;
unsigned long beaconFrontMillis  = 0;
unsigned long beaconRearMillis   = 0;
int beaconCounter   = 0;

// Engine sound state (0=stop,1=playing)
int engineState = 0;

// Beacon switch indication led
int beaconSwLEDState   = 0;  //1=light,0=dark
unsigned long beaconSwLEDMillis  = 0;

/***********************************************************************************/
/***************************** S E T U P *******************************************/
/***********************************************************************************/
void setup() {
  Serial.begin(9600);
  ttyBeacon.begin(9600);
  ttyEngine.begin(9600);

  dfpBeacon.begin(ttyBeacon);
  dfpEngine.begin(ttyEngine);

  dfpBeacon.volume(BEACON_VOLUME);
  dfpEngine.volume(ENGINE_VOLUME);

  dfpEngine.playMp3Folder(2); //start engine


  pinMode(PIN_WHITE_BTN, INPUT);
  pinMode(PIN_BLUE_BTN, INPUT);
  pinMode(PIN_BEACON_SW, INPUT);
  pinMode(PIN_BEACON_SWLED, OUTPUT);
  pinMode(PIN_BEACON_PWR, OUTPUT);

  FastLED.addLeds<WS2812, PIN_BEACON_DATA, GRB>(leds, BEACON_NUM_LEDS);

  for (int i = 0; i < pinUnusedCount; i++) {
    pinMode(pinUnused[i], INPUT_PULLUP);   // Set all unused digital pins to input mode with internal pullup.
  }

  delay(3000);

} //eof setup
/***********************************************************************************/
/**************************** L O O P **********************************************/
/***********************************************************************************/
void loop() {
  if (engineState == 0) {
    dfpEngine.playMp3Folder(3); //running engine
    engineState = 1;
  }

  currentMillis = millis();

  blueBtnRead   = digitalRead(PIN_BLUE_BTN);
  whiteBtnRead  = digitalRead(PIN_WHITE_BTN);
  beaconSwRead  = digitalRead(PIN_BEACON_SW);

  // If the button / switch changed, due to noise or pressing:
  if (whiteBtnRead != whiteBtnLast) lastWhiteBtnTime = currentMillis;  // Reset the debouncing timer
  if (blueBtnRead != blueBtnLast)   lastBlueBtnTime = currentMillis;     // Reset the debouncing timer
  if (beaconSwRead != beaconSwLast) lastBeaconSwTime = currentMillis;  // Reset the debouncing timer

  if ((currentMillis - lastWhiteBtnTime) > debounceDelay)  whiteBtnState = whiteBtnRead; // Set actual button state
  if ((currentMillis - lastBlueBtnTime) > debounceDelay)   blueBtnState = blueBtnRead; // Set actual button state
  if ((currentMillis - lastBeaconSwTime) > debounceDelay)  beaconSwState = beaconSwRead; // Set actual button state

  if (whiteBtnState == PUSSHED && whiteBtnLastStable  == NOT_PUSSHED ) {
    dfpEngine.advertise(1);
  }
  if (whiteBtnState == NOT_PUSSHED && whiteBtnLastStable  == PUSSHED ) {
    dfpEngine.stopAdvertise();
  }

  if (beaconSwState == SW_ON) {
    digitalWrite(PIN_BEACON_PWR, ON);
    //digitalWrite(PIN_BEACON_SWLED, ON);
    if (beaconMode == 0) {
      beaconMode = 1;
      darkAll();
    }
    if (blueBtnState == PUSSHED && blueBtnLastStable == NOT_PUSSHED) {
      Serial.println("INFO - blue button pusshed.");
      if (beaconMode < 3) beaconMode++;
      else beaconMode = 1;
      Serial.print("INFO - beacon mode: ");
      Serial.println(beaconMode);
      darkAll();
      if (beaconMode == 1) {
        dfpBeacon.stop();

      }
      if (beaconMode == 2) dfpBeacon.playMp3Folder(1);
      if (beaconMode == 3) dfpBeacon.playMp3Folder(2);
    }
    else if (blueBtnState == NOT_PUSSHED && blueBtnLastStable == PUSSHED) {
      Serial.println("INFO - blue button released.");
    }
  }
  else {
    digitalWrite(PIN_BEACON_PWR, OFF);
    digitalWrite(PIN_BEACON_SWLED, OFF);
    beaconMode = 0;
    dfpBeacon.stop();
  }

  switch (beaconMode) {
    case 0:
      darkAll();
      break;
    case 1:
      beaconFrontSlowBlink();
      beaconRearRotate();
      digitalWrite(PIN_BEACON_SWLED, ON);
      break;
    case 2:
      beaconFrontSlowBlink();
      beaconRearRotate();
      beaconSwLEDBlink1();
      break;
    case 3:
      beaconFastBlink();
      beaconSwLEDBlink2();
      break;
    default:
      Serial.println("ERROR - beaconMode out of range");
  }

  //Serial.print("Beacon Mode = ");
  //Serial.println(beaconMode);
  //Serial.println(blueBtnState);

  whiteBtnLast  = whiteBtnRead;
  blueBtnLast   = blueBtnRead;
  beaconSwLast  = beaconSwRead;

  whiteBtnLastStable  = whiteBtnState;
  blueBtnLastStable   = blueBtnState;
  beaconSwLastStable  = beaconSwState;

  FastLED.show();   //Set actual digital led strip's state.
} // eof loop()

/***********************************************************************************/
/**************************** F U N C T I O N S ************************************/
/***********************************************************************************/
void beaconSwLEDBlink1() {
  ton = 1000;
  toff = 200;
  if ((beaconSwLEDState == 0) && ((currentMillis - beaconSwLEDMillis) >= toff)) {
    digitalWrite(PIN_BEACON_SWLED, ON);
    beaconSwLEDMillis = currentMillis;
    beaconSwLEDState = 1;
  }
  if ((beaconSwLEDState == 1) && ((currentMillis - beaconSwLEDMillis) >= ton)) {
    digitalWrite(PIN_BEACON_SWLED, OFF);
    beaconSwLEDMillis = currentMillis;
    beaconSwLEDState = 0;
  }
}

void beaconSwLEDBlink2() {
  ton = 100;
  toff = 100;
  if ((beaconSwLEDState == 0) && ((currentMillis - beaconSwLEDMillis) >= toff)) {
    digitalWrite(PIN_BEACON_SWLED, ON);
    beaconSwLEDMillis = currentMillis;
    beaconSwLEDState = 1;
  }
  if ((beaconSwLEDState == 1) && ((currentMillis - beaconSwLEDMillis) >= ton)) {
    digitalWrite(PIN_BEACON_SWLED, OFF);
    beaconSwLEDMillis = currentMillis;
    beaconSwLEDState = 0;
  }
}

void beaconFrontSlowBlink() {
  ton = 500;
  toff = 500;
  if ((beaconFLState == 0) && ((currentMillis - beaconFLMillis) >= toff)) {
    lightAllFL();
    darkAllFR();
    beaconFLMillis = currentMillis;
  }
  if ((beaconFLState == 1) && ((currentMillis - beaconFLMillis) >= ton)) {
    darkAllFL();
    lightAllFR();
    beaconFLMillis = currentMillis;
  }

}
void beaconRearRotate() {
  static int t = 150;
  int i = beaconRotationI;
  int a1, b1, c1;
  int a2, b2, c2;

  if (currentMillis - beaconRotationMillis >= t) {
    a1 = i + 8;
    a2 = i + 8 + 3;
    b1 = (i + 1) % 3 + 8;
    b2 = (i + 1) % 3 + 8 + 3;
    c1 = (i + 2) % 3 + 8;
    c2 = (i + 2) % 3 + 8 + 3;

    leds[a1] = 0x0000FF;
    leds[a2] = 0x000000;
    leds[b1] = 0x000000;
    leds[b2] = 0x000001;
    leds[c1] = 0x000001;
    leds[c2] = 0x0000FF;
    //FastLED.show();
    beaconRotationMillis = currentMillis;
    i++;
    if (i > 2) i = 0;
    beaconRotationI = i;
  }
}

void beaconFastBlink() {
  ton = 50;
  toff = 50;
  tgap = 200;
  static int lrb = 0; //left or right toggle
  // Blink Front Left beacon
  if ((beaconCounter < 3) && (lrb == 0)) {
    if ((beaconFLState == 0) && ((currentMillis - beaconFLMillis) >= toff)) {
      lightAllFL();
      lightAllRR();
      beaconFLMillis = currentMillis;
    }
    if ((beaconFLState == 1) && ((currentMillis - beaconFLMillis) >= ton)) {
      darkAllFL();
      darkAllRR();
      beaconFLMillis = currentMillis;
      beaconCounter++;
    }
  }
  // Blink Front Right beacon
  else if ((beaconCounter < 3) && (lrb == 1)) {
    if ((beaconFRState == 0) && ((currentMillis - beaconFRMillis) >= toff)) {
      lightAllFR();
      lightAllRL();
      beaconFRMillis = currentMillis;
    }
    if ((beaconFRState == 1) && ((currentMillis - beaconFRMillis) >= ton)) {
      darkAllFR();
      darkAllRL();
      beaconFRMillis = currentMillis;
      beaconCounter++;
    }
  }
  // Blink both front beacons
  else if ((beaconCounter < 3) && (lrb == 2)) {
    if ((beaconFrontState == 0) && ((currentMillis - beaconFrontMillis) >= toff)) {
      lightAllFront();
      lightAllRear();
      beaconFrontMillis = currentMillis;
    }
    if ((beaconFrontState == 1) && ((currentMillis - beaconFrontMillis) >= ton)) {
      darkAllFront();
      darkAllRear();
      beaconFrontMillis = currentMillis;
      beaconCounter++;
    }
  }
  // Pause after 3 blinks - left side
  else if ((beaconCounter >= 3) && ((currentMillis - beaconFLMillis) >= tgap) && lrb == 0) {
    beaconFLMillis = currentMillis;
    beaconFRMillis = currentMillis;
    beaconFrontMillis = currentMillis;
    beaconCounter = 0;
    lrb = 1;
  }
  // Pause after 3 blinks - right side
  else if ((beaconCounter >= 3) && ((currentMillis - beaconFRMillis) >= tgap) && lrb == 1) {
    beaconFLMillis = currentMillis;
    beaconFRMillis = currentMillis;
    beaconFrontMillis = currentMillis;
    beaconCounter = 0;
    lrb = 2;
  }
  // Pause after 3 blinks - both sides
  else if ((beaconCounter >= 3) && ((currentMillis - beaconFrontMillis) >= tgap) && lrb == 2) {
    beaconFLMillis = currentMillis;
    beaconFRMillis = currentMillis;
    beaconFrontMillis = currentMillis;
    beaconCounter = 0;
    lrb = 0;
  }

}

void lightAll() {
  for (int i = 0; i < 14; i++) {
    leds[i] = 0x0000ff;
    beaconFLState   = 1;
    beaconFRState   = 1;
    beaconRLState   = 1;
    beaconRRState   = 1;
    //FastLED.show();
  }
}
void darkAll() {
  for (int i = 0; i < 14; i++) {
    leds[i] = 0x000000;
  }
  beaconFLState   = 0;
  beaconFRState   = 0;
  beaconRLState   = 0;
  beaconRRState   = 0;
  //FastLED.show();
  //return leds;
}
//-------------------------------------
void lightAllFL() {
  for (int i = 0; i < 4; i++) {
    leds[i] = 0x0000ff;
    beaconFLState   = 1;
    //FastLED.show();
  }
}
void darkAllFL() {
  for (int i = 0; i < 4; i++) {
    leds[i] = 0x000000;
    beaconFLState   = 0;
    //FastLED.show();
  }
}
//-------------------------------------
void lightAllFR() {
  for (int i = 4; i < 8; i++) {
    leds[i] = 0x0000ff;
    beaconFRState   = 1;
    //FastLED.show();
  }
}
void darkAllFR() {
  for (int i = 4; i < 8; i++) {
    leds[i] = 0x000000;
    beaconFRState   = 0;
    //FastLED.show();
  }
}
//-------------------------------------
void lightAllRR() {
  for (int i = 8; i < 11; i++) {
    leds[i] = 0x0000ff;
    beaconRRState   = 1;
  }
}
void darkAllRR() {
  for (int i = 8; i < 11; i++) {
    leds[i] = 0x000000;
    beaconRRState   = 0;
  }
}
//-------------------------------------
void lightAllRL() {
  for (int i = 11; i < 14; i++) {
    leds[i] = 0x0000ff;
    beaconRLState   = 1;
  }
}
void darkAllRL() {
  for (int i = 11; i < 14; i++) {
    leds[i] = 0x000000;
    beaconRLState   = 0;
  }
}
//-------------------------------------
void lightAllFront() {
  for (int i = 0; i < 8; i++) {
    leds[i] = 0x0000ff;
    beaconFLState      = 1;
    beaconFRState      = 1;
    beaconFrontState   = 1;
  }
}
void darkAllFront() {
  for (int i = 0; i < 8; i++) {
    leds[i] = 0x000000;
    beaconFLState     = 0;
    beaconFRState     = 0;
    beaconFrontState  = 0;
  }
}
//-------------------------------------
void lightAllRear() {
  for (int i = 8; i < 14; i++) {
    leds[i] = 0x0000ff;
    beaconRLState   = 1;
    beaconRRState   = 1;
    beaconRearState = 1;
  }
}
void darkAllRear() {
  for (int i = 8; i < 14; i++) {
    leds[i] = 0x000000;
    beaconRLState   = 0;
    beaconRRState   = 0;
    beaconRearState = 0;
  }
}
