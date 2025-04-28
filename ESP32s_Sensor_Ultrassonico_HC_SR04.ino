/*********************************************************************************************************

                                Projeto Medidor de Distância Ultrasonico 
  Neste projeto vamos usar um Modulo Ultrasonico HC-SR04 conectado a um ESP32s e um display OLED SSD1306 

*********************************************************************************************************/
// Carrega as bibliotecas do projeto
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuração do display
#define SCREEN_WIDTH 128          // Largura da imagem em pixels
#define SCREEN_HEIGHT 64          // Altura da imagem em pixels
#define OLED_RESET     4          // Pino de reset do display
#define SCREEN_ADDRESS 0x3C       // Consulte o manual para saber o endereço

// Instancia o display
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pino de conexão do sensor
const int pingPin = 14;   //GPIO14

void setup() {

  Serial.begin(115200);   // Inicializa o monitor serial

  // Verifica se o display está endereçado corretamente
  if(!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 não alocado."));
    for(;;);                         // Loop infinito  
  }
}

void loop() {
  // Define variáveis para duração do ping e o resultado da distância em polegadas e centímetros:
  long duration, inches, cm;

  // O PING))) é acionado por um pulso ALTO de 2 ou mais microssegundos.
  // Um pulso LOW curto antes, garante um pulso HIGH limpo.
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // O mesmo pino é usado para ler o sinal do PING))): um pulso HIGH,
  // cuja duração é o tempo (em microssegundos) desde o envio até a recepção do seu eco.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // Converte o tempo em distancia
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  // Exibe as informações no monitor serial
  Serial.print(inches);
  Serial.print(" polegadas, ");
  Serial.print(cm);
  Serial.print(" centimetros");
  Serial.println();

  // Exibe as informações no display
  oled.clearDisplay();                // Limpa o buffer do display
  oled.setTextSize(1);                // Define o tamanho da fonte
  oled.setTextColor(SSD1306_WHITE);   // Define a cor do texto
  oled.setCursor(0, 0);               // Posiciona o cursor
  oled.print("Distancia:");           // Exibe o texto entre parenteses
  oled.setCursor(0, 16);
  oled.print(inches);                 // Exibe o conteúdo da variável
  oled.setCursor(28, 16);
  oled.print("polegadas");            
  oled.setCursor(0, 32);
  oled.print(cm);
  oled.setCursor(28, 32);
  oled.print("centimetros");
  oled.display();                     // Exibe as informações no display

  delay(100);                         // Mantem as informações no display por 100 ms antes de atualizar
}

// Distancia em polegadas
long microsecondsToInches(long microseconds) {
  // De acordo com a planilha de dados da Parallax, o PING))) se desloca a uma velocidade de 73.746
  // microssegundos por polegada, desde a saída até o retorno,
  // então dividimos por 2 para obter a distância do obstáculo.
  return microseconds / 74 / 2;
}

// Distancia em centímetros
long microsecondsToCentimeters(long microseconds) {
  // A velocidade do som é 340 m/s ou 29 microssegundos por centímetro.
  // O ping vai e volta, então para encontrar a distância do objeto, dividimos o resultado por 2.
  return microseconds / 29 / 2;
}