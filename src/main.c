#include "stm32f4xx_hal.h"

#define LED1_PIN GPIO_PIN_5
#define LED2_PIN GPIO_PIN_6
#define LED3_PIN GPIO_PIN_7
#define LED4_PIN GPIO_PIN_6

#define LED_PORTA GPIOA
#define LED_PORTA_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define LED_PORTB GPIOB
#define LED_PORTB_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

TIM_HandleTypeDef htim2;

void LED_Init(void);
void Timer_Init(void);

int main(void) {
  HAL_Init();

  LED_Init();
  Timer_Init();

  while (1) {
    // Your main code can be placed here
  }
}

void LED_Init(void) {
  LED_PORTA_CLK_ENABLE();
  LED_PORTB_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStructA = {0};
  GPIO_InitStructA.Pin = LED1_PIN | LED2_PIN | LED3_PIN;
  GPIO_InitStructA.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructA.Pull = GPIO_NOPULL;
  GPIO_InitStructA.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PORTA, &GPIO_InitStructA);

  GPIO_InitTypeDef GPIO_InitStructB = {0};
  GPIO_InitStructB.Pin = LED4_PIN;
  GPIO_InitStructB.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructB.Pull = GPIO_NOPULL;
  GPIO_InitStructB.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PORTB, &GPIO_InitStructB);
}

void Timer_Init(void) {
  __HAL_RCC_TIM2_CLK_ENABLE();

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 8000 - 1;  // Adjust the prescaler to achieve a 1 ms timer interrupt
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000 - 1;  // Timer period in milliseconds
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  HAL_TIM_Base_Init(&htim2);

  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

  HAL_TIM_Base_Start_IT(&htim2);
}


void TIM2_IRQHandler(void) {
  //TIM2->SR &= ~TIM_SR_UIF;
  __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);

  static uint8_t currentLED = 1;

  // Turn off all LEDs
  HAL_GPIO_WritePin(LED_PORTA, LED1_PIN | LED2_PIN | LED3_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED_PORTB, LED4_PIN, GPIO_PIN_SET);

  // Turn on the current LED
  switch (currentLED) {
    case 1:
      HAL_GPIO_WritePin(LED_PORTA, LED1_PIN, GPIO_PIN_RESET);
      break;
    case 2:
      HAL_GPIO_WritePin(LED_PORTA, LED2_PIN, GPIO_PIN_RESET);
      break;
    case 3:
      HAL_GPIO_WritePin(LED_PORTA, LED3_PIN, GPIO_PIN_RESET);
      break;
    case 4:
      HAL_GPIO_WritePin(LED_PORTB, LED4_PIN, GPIO_PIN_RESET);
      break;
  }

  // Increment the LED index
  currentLED++;
  if (currentLED > 4) {
    currentLED = 1;
  }

}