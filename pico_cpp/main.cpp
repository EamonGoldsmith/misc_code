#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main() {
    // Initialize the standard I/O (USB serial)
    stdio_init_all();

    // Initialize the ADC
    adc_init();

    // Select ADC input channel (GPIO 26 is ADC0, GPIO 27 is ADC1, etc.)
    const uint adc_pin = 34; // Change this to the pin you're using
    adc_gpio_init(adc_pin);
    adc_select_input(2); // Select ADC input 0 (GPIO 26)

    // Wait for USB serial to connect
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("ADC Reading Example\n");

    while (1) {
        // Read the raw ADC value (12-bit resolution)
        uint16_t raw_value = adc_read();

        // Convert the raw value to voltage (Pico ADC reference voltage is 3.3V)
        float voltage = raw_value * 3.3f / (1 << 12);

        // Convert the calculated voltage to Amps
        float amps = 0.178 * (voltage - 0.33);

        // Print the voltage to the USB serial
        printf("Amps: %.3f A\n", amps);

        // Wait for a short period before reading again
        sleep_ms(500);
    }

    return 0;
}
