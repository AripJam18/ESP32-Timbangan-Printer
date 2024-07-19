#include <HardwareSerial.h>

// Inisialisasi objek serial untuk printer dan timbangan
HardwareSerial SerialPrinter(1); // UART1
HardwareSerial SerialScale(2);   // UART2

// Pin untuk push button dan LED
const int buttonPin = 12;
const int ledPin = 13;

void setup() {
  // Mulai komunikasi serial dengan baud rate yang sesuai (cek manual perangkat)
  SerialPrinter.begin(9600, SERIAL_8N1, 16, 17); // RX, TX pin untuk printer thermal
  SerialScale.begin(9600, SERIAL_8N1, 4, 5);    // RX, TX pin untuk timbangan digital

  // Inisialisasi pin push button dan LED
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  // Cek apakah printer terhubung
  SerialPrinter.write(27); // ESC
  SerialPrinter.write(64); // @ (Reset printer)
  delay(100); // Tunggu sebentar
  if (SerialPrinter) {
    digitalWrite(ledPin, HIGH); // Nyalakan LED jika printer terhubung
    SerialPrinter.print("Printer Connected!\n"); // Cetak teks
  } else {
    digitalWrite(ledPin, LOW); // Matikan LED jika printer tidak terhubung
  }
}

void loop() {
  // Membaca data dari timbangan digital
  String scaleData = "";
  if (SerialScale.available()) {
    scaleData = SerialScale.readStringUntil('\n');
  }

  // Jika push button ditekan, cetak data dari timbangan ke printer
  if (digitalRead(buttonPin) == LOW) {
    if (scaleData != "") {
      SerialPrinter.print("Weight: ");
      SerialPrinter.println(scaleData); // Cetak data dari timbangan ke printer
      SerialPrinter.write(27); // ESC
      SerialPrinter.write(100); // d
      SerialPrinter.write(4); // Jarak baris baru
      SerialPrinter.write(27); // ESC
      SerialPrinter.write(105); // i (Potong kertas)
    }
    delay(200); // Debounce delay
  }
}
