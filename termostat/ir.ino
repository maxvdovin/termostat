// Подключаем ИК
#include <IRremote.h>
IRrecv irrecv(2);
decode_results results;

// Настраиваем ИК приемник
void setupIR(){
  irrecv.enableIRIn();
}

// Проверяем получение ИК сигнала
void updateIR(){
  int screen_mode;

  if (irrecv.decode(&results)) {
    // При любом нажатии включаем экран
    turnOnScreen();
    screen_mode = getScreenMode();
    switch (results.value) {
        // #
        case 0xFF52AD:
            turnOffScreen();
            break;
        // *
        case 0xFF42BD:
            break;
        // OK
        case 0xFF02FD:
            if (screen_mode != 4) getTemp();
            updateVaillant();
            break;
        // вправо
        case 0xFFC23D:
            setNextScreen();
            break;
        // влево
        case 0xFF22DD:
            setPrevScreen();
            break;
        // вверх
        case 0xFF629D:
            if (screen_mode == 0) temp_desired += 0.25;
            if (screen_mode == 1) addCurve(0.05);
            if (screen_mode == 2) addDeltaT(0.5);
            if (screen_mode == 3) addDeltaK(0.5);
            if (screen_mode == 4) temp_desired += 0.25;
            break;
        // вниз
        case 0xFFA857:
            if (screen_mode == 0) temp_desired -= 0.25;
            if (screen_mode == 1) addCurve(-0.05);
            if (screen_mode == 2) addDeltaT(-0.5);
            if (screen_mode == 3) addDeltaK(-0.5);
            if (screen_mode == 4) temp_desired -= 0.25;
            break;
        // 0
        case 0xFF4AB5:
            if (screen_mode == 1) {
                setCurve(1.6);
            } else {
                temp_min = temp_out;
                temp_max = temp_out;
            }
            break;
        // 1
        case 0xFF6897:
            if (screen_mode == 4){
                temp_out = round(temp_out);
                temp_out += 1;
            }
            break;
        // 4
        case 0xFF30CF:
            if (screen_mode == 4){
                temp_out = round(temp_out);
                temp_out -= 1;
            }
            break;
        // 2
        case 0xFF9867:
            if (screen_mode == 4){
                temp_in = round(temp_in);
                temp_in += 1;
            }
            break;
        // 5
        case 0xFF18E7:
            if (screen_mode == 4){
                temp_in = round(temp_in);
                temp_in -= 1;
            }
            break;
      }          
      updateScreen();
      irrecv.resume();
    }
}
