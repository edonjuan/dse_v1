/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/drivers/GPIO.h>


#include <stdbool.h>
#include <driverlib/gpio.h>
#include <inc/hw_memmap.h>
#include <driverlib/interrupt.h>
#include <inc/hw_ints.h>

/* Board Header file */
#include "Board.h"

#define TASKSTACKSIZE   512
#define HWINT INT_GPIOP1

// Global variables and objects
Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
Hwi_Handle myHwi;

Void heartBeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep((unsigned int)arg0);
        GPIO_toggle(Board_LED0);
        //Hwi_post(HWINT);
    }
}

Void myIsr(UArg arg)
{
    GPIO_toggle(Board_LED1);
    GPIOIntClear(GPIO_PORTP_BASE, GPIO_INT_PIN_1);
}

/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;
    Hwi_Params hwiParams;
    Error_Block eb;

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams, NULL);

    /* Construct Hardware Interrupt thread */
    Hwi_Params_init(&hwiParams);
    Error_init(&eb);
    hwiParams.maskSetting = Hwi_MaskingOption_SELF;
    myHwi = Hwi_create(HWINT, &myIsr, &hwiParams, &eb);
    if(Error_check(&eb)){
        // Handle the error
    }

     /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    /* Set pin 1 of GPIO port P to digital input */
    GPIODirModeSet(GPIO_PORTP_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN);
    /* Configure STRENGHT (2mA) and PULL-UP Resistors */
    GPIOPadConfigSet(GPIO_PORTP_BASE, GPIO_PIN_1,
                     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    /* Interrupt type RISING */
    GPIOIntTypeSet(GPIO_PORTP_BASE, GPIO_INT_PIN_1, GPIO_RISING_EDGE | GPIO_DISCRETE_INT);

    /* Enable Interrupts */
    GPIOIntEnable(GPIO_PORTP_BASE, GPIO_INT_PIN_1);

    /* Start BIOS */
    BIOS_start();

    return (0);
}
