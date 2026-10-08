# STM32F446RE Bare-Metal MQ-7 Gas Monitor with Hardware Interrupts

A bare-metal C implementation for gas monitoring using the STM32F446RE microcontroller. This system interfaces an MQ-7 Carbon Monoxide (CO) sensor via 12-bit ADC, displays real-time gas levels on an I2C 16x2 LCD display, and triggers hardware interrupt alerts (EXTI / Analog Watchdog) when threshold limits are exceeded.

---

## 🛠️ Features
* *Bare-Metal C Driver:* Register-level peripheral programming without HAL abstraction.
* *12-bit ADC Sampling:* Interfacing MQ-7 sensor for precise gas signal acquisition.
* *Hardware Interrupt Handling:* Immediate interrupt triggering upon reaching threshold gas levels.
* *I2C 16x2 LCD Interface:* Real-time display of gas concentration and system status.
* *Non-blocking Architecture:* Efficient peripheral clock configuration and interrupt-driven logic.

---

## 🔌 Hardware Setup
* *Microcontroller:* STM32F446RE (ARM Cortex-M4)
* *Sensor:* MQ-7 Carbon Monoxide Sensor
* *Display:* 16x2 LCD with PCF8574 I2C Adapter
* *Peripherals Used:* GPIO, ADC, I2C, EXTI, Timers

---

## 📁 Project Structure
* Core/Inc/ - Custom register header files & driver function prototypes
* Core/Src/ - Main application logic, interrupt service routines (ISR), and driver implementations

---

## 🚀 How to Run
1. Clone this repository:
   ```bash
   git clone [https://github.com/aravindvenugopal30-ops/stm32f446-baremetal-mq7-interrupt-gas-monitor.git](https://github.com/aravindvenugopal30-ops/stm32f446-baremetal-mq7-interrupt-gas-monitor.git)
