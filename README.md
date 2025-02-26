# Projeto Final: **Monitoramento de Qualidade do Ar e Som utilizando um Raspberry Pi Pico W na BitDogLab**
**EMBARCATECH - UNIDADE 07**

## Desenvolvedora
**Aluna Graziele Coelho de Alencar**

## Links no PDF

## 📖 Descrição do Projeto
Este projeto implementa um **sistema de monitoramento ambiental** utilizando um **joystick** e um **microfone** como dispositivos de entrada. O sistema detecta **movimentação** e **níveis de ruído**, acionando **LEDs RGB indicativos** e exibindo mensagens de alerta em um **display OLED SSD1306**.

O código foi desenvolvido para rodar no **Raspberry Pi Pico W**, com suporte à plataforma **BitDogLab** para testes.

---

## 🛠️ Hardware Utilizado
- **Microcontrolador:** Raspberry Pi Pico W
- **Joystick:** Controle de movimento no eixo X
- **Microfone (KY-038 ou similar):** Captura nível de ruído
- **Display OLED SSD1306 (I2C):** Exibição de alertas
- **LEDs RGB:** Indicação de status do ambiente
  - 🟢 **Verde:** Condição normal
  - 🔴 **Vermelho:** Excesso de poeira (movimento no eixo X do joystick)
  - 🔵 **Azul:** Excesso de ruído (detecção pelo microfone)

---

## 📜 Funcionalidades
✔ Leitura do **joystick** e ativação de alerta de poeira quando há movimentação no eixo X.  
✔ Captura de **ruído** pelo microfone e ativação do alerta quando o nível ultrapassa o limite.  
✔ Controle de **LEDs RGB** para indicar status do ambiente.  
✔ Exibição de mensagens de **alerta no display OLED** quando condições críticas são detectadas.  

---

## 📂 Estrutura do Código
- `main.c` → Código principal do sistema.  
- `joystick.h / joystick.c` → Módulo responsável pela leitura do joystick.  
- `ssd1306.h / ssd1306.c` → Módulo para comunicação com o display OLED.  
- `hardware_adc.h / hardware_adc.c` → Leitura de valores analógicos do joystick e microfone.  
- `CMakeLists.txt` → Arquivo de configuração do projeto para compilação no **Pico SDK**.  

---

## ⚙️ Instalação e Configuração
### 1️⃣ Configuração do Ambiente
Antes de compilar o código, instale os pacotes necessários no **Raspberry Pi Pico W**

Clone o repositório e configure o Pico SDK

Após a compilação, conecte o Pico W e copie o arquivo .uf2 para a placa

**Como Executar:**

Conecte os sensores e LEDs conforme o esquema do projeto.

Ligue o Raspberry Pi Pico W na BitDogLab ou em um circuito físico.

O sistema monitorará os dados em tempo real e acionará os alertas necessários.

🛠️ Testes e Validação

📌 Joystick: Movimentar o eixo X deve acionar o LED vermelho e exibir "Excesso de Poeira" no OLED.

📌 Microfone: Emitir um som alto próximo ao sensor deve acionar o LED azul e exibir "Excesso de Ruído" no OLED.
