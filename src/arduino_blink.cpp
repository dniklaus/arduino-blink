// Do not remove the include below
#include "arduino_blink.h"

#include "Timer.h"
#include "BlinkTimerControl.h"

void toggleLed(int ledPin)
{
  bool isLedOn = digitalRead(ledPin);
  digitalWrite(ledPin, !isLedOn);
}

// global variable definition
BlinkTimerControl* blinkTimerControl = 0;

// global constant definition
const unsigned int BLINK_TIME_MILLIS = 200;

class BlinkTimerAdapter : public TimerAdapter
{
public:
  void timeExpired()
  {
    toggleLed(LED_BUILTIN);

    if (0 != blinkTimerControl)
    {
      blinkTimerControl->timeExpired();
    }
  }
};

//-----------------------------------------------------------------------------
/*
 SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent()
{
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    if ('d' == inChar)
    {
      blinkTimerControl->decrementBlinkTime();
    }
    else if ('i' == inChar)
    {
      blinkTimerControl->incrementBlinkTime();
    }
  }
}
//-----------------------------------------------------------------------------

//The setup function is called once at startup of the sketch
void setup()
{
  //---------------------------------------------------------------------------
  // Debugging
  //---------------------------------------------------------------------------
  Serial.begin(115200);
  Serial.println(F("Hello from Arduino Blink!\n"));

  //---------------------------------------------------------------------------
  // configure LED pin
  //---------------------------------------------------------------------------
  pinMode(LED_BUILTIN, OUTPUT);

  //---------------------------------------------------------------------------
  // setup blink timer control
  //---------------------------------------------------------------------------
  BlinkTimerAdapter* blinkTimerAdapter = new BlinkTimerAdapter();
  blinkTimerControl = new BlinkTimerControl(blinkTimerAdapter, BLINK_TIME_MILLIS);
}

// The loop function is called in an endless loop
void loop()
{
  yield();
}
