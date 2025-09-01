// ESP32-C3 MINI - EXPLORADOR TOTAL v6.2
// Versión robusta con Bluetooth, exportación y historial mejorado
#include <WiFi.h>
#include <esp_system.h>
#include <esp_sleep.h>
#include <esp_chip_info.h>
#include <soc/rtc.h>
#include <BLEDevice.h> // Para Bluetooth
#include <EEPROM.h>    // Para guardar historial y exportar

#define EEPROM_SIZE 4096 // Aumentado significativamente para el historial (4KB)
#define HISTORY_MAX_LEN 4000 // Máximo de caracteres para el historial (dejar un margen para '\0')

// Estado del diagnóstico
bool diagnosticoCompleto = false;

// Buffer para almacenar el historial de resultados
char historialBuffer[HISTORY_MAX_LEN];
int historialIdx = 0;

void setup() {
  // Inicialización serial robusta
  Serial.begin(115200);
  delay(1000);
  
  // Deshabilitar watchdog para evitar reinicios
  disableCore0WDT();

  // Inicializar EEPROM
  EEPROM.begin(EEPROM_SIZE);
  
  // Mensaje de bienvenida
  Serial.println("\n╔══════════════════════════════════════════════╗");
  Serial.println("║  🚀 ESP32-C3 MINI - EXPLORADOR TOTAL v6.2   ║");
  Serial.println("║  🔬 Diagnóstico completo con BT y Exportación║");
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
  Serial.println("│ A - Test de Bluetooth                  │"); 
  Serial.println("│ X - Exportar Datos (TXT)               │");
  Serial.println("│ C - Limpiar Historial                  │"); // Nueva opción para limpiar
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
  else if (cmd == "A" || cmd == "a") { 
    explorarBluetooth();
  }
  else if (cmd == "X" || cmd == "x") { 
    exportarDatos();
  }
  else if (cmd == "C" || cmd == "c") { // Nuevo comando para limpiar historial
    limpiarHistorial();
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

// Función para añadir texto al historial
void addToHistory(const String& text) {
  int len = text.length();
  // Si el texto es demasiado largo para el espacio restante,
  // solo añade lo que quepa o recorta si es necesario
  if (historialIdx + len >= HISTORY_MAX_LEN) {
    Serial.println("⚠️ Historial de RAM casi lleno. No se puede añadir todo el texto.");
    // Opcional: Recortar el texto para que quepa, o simplemente no añadirlo
    len = HISTORY_MAX_LEN - 1 - historialIdx; // Deja espacio para el nulo
    if (len <= 0) {
      Serial.println("⚠️ No hay espacio en el historial de RAM. Considera limpiarlo con 'C'.");
      return; // No hay espacio para nada
    }
  }
  
  memcpy(historialBuffer + historialIdx, text.c_str(), len);
  historialIdx += len;
  historialBuffer[historialIdx] = '\0'; // Asegurar terminador nulo
}

void limpiarHistorial() {
  historialIdx = 0;
  memset(historialBuffer, 0, HISTORY_MAX_LEN); // Limpiar buffer con ceros
  Serial.println("🗑️ Historial de comandos en RAM limpiado.");
  addToHistory("--- Historial limpiado manualmente ---\n"); // Añadir un marcador
}


// === 1. EXPLORACIÓN DEL CHIP ===
void explorarChip() {
  String output = "\n🔍 ANÁLISIS DEL CHIP ESP32-C3\n";
  output += "===============================\n";
  
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  
  output += "📋 IDENTIFICACIÓN:\n";
  output += "• Modelo: " + String(ESP.getChipModel()) + "\n";
  output += "• Revisión: " + String(ESP.getChipRevision()) + "\n";
  output += "• Núcleos: " + String(chip_info.cores) + "\n";
  output += "• Arquitectura: RISC-V 32-bit\n";
  output += "• WiFi: " + String(chip_info.features & CHIP_FEATURE_WIFI_BGN ? "SÍ" : "NO") + "\n";
  output += "• Bluetooth: " + String(chip_info.features & CHIP_FEATURE_BT ? "SÍ" : "NO") + "\n";
  
  uint64_t chipId = ESP.getEfuseMac();
  char chipIdStr[20];
  sprintf(chipIdStr, "• Chip ID: %04X%08X\n", (uint16_t)(chipId>>32), (uint32_t)chipId);
  output += String(chipIdStr);
  
  output += "\n💾 MEMORIA FLASH:\n";
  uint32_t flashSize = ESP.getFlashChipSize();
  output += "• Tamaño: " + String(flashSize/(1024*1024)) + " MB\n";
  output += "• Velocidad: " + String(ESP.getFlashChipSpeed()/1000000) + " MHz\n";
  output += "• Modo: " + getFlashModeStr() + "\n";
  
  output += "\n📊 PROGRAMA ACTUAL:\n";
  output += "• Tamaño sketch: " + String(ESP.getSketchSize()/1024) + " KB\n";
  output += "• Espacio libre: " + String(ESP.getFreeSketchSpace()/1024) + " KB\n";
  output += "• SDK Version: " + String(ESP.getSdkVersion()) + "\n";
  
  output += "\n✅ Análisis del chip completado\n";

  Serial.print(output);
  addToHistory(output);
}

// === 2. EXPLORACIÓN DE MEMORIA ===
void explorarMemoria() {
  String output = "\n🧠 ANÁLISIS DE MEMORIA\n";
  output += "=======================\n";
  
  output += "📊 HEAP PRINCIPAL:\n";
  uint32_t heapTotal = ESP.getHeapSize();
  uint32_t heapLibre = ESP.getFreeHeap();
  uint32_t heapUsado = heapTotal - heapLibre;
  
  output += "• Total: " + String(heapTotal/1024) + " KB\n";
  output += "• Libre: " + String(heapLibre/1024) + " KB\n";
  output += "• Usado: " + String(heapUsado/1024) + " KB\n";
  output += "• Utilización: " + String((heapUsado*100)/heapTotal) + "%\n";
  
  multi_heap_info_t info;
  heap_caps_get_info(&info, MALLOC_CAP_DEFAULT);
  
  output += "\n🔍 DETALLES DEL HEAP:\n";
  output += "• Bloques totales: " + String(info.total_blocks) + "\n";
  output += "• Bloques libres: " + String(info.free_blocks) + "\n";
  output += "• Bloque más grande: " + String(info.largest_free_block) + " bytes\n";
  output += "• Bytes libres: " + String(info.total_free_bytes) + "\n";
  
  output += "\n🧪 TEST DE FRAGMENTACIÓN:\n";
  void* testPtr = malloc(1024);
  if (testPtr) {
    output += "• Asignación de 1KB: ✅ Exitosa\n";
    free(testPtr);
    output += "• Liberación: ✅ Exitosa\n";
  } else {
    output += "• Asignación de 1KB: ❌ Falló\n";
  }
  
  output += "\n✅ Análisis de memoria completado\n";

  Serial.print(output);
  addToHistory(output);
}

// === 3. EXPLORACIÓN DE WIFI ===
void explorarWiFi() {
  String output = "\n📶 ANÁLISIS DE WIFI\n";
  output += "====================\n";
  
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_STA);
  delay(200);
  
  output += "📡 INFORMACIÓN BÁSICA:\n";
  output += "• MAC Address: " + WiFi.macAddress() + "\n";
  output += "• Modo: Station (STA)\n";
  
  output += "\n🔍 ESCANEANDO REDES...\n";
  Serial.print(output); // Imprimir hasta aquí para ver el progreso del scan
  addToHistory(output);

  Serial.print("⏳ ");
  
  int redes = WiFi.scanNetworks(false, true, false, 300);
  Serial.println("¡Completado!");
  
  output = ""; // Limpiar output para añadir resultados del scan
  if (redes > 0) {
    output += "\n📋 REDES ENCONTRADAS (" + String(redes) + "):\n";
    
    for (int i = 0; i < min(redes, 8); i++) {
      String seguridad = getSecurityType(WiFi.encryptionType(i));
      String intensidad = getRSSIQuality(WiFi.RSSI(i));
      
      output += "  " + String(i+1) + ". " + WiFi.SSID(i) + "\n";
      output += "     🔐 " + seguridad + " | 📶 " + intensidad + 
                    " (" + String(WiFi.RSSI(i)) + "dBm) | 📺 Ch" + String(WiFi.channel(i)) + "\n";
      
      delay(10);
    }
    
    if (redes > 8) {
      output += "  ... y " + String(redes - 8) + " redes más\n";
    }
  } else {
    output += "\n❌ No se encontraron redes WiFi\n";
  }
  
  WiFi.scanDelete();
  WiFi.mode(WIFI_OFF);
  output += "\n✅ Análisis WiFi completado\n";

  Serial.print(output);
  addToHistory(output);
}

// === 4. EXPLORACIÓN SEGURA DE GPIOS ===
void explorarGPIOs() {
  String output = "\n🔌 ANÁLISIS DE GPIOS\n";
  output += "=====================\n";
  
  int gpios[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10};
  int total = sizeof(gpios)/sizeof(gpios[0]);
  
  output += "📍 PINES DISPONIBLES:\n";
  output += "• Testeando: ";
  for(int i = 0; i < total; i++) {
    output += String(gpios[i]);
    if(i < total-1) output += ", ";
  }
  output += "\n• Reservados: 9(BOOT), 18-21(USB/UART)\n";
  
  output += "\n🧪 EJECUTANDO TESTS:\n";
  String funcionales = "";
  String problemáticos = "";
  int ok = 0;
  
  for (int i = 0; i < total; i++) {
    int pin = gpios[i];
    String currentPinOutput = "  GPIO " + String(pin) + ": ";
    
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(2);
    bool testHigh = digitalRead(pin);
    
    digitalWrite(pin, LOW);
    delay(2);
    bool testLow = digitalRead(pin);
    
    pinMode(pin, INPUT_PULLUP);
    delay(2);
    bool testPullup = digitalRead(pin);
    
    pinMode(pin, INPUT);
    
    if (testHigh && !testLow && testPullup) {
      currentPinOutput += "✅ Funcional\n";
      funcionales += String(pin) + " ";
      ok++;
    } else {
      currentPinOutput += "⚠️ Problemático\n";
      problemáticos += String(pin) + " ";
    }
    output += currentPinOutput; // Añadir el resultado del pin al output
    Serial.print(currentPinOutput); // Imprimir también al serial
    delay(50);
  }
  
  output += "\n📊 RESUMEN:\n";
  output += "• Funcionales (" + String(ok) + "): " + funcionales + "\n";
  if (problemáticos.length() > 0) {
    output += "• Problemáticos: " + problemáticos + "\n";
  }
  
  output += "\n✅ Análisis de GPIOs completado\n";

  Serial.print(output.substring(output.indexOf("📊 RESUMEN:"))); // Imprimir resumen final si no se hizo antes
  addToHistory(output);
}

// === 5. EXPLORACIÓN DEL SISTEMA ===
void explorarSistema() {
  String output = "\n⚙️ ANÁLISIS DEL SISTEMA\n";
  output += "========================\n";
  
  output += "🔄 INFORMACIÓN DE ARRANQUE:\n";
  output += "• Razón del reset: " + getResetReason() + "\n";
  output += "• Tiempo activo: " + String(millis()/1000) + " segundos\n";
  output += "• Núcleo actual: " + String(xPortGetCoreID()) + "\n";
  
  output += "\n🕐 FRECUENCIAS DE RELOJ:\n";
  output += "• CPU: " + String(ESP.getCpuFreqMHz()) + " MHz\n";
  output += "• APB: " + String(rtc_clk_apb_freq_get()/1000000) + " MHz\n";
  output += "• XTAL: " + String(rtc_clk_xtal_freq_get()) + " MHz\n";
  
  output += "\n🔋 GESTIÓN DE ENERGÍA:\n";
  output += "• Wake-up causa: " + String(esp_sleep_get_wakeup_cause()) + "\n";
  output += "• Modo actual: Rendimiento normal\n";
  
  output += "\n✅ Análisis del sistema completado\n";

  Serial.print(output);
  addToHistory(output);
}

// === 6. EXPLORACIÓN DE SENSORES ===
void explorarSensores() {
  String output = "\n🌡️ ANÁLISIS DE SENSORES\n";
  output += "=========================\n";
  
  output += "🌡️ SENSOR DE TEMPERATURA:\n";
  float temp = temperatureRead();
  Serial.print("• Temperatura del chip: " + String(temp, 1) + "°C");
  output += "• Temperatura del chip: " + String(temp, 1) + "°C\n";
  
  if (temp > 80) {
    Serial.println(" 🔥 ADVERTENCIA: Temperatura muy alta!");
    output += " 🔥 ADVERTENCIA: Temperatura muy alta!\n";
  } else if (temp > 60) {
    Serial.println(" ⚠️ Temperatura elevada");
    output += " ⚠️ Temperatura elevada\n";
  } else {
    Serial.println(" ✅ Temperatura normal");
    output += " ✅ Temperatura normal\n";
  }
  
  output += "\n⏱️ SISTEMA DE TIMING:\n";
  output += "• Millis(): " + String(millis()) + " ms\n";
  output += "• Micros(): " + String(micros()) + " μs\n";
  
  output += "🎯 Test de precisión delay(100ms): ";
  unsigned long inicio = millis();
  delay(100);
  unsigned long fin = millis();
  unsigned long precision = fin - inicio;
  output += String(precision) + "ms\n";
  Serial.print("🎯 Test de precisión delay(100ms): ");
  Serial.println(String(precision) + "ms");
  
  if (precision >= 98 && precision <= 102) {
    output += "✅ Excelente precisión\n";
    Serial.println("✅ Excelente precisión");
  } else {
    output += "⚠️ Desviación: " + String(abs((int)(precision-100))) + "ms\n";
    Serial.println("⚠️ Desviación: " + String(abs((int)(precision-100))) + "ms");
  }
  
  output += "\n✅ Análisis de sensores completado\n";

  // Serial.print(output); // Ya se imprimió parte
  addToHistory(output);
}

// === 7. TEST DE LEDS ===
void testLEDs() {
  String output = "\n💡 TEST DE LEDS\n";
  output += "================"; // No añadir \n aquí, se añade después de la línea de cabecera en Serial.print
  
  int candidatos[] = {2, 3, 7, 8, 10};
  String nombres[] = {"GPIO2", "GPIO3", "GPIO7", "GPIO8", "GPIO10"};
  int total = sizeof(candidatos)/sizeof(candidatos[0]);
  
  output += "\n🔍 Probando " + String(total) + " posibles ubicaciones de LEDs\n";
  output += "👀 Observa la placa durante cada test...\n\n";

  Serial.print(output);
  addToHistory(output);
  output = ""; // Limpiar para el loop

  for (int i = 0; i < total; i++) {
    int pin = candidatos[i];
    Serial.println("🧪 Testeando " + nombres[i] + ":");
    output += "🧪 Testeando " + nombres[i] + ":\n";
    
    pinMode(pin, OUTPUT);
    Serial.print("   Parpadeando: ");
    output += "   Parpadeando: ";
    
    for (int j = 0; j < 6; j++) {
      digitalWrite(pin, HIGH);
      delay(200);
      digitalWrite(pin, LOW);
      delay(200);
      Serial.print("●");
      output += "●";
    }
    
    pinMode(pin, INPUT);
    Serial.println(" [Completado]");
    output += " [Completado]\n";
    
    delay(500);
  }
  
  output += "\n💡 Test de LEDs completado\n";
  output += "ℹ️ Si no viste LEDs, pueden estar en otros pines o no existir\n";

  Serial.print(output);
  addToHistory(output);
}

// === 8. BENCHMARK DE RENDIMIENTO ===
void benchmark() {
  String output = "\n🏃 BENCHMARK DE RENDIMIENTO\n";
  output += "============================\n";
  
  output += "🧮 Test matemático (10k operaciones)... ";
  Serial.print(output); // Imprimir hasta aquí para ver el progreso
  addToHistory(output);
  output = "";

  unsigned long inicio = micros();
  volatile float resultado = 0;
  for (int i = 0; i < 10000; i++) {
    resultado += sqrt(i) * 3.14159;
  }
  unsigned long tiempoMath = micros() - inicio;
  output += String(tiempoMath) + " μs\n";
  Serial.println(String(tiempoMath) + " μs");
  
  output += "⚡ Test GPIO (5k toggles)... ";
  Serial.print(output); // Imprimir hasta aquí
  addToHistory(output);
  output = "";

  pinMode(2, OUTPUT);
  inicio = micros();
  for (int i = 0; i < 5000; i++) {
    digitalWrite(2, i % 2);
  }
  unsigned long tiempoGPIO = micros() - inicio;
  pinMode(2, INPUT);
  output += String(tiempoGPIO) + " μs\n";
  Serial.println(String(tiempoGPIO) + " μs");
  
  output += "💾 Test memoria (concatenación)... ";
  Serial.print(output); // Imprimir hasta aquí
  addToHistory(output);
  output = "";

  inicio = micros();
  String testStr = "";
  for (int i = 0; i < 500; i++) {
    testStr += String(i);
  }
  unsigned long tiempoMem = micros() - inicio;
  output += String(tiempoMem) + " μs\n";
  Serial.println(String(tiempoMem) + " μs");
  
  output += "\n📊 PUNTUACIÓN FINAL:\n";
  output += "• Matemáticas: " + String(10000000.0/tiempoMath, 1) + " ops/seg\n";
  output += "• GPIO: " + String(5000000.0/tiempoGPIO, 1) + " ops/seg\n";
  output += "• Memoria: " + String(500000.0/tiempoMem, 1) + " ops/seg\n";
  
  output += "\n✅ Benchmark completado\n";

  Serial.print(output);
  addToHistory(output);
}


// ===================================================================
// === A. EXPLORACIÓN DE BLUETOOTH (v4 - Corrección de Puntero) ===
// ===================================================================

// Esta clase de callback no necesita cambios
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      String msg = "  BLE Device found: ";
      if (advertisedDevice.getName().length() > 0) {
        msg += advertisedDevice.getName().c_str();
      } else {
        msg += "[Unnamed]";
      }
      msg += " Address: ";
      msg += advertisedDevice.getAddress().toString().c_str();
      msg += " RSSI: ";
      msg += String(advertisedDevice.getRSSI());
      msg += "\n";
      Serial.print(msg);
      addToHistory(msg);
    }
};

void explorarBluetooth() {
  String output = "\n📡 ANÁLISIS DE BLUETOOTH\n";
  output += "=========================\n";
  Serial.print(output);
  addToHistory(output);

  // Inicializar BLE (si no está ya inicializado)
  if (!BLEDevice::getInitialized()) {
    BLEDevice::init("");
    BLEDevice::setPower(ESP_PWR_LVL_P9);
    output = "• BLE Initialized: ✅\n";
  } else {
    output = "• BLE Already Initialized: ✅\n";
  }

  output += "• MAC Address: " + String(BLEDevice::getAddress().toString().c_str()) + "\n";
  Serial.print(output);
  addToHistory(output);

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);

  Serial.println("\n🔍 ESCANEANDO DISPOSITIVOS BLE por 10 segundos (en 5 ciclos de 2s)...");
  addToHistory("\n🔍 ESCANEANDO DISPOSITIVOS BLE por 10 segundos (en 5 ciclos de 2s)...\n");

  int scanCycles = 5;
  for (int i = 0; i < scanCycles; i++) {
    Serial.print("   Ciclo de escaneo " + String(i + 1) + "/" + String(scanCycles) + "...");
    pBLEScan->start(2, false); 
    Serial.println(" completado.");
    delay(50); 
  }
  
  // === LÍNEA CORREGIDA ===
  // Se declara como un puntero BLEScanResults*
  BLEScanResults* foundDevices = pBLEScan->getResults();
  
  String summary = "\n📊 RESUMEN DE ESCANEO BLE:\n";
  // Se usa el operador de puntero "->" para acceder a getCount()
  summary += "• Dispositivos encontrados: " + String(foundDevices->getCount()) + "\n";
  
  if (foundDevices->getCount() == 0) {
    summary += "• No se encontraron dispositivos BLE.\n";
  }

  summary += "\n✅ Análisis de Bluetooth completado\n";
  Serial.print(summary);
  addToHistory(summary);

  pBLEScan->clearResults(); 
  // BLEDevice::deinit(); 
}

// === 9. DIAGNÓSTICO TOTAL ===
void diagnosticoTotal() {
  Serial.println("\n🔬 DIAGNÓSTICO COMPLETO");
  Serial.println("========================");
  Serial.println("⏳ Ejecutando todos los análisis...\n");
  
  // Limpiar historial al inicio de un diagnóstico completo
  limpiarHistorial(); // Usar la nueva función de limpieza
  addToHistory("--- INICIO DIAGNÓSTICO COMPLETO ---\n\n");

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

  explorarBluetooth(); 
  
  Serial.println("\n🎉 DIAGNÓSTICO COMPLETO TERMINADO");
  Serial.println("📊 Todos los sistemas han sido analizados exitosamente");
  Serial.println("ℹ️ Usa el comando 'X' para exportar los resultados a un archivo TXT.");
  diagnosticoCompleto = true;
  addToHistory("\n--- FIN DIAGNÓSTICO COMPLETO ---\n");
}

// === X. EXPORTAR DATOS ===
void exportarDatos() {
  Serial.println("\n📤 EXPORTACIÓN DE DATOS");
  Serial.println("========================");
  
  if (historialIdx == 0) {
    Serial.println("❌ No hay datos en el historial para exportar.");
    Serial.println("💡 Ejecuta algún comando o el 'DIAGNÓSTICO COMPLETO' (9) primero.");
    return;
  }

  Serial.println("💾 Guardando historial en EEPROM...");
  // Guardar el historial en la EEPROM
  // Asegurarse de no exceder el tamaño de la EEPROM
  int bytesToSave = min(historialIdx, EEPROM_SIZE - 1); // Deja espacio para el terminador nulo
  for (int i = 0; i < bytesToSave; i++) {
    EEPROM.write(i, historialBuffer[i]);
  }
  EEPROM.write(bytesToSave, '\0'); // Asegurar terminador nulo
  EEPROM.commit(); // Guardar cambios en la EEPROM

  Serial.println("✅ Historial guardado en EEPROM. (" + String(bytesToSave) + " bytes)");
  Serial.println("⬇️ Copia el siguiente texto para exportar:");
  Serial.println("```text"); // Inicio del bloque de texto para facilitar la copia
  
  // Leer y enviar desde EEPROM para asegurar que se guardó correctamente
  for (int i = 0; i < bytesToSave; i++) {
    Serial.print((char)EEPROM.read(i));
  }
  Serial.println("\n```"); // Fin del bloque de texto
  Serial.println("\n💡 Puedes pegar este texto en un archivo .txt");
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