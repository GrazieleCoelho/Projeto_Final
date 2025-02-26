#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "joystick.h"
#include "hardware/adc.h"
#include <stdlib.h>  // para a função abs()

// Definição dos LEDs
#define LED_GREEN 11  // LED verde: joystick parado
#define LED_BLUE 12   // LED azul: acionado pelo microfone (excesso de ruído)
#define LED_RED  13   // LED vermelho: movimento no eixo X

// Parâmetros para detecção do movimento do joystick
#define ADC_CENTER    2048
#define ADC_THRESHOLD 200  // ajuste conforme sua aplicação

// Definição do microfone
const uint microfone = 28; // GPIO28 para ADC2 (Microfone)
const uint limiar = 2500;  // Limiar para acionar o LED azul e o alerta de ruído

int main(void) {
    stdio_init_all();

    // Configura os pinos dos LEDs como saída
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    // Configura o I2C para o display SSD1306
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C); // SDA
    gpio_set_function(15, GPIO_FUNC_I2C); // SCL
    gpio_pull_up(14);
    gpio_pull_up(15);

    // Inicializa e configura o display OLED
    ssd1306_t ssd;
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, 0x3C, i2c1);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Inicializa o joystick (pinos 26 e 27, conforme definido na instalação)
    setup_joystick(26, 27);

    // Inicializa o ADC para o microfone
    adc_init();
    adc_gpio_init(microfone);  // Configura GPIO28 como entrada ADC para o microfone

    // Variável de controle para a cor (usada nas funções de desenho)
    bool cor = true;

    while (true) {
        // Leitura dos valores do joystick
        uint16_t x_val = get_joystick_x_value();
        uint16_t y_val = get_joystick_y_value();

        // Calcula a diferença em relação ao centro (para detectar movimento)
        int diff_x = abs((int)x_val - ADC_CENTER);
        int diff_y = abs((int)y_val - ADC_CENTER);

        // Se o joystick estiver parado (dentro do limiar em ambos os eixos)
        if(diff_x < ADC_THRESHOLD && diff_y < ADC_THRESHOLD) {
            gpio_put(LED_GREEN, 1);
            gpio_put(LED_RED, 0);
            // O LED azul será controlado pelo microfone, então não é alterado aqui.
            ssd1306_fill(&ssd, false);
            ssd1306_send_data(&ssd);
        }
        // Se houver movimento predominante no eixo X
        else if(diff_x >= ADC_THRESHOLD && diff_x >= diff_y) {
            gpio_put(LED_GREEN, 0);
            gpio_put(LED_RED, 1);
            // O LED azul permanece sob controle do microfone.
            ssd1306_fill(&ssd, !cor); // Limpa o display
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, " ALERTA", 8, 10);
            ssd1306_draw_string(&ssd, "EXCESSO DE", 20, 30);
            ssd1306_draw_string(&ssd, " POEIRA", 15, 48);
            ssd1306_send_data(&ssd);
        }
        // A leitura do joystick para movimento no eixo Y foi removida,
        // pois o acionamento do LED azul passará a ser feito pelo microfone.

        // Leitura do microfone para detectar excesso de ruído
        adc_select_input(2);           // Seleciona o canal 2 (GPIO28)
        uint16_t mic_value = adc_read(); // Lê o valor do ADC

        if(mic_value > limiar) {
            // Aciona o LED azul e exibe a mensagem de alerta no display
            gpio_put(LED_BLUE, 1);
            gpio_put(LED_GREEN, 0);
            ssd1306_fill(&ssd, !cor); // Limpa o display
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, " ALERTA", 8, 10);
            ssd1306_draw_string(&ssd, "EXCESSO DE", 20, 30);
            ssd1306_draw_string(&ssd, " RUIDO", 15, 48);
            ssd1306_send_data(&ssd);
            sleep_ms(3000); // Mantém o alerta por 3 segundos
            gpio_put(LED_BLUE, 0);
        } else {
            // Garante que o LED azul esteja desligado se não houver ruído excessivo
            gpio_put(LED_BLUE, 0);
        }
        
        sleep_ms(100);
    }
    return 0;
}
