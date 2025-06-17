# 🌡️ Estufa com Controle de Temperatura via PID (ESP32 + DHT22 + OLED + Cartucho de Aquecimento)

Este projeto utiliza um **ESP32, sensor **DHT22**, display **OLED**, e um **cartucho de aquecimento controlado via PWM** com lógica **PID** para manter a temperatura da estufa constante em **50°C**. 
Um **MOSFET IRFZ44N** é usado para acionar o cartucho com segurança.

---

## 🔧 Componentes Utilizados

| Componente         | Modelo/Tipo          |
|--------------------|-----------------------|
| Microcontrolador   | ESP32 |
| Sensor de temperatura e umidade | DHT22 |
| Display OLED       | SSD1306 128x64 I2C    |
| Aquecedor          | Cartucho de aquecimento 24V |
| Conversor DC-DC    | 24V - 5V (Pode ser substituído por um carregador de celular)|
| Transistor NPN     | BC337                 |
| MOSFET             | IRFZ44N (Canal N)     |
| Fonte externa      | 24V (pelo menos 3.3 amperes) |
| Resistores         | 1kΩ (base do BC337), 10kΩ (gate do MOSFET) |

---

## ⚙️ Esquema de Ligações

### 🧠 Lógica de controle:
- O **ESP32** envia o sinal PWM para o **BC337**.
- O **BC337** atua como "chave" e libera o GATE do **MOSFET IRFZ44N**.
- O **IRFZ44N** então liga/desliga o **cartucho**.

### 🔌 Conexões do BC337:
- **Base** → GPIO do ESP32 (via resistor de 1kΩ)
- **Coletor** → Gate do IRFZ44N (com resistor pull-down de 10kΩ para GND)
- **Emissor** → GND (comum com o ESP32 e a fonte de 24V)

### 🔌 Conexões do IRFZ44N:
- **Gate** → Coletor do BC337
- **Drain** → Fio NEGATIVO do cartucho de aquecimento
- **Source** → GND da fonte de 24V

> ⚠️ Importante: o fio **positivo** do cartucho deve ir direto para o **+24V** da fonte.

---

## 📷 Display OLED

- Mostra a **temperatura atual**, **umidade**, e o **nível de potência do cartucho (em %)**
- Usa protocolo I2C (`SDA` e `SCL` no ESP32)
- Endereço padrão: `0x3C`

---

## 🧠 Lógica PID

O controle PID ajusta automaticamente o quanto o cartucho deve ficar ligado (via PWM) para manter a temperatura desejada.

- **Temperatura alvo**: 50°C
- **Ajuste manual do PWM** para temperaturas < 45°C (aquecimento rápido)
- Frequência PWM: **1 kHz** (para evitar ruídos audíveis)


## 🚀 Como Usar
1. Instale as bibliotecas necessárias no Arduino IDE:
- Adafruit_SSD1306
- Adafruit_GFX
- DHT sensor library
- PID_v1_bc (ou PID_v1, veja abaixo)

2. Faça o upload do código para o ESP32.

3. Conecte a fonte de 24V com o circuito montado corretamente.

4. O sistema começará a aquecer e estabilizar a temperatura.

🧪 Testes e Ajustes
- Barulho no PWM: se você ouvir zumbido, é por causa da frequência baixa. Subimos para 1 kHz para evitar esse efeito.
- Overshoot/undershoot? Ajuste os parâmetros Kp, Ki, Kd.
- Aquecendo devagar? Verifique a potência do cartucho, tensão da fonte, e integridade da montagem.

Foto do projeto montado

Print do display OLED

🧑‍💻 Autor
Projeto criado por Lael Ramos.
Se curtir ou usar esse projeto, sinta-se à vontade para dar uma estrela ⭐ ou contribuir!
