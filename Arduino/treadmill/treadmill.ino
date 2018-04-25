// treadmill hack

#define RPM_PIN 2  // RPM signal (active low) must be an interrupt pin - 2 or 3 for Uno
#define PWM_PIN 9  // PWM signal - use pin 9 or 10, for 16-bit resolution
#define REL_PIN 5  // relay signal (active high) - 0V for motor off, 5V for motor on

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

void setup() {
  Serial.begin(9600);
  Serial.println("### Treadmill Hack ###");
  pinMode(RPM_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(REL_PIN, OUTPUT);
  digitalWrite(REL_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(RPM_PIN), ISR_RPM_count, RISING);
  setupPWM16();
}

int time_start = millis();
int time_elapsed = 0;

void loop() {
  if (Serial.available() > 0) {
    parse_command(Serial.readString());
  }
  time_elapsed = millis() - time_start;
  if (time_elapsed >= 1000) {
    report();
  }
}

void ISR_RPM_count() {
  rpm_edge_count += 1;
}

void report() {
  rpm = rpm_edge_count * 250 / time_elapsed * 0.0003663 * 3600; // calculate RPM
  Serial.print("Motor is: "); Serial.print(system_on?"ON":"OFF");
  Serial.print("\tRPM edge: "); Serial.print(rpm_edge_count);
  Serial.print("\tspeed: "); Serial.print(rpm); Serial.print(" km/h");
  Serial.print("\tPWM: "); Serial.println(pwm);
  rpm_edge_count = 0;                           // clear edge count
  time_start = millis();                        // reset timer
  time_elapsed = 0;
}

void parse_command(String command) {
  if (command[0] == 'p') {
    command.remove(0, 1);
    pwm = command.toInt();
    analogWrite16(PWM_PIN, pwm);
    Serial.print("PWM: "); Serial.println(pwm);
  } else if ((command == "on") || (command == "ON")) {
    Serial.println("Motor will be ON in 3 seconds...");
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
}

