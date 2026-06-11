// --- 馬達接腳 ---
int motorA_1 = 6;  // 左輪前進 
int motorA_2 = 5;  // 左輪後退 
int motorB_1 = 10; // 右輪前進 
int motorB_2 = 11; // 右輪後退 

// --- 紅外線循跡接腳 ---
int irLL = A0; 
int irL  = A2; 
int irR  = A4; 
int irRR = A5; 


int trigPin = 3; // S3 對應數位腳位 3
int echoPin = 2; // S2 對應數位腳位 2

int baseSpeed = 140;  
int turnSpeed = 80;   
int pivotSpeed = 190; 
int safeDistance = 15; 

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
  // 1. 優先判斷前方是否有障礙物
  float distance = getDistance();

  // 如果距離小於安全距離，且不是雜訊 (大於 0)
  if (distance > 0 && distance < safeDistance) {
    bypassObstacle(); // 進入繞道避障副程式
    return; // 結束這一回合，重新開始
  }

  int valLL = digitalRead(irLL);
  int valL  = digitalRead(irL);
  int valR  = digitalRead(irR);
  int valRR = digitalRead(irRR);

  // --- 循跡核心邏輯 ---
  if ((valLL == LOW && valL == LOW && valR == LOW && valRR == LOW) ||
      (valLL == HIGH && valL == HIGH && valR == HIGH && valRR == HIGH)) {
    setMotors(0, 0);
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
  setMotors(0, 0); delay(500); // 看到障礙物，先嚇一跳煞車

  // 1. 右轉約 90 度
  setMotors(-pivotSpeed, pivotSpeed); delay(450); 
  
  // 2. 往前直走繞開
  setMotors(baseSpeed, baseSpeed); delay(700);

  // 3. 左轉約 90 度，車頭與黑線平行
  setMotors(pivotSpeed, -pivotSpeed); delay(450);

  // 4. 往前直走，直到再次看到黑線
  setMotors(baseSpeed, baseSpeed);
  while(digitalRead(irL) == LOW && digitalRead(irR) == LOW) {
    // 保持前進，什麼都不做
  }

  // 5. 壓回黑線了！車頭微調向左拉正
  setMotors(pivotSpeed, -pivotSpeed); delay(200);
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
