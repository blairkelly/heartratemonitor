int powerpin_xbee = 6;
int powerpin_gps = 7;
int powerpin_ninedof = 8;
int powerpin_hrmonitor = 9;
int powerpin_fivevoltreg = 12;
int powerpin_threevoltreg = 13;

int pin_led1 = 10;
int pin_led2 = 11;

int pin_battery = A3;

void setup() {
  Serial.begin(57600);

  pinMode(powerpin_xbee, OUTPUT);
  pinMode(powerpin_gps, OUTPUT);
  pinMode(powerpin_ninedof, OUTPUT);
  pinMode(powerpin_hrmonitor, OUTPUT);
  pinMode(powerpin_fivevoltreg, OUTPUT);
  pinMode(powerpin_threevoltreg, OUTPUT);

  pinMode(pin_led1, OUTPUT);
  pinMode(pin_led2, OUTPUT);

}

void loop () {

}