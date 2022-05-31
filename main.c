#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "lcd.h"

#define SEC (60 +(millis / 1000))
int countMenu = 0;
bool state;
bool sidePlayer;
bool Key_R, Key_U, Key_D, Key_L, Key_E;
bool Button_W, Button_B;
bool exitMenu = 1;
bool blinkState;
bool state;
int setSecWhite;
int hourWhite;
int minWhite;
int secWhite;

int setSecBlack;
int hourBlack;
int minBlack;
int secBlack;

int setInc;

int cTemp = 0;
int cSecWhite = 0;
int cSecBlack = 0;

int blinkTime;
int initBlinkTime;
int blinkDelay = 30;

uint32_t millis = 0;

void SysTickIntHandler(void)
{
    millis++;
}

void ReadButton()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    Key_R = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2);
    Key_U = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3);
    Key_D = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);
    Key_L = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5);
    Key_E = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6);
    if (Key_R == 1)
    {
        countMenu++;
        if (countMenu > 4) countMenu = 0;
    }
    if (Key_U == 1)
    {
        if (countMenu == 0)
        {
            hourWhite++;
            if (hourWhite > 9) hourWhite = 0;
        }
        else if (countMenu == 1)
        {
            minWhite++;
            if (minWhite > 59) minWhite = 0;
        }
        else if (countMenu == 2)
        {
            secWhite++;
            if (secWhite > 59) secWhite = 0;
        }
        else if (countMenu == 3)
        {
            setInc++;
            if (setInc > 99) setInc = 0;
        }
        else
        {
            sidePlayer ^= 1;
        }
    }
    if (Key_D == 1)
    {
        if (countMenu == 0)
        {
            hourWhite--;
            if (hourWhite < 0) hourWhite = 9;
        }
        else if (countMenu == 1)
        {
            minWhite--;
            if (minWhite < 0) minWhite = 59;
        }
        else if (countMenu == 2)
        {
            secWhite--;
            if (secWhite < 0) secWhite = 59;
        }
        else if (countMenu == 3)
        {
            setInc--;
            if (setInc < 0) setInc = 99;
        }
        else
        {
            sidePlayer ^= 1;
        }
    }
    if (Key_L == 1)
    {
        countMenu--;
        if (countMenu < 0) countMenu = 4;
    }
    if (Key_E == 1) exitMenu = 0;

}

void ReadPlayerButton()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2);
    //WHITE: GPIO_PORTF_BASE, GPIO_PIN_1
    //BLACK: GPIO_PORTF_BASE, GPIO_PIN_2
    Button_W = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1);
    Button_B = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2);
}

void printTimerWhite()
{
    blinkTime = millis - blinkDelay;
    if (exitMenu == 1 && countMenu == 0)
    {
        if (blinkTime >= initBlinkTime && blinkState == 1)
        {
            LCD_Print(1, 0, " ");
            blinkState = 0;
            initBlinkTime = millis;
        }
        if (blinkTime >= initBlinkTime && blinkState == 0)
        {
            LCD_Printa(1, 0, hourWhite);
            blinkState = 1;
            initBlinkTime = millis;
        }
    }
    else
    {
        LCD_Printa(1, 0, hourWhite);

    }
    LCD_Print(1, 1, ":");
    if (exitMenu == 1 && countMenu == 1)
    {
        if (blinkTime >= initBlinkTime && blinkState == 1)
        {
            LCD_Print(1, 3, " ");
            blinkState = 0;
            initBlinkTime = millis;
        }
        if (blinkTime >= initBlinkTime && blinkState == 0)
        {
            if ((minWhite < 10) && (minWhite >= 0))
            {
                LCD_Printa(1, 2, 0);
                LCD_Printa(1, 3, minWhite);
            }
            else
            {
                LCD_Printa(1, 2, minWhite);
            }
            blinkState = 1;
            initBlinkTime = millis;
        }
    }
        else
        {
            if ((minWhite < 10) && (minWhite >= 0))
            {
                LCD_Printa(1, 2, 0);
                LCD_Printa(1, 3, minWhite);
            }
            else
            {
                LCD_Printa(1, 2, minWhite);
            }
        }

        LCD_Print(1, 4, ":");

    if (exitMenu == 1 && countMenu == 2)
    {
        if (blinkTime >= initBlinkTime && blinkState == 1)
        {
            LCD_Print(1, 6, " ");
            blinkState = 0;
            initBlinkTime = millis;
        }
        if (blinkTime >= initBlinkTime && blinkState == 0)
        {
            if ((secWhite < 10) && (secWhite >= 0))
            {
                LCD_Printa(1, 5, 0);
                LCD_Printa(1, 6, secWhite);
            }
            else
            {
                LCD_Printa(1, 5, secWhite);
            }
            blinkState = 1;
            initBlinkTime = millis;
        }
    }
    else
    {
        if ((secWhite < 10) && (secWhite >= 0))
        {
            LCD_Printa(1, 5, 0);
            LCD_Printa(1, 6, secWhite);
        }
        else
        {
            LCD_Printa(1, 5, secWhite);
        }
    }
}

void printTimerBlack()
{
    LCD_Printa(1, 9, hourBlack);
    LCD_Print(1, 10, ":");

    if ((minBlack < 10) && (minBlack >= 0))
    {
        LCD_Printa(1, 11, 0);
        LCD_Printa(1, 12, minBlack);
    }
    else
    {
        LCD_Printa(1, 11, minBlack);
    }
    LCD_Print(1, 13, ":");

    if ((secBlack < 10) && (secBlack >= 0))
    {
        LCD_Printa(1, 14, 0);
        LCD_Printa(1, 15, secBlack);
    }
    else
    {
        LCD_Printa(1, 14, secBlack);
    }
}
void printMenu()
{
    LCD_PrintLn(0, "SET UP W/ KEYPAD");
    LCD_Print(1, 8, "inc");

    blinkTime = millis - blinkDelay;
    if (countMenu == 3)
    {
        if (blinkTime >= initBlinkTime && blinkState == 1)
        {
            LCD_Print(1, 12, " ");
            blinkState = 0;
            initBlinkTime = millis;
        }
        if (blinkTime >= initBlinkTime && blinkState == 0)
        {
            if ((setInc < 10) && (setInc >= 0))
            {
                LCD_Printa(1, 11, 0);
                LCD_Printa(1, 12, setInc);
            }
            else
            {
                LCD_Printa(1, 11, setInc);
            }
            blinkState = 1;
            initBlinkTime = millis;
        }
    }
    else
    {
        if ((setInc < 10) && (setInc >= 0))
        {
            LCD_Printa(1, 11, 0);
            LCD_Printa(1, 12, setInc);
        }
        else
        {
            LCD_Printa(1,11, setInc);
        }
    }
    if (exitMenu == 1 && countMenu == 4)
    {
        if (blinkTime >= initBlinkTime && blinkState == 1)
        {
            LCD_Print(1, 14, "  ");
            blinkState = 0;
            initBlinkTime = millis;
        }
        if (blinkTime >= initBlinkTime && blinkState == 0)
        {
            if (sidePlayer == 1)
            {
                LCD_Print(1, 14, "WB");
            }
            else
            {
                LCD_Print(1, 14, "BW");
            }
            blinkState = 1;
            initBlinkTime = millis;
        }
    }
    else
    {
        if (sidePlayer == 1)
        {
            LCD_Print(1, 14, "WB");
        }
        else
        {
            LCD_Print(1, 14, "BW");
        }
    }
}

void printLabels()
{
    LCD_Clear();
    if (sidePlayer == 1)
    {   LCD_Cursor(0,0);
        LCD_Prints(0, 0, "WHITE");
        LCD_Cursor(0,11);
        LCD_Prints(0, 11, "BLACK");
    }
    else
    {   LCD_Cursor(0,0);
        LCD_Prints(0, 0, "BLACK");
        LCD_Cursor(0,11);
        LCD_Prints(0, 11, "WHITE");
    }
    LCD_Print(1, 7, "||");
    LCD_Prints(0, 6, "|  |");
    if ((setInc < 10) && (setInc >= 0))
    {
        LCD_Printa(0, 7, 0);
        LCD_Printa(0, 8, setInc);
    }
    else
    {
        LCD_Printa(0, 7, setInc);
    }
    printTimerWhite();
    printTimerBlack();
}

void timerWhite()
{
    secWhite = setSecWhite - cSecWhite;
    printTimerWhite();
    if ((secWhite == -1) && (minWhite >= 0))
    {
        setSecWhite = cSecWhite + 59;
        minWhite--;
    }
    if ((secWhite == -1) && (minWhite == -1) && (hourWhite > 0))
    {
        setSecWhite = cSecWhite + 59;
        minWhite = 59;
        hourWhite--;
    }
    if ((secWhite == 0) && (minWhite == 0) && (hourWhite == 0))
    while(1)
    {
        LCD_Prints(0, 10, "      ");
        SysCtlDelay(SysCtlClockGet()/30);
        LCD_Prints(0, 10, "WINNER");
        SysCtlDelay(SysCtlClockGet()/30);
    }
}

void timerBlack()
{
    secBlack = setSecBlack - cSecBlack;
    printTimerBlack();
    if ((secBlack == -1) && (minBlack >= 0))
    {
        setSecBlack = cSecBlack + 59;
        minBlack--;
    }
    if ((secBlack == -1) && (minBlack == -1) && (hourBlack > 0))
    {
        setSecBlack = cSecBlack + 59;
        minBlack = 59;
        hourBlack--;
    }
    if ((secBlack == 0) && (minBlack == 0) && (hourBlack == 0))
    {
        while(1)
        {
            LCD_Prints(0, 0, "      ");
            SysCtlDelay(SysCtlClockGet()/30);
            LCD_Prints(0, 0, "WINNER");
            SysCtlDelay(SysCtlClockGet()/30);
        }
    }
}

void incWhite()
{
    if (setInc > 0)
    {
        secWhite = secWhite + setInc;
        cSecWhite = setSecWhite - secWhite;
        if (secWhite > 59 && secWhite < 120)
        {
            secWhite = secWhite - 60;
            cSecWhite = setSecWhite - secWhite;
            minWhite++;
        }
        else if (secWhite > 59)
        {
            secWhite = secWhite - 60;
            cSecWhite = setSecWhite - secWhite;
            minWhite = minWhite + 1;
        }
        if (minWhite > 59)
        {
            minWhite = minWhite - 60;
            hourWhite++;
        }
        printTimerWhite();
    }
}

void incBlack()
{
    if (setInc > 0)
    {
        secBlack = secBlack + setInc;
        cSecBlack = setSecBlack - secBlack;
        if (secBlack > 59 && secBlack < 120)
        {
            secBlack = secBlack - 60;
            secBlack = setSecBlack - secBlack;
            minBlack++;
        }
        else if (secBlack > 59)
        {
            secBlack = secBlack - 60;
            cSecBlack = setSecBlack - secBlack;
            minBlack = minBlack + 1;
        }
        if (minBlack > 59)
        {
            minBlack = minBlack - 60;
            hourBlack++;
        }
        printTimerBlack();
    }
}

 int main(void)
 {
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    SysTickPeriodSet(SysCtlClockGet()/1000);
    SysTickIntRegister(&SysTickIntHandler);
    SysTickIntEnable();
    SysTickEnable();
    LCD_init();
    LCD_Clear();
    LCD_Print2Ln("GROUP 11", "Chess Clock");
    SysCtlDelay(SysCtlClockGet()*2/3);
    LCD_Clear();
    while (exitMenu == 1)
    {
        ReadButton();
        printMenu();
        printTimerWhite();
    }
    hourBlack = hourWhite;
    minBlack = minWhite;
    secBlack = secWhite;
    setSecBlack = secWhite;
    setSecWhite = secWhite;
    printLabels();
    while(1)
    {
  while (exitMenu==0){
        ReadPlayerButton();
        if (Button_W == 1)
        {
            cTemp = SEC - cSecWhite;
            while (Button_B == 0)
            {
               cSecWhite = SEC - cTemp;
                timerWhite();
                ReadPlayerButton();
            }

            incWhite();
        }
        ReadPlayerButton();
        if (Button_B ==1)
        {
            cTemp = SEC - cSecBlack;
            while (Button_W == 0 )
            {
                cSecBlack = SEC - cTemp;
                timerBlack();
                ReadPlayerButton();
            }
            incBlack();
       }
    }
  }
}
