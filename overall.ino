//#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL);//do not connect to cloud
//#else
//SYSTEM_MODE(AUTOMATIC);//connect to cloud
//#endif

char ssid[] = "minote";
//char ssid[] = "MINI";
char password[] = "91294371";
char server[] = "www.thingspeak.com"; 

TCPClient client;
TCPClient client2;
TCPClient client3;
TCPClient client4;

int relaypin = D0;
int ledpin = A0;
int ldr = A1;
int pir = D3;
int readnum =0;
int state=0;

unsigned long lastConnectionTime = 1000;            
const unsigned long postingInterval = 10000;  ///getled
unsigned long lastConnectionTime2 = 0;            
const unsigned long postingInterval2 = 10000; //getstatus
unsigned long lastConnectionTime3 = 0;           
const unsigned long postingInterval3 = 30000;  //postdata
unsigned long lastConnectionTime4 = 0;           
const unsigned long postingInterval4 = 10000;  //postdata
unsigned long readcounter=0;
unsigned long ledon=0;
unsigned long ledadder = 0;

void getStatus();
void getLed();
void postData();
void getThreshold();

void readStatus();
void readLed();
void readThreshold();
void diagData();
void packageData();
void readSensors();
void turnonled();
void increaseled();

int Threshold;
int currentled=0;
String c="";
int startline=0;
String c2="";
int startline2=0;
String c4="";
int startline4=0;

int motion;
int avglightintensity;
int lightintensity;
int motioncounter;
int timestamp;
int wifistrength;
int Status;
int LedStrength;
int ledthreshold = 1000;
String avglightintensityStr;
String motioncounterStr;
String dataStr;
String timestampStr;
String wifistrengthStr;

void readLed(){
    if (client.available()) {
      char charIn = client.read();
      if (startline==1){
          c = c + charIn;}
        if (charIn=='\n'){
          charIn = client.read();
          if (charIn=='\r'){
            charIn=client.read();
            if (charIn=='\n'){
              startline=1;}}}}
    if (c.toInt()>0){
    LedStrength = c.toInt()-1;};}

void readStatus(){
    if (client2.available()) {
      char charIn2 = client2.read();
      if (startline2==1){
          c2 = c2 + charIn2;}
        if (charIn2=='\n'){
          charIn2 = client2.read();
          if (charIn2=='\r'){
            charIn2=client2.read();
            if (charIn2=='\n'){
              startline2=1;}}}}
    if (c2.toInt()>0){
    Status = c2.toInt()-1;};}

void readThreshold(){
    if (client4.available()) {
      char charIn4 = client4.read();
      if (startline4==1){
          c4 = c4 + charIn4;}
        if (charIn4=='\n'){
          charIn4 = client4.read();
          if (charIn4=='\r'){
            charIn4=client4.read();
            if (charIn4=='\n'){
              startline4=1;}}}}
    if (c4.toInt()>0){
    Threshold = c4.toInt()-1;};}

void getLed() {
  client.stop();
  client.flush();
  if (client.connect(server, 80)) {
    Serial.println("connected to server getLed()");
    client.println("GET /talkbacks/8751/commands/1655810?api_key=6KTH3KKTHWXXINJC HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();
    lastConnectionTime = millis();}
  else {
    Serial.println("connection failed"); }
  c="";
  startline=0;}    

void getStatus() {
  client2.stop();
  client2.flush();
  if (client2.connect(server, 80)) {
    Serial.println("connected to server getStatus()");
    client2.println("GET /talkbacks/8751/commands/1987333?api_key=6KTH3KKTHWXXINJC HTTP/1.1");
    client2.println("Host: api.thingspeak.com");
    client2.println("Connection: close");
    client2.println();
    lastConnectionTime2 = millis();}
  else {
    Serial.println("connection failed"); }
  c2="";
  startline2=0;}    

void getThreshold() {
  client4.stop();
  client4.flush();
  if (client4.connect(server, 80)) {
    Serial.println("connected to server getThreshold()");
    client4.println("GET /talkbacks/8751/commands/2017116?api_key=6KTH3KKTHWXXINJC HTTP/1.1");
    client4.println("Host: api.thingspeak.com");
    client4.println("Connection: close");
    client4.println();
    lastConnectionTime4 = millis();}
  else {
    Serial.println("connection failed"); }
  c4="";
  startline4=0;}   

void postData() {
    client3.stop();
  client3.flush();
  packageData();
  //diagData();
  if (client3.connect(server, 80)) {
    Serial.println(dataStr);
    client3.println(dataStr);
     ///client3.println("GET /update?api_key=H6KY13TB8GB2B543&field5=100 HTTP/1.1");
    client3.println("Host: api.thingspeak.com");
    client3.println("User-Agent: Mozilla/5.0");
    client3.println("Connection: close");
    client3.println();
    lastConnectionTime3 = millis();
  }
  else {
    Serial.println("connection failed");
  }
  }

void packageData(){
  timestamp = Time.now();
  wifistrength = WiFi.RSSI();
  timestampStr = String(timestamp);
  wifistrengthStr = String(wifistrength);
  avglightintensityStr =String(avglightintensity);
  motioncounterStr = String(motioncounter);
  dataStr = "GET /update?api_key=H6KY13TB8GB2B543&field1="+timestampStr+"&field2="+wifistrengthStr+"&field3="+motioncounterStr+"&field4="+avglightintensityStr+"&field5="+String(currentled)+" HTTP/1.1";
}

void diagData(){
  lightintensity=100;
  avglightintensityStr =String(avglightintensity);
  dataStr = "GET /update?api_key=H6KY13TB8GB2B543&field5="+avglightintensityStr+" HTTP/1.1";
  Serial.println(dataStr);
}

void operation(){
  //normal operating code
}

void setup(){
  Serial.begin(115200);
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid); 
  WiFi.on();
  WiFi.setCredentials(ssid,password);
  WiFi.connect();
  Time.zone(+8);
  getStatus();
  getLed();
  getThreshold();
  Serial.println("start http");
  pinMode(relaypin,OUTPUT);
  pinMode(ledpin,OUTPUT);
  pinMode(ldr, INPUT);
  pinMode(pir, INPUT);
}

void loop() {
  readStatus();
  readLed();
  readThreshold();
  if (Status==0){ //offlight  
    if (lightintensity<Threshold){
      if(millis()-ledon>20&&currentled<255){
      currentled+=1;
      //Serial.println(currentled);
      turnonled();
      }   
    }
    if (lightintensity>Threshold){
     if(millis()-ledon>20&&currentled>0){
     currentled-=1;
     Serial.println(currentled);
     turnonled();}
      if(currentled==0){
    digitalWrite(relaypin,LOW);
    analogWrite(ledpin, 0);
      } 
    }
  }

  if (Status==1){//onlight
    //relaypinon, control led brightness
    digitalWrite(relaypin,HIGH);
    analogWrite(ledpin,LedStrength);
    if(LedStrength==0){
      analogWrite(relaypin,LOW);
    }
  }

  if (Status==2){//diagnostics mode
    if (Time.second()<=29){
    digitalWrite(relaypin,HIGH);
    analogWrite(ledpin,255);}
    if (Time.second()>=30){
    digitalWrite(relaypin,HIGH);
    analogWrite(ledpin,0);}
    
  }

  if (Status==3){   //motion dectector mode
     if (motion==0&&state==0){
      state=1;
        turnonled();}
        //Serial.println("ledon");
        if (millis()-ledon<10000&&lightintensity<Threshold){
          if (millis()-ledadder>20&&currentled<255){
          currentled+=1;
          increaseled();
          }
        }
       if (millis()-ledon<10000&&lightintensity>Threshold){
          if (millis()-ledadder>20&&currentled>0){
          currentled-=1;
          increaseled();
          }
        }
        if (millis()-ledon >10000){
        //Serial.println("ledoff");
        digitalWrite(relaypin,HIGH);
        analogWrite(ledpin,0);
        currentled=0;
        state=0;}
        }
   
  if (Status==4){
    if (Time.hour()>=19&&Time.hour()<=7){
      Status=0;
    }
    else{
      Status=3;
    }
    if(Time.hour()==3&&Time.minute()==0&&Time.second()==00){
      Status=2;
    }
  }
  
  //millis() - lastConnectionTime > postingInterval
  //if (Time.second()==0||Time.second()==10||Time.second()==20|Time.second()==30||Time.second()==40||Time.second()==50) {
    if (millis() - lastConnectionTime > postingInterval){
    getLed();
    Serial.println("LED:");
    Serial.println(LedStrength);
    Serial.println(Time.timeStr());}
  //}
    if (Time.second()==0||Time.second()==10||Time.second()==20|Time.second()==30||Time.second()==40||Time.second()==50) {
      if (millis() - lastConnectionTime2 > postingInterval2){
    getStatus();
    Serial.println("Status:");
    Serial.println(Status);
    Serial.println(Time.timeStr());}
  }
  if (Time.second()==0||Time.second()==10||Time.second()==20|Time.second()==30||Time.second()==40||Time.second()==50) {
      if (millis() - lastConnectionTime4 > postingInterval4){
    getThreshold();
    Serial.println("Threshold:");
    Serial.println(Threshold);
    Serial.println(Time.timeStr());}
  }
    if (Time.second()==0||Time.second()==30){
      if (millis() - lastConnectionTime3 > postingInterval3){
      avglightintensity = avglightintensity/readnum;
      postData();
      Serial.println(Time.timeStr());
      readnum=0;
      motioncounter=0;
      avglightintensity=0;}
  }
  if (millis() - readcounter > 100){
    readSensors();
  Serial.println(motion);
//    Serial.println(lightintensity);
//    Serial.println(motioncounter);
//    Serial.println(avglightintensity);
  }
  
}


void readSensors(){
  lightintensity = analogRead(ldr);
  motion = digitalRead(pir);
  if (motion==0){
   motioncounter=motioncounter+1;}
  avglightintensity=avglightintensity+lightintensity;
  readnum=readnum+1;    
  readcounter = millis();
}


void turnonled(){
  digitalWrite(relaypin,HIGH);
  analogWrite(ledpin,currentled);
  ledon = millis();
}

void increaseled(){
  analogWrite(ledpin,currentled);
  ledadder=millis();
}

