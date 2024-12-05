#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Definisikan pin
#define MOISTURE_SENSOR_PIN 33  // Pin analog untuk sensor kelembaban tanah
#define RELAY_PIN 4            // Pin digital untuk relay
#define MOISTURE_THRESHOLD 500 // Batas kelembaban tanah (nilai analog, sesuaikan)
#define LDR_PIN 34             // Pin digital untuk output LDR (DO)

// Inisialisasi LCD (I2C, alamat 0x27 untuk 16x2 LCD, sesuaikan dengan alamat yang benar)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Konfigurasi WiFi
const char* ssid = "Titik Ruang Sambat";// Ganti dengan SSID WiFi
const char* password = "koeretito"; // Ganti dengan password WiFi

// URL server PHP
const char* serverURL = "http://192.168.1.2//port/project_IOT/php/receive_and_fetch_data.php?temp="; // Ganti dengan IP server Anda

void setup() {
  // Inisialisasi serial monitor
  Serial.begin(115200);

  // Inisialisasi LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Watering");

  // Inisialisasi pin
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT); // Set pin LDR sebagai input
  digitalWrite(RELAY_PIN, HIGH); // Pastikan relay mati di awal

  delay(2000); // Delay 2 detik untuk menunggu LCD tampil

  // Sambungkan ke WiFi
  WiFi.begin(ssid, password);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    lcd.setCursor(0, 1);
    lcd.print(".");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  Serial.println("WiFi Connected!");
}
void sendDataToServer(int moisture, int ldr) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String address = serverURL;
    address += String(moisture);
    address += "&ldr=";
    address += String(ldr);

    http.begin(client, address);
    int httpCode = http.GET();
    String payload;

    if (httpCode > 0)
    {
      payload = http.getString();
      payload.trim();
      if (payload.length() > 0)
      {
        Serial.println(payload + "\n");
      }
    }

    http.end();
  }
  else
  {
    Serial.print("Not connected to wifi ");
    Serial.println(ssid);
    connectWifi();
  }
}

void loop() {
  // Baca nilai kelembaban tanah dari sensor (0-4095)
  int moistureValue = analogRead(MOISTURE_SENSOR_PIN);

  // Baca nilai digital dari LDR
  int ldrValue = digitalRead(LDR_PIN);

  // Tampilkan nilai kelembaban di serial monitor
  Serial.print("Kelembaban Tanah: ");
  Serial.println(moistureValue);

  // Kirim data ke server
  sendDataToServer(moistureValue, ldrValue);

  // Tampilkan nilai kelembaban di LCD
  lcd.clear();
  if (ldrValue == HIGH) { // HIGH berarti cahaya tidak cukup
    lcd.setCursor(0, 0);
    lcd.print("Cahaya Kurang");
  } else { // Jika cahaya cukup
    lcd.setCursor(0, 0);
    lcd.print("Cahaya cukup");
  }

  // Jika kelembaban tanah di bawah threshold, hidupkan pompa
  if (moistureValue >= MOISTURE_THRESHOLD) {
    lcd.setCursor(0, 1);
    lcd.print("Pompa ON");
    digitalWrite(RELAY_PIN, LOW); // Hidupkan relay (pompa menyala)

    // Pompa hidup selama 5 detik
    delay(5000);

    // Matikan pompa setelah 5 detik
    digitalWrite(RELAY_PIN, HIGH); // Matikan relay (pompa mati)
    lcd.setCursor(0, 1);
    lcd.print("Pompa OFF");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Kelembaban cukup");
  }

  delay(1000); // Delay 5 detik sebelum pembacaan selanjutnya
}


