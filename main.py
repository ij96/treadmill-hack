''' Treadmill speed controller '''

from machine import Pin, PWM
from pyb import Timer

#################### PWM output ################################################

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

#################### read RPM by counting edges ################################
#   2 edges per revolution
#   diameter of rotor bar = 58.3mm -> 0.3663m travelled per revolution
#   assuming lowest speed is 0.8km/h = 0.2222m/s = 0.6rps = 36rpm
#                                       -> 1.2 edges per sec
#                                       -> counting edges every sec is enough
#   rps = num. of edges per sec / 2
#   speed = rps * 0.3663       (unit: m/s)
#         = rps * 0.3663 * 3.6 (unit: km/h)

speed = 0   # unit: km/h

def rpm_edge_increment():
    rpm_edge_counter += 1

def update_speed():
    speed = rpm_edge_counter / 2 * 0.3663 * 3.6
    rpm_edge_counter = 0                    # clear counter
    printf('Speed: {} km/h'.format(speed))  # print result

# timer
tim_rpm = Timer(1, freq=1)                  # trigger every 1 second
tim_rpm.callback(update_speed)

# hardware interrupt pin
pin_rpm = Pin(13, Pin.IN)                   # create Pin object on pin 13
pin_rpm.irq(trigger = Pin.IRQ_FALLING, handler=rpm_edge_increment) # set up IRQ
