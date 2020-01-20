// Подключаем экран
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Задаем адрес и размерность дисплея.

int screen_mode = 0; // Отображаемая информация

// Настройка экрана
void setupScreen() {
    lcd.init();  // Инициализация lcd
    turnOnScreen();
    lcd.setCursor(0, 0);
    lcd.print("Hello!");
}

// Обновить информацию на экране
void updateScreen() {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");

    if (screen_mode == 0) showDeafultScreen();
    if (screen_mode == 1) showCurveScreen();
    if (screen_mode == 2) showDeltaTScreen();
    if (screen_mode == 3) showDeltaKScreen();
    if (screen_mode == 4) showTestScreen();
    if (screen_mode == 5) showStatScreen();
}

// Следующий экран
void setNextScreen() {
    if (screen_mode == 5) {
        screen_mode = 0;
    } else {
        screen_mode++;
        updateScreen();     
    }
}

// Предыдущий экран
void setPrevScreen() {
    if (screen_mode == 0){
        screen_mode = 5;
    } else {
        screen_mode--;
        updateScreen(); 
    }
}

// Включить экран
void turnOnScreen(){
  lcd.backlight();
  setScreenTimer();
}

// Выключить экран
void turnOffScreen(){
  lcd.noBacklight();
}

// Возращает текущий экран
int getScreenMode(){
  return screen_mode;
}

// Отобразить сообщение
void showMessage(String message) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(message);
}

// Дисплей по умолчанию
void showDeafultScreen() {
    lcd.setCursor(0, 0);
    lcd.print(temp_desired); // Выводим желаемую температру
    lcd.setCursor(8, 0);
    lcd.print(temp_vessel); // Выводим температура контура отопления
    lcd.setCursor(0, 1); 
    lcd.print(temp_in); // Выводим значение температурных датчиков
    lcd.setCursor(8, 1);
    lcd.print(temp_out);
}

// Настройки темп. кривой
void showCurveScreen() {
    float i;
    i = getCurve();
    lcd.setCursor(0, 0);
    lcd.print("Curve: ");
    if (i > 1.5) {
        lcd.print("Manual");
    } else {
        lcd.print(i);
    }
}

// Настройки поправки термостата delta_t
void showDeltaTScreen() {
    lcd.setCursor(0, 0);
    lcd.print("Delta T: ");
    lcd.print(getDeltaT());
}

// Настройки поправки комнатной температуры delta_k
void showDeltaKScreen() {
    lcd.setCursor(0, 0);
    lcd.print("Delta K: ");
    lcd.print(getDeltaK());
}

// Тестирование
void showTestScreen() {
    lcd.setCursor(0, 0);
    lcd.print("T ");
    lcd.print(temp_desired); // Выводим желаемую температру
    lcd.print(" V ");
    lcd.print(temp_vessel); // Выводим температура контура отопления
    // Выводим значение температурных датчиков
    lcd.setCursor(0, 1);
    lcd.print("O ");
    lcd.print(temp_out);
    lcd.print("I ");
    lcd.print(temp_in);

}

// Статистика
void showStatScreen() {
    lcd.setCursor(0, 0);
    lcd.print("Stat");
    lcd.setCursor(0, 1);
    lcd.print(temp_min);
    lcd.setCursor(8, 1);
    lcd.print(temp_max);
}

// Экран по умолчанию
void setDefaultScreen() {
    screen_mode = 0;
    updateScreen();
}
