#include <WiFi.h>
#include <UbidotsEsp32Mqtt.h>

// Configuration of i/o devices
#define FLAME_SENSOR_PIN 13
#define LED_PIN 12

// Wi-Fi configuration
const char* ssid = "<SSID>";
const char* password = "<PASSWORD>";

// Ubidots Configuration
const char* UBIDOTS_TOKEN = "<UBIDOTS-TOKEN>";
const char* DEVICE_LABEL = "<DEVICE-LABEL>";
const char* VARIABLE_LABEL = "<VARIABLE-LABEL>";

Ubidots ubidots(UBIDOTS_TOKEN);

// Shared Variables
volatile int flameState = LOW;
SemaphoreHandle_t xSemaphore = NULL;

void activateAlarm() {
  for(int i = 1; i <= 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void checkFlameTask(void * parameter) {
  while (true) {
    int currentFlameState = digitalRead(FLAME_SENSOR_PIN);
    if (currentFlameState == HIGH) {
      activateAlarm();
    }

    // Secure update of the value of the sensor
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY)) {
      flameState = currentFlameState;
      xSemaphoreGive(xSemaphore);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS); // wait for 1 sec
  }
}

void sendDataTask(void * parameter) {
  while (true) {
    if (!ubidots.connected()) {
      ubidots.reconnect();
    }

    int currentFlameState;
    // Secure read of the state of sensor
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY)) {
      currentFlameState = flameState;
      xSemaphoreGive(xSemaphore);
    }

    ubidots.add(VARIABLE_LABEL, currentFlameState);
    bool bufferSent = ubidots.publish(DEVICE_LABEL);

    if (bufferSent) {
      Serial.println("Datos enviados con éxito a Ubidots");
    } else {
      Serial.println("Error al enviar los datos a Ubidots");
    }

    vTaskDelay(4000 / portTICK_PERIOD_MS); // wait for 4 secs
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Wi-Fi Connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Ubidots Connection
  ubidots.setDebug(true); // Print debug info on serial
  ubidots.setup();

  // semaphore creation
  xSemaphore = xSemaphoreCreateMutex();

  // Tasks
  xTaskCreatePinnedToCore(checkFlameTask, "CheckFlameTask", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(sendDataTask, "SendDataTask", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // No need any code, tasks are managed by FreeRTOS
}
