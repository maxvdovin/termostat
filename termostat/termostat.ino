// Новые переменные
// Датчики
float temp_out = 20; // Показания первого датчика температуры (наружная)
float temp_in = 20; // Показания второго датчика температуры (фактическая температура в доме)
float temp_desired = 20.5; // Желаемая температура в доме
int temp_vessel = 0; // Температура контура отопления котла
float temp_max = 0; // Максимальная наружная температура 
float temp_min = 0; // Минимальная наружная температура 

void setup() {
    setupSensors();
    setupScreen();
    setupIR();
    setupTimers();

    getTemp();
    temp_min = temp_out;
    temp_max = temp_out;
    updateVaillant();
    updateScreen();
}

void loop() {
    updateIR();
    updateTimers();
}
