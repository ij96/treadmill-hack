# Treadmill speed controller

Treadmill hack - controlling treadmill speed directly using Arduino UNO

## To-do

- [] RPM & speed calculation

- [] inclination control (up and down buttons for manual control)

## Usage

- upload code to Arduino UNO

- open Serial Monitor

- to turn on or off the motor: send `on` or `off` via Serial

- to set new PWM value: send `p<pwm>` via Serial, where `<pwm>` is an integer in the range of 0 to 65535 (default PWM on start-up is 0)
