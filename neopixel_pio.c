/******************************************************************************
 * Programa desenvolvido colaborativamente pelos residentes do Embarcatech
 * U4 - Microcontroladores
 * Tarefa: AS_U4C2O1234A_20_01_2025 - V1
 * Residentes: 
 *  - Lucas Meira de Souza Leite
 *  - Maria Clara Simões de Jesus
 *  -  
 *  -  
 *  -   
 *  -   
 *  -   
 *  -    
 *  -   
 *******************************************************************************/ 

/*
    Programa para animações na matriz de LEDs 5x5 com interação pelo teclado matricial
*/

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"


#define ROWS 4 //Definição do número de linhas do teclado
#define COLS 4 //Definição do numero de colunas do teclado
#define LOSANGO_G 13 //Definição do numero de leds p/ formar o losango grande
#define LOSANGO_P 5 //Definição do numero de leds p/ formar o losango pequeno

#define LED_COUNT 25 //Definição do número de LEDs da Matriz 5x5
#define LED_PIN 9 //Porta da Matriz 5x5

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B;
};  // Três valores de 8-bits compõem um pixel.

typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

const uint8_t rows_pins[ROWS] = {8, 7, 6, 5};
const uint8_t cols_pins[COLS] = {4, 3, 2, 28};

const uint8_t losango_leds_g[LOSANGO_G] = {2, 6, 7, 8, 10, 11, 12, 13, 14, 16, 17, 18, 22};
const uint8_t losango_leds_p[LOSANGO_P] = {7,11, 12, 13, 17};
char AcaoRealizada[50] = "Nenhuma ação.";

const char key_map[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

void AcenderLEDsAzul(){
    npClear();
    for (int i = 0; i <= 24; ++i){
        npSetLED(i, 0, 0, 255);
    }
    npWrite();
    sleep_ms(1000);
}

void AcenderLEDsVermelho() {
    npClear();
    for (int i = 0; i < LED_COUNT; i++) {
        npSetLED(i, 204, 0, 0);
    }
    npWrite();
}


void AnimacaoLosango(){ 
    // Animação para losango pequeno
    npClear();
    for (int i = 0; i < LOSANGO_P; ++i){
        npSetLED(losango_leds_p[i], 255, 0, 0);
    }
    npWrite();
    
    sleep_ms(1000);
    
    npClear();
    for (int i = 0; i < LOSANGO_P; ++i){
        npSetLED(losango_leds_p[i], 0, 255, 0);
    }
    npWrite();

    sleep_ms(1000);

    npClear();
    for (int i = 0; i < LOSANGO_P; ++i){
        npSetLED(losango_leds_p[i], 0, 0, 255);
    }
    npWrite();

    sleep_ms(1000);

    // Animação para losango grande
    npClear();
    for (int i = 0; i < LOSANGO_G; ++i){
        npSetLED(losango_leds_g[i], 255, 0, 0);
    }
    npWrite();
    
    sleep_ms(1000);
    
    npClear();
    for (int i = 0; i < LOSANGO_G; ++i){
        npSetLED(losango_leds_g[i], 0, 255, 0);
    }
    npWrite();

    sleep_ms(1000);

    npClear();
    for (int i = 0; i < LOSANGO_G; ++i){
        npSetLED(losango_leds_g[i], 0, 0, 255);
    }
    npWrite();

    sleep_ms(1000);
}

void AnimacaoSeta() {
    // LEDs que compõem a seta na matriz 5x5
    const uint8_t seta_leds[] = {2, 6, 7, 8, 12, 17, 22};
    const int SETA_LEDS = sizeof(seta_leds) / sizeof(seta_leds[0]);

    // Iluminação progressiva (efeito de expansão)
    for (int step = 0; step < SETA_LEDS; ++step) {
        npClear();
        for (int i = 0; i <= step; ++i) {
            npSetLED(seta_leds[i], 0, 255, 0); // Cor verde
        }
        npWrite();
        sleep_ms(300); // Pausa entre os passos
    }

    // Efeito pulsante (após expansão completa)
    for (int cycle = 0; cycle < 3; ++cycle) {
        npClear();
        for (int i = 0; i < SETA_LEDS; ++i) {
            npSetLED(seta_leds[i], 255, 255, 0); // Cor amarela
        }
        npWrite();
        sleep_ms(500);

        npClear();
        for (int i = 0; i < SETA_LEDS; ++i) {
            npSetLED(seta_leds[i], 0, 255, 255); // Cor ciano
        }
        npWrite();
        sleep_ms(500);
    }
    npClear();
    npWrite();

}



void setup() {
    for (int i = 0; i < ROWS; i++) {
        gpio_init(rows_pins[i]);
        gpio_set_dir(rows_pins[i], GPIO_OUT);
        gpio_put(rows_pins[i], 0);
    }
    for (int i = 0; i < COLS; i++) {
        gpio_init(cols_pins[i]);
        gpio_set_dir(cols_pins[i], GPIO_IN);
        gpio_pull_down(cols_pins[i]);
    }    
}

char read_keypad() {
    for (int row = 0; row < ROWS; row++) {
        gpio_put(rows_pins[row], 1);
        sleep_us(10); 

        for (int col = 0; col < COLS; col++) {
            if (gpio_get(cols_pins[col])) {
                gpio_put(rows_pins[row], 0); 
                return key_map[row][col];  
            }
        }

        gpio_put(rows_pins[row], 0);
    }
    return '\0';
}

void ApagarTodosLEDs() {
    npClear();
    npWrite();
}


int main() {
    stdio_init_all();
    setup();     

    // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);

    while (true) {
        sleep_ms(100);
        char key = read_keypad();
        if (key != '\0') {
            switch (key) {
                case '0':
                    
                    strcpy(AcaoRealizada, "");
                    break;
                case '1':
                    
                    strcpy(AcaoRealizada, "");
                    break;
                case '2':
                    
                    strcpy(AcaoRealizada, "");
                    break;
                case '3':
                    
                    strcpy(AcaoRealizada, "Animação rosto RGB");
                    break;
                case '4':
                    
                    strcpy(AcaoRealizada, "");
                    break;
                case '5':
                    
                    strcpy(AcaoRealizada, "");
                    break;
                case '6':
                    
                    strcpy(AcaoRealizada, "");
                    break;
                case '7':
                    AnimacaoSeta();
                    strcpy(AcaoRealizada, "Animação seta RGB.");
                    break;

                case '8':
                    AnimacaoLosango();                    
                    
                    strcpy(AcaoRealizada, "Animação losango RGB.");
                    break;
                case '9':
                    
                    strcpy(AcaoRealizada, "");
                    break;
                case 'A':
                    
                    ApagarTodosLEDs();
                    strcpy(AcaoRealizada, "Apagar LEDS.");
                    break;
                case 'B':
                    AcenderLEDsAzul();

                    strcpy(AcaoRealizada, "Acender LEDs em AZUL (100%%)");
                    break;
                case 'C':                    
                    AcenderLEDsVermelho();
                    strcpy(AcaoRealizada, "Acender LEDs em VERMELHO (80%%).");
                    break;
                case 'D':
                                        
                    strcpy(AcaoRealizada, "Acender LEDS em VERDE (50%%).");
                    break;
                case '#':
                    
                    strcpy(AcaoRealizada, "Acionar LEDS em BRANCO (20%%).");                    
                    break;                
            }
            printf("Tecla pressionada: %c\n", key);
            printf("Ação realizada: %s\n", AcaoRealizada);                   
        }
    }
    return 0;
}
