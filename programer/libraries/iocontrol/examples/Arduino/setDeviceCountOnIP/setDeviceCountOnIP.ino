/*
 *  Пример установки количества устройств в локальной сети
 * которая является одним IP адресом для внешнего соединений
 * на Arduino с Ethernet Shield'ом.
 *
 * Пояснение:
 * Сервер iocontrol ведёт учёт количества запросов по IP адресу.
 * Если Ваша локальная сеть работает через NAT, то все устройства
 * в ней будут выглядить как один IP адрес для сервера.
 * Функции библиотеки readUpdate() и writeUpdate() делают запросы
 * на сервер через интервал, установленный сервером. В случае если у Вас
 * в сети несколько устройств, использующих сервис, запросы на сервер
 * будут поступать чаще во столько раз, сколько у Вас устройств. В таком случае,
 * сервер может дать отказ в доступе из-за превышения лимита запросов.
 * Специально для этого в библиотеке есть функция, которой необходимо
 * передать количество устройств, использующих сервис в одной локальной
 * сети. Это функция setDeviceCountOnIP(). Её необходимо вызвать до
 * функции begin(). Так же можно обойтись без использования этой функии
 * посчитав время запросов вручную и установив соответствующий delay()
 * в функции loop().
 *
 * Данный пример предполагает наличие двух устройств, использующих
 * сервис в одной локальной сети.
 */

#include <iocontrol.h>
#include <SPI.h>
#include <Ethernet.h>

// Название панели на сайте iocontrol.ru
const char* myPanelName = "название_панели";

// Если панель использует ключ
// const char* key = "ключ";

// Создаём объект клиента класса EthernetClient
EthernetClient client;
// Создаём объект iocontrol, передавая в конструктор название панели и клиента
iocontrol mypanel(myPanelName, client);

// Если панель использует ключ
// iocontrol mypanel(myPanelName, key, client);

// MAC адреса Ethernet шилда. Должен быть уникальным в сети
byte mac[] = {
	0xFE, 0xED, 0xDE, 0xAD, 0xFA, 0xCC
};

//  Задаём статический IP-адрес на тот случай,
// если динамическое присвоение адреса даст сбой
// IPAddress ip(192, 168, 1, 31);
// IPAddress myDns(192, 168, 1, 1);


void setup()
{
	Serial.begin(9600);

	// Инициируем Ethernet Shield со статическим адресом
	// Ethernet.begin(mac, ip, myDns);

	// Инициируем Ethernet Shield с использованием DHCP
	Ethernet.begin(mac);

	// Вызываем функцию установки количества устройств на одном IP адресе
	mypanel.setDeviceCountOnIP(2);
	// Вызываем функцию первого запроса к сервису
	mypanel.begin();
}

void loop()
{
	// Если удалось получить данные
	if (mypanel.readUpdate() == OK) {

		// Выводим в монитор последовательного порта информацию о нашей панели
		Serial.println(mypanel.info());
	}
}
