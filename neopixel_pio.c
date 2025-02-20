/******************************************************************************
 * Programa desenvolvido colaborativamente pelos residentes do Embarcatech
 * U4 - Microcontroladores
 * Tarefa: AS_U4C2O1234A_20_01_2025 - V1
 * Residentes: 
 *  - Lucas Meira de Souza Leite
 *  - Maria Clara Simões de Jesus
 *  - Cauã Gomes Fraga
 *  - Hugo Martins Santana
 *  - Allef Silva Imbassahy
 *  - Josemar Rocha Pedroso  
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

#define buz 20 //Buzzer
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

void buzz() { // Configuração do buzzer
    gpio_init(20);
    gpio_set_dir(20, GPIO_OUT);
    gpio_put(20, 0);
}
void note(uint8_t buzzer, uint16_t notefreq, uint16_t duration_ms){
    int halfc = 1000000 / (2 * notefreq); // Calcula o tempo de espera para cada meio ciclo de onda
    int cycles = (notefreq * duration_ms) / 1000; // Número total de ciclos necessários para a duração

    for (uint8_t i = 0; i < cycles; i++) {
        gpio_put(20, 1);  // Muda o estado para HIGH iniciando 1º parte do ciclo
        sleep_us(halfc); // Aguarda metade do tempo de um ciclo (meio período)
        gpio_put(20, 0);  // Muda o estado para LOW  iniciando 2º parte do ciclo
        sleep_us(halfc); // Aguarda a outra metade do tempo (meio período)
    }
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

void AcenderLEDsVerde() {
    npClear();
    for (int i = 0; i <= 24; ++i){
        npSetLED(i, 0, 127, 0);
    }
    npWrite();
    sleep_ms(1000);
}

void AcenderLEDsBranco(){
    npClear();
    for (int i = 0; i <= 24; ++i){
        npSetLED(i, 51, 51, 51);
    }
    npWrite();
    sleep_ms(1000);
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

void AnimacaoQuadrado() {
    // Definição dos LEDs para o quadrado maior e menor
    const uint8_t quadrado_maior[] = {0, 1, 2, 3, 4, 5, 9, 10, 14, 15, 19, 20, 21, 22, 23, 24};
    const int TAM_QUADRADO_MAIOR = sizeof(quadrado_maior) / sizeof(quadrado_maior[0]);

    const uint8_t quadrado_menor[] = {6, 7, 8, 11, 13, 16, 17, 18};
    const int TAM_QUADRADO_MENOR = sizeof(quadrado_menor) / sizeof(quadrado_menor[0]);

    // Acender o quadrado maior
    npClear();
    for (int i = 0; i < TAM_QUADRADO_MAIOR; ++i) {
        npSetLED(quadrado_maior[i], 0, 0, 255); // Cor azul
    }
    npWrite();
    sleep_ms(1000);

    // Transição gradual para o quadrado menor
    for (int passo = 0; passo <= 3; ++passo) {
        npClear();

        // Apagar LEDs do quadrado maior gradualmente
        for (int i = 0; i < TAM_QUADRADO_MAIOR - (passo * 4); ++i) {
            npSetLED(quadrado_maior[i], 0, 0, 255);
        }

        // Acender LEDs do quadrado menor gradualmente
        for (int i = 0; i < passo * 2 && i < TAM_QUADRADO_MENOR; ++i) {
            npSetLED(quadrado_menor[i], 255, 0, 0); // Cor vermelha
        }

        npWrite();
        sleep_ms(300);
    }

    // Manter o quadrado menor aceso por 1 segundo
    npClear();
    for (int i = 0; i < TAM_QUADRADO_MENOR; ++i) {
        npSetLED(quadrado_menor[i], 255, 0, 0); // Cor vermelha
    }
    npWrite();
    sleep_ms(1000);

    npClear();
    npWrite();
}

void AnimacaoOi() {
    // Definição do LED para "O"
    const uint8_t O_leds[] = {6, 7, 8, 11, 13, 16, 17, 18};
    const int O_count = sizeof(O_leds) / sizeof(O_leds[0]);

    // Definição do LED para "I"
    const uint8_t I_leds[] = {7, 12, 17};
    const int I_count = sizeof(I_leds) / sizeof(I_leds[0]);

    // Animação para o "O" vindo da esquerda
    for (int step = 0; step < O_count; ++step) {
        npClear();
        for (int i = 0; i <= step; ++i) {
            npSetLED(O_leds[i], 255, 0, 255); // Cor verde
        }
        npWrite();
        sleep_ms(200);
    }

    // Piscar o "O" com som grave
    for (int cycle = 0; cycle < 2; ++cycle) {
        npClear();
        npWrite();
        note(buz, 300, 200); // Som grave para o "O"
        sleep_ms(200);

        for (int i = 0; i < O_count; ++i) {
            npSetLED(O_leds[i], 255, 0, 255); // Cor verde
        }
        npWrite();
        sleep_ms(200);
    }

    sleep_ms(1000); // Pausa entre "O" e "I"

    // Animação para o "I" vindo da direita
    for (int step = I_count - 1; step >= 0; --step) {
        npClear();
        for (int i = step; i < I_count; ++i) {
            npSetLED(I_leds[i], 255, 165, 0); // Cor vermelha
        }
        npWrite();
        sleep_ms(200);
    }

    // Piscar o "I" com som agudo
    for (int cycle = 0; cycle < 2; ++cycle) {
        npClear();
        npWrite();
        note(buz, 880, 200); // Som agudo para o "I"
        sleep_ms(200);

        for (int i = 0; i < I_count; ++i) {
            npSetLED(I_leds[i], 255, 165, 0); // Cor vermelha
        }
        npWrite();
        sleep_ms(200);
    }

    sleep_ms(1000); // Pausa final antes de terminar a animação

    npClear();
    npWrite();
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


// Animação cascata para botão 1
void AnimacaoCascata() {
    // Anima LEDs de 0 a 9 em sequência
    for (int cycle = 0; cycle < 3; cycle++) {
        // Cascata descendo (vermelho)
        for (int i = 0; i <= 9; i++) {
            npClear();
            npSetLED(i, 255, 0, 0);
            npWrite();
            sleep_ms(200);
        }
        
        // Cascata subindo (azul)
        for (int i = 9; i >= 0; i--) {
            npClear();
            npSetLED(i, 0, 0, 255);
            npWrite();
            sleep_ms(200);
        }
    }
    npClear();
    npWrite();
}

// Animação onda para botão 4
void AnimacaoOnda() {
    const uint8_t cores[3][3] = {
        {255, 0, 0},    // Vermelho
        {0, 255, 0},    // Verde
        {0, 0, 255}     // Azul
    };
    
    for (int wave = 0; wave < 3; wave++) {
        // Onda se expandindo do centro
        for (int i = 0; i <= 4; i++) {
            npClear();
            if (4 - i >= 0) npSetLED(4 - i, cores[wave][0], cores[wave][1], cores[wave][2]);
            if (4 + i <= 9) npSetLED(4 + i, cores[wave][0], cores[wave][1], cores[wave][2]);
            npWrite();
            sleep_ms(150);
        }
        
        // Onda se contraindo para o centro
        for (int i = 4; i >= 0; i--) {
            npClear();
            if (4 - i >= 0) npSetLED(4 - i, cores[wave][0], cores[wave][1], cores[wave][2]);
            if (4 + i <= 9) npSetLED(4 + i, cores[wave][0], cores[wave][1], cores[wave][2]);
            npWrite();
            sleep_ms(150);
        }
    }
    npClear();
    npWrite();
}

// Animação pisca-pisca para botão 5
void AnimacaoPiscaPisca() {
    // Define padrões de LEDs pares e ímpares
    for (int cycle = 0; cycle < 5; cycle++) {
        // Acende LEDs pares em verde
        npClear();
        for (int i = 0; i <= 9; i += 2) {
            npSetLED(i, 0, 255, 0);
        }
        npWrite();
        sleep_ms(500);
        
        // Acende LEDs ímpares em roxo
        npClear();
        for (int i = 1; i <= 9; i += 2) {
            npSetLED(i, 255, 0, 255);
        }
        npWrite();
        sleep_ms(500);
    }
    
    // Finaliza com todos os LEDs piscando juntos
    for (int i = 0; i < 3; i++) {
        npClear();
        for (int led = 0; led <= 9; led++) {
            npSetLED(led, 255, 255, 0); // Amarelo
        }
        npWrite();
        sleep_ms(300);
        
        npClear();
        npWrite();
        sleep_ms(300);
    }
}


void AnimacaoRosto() {
    // Arrays que definem os LEDs para cada parte do rosto
    const uint8_t rosto_piscando[] = {1, 2, 3, 5, 9, 16, 18};
    const uint8_t olhos[] = {16, 18};
    const uint8_t boca_feliz[] = {1, 2, 3, 5, 9};
    const uint8_t boca_neutra[] = {6, 7, 8};
    const uint8_t rosto_triste[] = {0, 4, 6, 7, 8, 15, 19, 21, 23};
    const uint8_t rosto_raivoso[] = {0, 4, 6, 7, 8, 20, 21, 23, 24};

    const int ROSTO_PISCANDO = sizeof(rosto_piscando) / sizeof(rosto_piscando[0]);
    const int OLHOS = sizeof(olhos) / sizeof(olhos[0]);
    const int BOCA_FELIZ = sizeof(boca_feliz) / sizeof(boca_feliz[0]);
    const int BOCA_NEUTRA = sizeof(boca_neutra) / sizeof(boca_neutra[0]);
    const int ROSTO_TRISTE = sizeof(rosto_triste) / sizeof(rosto_triste[0]);
    const int ROSTO_RAIVOSO = sizeof(rosto_raivoso) / sizeof(rosto_raivoso[0]);

    // Frame 1: Rosto Feliz
    npClear();
    for (int i = 0; i < BOCA_FELIZ; i++) {
        npSetLED(boca_feliz[i], 0, 0, 255); 
    }
    for (int i = 0; i < OLHOS; i++) {
        npSetLED(olhos[i], 0, 255, 0); 
    }
    npWrite();
    sleep_ms(1000);

    // Frame 2: Piscando
    npClear();
    for (int i = 0; i < ROSTO_PISCANDO; i++) {
        npSetLED(rosto_piscando[i], 0, 0, 255);
    }
    npWrite();
    sleep_ms(1000);

    // Frame 3: Rosto Feliz
    npClear();
    for (int i = 0; i < BOCA_FELIZ; i++) {
        npSetLED(boca_feliz[i], 0, 0, 255); 
    }
    for (int i = 0; i < OLHOS; i++) {
        npSetLED(olhos[i], 0, 255, 0); 
    }
    npWrite();
    sleep_ms(1000);

    // Frame 4: Rosto Nêutro
    npClear();
    for (int i = 0; i < OLHOS; i++){
        npSetLED(olhos[i], 0, 255, 0);
    }
    for (int i = 0; i < BOCA_NEUTRA; i++ ){
        npSetLED(boca_neutra[i], 0, 0, 255);
    }
    npWrite();
    sleep_ms(1000);

    // Frame 5: Rosto Triste
    npClear();
    for (int i = 0; i < ROSTO_TRISTE; i++) {
        npSetLED(rosto_triste[i], 0, 0, 255); 
    }
    for (int i = 0; i < OLHOS; i++) {
        npSetLED(olhos[i], 0, 255, 0);
    }
    npWrite();
    sleep_ms(1000);

    // Frame 6: Rosto Nêutro
    npClear();
    for (int i = 0; i < OLHOS; i++){
        npSetLED(olhos[i], 0, 255, 0);
    }
    for (int i = 0; i < BOCA_NEUTRA; i++ ){
        npSetLED(boca_neutra[i], 0, 0, 255);
    }
    npWrite();
    sleep_ms(1000);

    // Frame 7: Rosto Raivoso (olhos vermelhos)
    npClear();
    for (int i = 0; i < ROSTO_RAIVOSO; i++) {
        npSetLED(rosto_raivoso[i], 0, 0, 255); 
    }
    for (int i = 0; i < OLHOS; i++) {
        npSetLED(olhos[i], 255, 0, 0); 
    }
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
    buzz();

    // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);

    while (true) {
        sleep_ms(100);
        char key = read_keypad();
        if (key != '\0') {
            switch (key) {
                case '0':
                    AnimacaoOi();
                    strcpy(AcaoRealizada, "Animação 'Oi'");
                    break;
                case '1':
                    AnimacaoCascata();
                    strcpy(AcaoRealizada, "Animação Cascata");
                    break;
                case '2':
                    AnimacaoQuadrado();
                    strcpy(AcaoRealizada, "Animação Quadrado");
                    break;
                case '3':
                    AnimacaoRosto();
                    strcpy(AcaoRealizada, "Animação rosto RGB");
                    break;
                case '4':
                    AnimacaoOnda();
                    strcpy(AcaoRealizada, "Animação Onda");
                    break;
                case '5':
                    AnimacaoPiscaPisca();
                    strcpy(AcaoRealizada, "Animação Pisca-Pisca");
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
                    AcenderLEDsVerde();                    
                    strcpy(AcaoRealizada, "Acender LEDS em VERDE (50%%).");
                    break;
                case '#':
                    AcenderLEDsBranco();
                    strcpy(AcaoRealizada, "Acionar LEDS em BRANCO (20%%).");                    
                    break;                
            }
            printf("Tecla pressionada: %c\n", key);
            printf("Ação realizada: %s\n", AcaoRealizada);                   
        }
    }
    return 0;
}
