/*
 * DHT11.h - Library untuk membaca Humidity dan Temperature dari DHT11 Sensor
 * Dibuat berdasarkan Communication Protocol DHT11
 * 
 * Protocol Communication DHT11:
 * 1. Start Signal: MCU pull low minimal 18ms, kemudian pull up
 * 2. Response: DHT11 pull low 54us, lalu high 80us
 * 3. Data: 40-bit (5 byte) = Humidity Int + Humidity Dec + Temp Int + Temp Dec + Checksum
 * 4. Bit Encoding: 
 *    - Bit '0': 54us low + 24us high
 *    - Bit '1': 54us low + 70us high
 */

#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>

// Definisi konstanta timing berdasarkan datasheet DHT11
#define DHT11_START_SIGNAL_LOW  18000  // Minimum 18ms start signal (dalam microseconds)
#define DHT11_START_SIGNAL_HIGH 40     // Pull up delay 20-40us
#define DHT11_TIMEOUT           1000   // Timeout untuk pembacaan (dalam microseconds)
#define DHT11_DATA_BITS         40     // Total 40 bit data
#define DHT11_MIN_INTERVAL      2000   // Minimum sampling interval 2 detik (dalam ms)

// Enum untuk status pembacaan
enum DHT11_Status {
  DHT11_OK = 0,              // Pembacaan berhasil
  DHT11_ERROR_TIMEOUT,       // Timeout saat menunggu response
  DHT11_ERROR_CHECKSUM,      // Checksum tidak valid
  DHT11_ERROR_TOO_FAST       // Sampling terlalu cepat (< 2 detik)
};

class DHT11 {
private:
  uint8_t _pin;                    // Pin data DHT11
  unsigned long _lastReadTime;     // Waktu pembacaan terakhir
  
  // Data sensor yang dibaca (5 byte)
  uint8_t _data[5];                // [0]=RH_Int, [1]=RH_Dec, [2]=Temp_Int, [3]=Temp_Dec, [4]=Checksum
  
  float _humidity;                 // Humidity dalam percentage
  float _temperature;              // Temperature dalam Celsius
  DHT11_Status _status;            // Status pembacaan terakhir
  
  /*
   * Fungsi untuk mengirim start signal ke DHT11
   * Protocol: Pull low minimal 18ms, kemudian pull high
   */
  void sendStartSignal();
  
  /*
   * Fungsi untuk menunggu response dari DHT11
   * Response: DHT11 pull low 54us, kemudian high 80us
   * Return: true jika response valid, false jika timeout
   */
  bool waitForResponse();
  
  /*
   * Fungsi untuk membaca 1 bit dari DHT11
   * Bit '0': 54us low + 24us high (total ~78us high time)
   * Bit '1': 54us low + 70us high (total ~124us high time)
   * Return: 0 atau 1
   */
  uint8_t readBit();
  
  /*
   * Fungsi untuk membaca 1 byte (8 bit) dari DHT11
   * Return: byte value (0-255)
   */
  uint8_t readByte();
  
  /*
   * Fungsi untuk menunggu pin menjadi LOW atau HIGH dengan timeout
   * Parameter: state (LOW/HIGH), timeout dalam microseconds
   * Return: true jika berhasil, false jika timeout
   */
  bool waitForPinState(uint8_t state, unsigned long timeout);
  
public:
  /*
   * Constructor - Inisialisasi DHT11 dengan pin tertentu
   * Parameter: pin - nomor pin digital Arduino yang terhubung ke DHT11
   */
  DHT11(uint8_t pin);
  
  /*
   * Fungsi untuk memulai komunikasi dengan DHT11
   * Harus dipanggil di setup()
   */
  void begin();
  
  /*
   * Fungsi utama untuk membaca data dari DHT11
   * Mengikuti protocol communication:
   * 1. Kirim start signal
   * 2. Tunggu response
   * 3. Baca 40 bit data (5 byte)
   * 4. Verifikasi checksum
   * 
   * Return: DHT11_Status (OK, ERROR_TIMEOUT, ERROR_CHECKSUM, ERROR_TOO_FAST)
   */
  DHT11_Status read();
  
  /*
   * Getter untuk humidity dalam percentage (%)
   * Return: float (20.0 - 90.0%)
   */
  float getHumidity();
  
  /*
   * Getter untuk temperature dalam Celsius (°C)
   * Return: float (0.0 - 50.0°C)
   */
  float getTemperature();
  
  /*
   * Getter untuk temperature dalam Fahrenheit (°F)
   * Konversi: F = C * 9/5 + 32
   * Return: float (32.0 - 122.0°F)
   */
  float getTemperatureF();
  
  /*
   * Getter untuk status pembacaan terakhir
   * Return: DHT11_Status enum
   */
  DHT11_Status getStatus();
  
  /*
   * Fungsi untuk mendapatkan string keterangan status
   * Return: String deskripsi status
   */
  String getStatusString();
};

#endif // DHT11_H