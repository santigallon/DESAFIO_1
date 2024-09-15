//c++
#include <Adafruit_LiquidCrystal.h>

const int pinPulsador1 = 2;
const int pinPulsador0 = 4;
const int pinGen = A0;

Adafruit_LiquidCrystal lcd_1(0);

bool registrarDatos = false;
int numMuestras = 0;
float* valFuncion = nullptr;

void setup() {
  pinMode(pinPulsador1, INPUT);
  pinMode(pinPulsador0, INPUT);
  lcd_1.begin(16, 2);
}

void loop() {
  // Espera a que se presione y suelte el botón 1
  while (digitalRead(pinPulsador1) == LOW);
  delay(10);
  registrarDatos = true;
  lcd_1.clear();
  lcd_1.print("Registros activos");

  // Inicializa el arreglo dinámico
  valFuncion = new float[numMuestras];

  // Registra datos hasta que se presione y suelte el botón 0
  while (registrarDatos) {
    // Lee los valores del generador
    float valorFuncion = analogRead(pinGen);

    // Se realiza un incremento dinámico del tamaño del arreglo valFuncion
    numMuestras++;
    float* valFuncionTemporal = new float[numMuestras];
    for (int i = 0; i < numMuestras - 1; i++) {
      valFuncionTemporal[i] = valFuncion[i];
    }
    valFuncionTemporal[numMuestras - 1] = valorFuncion;
    delete[] valFuncion;
    valFuncion = valFuncionTemporal;

    // Tiempo de muestreo
    delay(10);

    // Verifica si se ha presionado y soltado el botón 0
    if (digitalRead(pinPulsador0) == HIGH) {
      delay(10); // Espera un poco para evitar rebotes
      if (digitalRead(pinPulsador0) == LOW) {
        registrarDatos = false;
        lcd_1.clear();
        lcd_1.print("Registro detenido");
      }
    }
  }

  // Imprimir valores
  for (int i = 0; i < numMuestras; i++) {
    lcd_1.setCursor(0, 1);
    lcd_1.print(valFuncion[i]);
    delay(1000);
  }

  // Liberar memoria dinámica
  delete[] valFuncion;
}
