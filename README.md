# Projeto: Animações na Matriz de LEDs 5x5 com Interação via Teclado Matricial

## Descrição do Projeto
Este projeto implementa um sistema para controlar uma matriz de LEDs 5x5, possibilitando criar animações interativas com a entrada de um teclado matricial 4x4. Ele também inclui funcionalidades adicionais, como controle de um buzzer para gerar sons. O programa é desenvolvido para a plataforma RP2040 (Raspberry Pi Pico), utilizando as capacidades da máquina PIO para controlar LEDs endereçáveis do tipo WS2812B.

## Objetivo
O objetivo principal é criar um sistema interativo para:
- Gerar animações visuais na matriz de LEDs 5x5.
- Controlar as animações por meio de um teclado matricial.
- Produzir sons usando um buzzer para complementar as interações visuais.

## Funcionalidades
1. **Controle de LEDs:**
   - Cada LED da matriz pode ser controlado individualmente, com configuração personalizada de cores no formato RGB.
   - Possibilidade de limpar a matriz ou acender padrões pré-definidos.

2. **Teclado Matricial:**
   - Reconhecimento de entradas de um teclado matricial 4x4.
   - Mapeamento de teclas com ações específicas (ex.: animações, alteração de cores).

3. **Buzzer:**
   - Geração de sons com frequência e duração configuráveis.
   - Sons podem ser utilizados para feedback de interações ou efeitos sonoros adicionais.

## Estrutura do Código
O código é dividido em várias seções principais:

1. **Definições e Estruturas:**
   - Configuração de pinos e constantes para a matriz de LEDs e teclado matricial.
   - Estrutura `pixel_t` para representar cada LED em formato RGB.

2. **Inicialização do Sistema:**
   - Configuração da máquina PIO para controlar LEDs WS2812B.
   - Inicialização de pinos para o teclado matricial e buzzer.

3. **Funções Principais:**
   - `npInit`: Inicializa a máquina PIO e configura o buffer de LEDs.
   - `npSetLED`: Define a cor de um LED específico na matriz.
   - `npClear`: Limpa o buffer da matriz de LEDs.
   - `npWrite`: Envia os dados do buffer para a matriz de LEDs.
   - `note`: Gera sons no buzzer com frequência e duração configuráveis.

4. **Mapeamento do Teclado:**
   - Associação de teclas do teclado matricial a animações e ações específicas, como acender padrões de LEDs.

5. **Animações Predefinidas:**
   - Implementação de padrões como losangos grandes e pequenos na matriz.

## Requisitos de Hardware
- **Raspberry Pi Pico (RP2040).**
- **Matriz de LEDs WS2812B (5x5):** Controlada pelo pino GPIO 9.
- **Teclado Matricial 4x4:**
  - Linhas conectadas aos pinos GPIO 8, 7, 6, 5.
  - Colunas conectadas aos pinos GPIO 4, 3, 2, 28.
- **Buzzer:** Conectado ao pino GPIO 20.

## Requisitos de Software
- SDK do Raspberry Pi Pico.
- Ambiente de desenvolvimento configurado com suporte a C/C++.
- Biblioteca gerada pelo arquivo `.pio` para controle dos LEDs WS2812B.

## Como Usar
1. **Conecte o Hardware:**
   - Monte o circuito conforme os requisitos de hardware acima.
   - Certifique-se de que todos os componentes estão conectados corretamente.

2. **Compile o Código:**
   - Use o SDK do Raspberry Pi Pico para compilar o código fornecido.

3. **Carregue o Programa:**
   - Envie o arquivo binário gerado para o Raspberry Pi Pico via USB.

4. **Interaja com o Sistema:**
   - Utilize o teclado matricial para acionar diferentes animações e efeitos sonoros.

## Exemplos de Mapeamento do Teclado
| Tecla | Ação                                                                 |
|-------|----------------------------------------------------------------------|
| `1`   | Acende LEDs na cor azul.                                             |
| `2`   | Acende LEDs formando um losango pequeno.                             |
| `3`   | Acende LEDs formando um losango grande.                              |
| `A`   | Limpa a matriz de LEDs.                                              |
| `B`   | Aciona uma animação personalizada.                                   |
| `C`   | Toca um som no buzzer.                                               |
| `*`   | Reseta todas as ações.                                               |

## Contribuidores
Este projeto foi desenvolvido colaborativamente pelos residentes do programa **Embarcatech - U4 Microcontroladores**:

- Lucas Meira de Souza Leite
- Maria Clara Simões de Jesus
- Cauã Gomes Fraga
- Hugo Martins Santana
- Allef Silva Imbassahy
- Josemar Rocha Pedroso
  
Link do vídeo: https://youtu.be/4HK-OcZhJcg

## Licença
Este projeto é disponibilizado sob a licença MIT. Consulte o arquivo `LICENSE` para mais informações.

---

**Nota:** Este README foi projetado para ser autoexplicativo e acessível tanto para iniciantes quanto para especialistas em microcontroladores e sistemas embarcados.
