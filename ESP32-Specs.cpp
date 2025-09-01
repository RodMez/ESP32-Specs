// ESP32-C3 MINI - EXPLORADOR TOTAL v6.0
// Versión robusta sin conflictos de watchdog
#include <WiFi.h>
#include <esp_system.h>
#include <esp_sleep.h>
#include <esp_chip_info.h>
#include <soc/rtc.h>

// Estado del diagnóstico
bool diagnosticoCompleto = false;

void setup() {
  // Inicialización serial robusta
  Serial.begin(115200);
  delay(1000);
  
  // Deshabilitar watchdog para evitar reinicios
  disableCore0WDT();
  
  // Mensaje de bienvenida
  Serial.println("\n╔══════════════════════════════════════════════╗");
  Serial.println("║  🚀 ESP32-C3 MINI - EXPLORADOR TOTAL v6.0   ║");
  Serial.println("║  🔬 Diagnóstico completo sin watchdog        ║");
  Serial.println("╚══════════════════════════════════════════════╝");
  
  delay(500);
  mostrarMenu();
}

void loop() {
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    
    if (comando.length() > 0) {
      ejecutarComando(comando);
    }
  }
  delay(100);
}

void mostrarMenu() {
  Serial.println("\n📋 MENÚ DE EXPLORACIÓN:");
  Serial.println("┌─────────────────────────────────────────┐");
  Serial.println("│ 1 - Información del Chip               │");
  Serial.println("│ 2 - Análisis de Memoria                │");
  Serial.println("│ 3 - Test de WiFi                       │");
  Serial.println("│ 4 - Test de GPIOs                      │");
  Serial.println("│ 5 - Estado del Sistema                 │");
  Serial.println("│ 6 - Sensores Internos                  │");
  Serial.println("│ 7 - Test de LEDs                       │");
  Serial.println("│ 8 - Benchmark de Rendimiento           │");
  Serial.println("│ 9 - DIAGNÓSTICO COMPLETO               │");
  Serial.println("│                                         │");
  Serial.println("│ help - Mostrar este menú               │");
  Serial.println("│ reset - Reiniciar                      │");
  Serial.println("│ sleep - Deep Sleep                     │");
  Serial.println("└─────────────────────────────────────────┘");
  Serial.print("💬 Comando: ");
}

void ejecutarComando(String cmd) {
  Serial.println();
  
  if (cmd == "1") {
    explorarChip();
  }
  else if (cmd == "2") {
    explorarMemoria();
  }
  else if (cmd == "3") {
    explorarWiFi();
  }
  else if (cmd == "4") {
    explorarGPIOs();
  }
  else if (cmd == "5") {
    explorarSistema();
  }
  else if (cmd == "6") {
    explorarSensores();
  }
  else if (cmd == "7") {
    testLEDs();
  }
  else if (cmd == "8") {
    benchmark();
  }
  else if (cmd == "9") {
    diagnosticoTotal();
  }
  else if (cmd == "help" || cmd == "h") {
    mostrarMenu();
    return;
  }
  else if (cmd == "reset") {
    Serial.println("🔄 Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  else if (cmd == "sleep") {
    Serial.println("💤 Entrando en Deep Sleep. Presiona RESET para despertar.");
    delay(500);
    esp_deep_sleep_start();
  }
  else {
    Serial.println("❌ Comando no reconocido: '" + cmd + "'");
    Serial.println("💡 Escribe 'help' para ver opciones");
  }
  
  Serial.println("\n" + String(char(196)) + String(char(196)) + String(char(196)) + " Listo " + String(char(196)) + String(char(196)) + String(char(196)));
  Serial.print("💬 Siguiente comando: ");
}

// === 1. EXPLORACIÓN DEL CHIP ===
void explorarChip() {
  Serial.println("🔍 ANÁLISIS DEL CHIP ESP32-C3");
  Serial.println("===============================");
  
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  
  Serial.println("📋 IDENTIFICACIÓN:");
  Serial.println("• Modelo: " + String(ESP.getChipModel()));
  Serial.println("• Revisión: " + String(ESP.getChipRevision()));
  Serial.println("• Núcleos: " + String(chip_info.cores));
  Serial.println("• Arquitectura: RISC-V 32-bit");
  Serial.println("• WiFi: " + String(chip_info.features & CHIP_FEATURE_WIFI_BGN ? "SÍ" : "NO"));
  Serial.println("• Bluetooth: " + String(chip_info.features & CHIP_FEATURE_BT ? "SÍ" : "NO"));
  
  // MAC único
  uint64_t chipId = ESP.getEfuseMac();
  Serial.printf("• Chip ID: %04X%08X\n", (uint16_t)(chipId>>32), (uint32_t)chipId);
  
  Serial.println("\n💾 MEMORIA FLASH:");
  uint32_t flashSize = ESP.getFlashChipSize();
  Serial.println("• Tamaño: " + String(flashSize/(1024*1024)) + " MB");
  Serial.println("• Velocidad: " + String(ESP.getFlashChipSpeed()/1000000) + " MHz");
  Serial.println("• Modo: " + getFlashModeStr());
  
  Serial.println("\n📊 PROGRAMA ACTUAL:");
  Serial.println("• Tamaño sketch: " + String(ESP.getSketchSize()/1024) + " KB");
  Serial.println("• Espacio libre: " + String(ESP.getFreeSketchSpace()/1024) + " KB");
  Serial.println("• SDK Version: " + String(ESP.getSdkVersion()));
  
  Serial.println("\n✅ Análisis del chip completado");
}

// === 2. EXPLORACIÓN DE MEMORIA ===
void explorarMemoria() {
  Serial.println("🧠 ANÁLISIS DE MEMORIA");
  Serial.println("=======================");
  
  Serial.println("📊 HEAP PRINCIPAL:");
  uint32_t heapTotal = ESP.getHeapSize();
  uint32_t heapLibre = ESP.getFreeHeap();
  uint32_t heapUsado = heapTotal - heapLibre;
  
  Serial.println("• Total: " + String(heapTotal/1024) + " KB");
  Serial.println("• Libre: " + String(heapLibre/1024) + " KB");
  Serial.println("• Usado: " + String(heapUsado/1024) + " KB");
  Serial.println("• Utilización: " + String((heapUsado*100)/heapTotal) + "%");
  
  // Información detallada del heap
  multi_heap_info_t info;
  heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);
  
  Serial.println("\n🔍 DETALLES DEL HEAP:");
  Serial.println("• Bloques totales: " + String(info.total_blocks));
  Serial.println("• Bloques libres: " + String(info.free_blocks));
  Serial.println("• Bloque más grande: " + String(info.largest_free_block) + " bytes");
  Serial.println("• Bytes libres: " + String(info.total_free_bytes));
  
  // Test de fragmentación
  Serial.println("\n🧪 TEST DE FRAGMENTACIÓN:");
  void* testPtr = malloc(1024);
  if (testPtr) {
    Serial.println("• Asignación de 1KB: ✅ Exitosa");
    free(testPtr);
    Serial.println("• Liberación: ✅ Exitosa");
  } else {
    Serial.println("• Asignación de 1KB: ❌ Falló");
  }
  
  Serial.println("\n✅ Análisis de memoria completado");
}

// === 3. EXPLORACIÓN DE WIFI ===
void explorarWiFi() {
  Serial.println("📶 ANÁLISIS DE WIFI");
  Serial.println("====================");
  
  // Configuración inicial
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_STA);
  delay(200);
  
  Serial.println("📡 INFORMACIÓN BÁSICA:");
  Serial.println("• MAC Address: " + WiFi.macAddress());
  Serial.println("• Modo: Station (STA)");
  
  // Escaneo progresivo de redes
  Serial.println("\n🔍 ESCANEANDO REDES...");
  Serial.print("⏳ ");
  
  int redes = WiFi.scanNetworks(false, true, false, 300);
  Serial.println("¡Completado!");
  
  if (redes > 0) {
    Serial.println("\n📋 REDES ENCONTRADAS (" + String(redes) + "):");
    
    for (int i = 0; i < min(redes, 8); i++) {
      String seguridad = getSecurityType(WiFi.encryptionType(i));
      String intensidad = getRSSIQuality(WiFi.RSSI(i));
      
      Serial.println("  " + String(i+1) + ". " + WiFi.SSID(i));
      Serial.println("     🔐 " + seguridad + " | 📶 " + intensidad + 
                    " (" + String(WiFi.RSSI(i)) + "dBm) | 📺 Ch" + String(WiFi.channel(i)));
      
      delay(10); // Pequeña pausa entre redes
    }
    
    if (redes > 8) {
      Serial.println("  ... y " + String(redes - 8) + " redes más");
    }
  } else {
    Serial.println("\n❌ No se encontraron redes WiFi");
  }
  
  WiFi.scanDelete();
  WiFi.mode(WIFI_OFF);
  Serial.println("\n✅ Análisis WiFi completado");
}

// === 4. EXPLORACIÓN SEGURA DE GPIOS ===
void explorarGPIOs() {
  Serial.println("🔌 ANÁLISIS DE GPIOS");
  Serial.println("=====================");
  
  // Lista de GPIOs seguros para testear
  int gpios[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10};
  int total = sizeof(gpios)/sizeof(gpios[0]);
  
  Serial.println("📍 PINES DISPONIBLES:");
  Serial.print("• Testeando: ");
  for(int i = 0; i < total; i++) {
    Serial.print(String(gpios[i]));
    if(i < total-1) Serial.print(", ");
  }
  Serial.println("\n• Reservados: 9(BOOT), 18-21(USB/UART)");
  
  Serial.println("\n🧪 EJECUTANDO TESTS:");
  String funcionales = "";
  String problemáticos = "";
  int ok = 0;
  
  for (int i = 0; i < total; i++) {
    int pin = gpios[i];
    Serial.print("  GPIO " + String(pin) + ": ");
    
    // Test básico de salida
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(2);
    bool testHigh = digitalRead(pin);
    
    digitalWrite(pin, LOW);
    delay(2);
    bool testLow = digitalRead(pin);
    
    // Test de entrada con pull-up
    pinMode(pin, INPUT_PULLUP);
    delay(2);
    bool testPullup = digitalRead(pin);
    
    // Restaurar a entrada
    pinMode(pin, INPUT);
    
    // Evaluar resultados
    if (testHigh && !testLow && testPullup) {
      Serial.println("✅ Funcional");
      funcionales += String(pin) + " ";
      ok++;
    } else {
      Serial.println("⚠️ Problemático");
      problemáticos += String(pin) + " ";
    }
    
    delay(50); // Pausa segura entre tests
  }
  
  Serial.println("\n📊 RESUMEN:");
  Serial.println("• Funcionales (" + String(ok) + "): " + funcionales);
  if (problemáticos.length() > 0) {
    Serial.println("• Problemáticos: " + problemáticos);
  }
  
  Serial.println("\n✅ Análisis de GPIOs completado");
}

// === 5. EXPLORACIÓN DEL SISTEMA ===
void explorarSistema() {
  Serial.println("⚙️ ANÁLISIS DEL SISTEMA");
  Serial.println("========================");
  
  Serial.println("🔄 INFORMACIÓN DE ARRANQUE:");
  Serial.println("• Razón del reset: " + getResetReason());
  Serial.println("• Tiempo activo: " + String(millis()/1000) + " segundos");
  Serial.println("• Núcleo actual: " + String(xPortGetCoreID()));
  
  Serial.println("\n🕐 FRECUENCIAS DE RELOJ:");
  Serial.println("• CPU: " + String(ESP.getCpuFreqMHz()) + " MHz");
  Serial.println("• APB: " + String(rtc_clk_apb_freq_get()/1000000) + " MHz");
  Serial.println("• XTAL: " + String(rtc_clk_xtal_freq_get()) + " MHz");
  
  Serial.println("\n🔋 GESTIÓN DE ENERGÍA:");
  Serial.println("• Wake-up causa: " + String(esp_sleep_get_wakeup_cause()));
  Serial.println("• Modo actual: Rendimiento normal");
  
  Serial.println("\n✅ Análisis del sistema completado");
}

// === 6. EXPLORACIÓN DE SENSORES ===
void explorarSensores() {
  Serial.println("🌡️ ANÁLISIS DE SENSORES");
  Serial.println("=========================");
  
  Serial.println("🌡️ SENSOR DE TEMPERATURA:");
  float temp = temperatureRead();
  Serial.println("• Temperatura del chip: " + String(temp, 1) + "°C");
  
  if (temp > 80) {
    Serial.println("🔥 ADVERTENCIA: Temperatura muy alta!");
  } else if (temp > 60) {
    Serial.println("⚠️ Temperatura elevada");
  } else {
    Serial.println("✅ Temperatura normal");
  }
  
  Serial.println("\n⏱️ SISTEMA DE TIMING:");
  Serial.println("• Millis(): " + String(millis()) + " ms");
  Serial.println("• Micros(): " + String(micros()) + " μs");
  
  // Test de precisión
  Serial.print("🎯 Test de precisión delay(100ms): ");
  unsigned long inicio = millis();
  delay(100);
  unsigned long fin = millis();
  unsigned long precision = fin - inicio;
  Serial.println(String(precision) + "ms");
  
  if (precision >= 98 && precision <= 102) {
    Serial.println("✅ Excelente precisión");
  } else {
    Serial.println("⚠️ Desviación: " + String(abs((int)(precision-100))) + "ms");
  }
  
  Serial.println("\n✅ Análisis de sensores completado");
}

// === 7. TEST DE LEDS ===
void testLEDs() {
  Serial.println("💡 TEST DE LEDS");
  Serial.println("================");
  
  int candidatos[] = {2, 3, 7, 8, 10};
  String nombres[] = {"GPIO2", "GPIO3", "GPIO7", "GPIO8", "GPIO10"};
  int total = sizeof(candidatos)/sizeof(candidatos[0]);
  
  Serial.println("🔍 Probando " + String(total) + " posibles ubicaciones de LEDs");
  Serial.println("👀 Observa la placa durante cada test...\n");
  
  for (int i = 0; i < total; i++) {
    int pin = candidatos[i];
    Serial.println("🧪 Testeando " + nombres[i] + ":");
    
    pinMode(pin, OUTPUT);
    Serial.print("   Parpadeando: ");
    
    // Patrón distintivo de parpadeo
    for (int j = 0; j < 6; j++) {
      digitalWrite(pin, HIGH);
      delay(200);
      digitalWrite(pin, LOW);
      delay(200);
      Serial.print("●");
    }
    
    pinMode(pin, INPUT);
    Serial.println(" [Completado]");
    
    delay(500); // Pausa entre tests
  }
  
  Serial.println("\n💡 Test de LEDs completado");
  Serial.println("ℹ️ Si no viste LEDs, pueden estar en otros pines o no existir");
}

// === 8. BENCHMARK DE RENDIMIENTO ===
void benchmark() {
  Serial.println("🏃 BENCHMARK DE RENDIMIENTO");
  Serial.println("============================");
  
  // Test 1: Cálculo matemático
  Serial.print("🧮 Test matemático (10k operaciones)... ");
  unsigned long inicio = micros();
  volatile float resultado = 0;
  for (int i = 0; i < 10000; i++) {
    resultado += sqrt(i) * 3.14159;
  }
  unsigned long tiempoMath = micros() - inicio;
  Serial.println(String(tiempoMath) + " μs");
  
  // Test 2: Operaciones GPIO
  Serial.print("⚡ Test GPIO (5k toggles)... ");
  pinMode(2, OUTPUT);
  inicio = micros();
  for (int i = 0; i < 5000; i++) {
    digitalWrite(2, i % 2);
  }
  unsigned long tiempoGPIO = micros() - inicio;
  pinMode(2, INPUT);
  Serial.println(String(tiempoGPIO) + " μs");
  
  // Test 3: Operaciones de memoria
  Serial.print("💾 Test memoria (concatenación)... ");
  inicio = micros();
  String testStr = "";
  for (int i = 0; i < 500; i++) {
    testStr += String(i);
  }
  unsigned long tiempoMem = micros() - inicio;
  Serial.println(String(tiempoMem) + " μs");
  
  // Resultados finales
  Serial.println("\n📊 PUNTUACIÓN FINAL:");
  Serial.println("• Matemáticas: " + String(10000000.0/tiempoMath, 1) + " ops/seg");
  Serial.println("• GPIO: " + String(5000000.0/tiempoGPIO, 1) + " ops/seg");
  Serial.println("• Memoria: " + String(500000.0/tiempoMem, 1) + " ops/seg");
  
  Serial.println("\n✅ Benchmark completado");
}

// === 9. DIAGNÓSTICO TOTAL ===
void diagnosticoTotal() {
  Serial.println("🔬 DIAGNÓSTICO COMPLETO");
  Serial.println("========================");
  Serial.println("⏳ Ejecutando todos los análisis...\n");
  
  explorarChip();
  delay(1000);
  
  explorarMemoria();
  delay(1000);
  
  explorarWiFi();
  delay(1000);
  
  explorarGPIOs();
  delay(1000);
  
  explorarSistema();
  delay(1000);
  
  explorarSensores();
  delay(1000);
  
  benchmark();
  
  Serial.println("\n🎉 DIAGNÓSTICO COMPLETO TERMINADO");
  Serial.println("📊 Todos los sistemas han sido analizados exitosamente");
  diagnosticoCompleto = true;
}

// === FUNCIONES AUXILIARES ===
String getFlashModeStr() {
  switch(ESP.getFlashChipMode()) {
    case FM_QIO: return "QIO";
    case FM_QOUT: return "QOUT"; 
    case FM_DIO: return "DIO";
    case FM_DOUT: return "DOUT";
    default: return "Desconocido";
  }
}

String getResetReason() {
  switch(esp_reset_reason()) {
    case ESP_RST_POWERON: return "Power-On";
    case ESP_RST_EXT: return "Reset externo";
    case ESP_RST_SW: return "Software";
    case ESP_RST_PANIC: return "Excepción/Pánico";
    case ESP_RST_INT_WDT: return "Interrupt WDT";
    case ESP_RST_TASK_WDT: return "Task WDT";
    case ESP_RST_WDT: return "Other WDT";
    case ESP_RST_DEEPSLEEP: return "Deep Sleep";
    case ESP_RST_BROWNOUT: return "Brownout";
    default: return "Desconocido";
  }
}

String getSecurityType(wifi_auth_mode_t tipo) {
  switch(tipo) {
    case WIFI_AUTH_OPEN: return "Abierta";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA-PSK";
    case WIFI_AUTH_WPA2_PSK: return "WPA2-PSK";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
    case WIFI_AUTH_WPA3_PSK: return "WPA3-PSK";
    default: return "Desconocida";
  }
}

String getRSSIQuality(int rssi) {
  if (rssi > -30) return "Excelente";
  else if (rssi > -50) return "Muy buena";
  else if (rssi > -60) return "Buena";
  else if (rssi > -70) return "Regular";
  else return "Débil";
}