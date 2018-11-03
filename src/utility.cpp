
#include <Arduino.h>
#include "utility.h"
#include <WiFi101.h>

typedef enum {
	M2M_PERIPH_GPIO3, /*!< GPIO15 pad	*/
	M2M_PERIPH_GPIO4, /*!< GPIO16 pad	*/
	M2M_PERIPH_GPIO5, /*!< GPIO18 pad	*/
	M2M_PERIPH_GPIO6, /*!< GPIO18 pad	*/
	M2M_PERIPH_GPIO15, /*!< GPIO15 pad	*/
	M2M_PERIPH_GPIO16, /*!< GPIO16 pad	*/
	M2M_PERIPH_GPIO18, /*!< GPIO18 pad	*/
	M2M_PERIPH_GPIO_MAX
} tenuGpioNum;

static uint8_t blinkVal;

void utility::debugPrint(char *stuff) {
#ifdef TEST
    Serial.print(stuff);
#endif
    return;
}

void utility::debugPrintln(char *stuff) {
#ifdef TEST
    Serial.println(stuff);
#endif
    return;
}


void utility::setWiFiLed(uint8_t val) {
    blinkVal = val;
    set_gpio_dir(M2M_PERIPH_GPIO15, OUTPUT);
    set_gpio_val(M2M_PERIPH_GPIO15, val);
    return;

}
void utility::blinkWiFiLed(void) {
    blinkVal ^= blinkVal;
    this->setWiFiLed(blinkVal);

}