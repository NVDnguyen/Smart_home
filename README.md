## Project Overview

This project utilizes various sensors and technologies to create a smart system for monitoring air quality and controlling peripheral devices. Below is an overview of the components and functionalities incorporated:

### Sensors Used:
1. **Air Quality Sensor:**
   - Analog values are read from the air quality sensor connected to the `airSensorPin` (pin 35).
   - The read data is then converted and displayed on an LCD screen and transmitted to the Blynk app.
   - A buzzer provides an audible alert when air quality deteriorates.

2. **Buttons:**
   - Two buttons (`button1` on pin 32 and `button2` on pin 33) are declared as inputs with pull-up state to read signals from switches.

### Output Control:
1. **Connected Indicator Pin:**
   - Pin `pinConnectedIndicator` (pin 14) controls power to the relay.
   - When ESP32 successfully connects to Wi-Fi, this pin is set high (HIGH) to supply power to the relay through an opto-coupler and transistor, ensuring noise immunity and power efficiency during Wi-Fi operation.

2. **Relays:**
   - Digital pins `relay1` (pin 13) and `relay2` (pin 12) are used to control two corresponding relays.
   - These relays are employed to control external devices, such as lights in the electrical grid system.

### Integration with Blynk:
- The project is integrated with the Blynk app for remote monitoring and control.
- Blynk library is utilized to connect to the Blynk Server, enabling remote device control via the mobile app.
- ESP32 responds to events triggered from the Blynk app (button presses, state changes) and controls the corresponding relays accordingly.



