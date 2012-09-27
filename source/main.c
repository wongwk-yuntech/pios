//
//  main.c
//  BakingPi
//
//  Created by Jeremy Pereira on 24/09/2012.
//  Copyright (c) 2012 Jeremy Pereira. All rights reserved.
//

#include "bptypes.h"
#include "gpio.h"
#include "SystemTimer.h"

extern const char* kernelArgs[];
const char* kernelArgs[] = { "BakeOS", NULL };

static bool SOSSequence[] =
{
    false, false, false, false,
	true, false, true, false, true, false,
	true, true, true, false, true, true, true, false, true, true, true, false,
	true, false, true, false, true
};
/*!
 *  @brief Run a sequence of LED flashes.
 *  @param iterations how many times to run the sequence.  If negative, we 
 *  don't ever stop.
 *  @param flashLength Length of each flash or non flash in microseconds.
 *  @param sequence A sequence of booleans true = LED on, false = LED off
 *  @param sequenceLength How long is the sequence.
 */
static void runLEDSequence(int iterations,
                           unsigned int flashLength,
                           bool* sequence,
                           size_t sequenceLength);

int main(int argc, char** argv)
{
    GPIO* gpio = getGPIOAddress();
    SystemTimer* timer = getSystemTimerAddress();
    setGPIOFunction(gpio, 16, GPIO_FN_OUTPUT);
    setGPIOPin(gpio, 16, false); // Turn on OK to start with as a diagnostic
    st_microsecondSpin(timer, 1000000); // and wait 1 second
    
    runLEDSequence(10, 250000, SOSSequence, sizeof(SOSSequence) / sizeof(bool));
    
    setGPIOPin(gpio, 16, false); // Turn on OK to start with as a diagnostic

    return 0;
}

void runLEDSequence(int iterations,
                    unsigned int flashLength,
                    bool* sequence,
                    size_t sequenceLength)
{
    GPIO* gpio = getGPIOAddress();
    SystemTimer* timer = getSystemTimerAddress();
    setGPIOFunction(gpio, 16, GPIO_FN_OUTPUT);

    int iterationsToGo = iterations > 0 ? iterations : 1;
    while (iterationsToGo > 0)
    {
        for (unsigned int i = 0 ; i < sequenceLength ; ++i)
        {
            setGPIOPin(gpio, 16, !sequence[i]);
            st_microsecondSpin(timer, flashLength);
        }
        if (iterations >= 0)
        {
            iterationsToGo--;
        }
    }
}
