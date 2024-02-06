#define BLYNK_TEMPLATE_ID "TMPL6bC8u-vgY"
#define BLYNK_TEMPLATE_NAME "vxl"
#define BLYNK_AUTH_TOKEN "fQnlhAbLmRJRFpmen_9TR5TiCsfSSbaB"
#include <DHT.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp32.h>

// gfx và ssh1306 control led
#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
#define  DHT_PIN 4
#define DHT_TYPE DHT22 
#define Led_Pin 2
#define Heater_Pin 12
#define Relay_Pin 26
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// connect blynk
char ssid[] = "Galaxy A34 5G 8FA2"; // Use the available access point name
 char pass[] = "882002qqq"; // If it doesn't require a password, leave it empty
int button_mode; //  của blynk
int button_hitter;
// char auth[]="PPJEKmVMTveD_YUpZu3RxH4cPqmRiKt6";
double safeHumidity=0;
  double safeTemp=0;
BLYNK_WRITE(V2) {
  safeTemp = param.asDouble(); // Đọc giá trị từ nút V2 trên Blynk

}

BLYNK_WRITE(V3) {
  safeHumidity = param.asDouble(); // Đọc giá trị từ nút V3 trên Blynk
}
BLYNK_WRITE(V4) {
  button_mode=param.asInt();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  display.display();
  delay(2000);
  display.clearDisplay();
    Blynk.begin( BLYNK_AUTH_TOKEN,ssid, pass);
    dht.begin();
    pinMode(Led_Pin, OUTPUT);
    // cấu hình điều khiẻn thôgn qua switch 
    pinMode(Relay_Pin, OUTPUT);
  pinMode(Heater_Pin, INPUT);
  // button_hitter = digitalRead(Heater_Pin);
  

}

void loop() {
  Blynk.run();
  
  double humidity = dht.readHumidity();
  double temperature = dht.readTemperature();
  Serial.println(safeTemp);
    Serial.println(safeHumidity);
   // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
    button_hitter=digitalRead(Heater_Pin); 
    // Serial.println(button_mode);
    // Serial.println(button_hitter);
    // đèn cảnh báo
     if(humidity>safeHumidity||temperature<safeTemp){
      digitalWrite(Led_Pin, HIGH); // turn on
      // send mail khi not safe
      Blynk.logEvent("alert","Please check the furniture in the room, the temperature and humidity are not safe");

  }
  else{
        digitalWrite(Led_Pin, LOW); // turn off

  }

// set up mode
if(button_mode==HIGH){
      Serial.println("Auto Mode");
   if(humidity>safeHumidity||temperature<safeTemp){
        digitalWrite(Relay_Pin, HIGH);
   }else{
    digitalWrite(Relay_Pin,LOW);
   }
}else{
  Serial.println("Manual Mode");
  // if(button_hitter==HIGH){
  //   digitalWrite(Relay_Pin, HIGH);
  // }else{
    digitalWrite(Relay_Pin, LOW);
  // }
}

    
 
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  if (button_mode == HIGH ) {
    display.println("Mode: Auto");
  } else {
    display.println("Mode: Manual");
  }  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.print("Humidity: ");
  display.print(humidity);
  display.println("%");

  display.display();

  // Send temperature and humidity to Blynk virtual pins V0 and V1
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V0, humidity);
  

  delay(2000);
}