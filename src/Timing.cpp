#include "Timing.h"

void Timing::waitingLoop(unsigned int timeInMs)
{
    unsigned long previousTime = millis();
    bool waiting = false;
    while (millis() <= previousTime + timeInMs)
    {
        waiting = true;
    }
    waiting = false;
}