# README - Projeto Raspberry Pi Pico com Matriz de LEDs e Display OLED

## Descrição
Este projeto implementa uma matriz de LEDs controlada por um Raspberry Pi Pico, utilizando um display OLED SSD1306 para exibição de informações. O sistema também inclui botões para interação e LEDs RGB para sinalização.

## Hardware Utilizado
- **Raspberry Pi Pico**
- **Matriz de LEDs** (5x5)
- **Display OLED SSD1306** (conectado via I2C)
- **LEDs RGB** (Vermelho, Verde e Azul)
- **Botões de entrada** (2 botões para controle)

## Bibliotecas Necessárias
O código faz uso das seguintes bibliotecas:
- `pico/stdlib.h`: Biblioteca padrão do Raspberry Pi Pico
- `hardware/gpio.h`: Controle de GPIO
- `hardware/pio.h`: Interface de comunicação PIO
- `hardware/clocks.h`: Controle de clocks do microcontrolador
- `ws2818b.pio.h`: Controle de LEDs endereçáveis
- `ssd1306.h`: Biblioteca para controle do display OLED

## Configuração de Pinos
- **LEDs RGB**: 
  - Verde: Pino 11
  - Azul: Pino 12
- **Botões**:
  - Botão A: Pino 5
  - Botão B: Pino 6
- **Matriz de LEDs**:
  - Pino de controle: 7
- **Display OLED** (I2C):
  - SDA: Pino 14
  - SCL: Pino 15
  - Endereço I2C: `0x3C`

## Funcionalidades
- Exibição de números de 0 a 9 na matriz de LEDs quando digitados no monitor serial
- Controle via botões para alternar entre os estados do LED
- Implementação de debounce e inturrupções nos botões
- Informações do estado dos LEDs no monitor serial
- Exibição do caractere digitado no display OLED (maiusculo, minusculo ou número)
- Configuração inicial do display para limpar a tela antes da exibição
- Implementação de buffer para atualização do display OLED
- Uso de comunicação I2C para escrita otimizada no display

## Compilação e Upload
1. Instale o SDK do Raspberry Pi Pico.
2. Compile o código utilizando `cmake` e `make`.
3. Carregue o binário para o Pico via USB.

