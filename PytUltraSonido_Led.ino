#include <Wire.h> // Librería para comunicación I2C
#include <Adafruit_GFX.h> // Librería gráfica para pantallas OLED
#include <Adafruit_SSD1306.h> // Librería específica para el controlador SSD1306

// Definición de pines para el sensor HC-SR04
const int trigPin = 23; // Pin de Trigger del sensor (salida)
const int echoPin = 19; // Pin de Echo del sensor (entrada) - CAMBIADO A GPIO 19

// Variables para el cálculo de la distancia
long duration; // Duración del pulso de Echo
int distanceCm; // Distancia en centímetros

// Configuración de la pantalla OLED
#define SCREEN_WIDTH 128    // Ancho de la pantalla OLED en píxeles
#define SCREEN_HEIGHT 64   // Alto de la pantalla OLED en píxeles

// Declaración para una pantalla SSD1306 conectada a I2C (pines SDA, SCL)
// El '-1' al final indica que no hay pin RESET dedicado para la OLED.
// La dirección I2C común es 0x3C o 0x3D. Si no funciona, pruébalas o ejecuta un escáner I2C.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Inicializa la comunicación serial a 115200 baudios (para depuración)
  Serial.begin(115200);

  // Configura los pines del sensor
  pinMode(trigPin, OUTPUT); // El pin Trig es una salida
  pinMode(echoPin, INPUT);  // El pin Echo es una entrada

  // Inicializa la pantalla OLED con la dirección I2C 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Dirección 0x3C (común)
    Serial.println(F("Error de asignación de SSD1306"));
    for (;;); // No continuar si la pantalla no se inicializa
  }

  // Muestra un mensaje inicial en la pantalla
  display.clearDisplay(); // Limpia el buffer de la pantalla
  display.setTextSize(1); // Tamaño del texto (1 = 6x8 píxeles)
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(0, 0); // Posición del cursor (columna, fila)
  display.println("Iniciando...");
  display.println("Sensor HC-SR04");
  display.display(); // Muestra el contenido del buffer en la pantalla
  delay(2000); // Pequeña pausa
}

void loop() {
  // Limpiar el pin Trig para asegurar un pulso limpio
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Generar un pulso de 10 microsegundos en el pin Trig
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir la duración del pulso en el pin Echo
  duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  distanceCm = duration * 0.034 / 2;

  // Limpiar la pantalla para la nueva lectura
  display.clearDisplay();

  // Mostrar el título
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Distancia:");

  // Mostrar la distancia (más grande si es posible)
  if (distanceCm >= 400 || distanceCm <= 0) {
    display.setTextSize(2); // Tamaño más grande para el mensaje
    display.setCursor(0, 20);
    display.println("Fuera de Rango");
  } else {
    display.setTextSize(3); // Tamaño grande para la distancia
    display.setCursor(0, 20); // Posicionar más abajo
    display.print(distanceCm);
    display.setTextSize(1); // Volver a tamaño normal para la unidad
    display.print(" cm");
  }

  // Actualizar la pantalla
  display.display();

  // Imprimir en el monitor serial también (para depuración)
  if (distanceCm >= 400 || distanceCm <= 0) {
    Serial.println("Fuera de rango");
  } else {
    Serial.print("Distancia: ");
    Serial.print(distanceCm);
    Serial.println(" cm");
  }

  // Pequeña pausa antes de la siguiente lectura
  delay(100);
}