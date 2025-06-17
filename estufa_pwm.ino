#include <PID_v1_bc.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
//#include <PID_v1.h>

// === OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// === SENSOR DHT22 ===
#define DHTPIN 27
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// === CARTUCHO DE AQUECIMENTO ===
#define CARTUCHO_PIN 23  // controle via digitalWrite

// === PID ===
double temperaturaAtual, saidaPWM;
double temperaturaAlvo = 50.0;
double Kp = 4.0, Ki = 1.0, Kd = 2.0;
PID pid(&temperaturaAtual, &saidaPWM, &temperaturaAlvo, Kp, Ki, Kd, DIRECT);

// === TEMPO DE PWM MANUAL ===
unsigned long pwmPeriod = 10000;  // 10ms = 100Hz
unsigned long ultimaLeitura = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Cartucho como saída
  pinMode(CARTUCHO_PIN, OUTPUT);
  digitalWrite(CARTUCHO_PIN, LOW);

  // PID
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(0, pwmPeriod);  // saída em microssegundos

  // Display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Erro ao iniciar o display OLED"));
    while (1)
      ;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Ler temperatura a cada 1s
  if (millis() - ultimaLeitura >= 1000) {
    ultimaLeitura = millis();

    float temp = dht.readTemperature();
    float umid = dht.readHumidity();

    if (!isnan(temp)) {
      temperaturaAtual = temp;
      if (temperaturaAtual < 45.0) {
        saidaPWM = pwmPeriod;  // 100% ligado
      } else {
        pid.Compute();  // só entra no PID se estiver acima de 45°C
      }
    }

    // Debug serial
    Serial.print("Temp: ");
    Serial.print(temperaturaAtual);
    Serial.print(" C | PWM: ");
    Serial.print(saidaPWM);
    Serial.print(" us | Umid: ");
    Serial.println(umid);

    // Display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print("T: ");
    display.print(temperaturaAtual, 1);
    display.println(" C");

    display.print("H: ");
    display.print(umid, 1);
    display.println(" %");

    display.setCursor(0, 40);
    display.print("P: ");
    display.print((int)(saidaPWM * 100.0 / pwmPeriod));
    display.println(" %");

    display.display();
  }

  // PWM manual
  digitalWrite(CARTUCHO_PIN, HIGH);
  delayMicroseconds((int)saidaPWM);
  digitalWrite(CARTUCHO_PIN, LOW);
  delayMicroseconds((int)(pwmPeriod - saidaPWM));
}
