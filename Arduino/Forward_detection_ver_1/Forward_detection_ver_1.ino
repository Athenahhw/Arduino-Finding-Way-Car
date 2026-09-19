int motorA_1 = 5;
int motorA_2 = 6;
int motorB_1 = 10;
int motorB_2 = 11;

int trigPin = 2;
int echoPin = 3;

int irLL = A0;
int irL  = A2;
int irR  = A4;
int irRR = A5;
int moveSpeed  = 170;
int trimOffset = 10;
int turnSmall  = 35;
int turnLarge  = 80; 
int searchSpeed = 120;
int searchTime  = 400; 

bool stableRead(int pin) {
  int count = 0;
  for (int i = 0; i < 5; i++) {
    if (digitalRead(pin) == HIGH) count++;
    delay(1);
  }
  return count >= 3;
}

void drive(int speedA, int speedB) {
  speedA = constrain(speedA + trimOffset, 0, 255);
  speedB = constrain(speedB, 0, 255);
  analogWrite(motorA_1, speedA);
  digitalWrite(motorA_2, LOW);
  analogWrite(motorB_2, speedB);
  digitalWrite(motorB_1, LOW);
}

void stopCar() {
  digitalWrite(motorA_1, LOW); digitalWrite(motorA_2, LOW);
  digitalWrite(motorB_1, LOW); digitalWrite(motorB_2, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(motorA_1, OUTPUT); pinMode(motorA_2, OUTPUT);
  pinMode(motorB_1, OUTPUT); pinMode(motorB_2, OUTPUT);
  pinMode(trigPin, OUTPUT);  pinMode(echoPin, INPUT);
  pinMode(irLL, INPUT); pinMode(irL, INPUT);
  pinMode(irR,  INPUT); pinMode(irRR, INPUT);
  delay(1000);
}

void loop() {
  bool ll = stableRead(irLL); 
  bool l  = stableRead(irL);
  bool r  = stableRead(irR);
  bool rr = stableRead(irRR);

  Serial.print("LL:"); Serial.print(ll);
  Serial.print(" L:"); Serial.print(l);
  Serial.print(" R:"); Serial.print(r);
  Serial.print(" RR:"); Serial.println(rr);

  if (!ll && !l && !r && !rr) {
    stopCar();

  } else if (ll && !l && !r && !rr) {
    drive(moveSpeed - turnLarge, moveSpeed + turnLarge);
  } else if (!ll && !l && !r && rr) {
    drive(moveSpeed + turnLarge, moveSpeed - turnLarge);

  } else if (!ll && l && r && !rr) {
    drive(moveSpeed, moveSpeed);
  } else if (!ll && !l && r && !rr) {
    drive(moveSpeed - turnSmall, moveSpeed + turnSmall);
  } else if (!ll && l && !r && !rr) {
    drive(moveSpeed + turnSmall, moveSpeed - turnSmall);
  } else {
    drive(moveSpeed, moveSpeed);
  }
}