# ESP32-C3 MINI - EXPLORADOR TOTAL v6.4

## Descripción General

**ESP32-C3 MINI - EXPLORADOR TOTAL** es una herramienta de diagnóstico y exploración avanzada para microcontroladores ESP32-C3. Proporciona análisis completo del hardware, benchmark de rendimiento, gestión de archivos vía web y utilidades de desarrollo integradas en un solo sistema interactivo.

### Características Principales

- **Diagnóstico integral** del chip ESP32-C3 con análisis detallado de componentes
- **Servidor web integrado** para gestión remota de archivos (File Manager)
- **Sistema de exportación** de resultados en formato TXT
- **Benchmark de rendimiento** con métricas comparativas
- **Interfaz interactiva** vía Monitor Serie con menú intuitivo
- **Gestión de memoria** optimizada con historial en RAM y respaldo en EEPROM
- **Compatibilidad multiplataforma** (adaptable a otros modelos ESP32)

## Especificaciones Técnicas

### Requisitos de Hardware
- **Microcontrolador**: ESP32-C3 MINI (o compatible)
- **Memoria Flash**: Mínimo 4MB recomendado
- **RAM**: 400KB (utiliza ~43KB en operación normal)
- **Conectividad**: WiFi 802.11 b/g/n, Bluetooth 5.0 LE

### Requisitos de Software
- **IDE**: Arduino IDE 2.0+ o PlatformIO
- **Librerías requeridas**:
  - `WiFi.h` - Gestión de conectividad WiFi
  - `BLEDevice.h` - Funcionalidad Bluetooth Low Energy
  - `EEPROM.h` - Almacenamiento persistente
  - `SPIFFS.h` - Sistema de archivos interno
  - `WebServer.h` - Servidor HTTP integrado
  - `esp_system.h` - APIs del sistema ESP-IDF



## Instalación y Configuración



### 1. Configuración de Librerías
Instalar las librerías requeridas desde el Library Manager de Arduino IDE:
- ESP32 Core Libraries (incluidas por defecto)
- Librerías adicionales se instalan automáticamente



### 2 Verificación de Instalación
1. Conectar ESP32-C3 vía USB
2. Abrir Monitor Serie a **115200 baudios**


## Guía de Uso

### Interfaz Principal

Al iniciar el sistema, se presenta un menú interactivo:

```
╔═══════════════════════════════════════════╗
║  🚀 ESP32-C3 MINI - EXPLORADOR TOTAL     ║
╚═══════════════════════════════════════════╝

📋 MENÚ DE EXPLORACIÓN:
┌───────────────────────────┐
│ 1 - Información del Chip  │
│ 2 - Análisis de Memoria   │
│ [... más opciones ...]    │
└───────────────────────────┘
💬 Comando: 
```

### Comandos Detallados

#### Diagnóstico de Hardware

| Comando | Función | Descripción Detallada |
|---------|---------|----------------------|
| `1` | **Información del Chip** | Análisis completo del microcontrolador: familia, arquitectura RISC-V, núcleos, capacidades WiFi/BLE, revisión del chip, ID único, información de Flash (tamaño, velocidad), espacio de sketch y versión del SDK |
| `2` | **Análisis de Memoria** | Diagnóstico integral de RAM: heap total/libre/usado, porcentaje de utilización, detalles de bloques de memoria, análisis de fragmentación y test de asignación dinámica |
| `4` | **Test de GPIOs** | Verificación exhaustiva de pines: test de funcionalidad digital (HIGH/LOW), verificación de pull-up interno, identificación de pines problemáticos y mapeo completo de GPIOs disponibles |
| `6` | **Sensores Internos** | Lectura de sensores integrados: temperatura del chip con alertas térmica, sistema de timing (millis/micros), test de precisión de delays y verificación de osciladores |

#### Conectividad y Comunicación

| Comando | Función | Descripción Detallada |
|---------|---------|----------------------|
| `3` | **Test de WiFi** | Análisis completo de WiFi: información de interfaz (MAC address), escaneo de redes disponibles, análisis de calidad de señal (RSSI), identificación de canales y tipos de seguridad |
| `A` | **Test de Bluetooth** | Diagnóstico BLE: inicialización del stack Bluetooth, escaneo de dispositivos cercanos, análisis de señal RSSI, identificación de servicios y gestión de conexiones |

#### Análisis de Rendimiento

| Comando | Función | Descripción Detallada |
|---------|---------|----------------------|
| `8` | **Benchmark de Rendimiento** | Suite completa de pruebas: operaciones matemáticas (10K iteraciones de sqrt/multiplicación), velocidad de GPIO (5K toggles), rendimiento de memoria (concatenación de strings), métricas comparativas en ops/segundo |
| `7` | **Test de LEDs** | Prueba sistemática de LEDs: test de múltiples GPIOs candidatos, secuencias de parpadeo visibles, identificación de LEDs onboard y verificación de polaridad |

#### Sistema y Diagnóstico

| Comando | Función | Descripción Detallada |
|---------|---------|----------------------|
| `5` | **Estado del Sistema** | Análisis del sistema: razón del último reset, tiempo de actividad (uptime), núcleo en uso, frecuencias de CPU/APB/XTAL, gestión de energía y wake-up sources |
| `9` | **Diagnóstico Completo** | Ejecución automática secuencial de todos los tests (1-8 + A), generación de informe consolidado, guardado en historial RAM y preparación para exportación |

#### Gestión de Archivos

| Comando | Función | Descripción Detallada |
|---------|---------|----------------------|
| `W` | **Servidor Web** | Activación del File Manager web: creación de Access Point WiFi, servidor HTTP en puerto 80, interfaz web responsive, gestión remota de archivos SPIFFS |
| `X` | **Exportar a Archivo** | Exportación de resultados: creación de archivo TXT timestamped, guardado en SPIFFS, respaldo en EEPROM, preparación para descarga web |
| `Y` | **Mostrar Archivos** | Listado de archivos SPIFFS: información detallada de cada archivo, preview de contenido, estadísticas de uso de espacio, enlaces de acceso rápido |
| `C` | **Limpiar Historial** | Limpieza segura del buffer RAM de historial, liberación de memoria, mantenimiento de logs esenciales |

#### Utilidades del Sistema

| Comando | Función | Descripción Detallada |
|---------|---------|----------------------|
| `help` | **Mostrar Menú** | Redespliegue del menú completo de comandos con descripciones |
| `reset` | **Reiniciar Sistema** | Reinicio controlado del ESP32-C3 con limpieza de estados |
| `sleep` | **Deep Sleep** | Activación del modo de ultra-bajo consumo, wake-up por botón RESET |

## Servidor Web File Manager

### Características del File Manager

El servidor web integrado proporciona una interfaz completa para gestión de archivos:

#### Funcionalidades
- **Listado dinámico** de archivos con actualización automática
- **Descarga directa** de archivos TXT generados
- **Eliminación segura** con confirmación

- **Interfaz responsive** compatible con móviles

#### Acceso al File Manager
```bash
1. Ejecutar comando 'W' en Monitor Serie
2. Conectar dispositivo a WiFi "ESP32-FileManager"
3. Usar contraseña: "12345678"
4. Abrir navegador en: http://192.168.4.1
5. Gestionar archivos desde interfaz web
```

#### API REST Disponible
| Endpoint | Método | Función |
|----------|--------|---------|
| `/` | GET | Interfaz principal del File Manager |
| `/list` | GET | Lista archivos en formato JSON |
| `/download?file=<nombre>` | GET | Descarga archivo específico |
| `/delete?file=<nombre>` | GET | Elimina archivo (con confirmación) |



---
## Indicadores de Salud del Sistema
```yaml
Memoria RAM:
  Óptimo: <30% uso
  Aceptable: 30-60% uso
  Crítico: >70% uso

Temperatura:
  Normal: <50°C
  Elevada: 50-70°C
  Crítica: >70°C

GPIOs:
  Funcional: Respuesta correcta HIGH/LOW
  Problemático: Stuck-at o respuesta errática
```

### Diagnóstico de Problemas

#### Errores Comunes y Soluciones

| Síntoma | Causa Probable | Solución |
|---------|----------------|-----------|
| "SPIFFS Error" | Sistema archivos corrupto | Reformatear SPIFFS desde IDE |
| "GPIO Problemático" | Pin dañado o en uso | Verificar conexiones físicas |
| "WiFi Scan Failed" | Interferencia o hardware | Reiniciar y cambiar ubicación |
| "Memory Warning" | Fragmentación excesiva | Ejecutar comando 'C' y reiniciar |
| "BLE Init Failed" | Conflicto de recursos | Reiniciar sistema |

## Casos de Uso Prácticos

### Desarrollo y Prototipado
```bash
# Verificación de nueva placa ESP32-C3
1. Ejecutar diagnóstico completo (comando '9')
2. Verificar todos los GPIOs funcionales
3. Confirmar conectividad WiFi/BLE
4. Validar rendimiento con benchmarks
5. Exportar informe para documentación
```

### Depuración de Hardware
```bash
# Identificar componente defectuoso
1. Test individual por subsistema (comandos 1-8)
2. Comparar resultados con valores de referencia
3. Aislar componente problemático
4. Documentar hallazgos con exportación
```

### Monitoreo de Producción
```bash
# Verificación pre-despliegue
1. Análisis térmico (comando '6')
2. Verificación de conectividad (comando '3', 'A')
3. Test de estabilidad de memoria (comando '2')
4. Benchmark de rendimiento (comando '8')
```

## Personalización y Extensión

### Modificación de Tests
```cpp
// Agregar nuevo GPIO test
int customGPIOs[] = {11, 12, 13}; // Pines específicos del proyecto
// Modificar función explorarGPIOs()
```

### Configuración de Red
```cpp
// Cambiar credenciales del AP
const char* ap_ssid = "MiProyecto-ESP32";
const char* ap_password = "MiPassword123";
```

### Personalización de Benchmarks
```cpp
// Ajustar iteraciones según necesidades
const int MATH_ITERATIONS = 20000;  // Más iteraciones = mayor precisión
const int GPIO_ITERATIONS = 10000;  // Ajustar según tiempo disponible
```
