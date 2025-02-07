#include <stdio.h>
#include "pico/stdlib.h"
#include "inc/font.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include <ctype.h>
#include "ws2818b.pio.h"
#include "inc/ssd1306.h"
#include <stdio.h>

//Define os leds e os botões
#define led_red 13
#define led_green 11
#define led_blue 12
#define button_a 5
#define button_b 6
bool led_state = false;

//Define os pinos da matriz
const uint32_t LED_PIN = 7;
#define DIGIT_SIZE 5
#define LED_COUNT 25
int number_matriz = 0;

//Define os pinos do display
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

//Variaveis para tratar os botões
const uint32_t debounce_time_ms = 200;

volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;
absolute_time_t last_interrupt_time = 0;

static void gpio_irq_handler(uint gpio, uint32_t events);


/* ===========================================(inicio) Funções para tratar da matriz de LED ===========================================*/

// Matriz tridimensional para armazenar os números 0 a 9
int digits[10][DIGIT_SIZE][DIGIT_SIZE][3] = {
    { // Número 0
        {{0, 0, 0}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {0, 0, 0}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{0, 0, 0}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {0, 0, 0}}
    },
    { // Número 1
        {{0, 0, 0}, {10, 10, 10}, {10, 10, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {10, 10, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {10, 10, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {10, 10, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {10, 10, 10}, {0, 0, 0}, {0, 0, 0}}
    },
    { /* Número 2 */ 
        {{0, 0, 0}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {0, 0, 0}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {10, 10, 10}, {0, 0, 0}, {0, 0, 0}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}}
    }, 
    { /* Número 3 */ 
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{0, 0, 0}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}}
        
    }, 
    { /* Número 4 */ 
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}}
    
    }, 
    { /* Número 5 */ 
        {{0, 0, 0}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}}
    
    }, 
    { /* Número 6 */ 
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}}
    
    }, 
    { /* Número 7 */ 
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {10, 10, 10}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
    
    }, 
    { /* Número 8 */ 
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{0, 0, 0}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {0, 0, 0}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}}
    
    }, 
    { /* Número 9 */ 
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{10, 10, 10}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {10, 10, 10}},
        {{10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}, {10, 10, 10}}
    
    }, 
};


// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

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


void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

//Limpa o buffer
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

//Escreve os dados no buffer
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// Função para converter a posição do matriz para uma posição do vetor.
int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return 24-(y * 5 + x); 
    } else {
        return 24-(y * 5 + (4 - x)); 
    }
}

// Atualizar os LEDs com os valores ajustados
void brightness(int matriz[5][5][3]){

    // Atualizar os LEDs com os valores ajustados
    for (int linha = 0; linha < 5; linha++) {
        for (int coluna = 0; coluna < 5; coluna++) {
            int posicao = getIndex(linha, coluna);
            npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
        }
    }

    npWrite();

}

void numberMatrix(char c){
    switch (c)
            {
            case '0':
                brightness(digits[0]);
                break;
            
            case '1':
                brightness(digits[1]);
                break;
            
            case '2':
                brightness(digits[2]);
                break;
            
            case '3':
                brightness(digits[3]);
                break;
            
            case '4':
                brightness(digits[4]);
                break;
            
            case '5':
                brightness(digits[5]);
                break;
            
            case '6':
                brightness(digits[6]);
                break;
            
            case '7':
                brightness(digits[7]);
                break;
            
            case '8':
                brightness(digits[8]);
                break;
            
            case '9':
                brightness(digits[9]);
                break;
            
            default:
                break;
            }
}

/* ===========================================(Fim) Funções para tratar da matriz de LED ===========================================*/


//Função de interrupção
void gpio_irq_handler(uint gpio, uint32_t events) {
    absolute_time_t now = get_absolute_time();
    if (absolute_time_diff_us(last_interrupt_time, now) < debounce_time_ms * 1000)
        return;

    last_interrupt_time = now;
    if (gpio == button_a)
        button_a_pressed = true;
    else if (gpio == button_b)
        button_b_pressed = true;
}



int main()
{
    stdio_init_all();

    gpio_init(led_red);
    gpio_set_dir(led_red, GPIO_OUT);

    gpio_init(led_green);
    gpio_set_dir(led_green, GPIO_OUT);
    gpio_put(led_green, 0);

    gpio_init(led_blue);
    gpio_set_dir(led_blue, GPIO_OUT);
    gpio_put(led_blue, 0);

    gpio_init(button_a);
    gpio_set_dir(button_a, GPIO_IN);
    gpio_pull_up(button_a);

    gpio_init(button_b);
    gpio_set_dir(button_b, GPIO_IN);
    gpio_pull_up(button_b);

    npInit(LED_PIN);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    gpio_set_irq_enabled_with_callback(button_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    bool cor = true;

    

    while (true) {
        printf("Está no inicio do While\n");
        if (stdio_usb_connected()) {

            printf("Esperando o caracter ser lido\n");
            char c = getchar();  // Lê o caractere digitado
            printf("Caractere lido: %c\n", c);
            npClear();

            if(isdigit(c)){
                numberMatrix(c);
            }
            
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_char(&ssd, c, 20, 30); // Desenha o caractere no display
            ssd1306_send_data(&ssd); // Atualiza o display
            printf("Atualizou o OLED\n");

        }

        if (button_a_pressed){
            button_a_pressed = false; 
            printf("Botão A pressionado\n");
            led_state = gpio_get(led_green);
            gpio_put(led_green, !led_state);

            ssd1306_fill(&ssd, false);
            if(gpio_get(led_green) == true){
                ssd1306_draw_string(&ssd, "LED   VERDE", 20, 10); // Desenha uma string
                ssd1306_draw_string(&ssd, "ACESSO", 36, 30); // Desenha uma string
            } else {
                ssd1306_draw_string(&ssd, "LED   VERDE", 20, 10); // Desenha uma string
                ssd1306_draw_string(&ssd, "APAGADO", 33, 30); // Desenha uma string
            }

            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(1000);


        }
        if (button_b_pressed){
            button_b_pressed = false;
            printf("Botão B pressionado\n");
            led_state = gpio_get(led_blue);
            gpio_put(led_blue, !led_state);

            ssd1306_fill(&ssd, false);
            if(gpio_get(led_blue) == true){
                ssd1306_draw_string(&ssd, "LED   AZUL", 20, 10); // Desenha uma string
                ssd1306_draw_string(&ssd, "ACESSO", 33, 30); // Desenha uma string

            } else {
                ssd1306_draw_string(&ssd, "LED   AZUL", 20, 10); // Desenha uma string
                ssd1306_draw_string(&ssd, "APAGADO", 33, 30); // Desenha uma string
            }

            ssd1306_send_data(&ssd); // Atualiza o display
            sleep_ms(1000);
            
        }

        cor = !cor;
        // Atualiza o conteúdo do display com animações
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
        ssd1306_send_data(&ssd); // Atualiza o display
        sleep_ms(1000);
    }
}
