#include <__cross_studio_io.h>
#include "stm32f0xx.h"

//Comments are based on Reference Manual RM0091
//available at http://www.st.com/content/ccc/resource/technical/document/reference_manual/c2/f8/8a/f2/18/e6/43/96/DM00031936.pdf/files/DM00031936.pdf/jcr:content/translations/en.DM00031936.pdf

volatile int cnt; //Declared voaltile to prevent compiler from register optimization. This is MANDATORY when a variable is accessed by interrupts and compiler optimization is enabled.

uint32_t usr_btn()
{
return ((GPIOA->IDR) & (1<<0)); //Read the input register of GPIOA and mask Pin A0 (1<<0) Returns 1 of pin is high and 0 if pin is low.
}

void gpio_set(GPIO_TypeDef * gpio,uint8_t pin)
{
  gpio->BSRR=(1<<(pin%16)); //Write pin number to the corresponding position of the Bit Set part of the BSRR register (see section 9.4.7)
}
void gpio_reset(GPIO_TypeDef * gpio,uint8_t pin)
{
  gpio->BSRR=(1<<(16+(pin%16))); //Write pin number to the corresponding position of the Bit Reset part of the BSRR register (see section 9.4.7)
}

void main(void)
{
  SystemInit();
 // Turn on GPIOC and GPIOA
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN; //Give Clock to the GPIO peripherals of the Chip (See section 7.4.6)
  GPIOC->MODER |=(1<<(2*9))|(1<<(2*8)); //Set Pins C8 and C9 to Output Push-Pull (see section 9.4.1)

    if (SysTick_Config((8000000 / 1000))) //Running at 8MHz we want to have 1000 Systick Interrupts per second -> One SysTick each 8000 cycles
    { 
        /* Capture error */ 
        while (1); //If this happens something is really wrong with the core
    }

  for(;;) //Infinite loop in the main loop because everything is running in the SysTick interrupt
  {
  }
  //Will not be reached anyway
  debug_exit(0); 
}

void SysTick_Handler(void)
{
    if(!usr_btn()) //While the button is pushed the counter stops counting
      cnt=(cnt+1)%1000; //Count up, if 1000 is reached the counter variabl is reset.
    if(500==cnt) //When Counter value is set to 500...
    {
      gpio_reset(GPIOC,8); //GPIO C8 goes low
      gpio_set(GPIOC,9);   //GPIO C9 goes high      
    }
    if(999==cnt)
    {
      gpio_set(GPIOC,8);   //GPIO C8 goes high      
      gpio_reset(GPIOC,9); //GPIO C9 goes low
    }
}