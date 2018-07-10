const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println(readPeakToPeak());
}

int readPeakToPeak() {
  unsigned long startMillis= millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
 
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(0);
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;
      }
      else if (sample < signalMin) {
        signalMin = sample;
      }
    }
  }
  return signalMax - signalMin;
}
