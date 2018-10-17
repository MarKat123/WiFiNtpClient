
#include "modulateOutArray.h"
#include "freq60Khz.h"
#include "computeDate.h"
#include <arduino.h>

int hour;
int timerSel ;

freq60Khz thisFreq;

void setModulator(int);

bool modulateOutArray::begin(int hour) {
    if (thisFreq.isRunning()) {
        return true;
    }
    if ((hour >= 20) || (hour < 2)) {
        timerSel = thisFreq.begin(11);
        thisFreq.out( 60000 , 0 ) ;

#ifdef TEST
        Serial.print("\nTimer: ");
        Serial.println(timerSel);
#endif
        return true;
    }
    return false;
}


void modulateOutArray::doList(int outArray[]) {
    int ii, symbol, secsLeft;
    // int lastInx = 59;
    // Usually we would want to wait  until the number of seconds times out
    // to pass the 00 boundary
    // so - need to increase the original seconds from 1900 by 60 seconds to 
    // put us ready for the next minute
    // This needs to be done in WiFiNtpClient.ino
    // One other thing is that there is a 00 marker that starts on .59 of the
    // minute prior.  I will initially start this off as a -1 entry to modulate
    // a marker and then start the Array
    secsLeft = outArray[64];
    if (secsLeft >= 1) {
        delay( (59-secsLeft) * 1000);
    }

    // Send out first of double markers, second will be bit 0
    // of array list

    setModulator(0);
    delay(800);
    setModulator(1);
    delay(200);

    for (ii=0; ii <= 59; ii++) {
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
}

void setModulator(int symOut) {
    const int modPin = 13;
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
    const int modPin = 13;
    const int oscPin = 11;
    thisFreq.stop();
    pinMode(modPin, INPUT);
    digitalWrite(modPin, LOW);
    pinMode(oscPin, INPUT);
    digitalWrite(oscPin, LOW);
}