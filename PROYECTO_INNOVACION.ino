  
 #include <LiquidCrystal.h>
#include <Servo.h>
#include <DHT.h>
#include <DHT_U.h>

// Definir pines
int rs = 12, e = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
int greenled = 6, redled = 7, bombaAgua = 8, buttonPin = 10, servoPin = 13;
int Y = 0, Min = 60, Max = 72; // Humedad mínima y máxima
int dhtPin = 9;

Servo myServo;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);
#define Type DHT11
DHT HT(dhtPin, Type);

// Variables globales
int humidity;
float tempC;
int buttonState = 0;
int drtmi=1000;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(bombaAgua, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  myServo.attach(servoPin);
  HT.begin();
}

void loop() {
  // Leer sensor y botón
  leerSensorDHT();
  buttonState = digitalRead(buttonPin);


  // Mostrar datos en pantalla LCD
  mostrarDatosEnLCD();

  // Control de LEDs basado en la humedad
  controlLEDs();

  // Control del servo con el joystick
  controlServo();

  // Control de la bomba de agua con el botón
  controlBombaAgua();

  // Verificación del sensor DHT

  
  delay(1000); // Esperar antes de la próxima actualización
}

// Función para leer los datos del sensor DHT
void leerSensorDHT() {
  humidity = HT.readHumidity();
  tempC = HT.readTemperature();
}

// Función para mostrar datos en el LCD
void mostrarDatosEnLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(humidity);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Temp.: ");
  lcd.print(tempC);
  lcd.print("C   ");
}

// Función para controlar los LEDs
void controlLEDs() {
  if (humidity < Min) {
    digitalWrite(redled, LOW);
    digitalWrite(greenled, HIGH);
    Serial.println("LED verde encendido: Humedad baja");
  } else if (humidity >= Max) {
    digitalWrite(redled, HIGH);
    digitalWrite(greenled, LOW);
    Serial.println("LED rojo encendido: Humedad alta");
  } else {
    digitalWrite(redled, LOW);
    digitalWrite(greenled, LOW);
    Serial.println("Ambos LEDs apagados: Humedad normal");
  }
}

// Función para controlar el servo
void controlServo() {
  Y = analogRead(A1);
  int angle = map(Y, 0, 1023, 0, 180);
  myServo.write(angle);
}

// Función para controlar la bomba de agua
void controlBombaAgua() {
  if (buttonState == HIGH) {
 
  
    digitalWrite(bombaAgua, HIGH);
  }
  
    

   else {
    digitalWrite(bombaAgua, LOW);
       lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("####REGANDO####");
    lcd.setCursor(0, 1);
    lcd.print("################");
  }
}

// Función para verificar el estado del sensor DHT
void verificarSensorDHT() {
  if (isnan(humidity) || isnan(tempC)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("####ERROR####");
    lcd.setCursor(0, 1);
    lcd.print("##NO-SENSOR-DHT##");
    Serial.println("Error: Sensor DHT no conectado o no funcionando");
  }
}
