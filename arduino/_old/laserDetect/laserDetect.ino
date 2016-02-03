// buffer for mean filter
const int BUFFER_LENGTH = 5;
int buffer[BUFFER_LENGTH];
int index = 0;

// detect laser
int threshold = 3;

// for reaction 
unsigned long timeStamp;
boolean reaction = false;
int reactionLength = 1000;

// pin assign
const int cds = 0;
const int led = 13;

void setup() {
  Serial.begin(57600);
  pinMode(led, OUTPUT);

  for(int i=0; i<BUFFER_LENGTH; i++){
    buffer[i] = analogRead(cds);
  }
}

void loop() {
  int raw = analogRead(cds);
  //mean filter
  int smoothedByMeanFilter = smoothByMeanFilter();

  if(!reaction && raw - smoothedByMeanFilter > threshold){
    digitalWrite(led, HIGH);
    Serial.println("light detected");
    timeStamp = millis();
    reaction = true;	
  }

  if(reaction && (timeStamp + reactionLength) < millis()){
    digitalWrite(led, LOW);
    Serial.println("reaction finished");
    reaction = false;
  }

  buffer[index] = raw;
  index = (index + 1) % BUFFER_LENGTH;

  Serial.print(raw);
  Serial.print(' ');
  Serial.println(smoothedByMeanFilter);
  delay(10);
}


int smoothByMeanFilter(){
  long sum = 0;
  for(int i=0; i<BUFFER_LENGTH; i++){
    sum += buffer[i];
  }
  return (int)(sum / BUFFER_LENGTH);
}

