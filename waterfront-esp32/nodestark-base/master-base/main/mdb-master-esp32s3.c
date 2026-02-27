/*
 * Waterfront Kayak Rental Controller – Bremen
 * MDB/vending completely removed
 * MQTT remote unlock + sensor base
 * Adapted from Nodestark/mdb-esp32-cashless master
 */

/*
 * VMflow.xyz
 *
 * mdb-master-esp32s3.c - Vending machine controller
 *
 */

#include <esp_log.h>

#include <string.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <driver/gpio.h>
#include <driver/uart.h>
#include <driver/gpio.h>
#include <rom/ets_sys.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "led_strip.h"

#define TAG "kayak_rental"

void app_main(void) {

    //--------------- Strip LED configuration ---------------//
    led_strip_handle_t led_strip;
    led_strip_config_t strip_config = {
        .strip_gpio_num = GPIO_NUM_2,
        .max_leds = 1,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .led_model = LED_MODEL_WS2812,
        .flags.invert_out = false,
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000, // 10 MHz → good precision
        .mem_block_symbols = 64,
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    //
	button_receive_queue = xQueueCreate(1 /*queue-length*/, sizeof(uint8_t));

	gpio_config_t io_conf = {
			.intr_type = GPIO_INTR_NEGEDGE, // Borda de descida (pressionado)
			.mode = GPIO_MODE_INPUT,
			.pin_bit_mask = (1ULL << GPIO_NUM_0),
			.pull_up_en = GPIO_PULLUP_ENABLE,
			.pull_down_en = GPIO_PULLDOWN_DISABLE, };

	gpio_config(&io_conf);

	gpio_install_isr_service(0 /*default*/);
	gpio_isr_handler_add(GPIO_NUM_0, button0_isr_handler, (void*) 0);

	//
}
