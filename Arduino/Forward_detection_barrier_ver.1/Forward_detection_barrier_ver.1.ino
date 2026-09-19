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

// ── 可調參數 ──────────────────────────
int moveSpeed  = 120;  // 直走速度 (0~255)
int trimOffset = 10;   // 左右馬達補償（正值 = 左輪補快）
int turnSmall  = 35;   // L/R 離線微調幅度
int turnLarge  = 80;   // LL/RR 離線大幅修正幅度
// ─────────────────────────────────────

bool stableRead(int pin) {
  int count = 0;
  for (int i = 0; i < 5; i++) {
    if (digitalRead(pin) == HIGH) count++;  // 1 = 黑線
    delay(1);
  }
  return count >= 3;  // true = 偵測到黑線
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
  bool ll = stableRead(irLL);  // true = 壓到黑線
  bool l  = stableRead(irL);
  bool r  = stableRead(irR);
  bool rr = stableRead(irRR);

  Serial.print("LL:"); Serial.print(ll);
  Serial.print(" L:"); Serial.print(l);
  Serial.print(" R:"); Serial.print(r);
  Serial.print(" RR:"); Serial.println(rr);

  if (!ll && !l && !r && !rr) {
    // 四個都離線 → 完全脫軌，停車
    stopCar();

  } else if (ll && !l && !r && !rr) {
    // 只有 LL 壓線 → 車大幅偏右 → 大幅向左修正（右輪快）
    drive(moveSpeed - turnLarge, moveSpeed + turnLarge);

  } else if (!ll && !l && !r && rr) {
    // 只有 RR 壓線 → 車大幅偏左 → 大幅向右修正（左輪快）
    drive(moveSpeed + turnLarge, moveSpeed - turnLarge);

  } else if (!ll && l && r && !rr) {
    // L 和 R 都壓線 → 正常直走
    drive(moveSpeed, moveSpeed);

  } else if (!ll && !l && r && !rr) {
    // 只有 R 壓線，L 離線 → 車偏右 → 向左修正（右輪快）
    drive(moveSpeed - turnSmall, moveSpeed + turnSmall);

  } else if (!ll && l && !r && !rr) {
    // 只有 L 壓線，R 離線 → 車偏左 → 向右修正（左輪快）
    drive(moveSpeed + turnSmall, moveSpeed - turnSmall);

  } else {
    // 其他狀況 → 直走
    drive(moveSpeed, moveSpeed);
  }
}