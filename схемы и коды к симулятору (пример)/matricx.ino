#include <SimpleVector.h>
#include <Keypad.h>

const byte Rows= 4; // число строк 4
const byte Cols= 3; // число столбцов 3

// расположение цифр и символов в матрице:

char keymap[Rows][Cols]=
{
{'1', '2', '3'},
{'4', '5', '6'},
{'7', '8', '9'},
{'*', '0', '#'}
};


byte rPins[Rows]= {A6,A5,A4,A3}; // строки с 0 по 3
byte cPins[Cols]= {A2,A1,A0}; // столбцы с 0 по 2

// инициализация класса Keypad
Keypad kpd= Keypad(makeKeymap(keymap), rPins, cPins, Rows, Cols);
 
void setup()
{
  Serial.begin(9600);
}
  
void loop(){
  SimpleVector<int> zakazi;
  int i;
  char key = kpd.getKey();  // считываем нажатую кнопку
  for (i = 6; key != '*';){
    Serial.print("Key Pressed : "); // выводим нажатую кнопку в Serial Monitor
    Serial.println(key);
    zakazi.put(key); // добавляем в массив 
    char key = kpd.getKey(); 
  }
}