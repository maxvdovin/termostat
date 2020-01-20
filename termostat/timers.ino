unsigned long one_minute_timer;
unsigned long ten_minutes_timer;
unsigned long screeen_timer;

void setupTimers(){
  unsigned long now = millis();
  one_minute_timer = now;
  ten_minutes_timer = now;
  screeen_timer = now;
}

void updateTimers() {
  unsigned long now = millis();
  everyMinuteEvent(now);
  every10MinutesEvent(now);
  updateScreenTimer(now);
}

void everyMinuteEvent(unsigned long now) {
  int screen_mode;
  screen_mode = getScreenMode();
  if (now - one_minute_timer >= 60000) {
    one_minute_timer =  now;
    if (screen_mode != 4) getTemp();
    updateScreen();
  }
}

void every10MinutesEvent(unsigned long now) {
  if (now - ten_minutes_timer >= 600000) {
    ten_minutes_timer = now;
    updateVaillant();
    updateScreen();
  }
}

void updateScreenTimer(unsigned long now) {
  if (screeen_timer > 0) { // Если таймер запущен
    if (now - screeen_timer >= 180000) { // Через три минуты
        screeen_timer = 0;
        setDefaultScreen();
        turnOffScreen(); // Выключаем экран
    }
  }
}

void setScreenTimer() {
    screeen_timer = millis();
}
