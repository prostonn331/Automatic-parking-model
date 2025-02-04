// порты для подключения модуля ULN2003 к Arduino
#define in1 8
#define in2 9
#define in3 10
#define in4 11
 
int del = 5; // время задержки между импульсами => скорость вращения
 
void setup() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
}
 
void loop() {
  rotateClockwise(100);
  delay(1000);
  rotateCounterClockwise(100);
  delay(1000);
}
 
// Вращение шагового двигателя по часовой стрелке
void rotateClockwise(int n){
  for (int i=0; i<n; i++) {
    phase4();
    phase3();
    phase2();
    phase1();
  }
}
 
// Вращение шагового двигателя против часовой стрелки
void rotateCounterClockwise(int n){
  for (int i=0; i<n; i++) {
    phase1();
    phase2();
    phase3();
    phase4();
  }
}
 
// Фазы 1...4 шагового двигателя:
void phase1(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(del);
}
 
void phase2(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(del);
}
 
void phase3(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(del);
}
 
void phase4(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, HIGH);
  delay(del);
}
