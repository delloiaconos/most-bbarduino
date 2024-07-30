
// Libreria necessaria per leggere i dati dal sensore BMI270
#include "Arduino_BMI270_BMM150.h"
//Accelerometer range is set at [-4, +4]g -/+0.122 mg.
//Gyroscope range is set at [-2000, +2000] dps +/-70 mdps

// Libreria necessaria per leggere il sensore di prossimità
#include "Arduino_APDS9960.h"

#include <SD.h>
#include <SPI.h>
// Pin del modulo SD
const int chipSelect = 10;


float x, y, z;
float gx, gy, gz;
unsigned long logCount = 0;


// Setup per comunicazione seriale: 9600=baud rate
// Sto fermo finchè la seriale non è pronta, dopodichè:
// se la IMU è pronta stampo frequenza di acquisizione di acceleromtro e giroscopio


  char filenameIMU[20];


void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  // Inizializza la scheda SD
  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization of SD card failed!");
    while (1);
  }
  Serial.println("SD card initialized.");


  sprintf(filenameIMU, "IMU_%lu.txt", millis());


  // Inizializza la IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor.");
  }

}

void loop() {

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && APDS.proximityAvailable()) {
    IMU.readAcceleration(x, y, z);
    IMU.readGyroscope(gx, gy, gz);
    uint8_t proximity = APDS.readProximity();


    // Ottieni il timestamp relativo in millisecondi
    unsigned long timestamp = millis();

    // Incrementa il contatore del numero di log
    logCount++;

    // Apri il file per aggiungere i dati
    File dataFile = SD.open(filenameIMU, FILE_WRITE);

    // Se il file è aperto, scrivi i dati
    if (dataFile) {
      dataFile.print(logCount);
      dataFile.print(", ");
      dataFile.print(timestamp);
      dataFile.print(", ");
      dataFile.print(x, 4);
      dataFile.print(", ");
      dataFile.print(y, 4);
      dataFile.print(", ");
      dataFile.print(z, 4);
      dataFile.print(", ");
      dataFile.print(gx, 4);
      dataFile.print(", ");
      dataFile.print(gy, 4);
      dataFile.print(", ");
      dataFile.print(gz, 4);
      dataFile.print(", ");
      dataFile.print(proximity);
      dataFile.println();
      dataFile.close();
    } else {
      // Se il file non può essere aperto, stampa un errore
      Serial.println("Error opening IMU file");
    }

    // Stampa i valori di accelerazione, giroscopio e il timestamp su Serial Monitor
    Serial.print("Log #");
    Serial.print(logCount);
    Serial.print(", Timestamp: ");
    Serial.print(timestamp);
    Serial.print(" ms, ");
    Serial.print("Acceleration in X: ");
    Serial.print(x, 4);
    Serial.print(" g, Acceleration in Y: ");
    Serial.print(y, 4);
    Serial.print(" g, Acceleration in Z: ");
    Serial.print(z, 4);
    Serial.print(" g, Gyroscope in X: ");
    Serial.print(gx, 4);
    Serial.print(" dps, Gyroscope in Y: ");
    Serial.print(gy, 4);
    Serial.print(" dps, Gyroscope in Z: ");
    Serial.print(gz, 4);
    Serial.println(" dps");
    Serial.println(proximity);
  }

}
