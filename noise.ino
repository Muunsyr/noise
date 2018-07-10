const int sampleWindow = 20; // Sample window width in mS (50 mS = 20Hz)
const int SIZE = 10;
unsigned int sample;
int dig1; // low order
int dig2; // high order
bool dig = false;
int index;
long sum;
int vals[SIZE];

void setup() {
  pinMode(8, OUTPUT);
  DDRD = DDRD | B11111100;
  Serial.begin(115200);
  index = 0;
  sum = 0;
} 
 
void loop() 
{
  sum -= vals[index];
  vals[index] = readPeakToPeak();
  sum += vals[index];
  index++;
  if (index == SIZE) {
    index = 0;
    calcDigits(sum/SIZE);
    Serial.println(sum/SIZE);
  }

  display(dig);
  dig = !dig;
}

void display(bool lowDig) {
  byte portd;
  int pin8 = HIGH;
  if (lowDig) {
    pin8 = LOW;
    portd = dig1 << 3;
  }
  else {
    portd = dig2 << 3;
  }
  digitalWrite(8, pin8);
  portd = portd | B00000100;
  PORTD = (PIND & B00000011) | (portd & B11111100);
}

void calcDigits(int peakToPeak) {
  if (peakToPeak > 99) {
    dig1 = 9;
    dig2 = 9;
  } else if (peakToPeak > 9) {
    dig1 = peakToPeak % 10;
    dig2 = peakToPeak / 10;
  } else {
    dig1 = peakToPeak;
    dig2 = 0;
  }
}

int readPeakToPeak() {
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
 
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(0);
    if (sample < 1024) { // toss out spurious readings
      if (sample > signalMax) {
         signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin) {
         signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

  return peakToPeak;
}
