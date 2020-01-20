// Подключаем датчики температуры
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(A0); // Датчки температуры подключены к порту A0
DallasTemperature sensors(&oneWire);

DeviceAddress Thermometer1 = {
  0x28, 0x9C, 0xD3, 0x07, 0x00, 0x00, 0x80, 0xC8
  // адрес датчика DS18B20 289CD307000080C8
  //0x28, 0x9D, 0xC6, 0x07, 0x00, 0x00, 0x80, 0x51
  //Device 0 Address: 28 9D C6 07 00 00 80 51
};
DeviceAddress Thermometer2 = {
  0x28, 0xBA, 0xC9, 0x07, 0x00, 0x00, 0x80, 0x0D
  // адрес датчика DS18B20 28BAC9070000800D
  //0x28, 0xF7, 0xC7, 0x07, 0x00, 0x00, 0x80, 0x90
  //Device 1 Address: 28 F7 C7 07 00 00 80 90
};

// Настройки датчиков и реле
void setupSensors() {
  // Запуск температурных датчиков
  sensors.begin();
  sensors.setResolution(Thermometer1, 12);
  sensors.setResolution(Thermometer2, 12);

  // Активируем реле 4-8
  for (int i = 4; i <= 8; i++) {
    pinMode (i, OUTPUT);
    digitalWrite (i, HIGH);
  }

  // Включаем предохранитель
  pinMode (3, OUTPUT);
  digitalWrite (3, LOW);
}

// Функция считывает температуру с датчиков
void getTemp() {
  float temp = 20;

  // Запрашиваем температуру
  sensors.requestTemperatures();

  // Сохраняем температуру
  temp = sensors.getTempC(Thermometer1);
  if (temp > -100) temp_out = temp; // проверка на ошибку
  if (temp_out > temp_max) temp_max = temp_out;
  if (temp_out < temp_min) temp_min = temp_out;

  temp = sensors.getTempC(Thermometer2);
  if (temp > -100) temp_in = temp;
}


// Управляем котлом
void updateVaillant() {
    // Температура контура отопления зависит от сопротивления нелинейно, поэтому воспользуемся табличными данными
    // Схема управления простая:
    //  24 - желаемая температура котла, 0 - номер элемента в массиве, реле - 00000 (0 в двоичной системе)
    //  28 - желаемая температура котла, 1 - номер элемента в массиве, реле - 00001 (1 в двоичной системе)
    //  и т.д.
    int vaillant[] = {24, 28, 32, 35, 38, 41, 43, 46, 48, 51, 53, 55, 57, 59, 60, 62, 63, 64, 66, 68, 69, 70, 71, 72, 74, 75, 76, 77, 78, 79, 79, 80};
    float controller;

    // Расчитываем сопротивление на основе желаемой температуры конура отопления
    controller = getController(temp_desired, temp_in, temp_out);
    for (int i = 1; i <= 31; i++) { // перебераем массив
        if (controller <= vaillant[i]) { // ищем подходящую темепературу
            float j1, j2;
            j1 = vaillant[i] - controller;
            j2 = controller - vaillant[i - 1];
            // выбираем лучшее значение
            if (j2 > j1) {
                setVaillantRelay(i); // устанваливаем реле
                temp_vessel = vaillant[i];
            } else {
                setVaillantRelay(i - 1); // устанваливаем реле
                temp_vessel = vaillant[i-1];
            }
            break;
        }
    }
}

// Функция устанавливает реле управления котлом
// relay - состояние реле в двоичном коде
// Например, relay = 2, в двиочном коде - 10, значит первое реле - включено, а второе - выключено
void setVaillantRelay(int relay) {
  for (int i = 4; i >= 0; i--) {
    bool bit;
    bit = bitRead(relay, i); // читаем текущий бит
    digitalWrite (i + 4, !bit); // выставляем реле
    delay(200);
  }
}
