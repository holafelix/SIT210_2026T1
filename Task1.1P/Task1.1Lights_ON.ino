int porchLight = 12;
int hallwayLight = 11;
int switchPin = 2;

bool lastState = HIGH;

void setup() {
  pinMode(porchLight, OUTPUT);
  pinMode(hallwayLight, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
}

bool isSwitchPressed(int currentState) {
  return (lastState == HIGH && currentState == LOW);
}

void turnLightsOn() {
  digitalWrite(porchLight, HIGH);
  digitalWrite(hallwayLight, HIGH);
}

void controlLights() {
  turnLightsOn();

  delay(30000); 
  digitalWrite(porchLight, LOW);

  delay(30000); 
  digitalWrite(hallwayLight, LOW);
}

void loop() {
  int currentState = digitalRead(switchPin);

  if (isSwitchPressed(currentState)) {
    controlLights();
  }

  lastState = currentState;
}
