#include <Adafruit_LiquidCrystal.h>

const int pinPulsador1 = 2;
const int pinPulsador0 = 4;
const int pinGen = A0;

Adafruit_LiquidCrystal lcd_1;

bool registrarDatos = false;
int numMuestras = 1000;
float valFuncion[1000];

void setup() {
  pinMode(pinPulsador1, INPUT);
  pinMode(pinPulsador0, INPUT);
  lcd_1.begin(16, 2);
  lcd_1.clear();
  lcd_1.print("Iniciando...");
  delay(1000);
}

void loop() {
  if (digitalRead(pinPulsador1) == HIGH) {
    registrarDatos = true;
    lcd_1.clear();
    lcd_1.print("Iniciando muestreo...");
  }

  if (registrarDatos) {
    for (int i = 0; i < numMuestras; i++) {
      valFuncion[i] = analogRead(pinGen);
      delay(1);

      if (digitalRead(pinPulsador0) == HIGH) {
        registrarDatos = false;
        lcd_1.clear();
        lcd_1.print("Muestreo detenido");
        break;
      }
    }

    if (!registrarDatos) {
      analizarOnda(valFuncion, numMuestras);
    }
  }
}

void analizarOnda(float* valores, int numMuestras) {
  String tipoOnda = identificarTipoOnda(valores, numMuestras);

  if (tipoOnda == "Onda Cuadrada") {
    analizarOndaCuadrada(valores, numMuestras);
  } else if (tipoOnda == "Onda Senoidal") {
    analizarOndaSenoidal(valores, numMuestras);
  } else if (tipoOnda == "Onda Dientes de Sierra") {
    analizarOndaDientesDeSierra(valores, numMuestras);
  } else {
    lcd_1.clear();
    lcd_1.print("Onda Desconocida");
  }
}

String identificarTipoOnda(float* valores, int numMuestras) {
  float amplitud = 0;
  float frecuencia = 0;
  int crucesCero = 0;
  int crucesFlanco = 0;
  int estadoAnterior = 0;

  for (int i = 0; i < numMuestras; i++) {
    if (abs(valores[i] - 512) > amplitud) {
      amplitud = abs(valores[i] - 512);
    }
  }

  for (int i = 1; i < numMuestras; i++) {
    if ((valores[i] - 512) * (valores[i-1] - 512) < 0) {
      crucesCero++;
    }
  }

  for (int i = 1; i < numMuestras; i++) {
    int estadoActual = (valores[i] > 512) ? 1 : 0;
    if (estadoActual != estadoAnterior) {
      crucesFlanco++;
    }
    estadoAnterior = estadoActual;
  }

  if (crucesFlanco > crucesCero * 2){
    return "Onda Cuadrada";
  } else if (crucesCero > crucesFlanco * 2) {
    if (amplitud > 200) {
      return "Onda Senoidal";
    } else {
      return "Onda Dientes de Sierra";
    }
  } else {
    return "Onda Desconocida";
  }
}

void analizarOndaCuadrada(float* valores, int numMuestras) {
  float amplitud = 0;
  float frecuencia = 0;
  int crucesFlanco = 0;
  int estadoAnterior = 0;

  for (int i = 0; i < numMuestras; i++) {
    if (abs(valores[i] - 512) > amplitud) {
      amplitud = abs(valores[i] - 512);
    }
  }

  for (int i = 1; i < numMuestras; i++){
    int estadoActual = (valores[i] > 512) ? 1 : 0;
    if (estadoActual != estadoAnterior) {
      crucesFlanco++;
    }
   estadoAnterior = estadoActual;
  }
  frecuencia = crucesFlanco / (2 * 1 * numMuestras) * 1000; // Hz

  lcd_1.clear();
  lcd_1.print("Onda Cuadrada");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Amp: ");
  lcd_1.print(amplitud);
  lcd_1.print(" Freq: ");
  lcd_1.print(frecuencia);
  lcd_1.print(" Hz");
}

void analizarOndaSenoidal(float* valores, int numMuestras) {
  float amplitud = 0;
  float frecuencia = 0;
  int crucesCero = 0;

  for (int i = 0; i < numMuestras; i++) {
    if (abs(valores[i] - 512) > amplitud) {
      amplitud = abs(valores[i] - 512);
    }
  }

  for (int i = 1; i < numMuestras; i++) {
    if ((valores[i] - 512) * (valores[i-1] - 512) < 0) {
      crucesCero++;
    }
  }
  frecuencia = crucesCero / (2 * 1 * numMuestras) * 1000; // Hz

  lcd_1.clear();
  lcd_1.print("Onda Senoidal");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Amp: ");
  lcd_1.print(amplitud);
  lcd_1.print(" Freq: ");
  lcd_1.print(frecuencia);
  lcd_1.print(" Hz");
}

void analizarOndaDientesDeSierra(float* valores, int numMuestras) {
  float amplitud = 0;
  float frecuencia = 0;
  int crucesCero = 0;

  for (int i = 0; i < numMuestras; i++) {
    if (abs(valores[i] - 512) > amplitud) {
      amplitud = abs(valores[i] - 512);
    }
  }

  for (int i = 1; i < numMuestras; i++) {
    if ((valores[i] - 512) * (valores[i-1] - 512) < 0) {
      crucesCero++;
    }
  }
  frecuencia = crucesCero / (2 * 1 * numMuestras) * 1000; // Hz

  lcd_1.clear();
  lcd_1.print("Onda Dientes");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Amp: ");
  lcd_1.print(amplitud);
  lcd_1.print(" Freq: ");
  lcd_1.print(frecuencia);
  lcd_1.print(" Hz");
}
