#include <stdint.h>

#define GPIO_ENABLE_REG 0x3FF44020
#define GPIO_OUT_REG 0x3FF44004
#define GPIO_PIN (1ULL << 2)

#define TIM_G0_WDT_WPROTECT_REG   0x3FF5F064
#define TIMG0_WDTFEED_REG       0x3FF5F060

void feed_wdt() {
    *((volatile uint32_t*) TIM_G0_WDT_WPROTECT_REG) = 0x50D83AA1;  
    *((volatile uint32_t*) TIMG0_WDTFEED_REG) = 1;                
    *((volatile uint32_t*) TIM_G0_WDT_WPROTECT_REG) = 0;            
}


void delay(uint32_t ms) {
    while (ms--) {
        __asm__ volatile("nop");
        feed_wdt();
    }
}

void app_main(void) {
    *(volatile uint32_t *)GPIO_ENABLE_REG |= GPIO_PIN;

    while (1) {
        *(volatile uint32_t *)GPIO_OUT_REG |= GPIO_PIN;
        delay(10000000);

        *(volatile uint32_t *)GPIO_OUT_REG &= ~GPIO_PIN;
        delay(10000000);
    }
}
