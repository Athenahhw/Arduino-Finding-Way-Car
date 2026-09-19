int motorA_1 = 6;
int motorA_2 = 5;
int motorB_1 = 10;
int motorB_2 = 11;

int irLL = A0; 
int irL  = A2; 
int irR  = A4; 
int irRR = A5; 

int trigPin = 3;
int echoPin = 2; 

int baseSpeed = 120;  
int turnSpeed = 80;   
int pivotSpeed = 190; 
int safeDistance = 8;  
int backSpeed = 100;

void setup() {
  pinMode(motorA_1, OUTPUT); pinMode(motorA_2, OUTPUT);
  pinMode(motorB_1, OUTPUT); pinMode(motorB_2, OUTPUT);
  
  pinMode(irLL, INPUT); pinMode(irL, INPUT);
  pinMode(irR, INPUT);  pinMode(irRR, INPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  int valLL = digitalRead(irLL);
  int valL  = digitalRead(irL);
  int valR  = digitalRead(irR);
  int valRR = digitalRead(irRR);

  if (valLL == HIGH && valL == HIGH && valR == HIGH && valRR == HIGH) {
    setMotors(0, 0);
    return; 
  }

  float distance = getDistance();
  if (distance > 0 && distance < safeDistance) {
    bypassObstacle(); 
    return; 
  }

  if (valLL == LOW && valL == LOW && valR == LOW && valRR == LOW) {
    setMotors(-backSpeed, -backSpeed);
  } else if (valL == HIGH && valR == HIGH) {
    setMotors(baseSpeed, baseSpeed);
  } else if (valL == HIGH && valR == LOW) {
    setMotors(turnSpeed, baseSpeed); 
  } else if (valL == LOW && valR == HIGH) {
    setMotors(baseSpeed, turnSpeed);
  } else if (valLL == HIGH) {
    setMotors(-pivotSpeed, pivotSpeed); 
  } else if (valRR == HIGH) {
    setMotors(pivotSpeed, -pivotSpeed); 
  }
}

void bypassObstacle() {
  setMotors(0, 0); delay(500); 
  setMotors(-pivotSpeed, pivotSpeed); delay(250); 
  setMotors(220, 90); delay(2000); 
  setMotors(90, 160); 
  while(digitalRead(irL) == LOW && digitalRead(irR) == LOW) {
  }
  //setMotors(-pivotSpeed, pivotSpeed); delay(300); 
}

float getDistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH, 30000); 
  return (duration * 0.0343) / 2;
}

void setMotors(int speedL, int speedR) {
  if(speedL > 255) speedL = 255;
  if(speedR > 255) speedR = 255;
  if(speedL < -255) speedL = -255;
  if(speedR < -255) speedR = -255;

  if (speedL >= 0) {
    analogWrite(motorA_1, speedL); analogWrite(motorA_2, 0);
  } else {
    analogWrite(motorA_1, 0); analogWrite(motorA_2, -speedL); 
  }
  if (speedR >= 0) {
    analogWrite(motorB_1, speedR); analogWrite(motorB_2, 0);
  } else {
    analogWrite(motorB_1, 0); analogWrite(motorB_2, -speedR); 
  }
}