/* Treadmill hack
  Signal wires:
    Name  | Colour  | Description
    Vref    black     5V
    VR      white     ~4.73V
    GND     grey      0V (ground)
    RPM     purple    RPM signal (active low, 2 peaks per rotation)
    VDD     blue      ~15V
    PWM     green     PWM signal (native controller use 400Hz)
    DN      yellow    inclination up (active high)
    UP      orange    inclination down (active high)
    S/W     red       ~15V
    relay   brown     relay signal (active high)
*/

#define RPM_PIN 2  // RPM signal - must be an interrupt pin, which is 2 or 3 on Uno
#define PWM_PIN 9  // PWM signal - use pin 9 or 10, for 16-bit resolution
#define REL_PIN 5  // relay signal - 0V for motor off, 5V for motor on

int rpm_edge_count = 0;
float rpm = 0;
uint16_t pwm = 0;
void ISR_RPM_count();
bool system_on = false;

// configure digital pins 9 and 10 as 16-bit PWM outputs
void setupPWM16() {
  DDRB |= _BV(PB1) | _BV(PB2);                      // set pins as outputs
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);  // non-inverting PWM, mode 14: fast PWM, TOP=ICR1
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);     // no prescaling
  ICR1 = 0xffff;                                    // TOP counter value
}

// 16-bit version of analogWrite()
void analogWrite16(uint8_t pin, uint16_t val) {
  switch (pin) {
    case  9: OCR1A = val; break;
    case 10: OCR1B = val; break;
  }
}

//void ISR_RPM_count() {
//  rpm_edge_count += 1;
//}

void report() {
  //rpm = rpm_edge_count * 250 / time_elapsed * 0.0003663 * 3600; // calculate RPM
  Serial.print("Motor is: "); Serial.print(system_on?"ON":"OFF");
  //Serial.print("\tRPM edge: "); Serial.print(rpm_edge_count);
  //Serial.print("\tspeed: "); Serial.print(rpm); Serial.print(" km/h");
  Serial.print("\tPWM: "); Serial.println(pwm);
  //rpm_edge_count = 0;                           // clear edge count
  //time_start = millis();                        // reset timer
  //time_elapsed = 0;
}

void parse_command(String command) {
  if (command[0] == 'p') {
    command.remove(0, 1);
    pwm = command.toInt();
    analogWrite16(PWM_PIN, pwm);
    Serial.print("New PWM: "); Serial.println(pwm);
  } else if ((command == "on") || (command == "ON")) {
    Serial.print("Motor will be ON in 3 seconds...");
    Serial.print("3..."); delay(1000);
    Serial.print("2..."); delay(1000);
    Serial.print("1..."); delay(1000);
    Serial.println("ON");
    digitalWrite(REL_PIN, HIGH);
    system_on = true;
  } else if ((command == "off") || (command == "OFF")) {
    digitalWrite(REL_PIN, LOW);
    Serial.println("Motor OFF");
    system_on = false;
  }
  report();
}

void setup() {
  Serial.begin(9600);
  Serial.println("#############################################################################");
  Serial.println("#                                                                           #");
  Serial.println("#                              Treadmill Hack                               #");
  Serial.println("#                       Model: Dream Treadmill D1000                        #");
  Serial.println("# Serial command:                                                           #");
  Serial.println("#   to turn on or off the motor:                                            #");
  Serial.println("#     send \"on\" or \"off\"                                                    #");
- Serial.println("#   to set new PWM value:                                                   #");
- Serial.println("#     send \"p<pwm>\", where \"<pwm>\" is an integer in the range of 0 to 65535 #");
  Serial.println("#                                                                           #");
  Serial.println("#############################################################################");
  pinMode(RPM_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(REL_PIN, OUTPUT);
  digitalWrite(REL_PIN, LOW);
  //attachInterrupt(digitalPinToInterrupt(RPM_PIN), ISR_RPM_count, RISING);
  setupPWM16();
  report();
}

//int time_start = millis();
//int time_elapsed = 0;

void loop() {
  if (Serial.available() > 0) {
    parse_command(Serial.readString());
  }
  //time_elapsed = millis() - time_start;
  //if (time_elapsed >= 2000) {
  //  report();
  //}
}



