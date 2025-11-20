// Program Dual Task System dengan Shared Data Buffer
// Task Sensor: Membaca 5 sensor dan menyimpan ke shared buffer
// Task Communication: Mengambil data dari buffer dan mengirim ke server

const int SENSOR_COUNT = 5;
const int sensorPins[SENSOR_COUNT] = {A0, A1, A2, A3, A4};

// Shared Data Buffer
struct SharedDataBuffer {
  int sensorValues[SENSOR_COUNT];
  unsigned long timestamp;
  bool dataReady;
} sharedDataBuffer;

// Timing variables
unsigned long lastSensorRead = 0;
unsigned long lastCommSend = 0;

const unsigned long SENSOR_INTERVAL = 200;  // Interval pembacaan sensor (ms)
const unsigned long COMM_INTERVAL = 500;     // Interval pengiriman komunikasi (ms)

void setup() {
  Serial.begin(9600);
  
  // Inisialisasi pin sensor
  for (int i = 0; i < SENSOR_COUNT; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  
  // Inisialisasi shared buffer
  sharedDataBuffer.dataReady = false;
  sharedDataBuffer.timestamp = 0;
  
  Serial.println("=== Dual Task System Started ===");
  Serial.println("Task Sensor: Reading 5 sensors");
  Serial.println("Task Communication: Sending to server");
  Serial.println("================================\n");
}

void loop() {
  // Task Sensor: Membaca data dari 5 sensor
  taskSensor();
  
  // Task Communication: Mengirim data ke server
  taskCommunication();
}

// Task Sensor: Membaca data dari sensor dan menyimpan ke shared buffer
void taskSensor() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastSensorRead >= SENSOR_INTERVAL) {
    Serial.println("Sensor Task: Updating buffer...");
    
    // Baca semua sensor
    for (int i = 0; i < SENSOR_COUNT; i++) {
      sharedDataBuffer.sensorValues[i] = analogRead(sensorPins[i]);
      delay(40); // Simulasi waktu pembacaan 40ms per sensor
    }
    
    // Update metadata buffer
    sharedDataBuffer.timestamp = currentTime;
    sharedDataBuffer.dataReady = true;
    
    // Tampilkan data yang dibaca
    Serial.print("  Data read at: ");
    Serial.print(currentTime);
    Serial.println(" ms");
    
    for (int i = 0; i < SENSOR_COUNT; i++) {
      Serial.print("  Sensor ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(sharedDataBuffer.sensorValues[i]);
    }
    
    Serial.println("  Buffer updated successfully!\n");
    
    lastSensorRead = currentTime;
  }
}

// Task Communication: Mengambil data dari shared buffer dan mengirim ke server
void taskCommunication() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastCommSend >= COMM_INTERVAL) {
    Serial.println("Communication Task: Sending data...");
    
    // Cek apakah ada data yang siap dikirim
    if (sharedDataBuffer.dataReady) {
      // Simulasi pengiriman data ke server
      Serial.println("  Retrieving data from shared buffer...");
      Serial.print("  Timestamp: ");
      Serial.print(sharedDataBuffer.timestamp);
      Serial.println(" ms");
      
      Serial.println("  Sending to server:");
      Serial.print("  Packet: [");
      for (int i = 0; i < SENSOR_COUNT; i++) {
        Serial.print(sharedDataBuffer.sensorValues[i]);
        if (i < SENSOR_COUNT - 1) {
          Serial.print(", ");
        }
      }
      Serial.println("]");
      
      delay(40); // Simulasi waktu pengiriman
      
      Serial.println("  Data sent successfully!");
      
      // Reset flag setelah data dikirim (opsional)
      // sharedDataBuffer.dataReady = false;
    } else {
      Serial.println("  No data available in buffer!");
    }
    
    Serial.println();
    lastCommSend = currentTime;
  }
}