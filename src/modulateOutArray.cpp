
#include "modulateOutArray.h"
#include "freq60Khz.h"
#include "computeDate.h"
#include <arduino.h>
#include "arrLabels.h"

int hour;
int timerSel ;
static uint8_t blinkVal = LOW;

freq60Khz thisFreq;

void setModulator(int);

void modulateOutArray::setBlinkVal(int val) {
    blinkVal = val;
    digitalWrite(LED_BUILTIN,blinkVal);
}

void modulateOutArray::blink(void) {
    blinkVal ^= 1;
    digitalWrite(LED_BUILTIN,blinkVal);
    return;
}

bool modulateOutArray::begin(int hour) {
    if ((hour >= 0) && (hour < 25)) {
        if (thisFreq.isRunning())  return true;
        
        timerSel = thisFreq.begin(11);
        thisFreq.out( 60000 , 0 ) ;
        return true;
    }
    modulateOutArray::stopModulator();
    return false;
}


void modulateOutArray::doList(int outArray[]) {
    int ii, symbol, secsLeft;
    const long lastInx = 59L;
    const int secTomSec = 1000;
    long waitForMarker ;
    // Usually we would want to wait  until the number of seconds times out
    // to pass the 00 boundary
    // so - need to increase the original seconds from 1900 by 60 seconds to 
    // put us ready for the next minute
    // This needs to be done in WiFiNtpClient.ino
    // One other thing is that there is a 00 marker that starts on .59 of the
    // minute prior.  I will initially start this off as a -1 entry to modulate
    // a marker and then start the Array
    secsLeft = outArray[Sec];
    if (secsLeft > 59) {
        Serial.print("secsLeft");
        Serial.print(secsLeft);
        Serial.println(" seconds .. Error!");
    }
    if (secsLeft >= 1) {
        waitForMarker = (lastInx - secsLeft ) * secTomSec ;
        //Serial.print("Wait for ");
        //Serial.println(waitForMarker);
        delay(waitForMarker);
    }

    // Send out first of double markers, second will be bit 0
    // of array list


    //Serial.println("Modulate!");
    setModulator(0);
    delay(800);
    setModulator(1);
    delay(200);

    for (ii=0; ii <= 59; ii++) {
        this->blink();
        //Serial.println("this->blink()");
        symbol = outArray[ii];
        switch(symbol) {
            case 3:
               setModulator(0);
               delay(800);
               setModulator(1);
               delay(200);
               break;
            
            case 0:
                setModulator(0);
                delay(200);
                setModulator(1);
                delay(800);
                break;

            case 1:
                setModulator(0);
                delay(500);
                setModulator(1);
                delay(500);
                break;

            default:
                setModulator(0);
                delay(800);
                setModulator(1);
                delay(200);
                break;
        }
    }
    this->setBlinkVal(LOW);
}

void setModulator(int symOut) {
    const int modPin = 12;
    switch(symOut) {
        case 0:
            pinMode(modPin, OUTPUT);
            digitalWrite(modPin, LOW);
            return;
        case 1:
            pinMode(modPin, INPUT);
            digitalWrite(modPin, LOW);
            return;
    }
}

void modulateOutArray::stopModulator(void) {
    const int modPin = 12;
    const int oscPin = 11;
    thisFreq.stop();
    pinMode(modPin, INPUT);
    digitalWrite(modPin, LOW);
    pinMode(oscPin, INPUT);
    digitalWrite(oscPin, LOW);
}