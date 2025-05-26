#define BLYNK_PRINT Serial // Habilita la salida serial para depuración con Blynk

// Tus credenciales de Blynk
#define BLYNK_TEMPLATE_ID   "YOUR_BLYNK_TEMPLATE_ID" // ¡Cámbialo!
#define BLYNK_DEVICE_NAME   "YOUR_BLYNK_DEVICE_NAME"   // ¡Cámbialo!
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN"    // ¡Cámbialo!

// Tus credenciales WiFi
#define WIFI_SSID           "YOUR_WIFI_SSID"     // ¡Cámbialo!
#define WIFI_PASS           "YOUR_WIFI_PASSWORD" // ¡Cámbialo!

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h> // Librería de Blynk para ESP32

// Definición de pines para el sensor HC-SR04
const int trigPin = 23; // Pin de Trigger del sensor (salida)
const int echoPin = 19; // Pin de Echo del sensor (entrada)

// Variables para el cálculo de la distancia
long duration; // Duración del pulso de Echo
int distanceCm; // Distancia en centímetros

// Objeto temporizador de Blynk para enviar datos periódicamente
BlynkTimer timer;

// Función para leer la distancia y enviarla a Blynk
void sendDistanceToBlynk() {
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

  // Asegurarse de que la distancia esté dentro de un rango razonable para evitar valores erróneos
  // El HC-SR04 tiene un rango de 2cm a 400cm típicamente.
  if (distanceCm > 400 || distanceCm < 2) {
    // Si está fuera de rango, puedes enviar un valor especial o simplemente no enviar nada.
    // Para este ejemplo, enviaremos 0 si está fuera de rango para indicar "desconocido" o "muy lejos/cerca".
    Blynk.virtualWrite(V0, 0); // V0 es el pin virtual que creamos en Blynk
    Serial.println("Distancia: Fuera de rango o error (Enviando 0 a Blynk)");
  } else {
    // Enviar la distancia al pin virtual V0 en Blynk
    Blynk.virtualWrite(V0, distanceCm);
    Serial.print("Distancia: ");
    Serial.print(distanceCm);
    Serial.println(" cm (Enviado a Blynk)");
  }
}

void setup() {
  // Inicializa la comunicación serial a 115200 baudios
  Serial.begin(115200);

  // Configura los pines del sensor
  pinMode(trigPin, OUTPUT); // El pin Trig es una salida
  pinMode(echoPin, INPUT);  // El pin Echo es una entrada

  // Conectar a Blynk usando WiFi y las credenciales
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  // Configurar el temporizador para llamar a sendDistanceToBlynk cada 1000 ms (1 segundo)
  timer.setInterval(1000L, sendDistanceToBlynk);
}

void loop() {
  Blynk.run(); // Esta función debe ser llamada repetidamente en loop() para que Blynk funcione
  timer.run(); // Esta función debe ser llamada repetidamente para que el temporizador de Blynk funcione
}