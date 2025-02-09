# WLS_Comunica-o_Serial

Este projeto tem como objetivo o controle de uma matriz de LEDs e display OLED em um microcontrolador Raspberry Pi Pico utilizando C. Ele interage com a matriz de LEDs, mostrando números de 0 a 9, e também com um display OLED que exibe texto e status de LEDs. Além disso, há interação com dois botões para acionar LEDs de cores diferentes.

Componentes
Raspberry Pi Pico: Microcontrolador que gerencia o código.
Matriz de LEDs (5x5): Usada para exibir números de 0 a 9.
Display OLED SSD1306 (I2C): Exibe texto e status dos LEDs.
Botões físicos: Controlam o acendimento de LEDs (verde e azul).
Funcionalidades
Matriz de LEDs: Exibe números de 0 a 9 usando uma matriz de LEDs 5x5. Cada número é desenhado com LEDs em uma cor específica.
Controle de LEDs: O sistema controla dois LEDs (verde e azul), podendo acendê-los e apagá-los com a interação do usuário através de dois botões físicos.
Display OLED: O display SSD1306 exibe textos, incluindo mensagens sobre o estado dos LEDs e o número digitado na entrada serial. Ele também exibe o status dos LEDs verde e azul após o pressionamento dos botões.
Leitura de entrada serial: O código lê caracteres digitados no terminal serial e exibe o número correspondente na matriz de LEDs.
Funcionalidade dos Botões
Botão A: Acende o LED verde e atualiza o display OLED para refletir seu estado ("Acesso" ou "Apagado").
Botão B: Acende o LED azul e atualiza o display OLED para refletir seu estado.
Estrutura de Código
Configuração de GPIO: Define os pinos para os LEDs, botões e matriz de LEDs.
Funções de Matriz de LEDs: Contém uma matriz tridimensional que armazena os números de 0 a 9 e define como esses números serão representados.
Funções de Interrupção: Lida com as interrupções dos botões para acionar LEDs.
I2C e SSD1306: Configura o display OLED para exibir textos e status.
Função Principal: Executa as operações de leitura de entradas e controle de LEDs e display.
Como Usar
Conecte a Raspberry Pi Pico à sua máquina.
Compile e carregue o código no Pico.
Acesse o terminal serial para interagir com o programa.
Pressione os botões A ou B para acender os LEDs verde ou azul, respectivamente.
Digite números de 0 a 9 no terminal serial para visualizar o número correspondente na matriz de LEDs.
Requisitos
Raspberry Pi Pico
Matriz de LEDs WS2818B
Display OLED SSD1306
Dois botões físicos
Ambiente de desenvolvimento C com suporte à Raspberry Pi Pico
Bibliotecas
pico/stdlib.h: Biblioteca padrão da Raspberry Pi Pico.
hardware/gpio.h: Biblioteca para controlar GPIOs.
hardware/pio.h: Usada para controle da matriz de LEDs via PIO (Programable I/O).
hardware/clocks.h: Para gerenciar os relógios de hardware.
ws2818b.pio.h: Controle da matriz de LEDs.
ssd1306.h: Controle do display OLED.
Conexões de Hardware
Botão A: Conectado ao pino GPIO 5.
Botão B: Conectado ao pino GPIO 6.
LED Verde: Conectado ao pino GPIO 11.
LED Azul: Conectado ao pino GPIO 12.
Matriz de LEDs: Conectada ao pino GPIO 7.
Display OLED (I2C):
SDA: GPIO 14
SCL: GPIO 15
Notas
O código lida com debouncing para evitar leituras incorretas dos botões.
O display OLED utiliza o protocolo I2C com comunicação a 400 kHz.
A matriz de LEDs usa uma configuração PIO para controlar os LEDs WS2818B.
