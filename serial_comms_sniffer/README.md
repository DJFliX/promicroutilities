# promicroutilities
<h1>Serial Sniffer</h1>
The Serial Sniffer Sketch is useful for debugging bidirectional UART communication. We use the fact that the Atmega32u4 has two hardware UARTS so that we only need a single software UART. It is possible to use an SSD1306-based Display (connected through I2C) to display the data as it arrives. Additionally you can open a serial monitor to the USB serial port and see the traffic as it happens.

The code for the SSD1306 is based on mike-rankin's <a href="https://github.com/mike-rankin/i2c_oled_3.3v">i2c_oled_3.3v</a>.