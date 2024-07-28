#include <PDM.h>
#include <SD.h>
#include <SPI.h>


/* SD Card Chip Select */
#define SD_CS  10

/* PDM Configuration constants */
#define PDM_FS 16000  // Sampling Frequency [Hz]
#define PDM_CH 1      // Number of channels


// Create e new type, for holding together:
// - PDM buffer size, for raw data
// - Buffer size, for elborated audio data
typedef struct buff_conf_s {
  uint32_t pdm_size;
  uint32_t buff_len;
} buff_conf_t;

// The create an array of configurations, using the type defined above
buff_conf_t configs[] = { {256, 512}, {256, 1024}, {256, 2048}, {256, 4096}, 
                          {512, 512}, {512, 1024}, {512, 2048}, {512, 4096},
                          {1024, 1024}, {1024, 2048}, {1024, 4096}, 
                          {2048, 2048}, {2048, 4096}, {2048, 8192},
                          {4096, 4096}, {4096, 8192} };

#define  CONFIG_LEN  ( sizeof( configs ) / sizeof( configs[0] ) )

uint32_t configPtr = 0;


#define AUDIOBUFF_SIZE 4096*4
int16_t audioBuffer[AUDIOBUFF_SIZE]; // audio buffer
uint32_t bufferLen;
int audioSamples; // number of samples read

// Variabili per gestire il tempo di registrazione
unsigned long startTime;
#define recordingDuration 5000 // Recording length [ms]

char fileName[32]; 
File audioFile;
int audioFileInx = 0;  // Index for audio files

/* PDM Callback */
// Callback function, called when new PDM data are available
void onPDMdata() {

  static int status = 0;
  static uint32_t istart = 0;

  // Query the number of available bytes
  int bytesAvailable = PDM.available();

  // Read into the sample buffer, from istart to istart+bytesAvailable
  PDM.read(&audioBuffer[istart], bytesAvailable);

  istart = istart + bytesAvailable;
  // 16-bit, 2 bytes per sample

  if( istart >= bufferLen ) {
    audioSamples = istart;
    istart = 0;
  }

  if (status == 0) {
    digitalWrite(3, HIGH);
    status = 1;
  } else {
    digitalWrite(3, LOW);
    status = 0;
  }

}


void setup() {

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);



  // Inizializzo la seriale
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  // Inizializza la scheda SD
  if (!SD.begin(SD_CS)) {
    Serial.println("Initialization of SD card failed!");
    while (1);
  }
  Serial.println("SD card initialized.");


  // Configure the data receive callback
  PDM.onReceive(onPDMdata);
  Serial.println("PDM callback");

  bufferLen = 0;
  configPtr = 0;
  audioSamples = 0;

  // Optionally set the gain, defaults to 20
  PDM.setGain(0);
  // Set the size buffers' size, using the ones in configs[], using the index configPtr
  PDM.setBufferSize( configs[configPtr].pdm_size );
  bufferLen = configs[configPtr].buff_len;

  // Attempt to create a new audio file with a unique name
  sprintf(fileName, "%03d_%03d.raw", configPtr, audioFileInx++ );
  audioFile = SD.open( fileName, FILE_WRITE );

  if (!audioFile) {
    Serial.println("Error opening new audio file");
    while (1);
  } else {
    Serial.print("New audio file created: ");
    Serial.println(fileName);
  }

  // Increments the index for configs[] 
  //(if exceed CONFIG_LEN, with % it restarts from 0)
  configPtr = ( configPtr + 1 ) % CONFIG_LEN;

  // Initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
  if (!PDM.begin(PDM_CH, PDM_FS)) {
    Serial.println("Failed to start PDM!");
    while (1);
  } else {
    Serial.println("PDM started");
  }

  // Registra il tempo di inizio della registrazione
  startTime = millis();
}



void loop() {
  // Scrittura dati audio
  if (audioSamples) {

    digitalWrite(2, HIGH);

    audioFile.write( (uint8_t*) audioBuffer, audioSamples*2 );
    audioSamples = 0;

    digitalWrite(2, LOW);
  }

  // Controlla se è trascorso il tempo di registrazione desiderato
  if (millis() - startTime >= recordingDuration) {
    
    PDM.end();
    
    Serial.println("Recording finished!");

    if(audioFile) {
      audioFile.close();
      Serial.println("Previous audio file closed.");
    }

    // Every 5 seconds, the recording finish and a new one is started, with a new configuration
    Serial.println( "Starting new recording..." );

    // Attempt to create a new audio file with a unique name
    sprintf( fileName, "%03d_%03d.raw", configPtr, audioFileInx++ );
    audioFile = SD.open( fileName, FILE_WRITE );

    if (!audioFile) {
      Serial.println("Error opening new audio file!");
      while (1);
    } else {
      Serial.print("New audio file created: ");
      Serial.println(fileName);
    }

    // Optionally set the gain, defaults to 20
    PDM.setGain(0);
    PDM.setBufferSize( configs[configPtr].pdm_size );
    bufferLen = configs[configPtr].buff_len;

    configPtr = ( configPtr + 1 ) % CONFIG_LEN;

    // Initialize PDM with:
    // - one channel (mono mode)
    // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
    if ( !PDM.begin( PDM_CH, PDM_FS ) ) {
      Serial.println("Failed to start PDM!");
      while (1);
    } else {
      Serial.println("PDM started");
    }

    startTime = millis(); // Reset the start time for the next recording
  }
}


