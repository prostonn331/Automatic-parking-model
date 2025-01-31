// подсистема перемещения ячеек (ППЯ) 

#include <TaskScheduler.h>

// шаговый мотор
#define in1 2
#define in2 3
#define in3 4
#define in4 5
#define STOP_PIN 8 // пин выключатель останова

const unsigned int cnt_60deg = 2048/6; // счетчик на один оборот
byte ph_cnt = 1; // счетчик фаз
unsigned long step_cnt = 0; // счетчик шагов двигателя
bool stopBtn = false; // кнопка останова
byte cur_position = 1 ; // текущая позиция
byte go_position = 1 ; // заданная позиция
boolean recievedFlag = false; // флаг получения данных на 
String strData = ""; // для данных с Serial
String tempStr = "";
unsigned int cnt_toStop = 0; // счетчик до останова

// прототип функций
void motorClockwise();   //задаем прототип мотор по часовой
void motorCounterClockwise();   //задаем прототип мотор против часовой
String recieveData(); // данные с Serial

// Создаем объекты 
Scheduler userScheduler;   // планировщик

Task taskMotorUp(TASK_MILLISECOND * 30 , TASK_FOREVER, &motorClockwise);   // задание для мотора по часовой
Task taskMotorDown(TASK_MILLISECOND * 30 , TASK_FOREVER, &motorCounterClockwise);   // задание для мотора против часовой

void setup() {

Serial.begin(9600);
pinMode(STOP_PIN, INPUT_PULLUP);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);

// добавляем задания в обработчик
userScheduler.addTask(taskMotorUp);
userScheduler.addTask(taskMotorDown);

 
// калибровка при нажатой кнопке стоп
 if (!digitalRead(STOP_PIN)) {
   taskMotorUp.enable();
   while (!digitalRead(STOP_PIN)) {
    userScheduler.execute(); // обновляем задания планировщика
   }
  taskMotorUp.disable();
  step_cnt = 0;
 }

}

void loop() {
userScheduler.execute();
tempStr = recieveData();
 if (tempStr != "") {
 go_position = tempStr.toInt();
  if (go_position != cur_position) {
      if (go_position > cur_position) {
        // поворачиваем по часовой
        cnt_toStop = (go_position - cur_position)*cnt_60deg;
        cur_position = go_position;
        taskMotorUp.enable();
          while (step_cnt < cnt_toStop) {
             userScheduler.execute();
         }
        taskMotorUp.disable();
        step_cnt = 0; 
      }

        if (go_position < cur_position) {
        // поворачиваем против часовой
        cnt_toStop = (cur_position - go_position)*cnt_60deg;
        cur_position = go_position;
        taskMotorDown.enable();
          while (step_cnt < cnt_toStop) {
             userScheduler.execute();
         }
        taskMotorDown.disable();
        step_cnt = 0; 
      }
  }
 }
}

// Фазы 1...4 шагового двигателя:
void phase1(){
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW); 
  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);
  }

void phase2(){
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
 }

void phase3(){
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH); 
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW);
 }

void phase4(){
  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, HIGH);
}

void motorClockwise(){
  switch (ph_cnt) { 
   case 1:
   phase4();
   break;
   case 2:
   phase3();
   break;
   case 3:
   phase2();
   break;
   case 4:
   phase1();
   break;
 }
 ph_cnt++;
 step_cnt++;
 if (ph_cnt>4)
   ph_cnt = 1;
}

void motorCounterClockwise(){

  switch (ph_cnt) { 
   case 1:
   phase1();
   break;
   case 2:
   phase2();
   break;
   case 3:
   phase3();
   break;
   case 4:
   phase4();
   break;
 }
 ph_cnt++;
 step_cnt++;
 if (ph_cnt>4)
   ph_cnt = 1;
}

//прием данных с serial
String recieveData(){
if (Serial.available() > 0) {  // если есть что-то на вход
  strData = "";                // очистить строку
    while (Serial.available() > 0) {
      // пока идут данные
      strData += (char)Serial.read();  // получаем данные
      delay(2);                        // обязательно задержка, иначе вылетим из цикла раньше времени
    }
    recievedFlag = true;  // поднять флаг что получили данные
  }

 if (recievedFlag) {
      recievedFlag = false;  // данные приняты
      return strData;
     }
  else
     return "";

}
