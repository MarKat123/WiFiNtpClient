#include <stdint.h>

class freq60Khz
{
  public:
    char begin(uint8_t oscPin);
    bool isRunning();
    void out(uint16_t frequency, uint32_t duration = 0);
    void stop();

  private:
    static uint8_t _osc_pin_count;
    uint8_t _pin;
    int8_t _timer;
};


