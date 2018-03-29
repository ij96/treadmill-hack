''' Treadmill speed controller '''

from machine import Pin, PWM
import utime

pin_pwm = Pin(12, Pin.OUT)      # create Pin object on pin 12
pwm = PWM(pin_pwm, freq=100)    # create PWM object, freq. = 400Hz
pwm.duty(0)                     # set duty cycle (to 0 at first)

pin_0 = Pin(0, Pin.OUT)
pin_0.on()

''' PWM lookup table (measured using analogue oscilloscope - not very accurate)
    Speed / km  |   Duty cycle / %  |   Duty cycle / 1024
    3               8                   82
    6               16                  164
    9               20                  205
    12              28                  287
    15              32                  328
    18              40                  410
'''

