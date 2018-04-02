# Treadmill speed controller

Treadmill hack - controlling treadmill speed directly using ESP8266 and MicroPython

## To-do

- [x] PWM control

- [x] RPM & speed calculation

## Usage

- connect ESP8266 to PC via USB

- upload `main.py` to ESP8266

  ```
  sudo ampy --port /dev/ttyS* put main.py
  ```

- open serial port to communicate with the ESP8266

  ```
  sudo microcom -p /dev/ttyS* -s 115200
  ```

- change raw PWM to x (x is an integer in between and including 0 and 1023)

  ```
  pwm.duty(x)
  ```
