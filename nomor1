// Pin LED
#define LED_RED_PIN   13
#define LED_GREEN_PIN 12
#define LED_BLUE_PIN  11

// Mode kerja LED
enum LedMode {
  MODE_OFF,
  MODE_ON,
  MODE_BLINK
};

// Kelas LED
class LED {
private:
  String name;
  int pin;
  bool activeLow;
  int freq;
  unsigned long halfPeriod;
  LedMode mode;
  bool state;
  unsigned long lastBlink;

public:
  LED(String n, int p, bool aLow, int f) {
    name = n;
    pin = p;
    activeLow = aLow;
    freq = f;
    halfPeriod = (1000 / f) / 2;

    mode = MODE_OFF;
    state = false;
    lastBlink = 0;

    pinMode(pin, OUTPUT);
    setPin(false);
  }

  // Ganti mode LED
  void setMode(LedMode m) {
    mode = m;

    if (m == MODE_ON) {
      turnOn();
    }
    else if (m == MODE_OFF) {
      turnOff();
    }
    else if (m == MODE_BLINK) {
      lastBlink = millis();
      printInfo();
    }
  }

  void turnOn() {
    state = true;
    setPin(true);
    if (mode != MODE_BLINK) printInfo();
  }

  void turnOff() {
    state = false;
    setPin(false);
    if (mode != MODE_BLINK) printInfo();
  }

private:
  void setPin(bool s) {
    int out = s ? (activeLow ? LOW : HIGH)
        : (activeLow ? HIGH : LOW);
    digitalWrite(pin, out);
  }

  void toggle() {
    state = !state;
    setPin(state);
  }

public:
  // Dipanggil di loop()
  void update() {
    if (mode != MODE_BLINK) return;

    unsigned long now = millis();
    if (now - lastBlink >= halfPeriod) {
      toggle();
      lastBlink = now;
      printInfo();
    }
  }

  // Print info ke Serial
  void printInfo() {
    Serial.print("[");
    Serial.print(millis());
    Serial.print(" ms] ");
    Serial.print(name);
    Serial.print(" | Mode: ");

    switch (mode) {
      case MODE_OFF:   Serial.print("OFF"); break;
      case MODE_ON:    Serial.print("ON"); break;
      case MODE_BLINK: 
        Serial.print("BLINK ");
        Serial.print(freq);
        Serial.print(" Hz");
        break;
    }

    Serial.print(" | State: ");
    Serial.println(state ? "ON" : "OFF");
  }
};

// Object LED
LED ledRed("Red", LED_RED_PIN, true, 10);
LED ledGreen("Green", LED_GREEN_PIN, false, 5);
LED ledBlue("Blue", LED_BLUE_PIN, false, 2);

void setup() {
  Serial.begin(9600);

  Serial.println("=== 3 LED Control Blink OOP ===");

  ledRed.setMode(MODE_BLINK);
  ledGreen.setMode(MODE_BLINK);
  ledBlue.setMode(MODE_BLINK);
}

void loop() {
  ledRed.update();
  ledGreen.update();
  ledBlue.update();

  // Uji input dari Serial
  if (Serial.available()) {
    char sel = Serial.read();
    delay(10);

    if (Serial.available()) {
      char m = Serial.read();

      LedMode newMode;
      if (m == '0') newMode = MODE_OFF;
      else if (m == '1') newMode = MODE_ON;
      else if (m == '2') newMode = MODE_BLINK;
      else return;

      if (sel == 'r') ledRed.setMode(newMode);
      else if (sel == 'g') ledGreen.setMode(newMode);
      else if (sel == 'b') ledBlue.setMode(newMode);

      Serial.print("Set ");
      Serial.print(sel == 'r' ? "Red" : sel == 'g' ? "Green" : "Blue");
      Serial.print("to Mode ");
      Serial.println(m);
    }
  }
}
