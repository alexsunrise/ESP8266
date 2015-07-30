/*
 *  Examples read voltage from SD1306
 *
 *	INA219 connected to
 *	I2C SDA - GPIO2
 *	I2C SCK - GPIO14
 *
 */

#include "ets_sys.h"
#include "driver/i2c.h"
#include "driver/uart.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"

#include "../include/driver/i2c_sd1306.h"
#include "user_config.h"


static ETSTimer timer_1;

void user_rf_pre_init(void)
{

}

static void ICACHE_FLASH_ATTR timer_1_int(void *arg)
{
	static uint8_t state;
	os_timer_disarm(&timer_1);
	ets_uart_printf("Timer Interrupt\r\n");
	if(state==1){
		gpio_output_set(0, 1 << I2C_SCK_PIN, 1 << I2C_SCK_PIN, 0);
		state=0;
	}else{
		gpio_output_set(1 << I2C_SCK_PIN, 0, 1 << I2C_SCK_PIN, 0);
		state=1;
	}

	os_timer_setfn(&timer_1, (os_timer_func_t *)timer_1_int, NULL);
	os_timer_arm(&timer_1, 500, 0);
}

void user_init(void)
{
    // Init uart
    uart_init(BIT_RATE_115200, BIT_RATE_115200);
    os_delay_us(1000);

    ets_uart_printf("Booting...\r\n");
    i2c_init();
//  ets_uart_printf("Pin 14 1\r\n");
//  gpio_output_set(1 << I2C_SCK_PIN, 0, 1 << I2C_SCK_PIN, 0);
//  ets_uart_printf("Pin 14 0\r\n");
//  gpio_output_set(0, 1 << I2C_SCK_PIN, 1 << I2C_SCK_PIN, 0);
// 	Init
//  Arm timer for every 10 sec.
    os_timer_disarm(&timer_1);
    os_timer_setfn(&timer_1, (os_timer_func_t *)timer_1_int, NULL);
    os_timer_arm(&timer_1, 1000, 1);

    if (SD1306_Init()) {
    	ets_uart_printf("SD1306 init done.\r\n");
        //Disarm timer
        os_timer_disarm(&timer_1);
        //Setup timer

        //Arm timer for every 10 sec.
        os_timer_arm(&timer_1_int, 5000, 1);
    }
    else
    	ets_uart_printf("SD1306 init error.\r\n");


}

