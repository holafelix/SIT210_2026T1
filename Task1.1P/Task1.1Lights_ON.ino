int porchLight = 12;
int hallwayLight = 11;
int switchPin = 2;

bool lastState = HIGH;

void setup() {
  pinMode(porchLight, OUTPUT);
  pinMode(hallwayLight, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
}

void loop() {
  int currentState = digitalRead(switchPin);

  if (lastState == HIGH && currentState == LOW) {

    digitalWrite(porchLight, HIGH);
    digitalWrite(hallwayLight, HIGH);

    delay(30000); 
    digitalWrite(porchLight, LOW); 

    delay(30000); 
    digitalWrite(hallwayLight, LOW); 
  }

  lastState = currentState;
}
