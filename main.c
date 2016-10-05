#include <__cross_studio_io.h>
#include "stm32f0xx.h"

volatile int cnt;

void gpio_set(GPIO_TypeDef * gpio,uint8_t pin)
{
  gpio->BSRR=(1<<pin);
}
void gpio_reset(GPIO_TypeDef * gpio,uint8_t pin)
{
  gpio->BSRR=(1<<(16+pin));
}

void main(void)
{
  SystemInit();
 // Turn on GPIOC and GPIOA
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
  GPIOC->MODER |=(1<<(2*9))|(1<<(2*8));

    if (SysTick_Config((8000000 / 1000)))
    { 
        /* Capture error */ 
        while (1);
    }

  for(;;) 
  {
  }
  //Will not be reached anyway
  debug_exit(0);
}

void SysTick_Handler(void)
{
    cnt=(cnt+1)%1000;
    if(500==cnt)
    {
      gpio_set(GPIOC,9);
      gpio_reset(GPIOC,8);
    }
    if(999==cnt)
    {
      gpio_set(GPIOC,8);
      gpio_reset(GPIOC,9);
    }
}