#include <stdio.h>              // Global includes
#include "lpc17xx_gpio.h"       // Central include files
#include "lpc17xx_systick.h"	
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "utils.h"              // Local functions
#include "i2c.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "utils.h"

char output[128];
int len;

void main(void)
{
    initAll();

    i2cScanAll();    
    timerSleep(1);

    //Display Hello
    len = sprintf(output, "Hello");
    lcdWrite(output, len);

    timerSleep(1);

    //Display World
    lcdClear();
    lcdLineSelect(2);
    len = sprintf(output, " World");
    lcdWrite(output, len);

    timerSleep(1);

    lcdClear();

    int bufferSize = 7;
    int buffer[bufferSize];
    fillArray(buffer, 32, bufferSize);

    int count = 0;
    int keypadButton;
    while(1)
    {
        //Check Every 10ms
        if(timeElapsed() > (float)0.01)
        {    
            //Read key value        
            keypadButton = keypadRead();
            timerReset();
            if(keypadButton > 0)
            {
                //Add to array of keys
                buffer[count] = keypadButton;
                if(++count>=bufferSize)
                {
                    count = 0;
                }
                
                //Print keys to screen
                checkButton(keypadButton, buffer, &count, bufferSize);

            }

            lcdLineSelect(1);
            len = sprintf(output, "Buttons:");

            //Add keys to end of output
            for(int a=0; a<bufferSize; a++)
            {
                output[a+len] = buffer[a];
            }
            lcdWrite(output, len+bufferSize);
        }
    }  
}