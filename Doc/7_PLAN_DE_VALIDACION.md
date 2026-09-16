# 7. Plan de validacion

## Comportamiento comun

- LED tick cada 250 ms;
- evento de segundo cada 1000 ms;
- cambio lento/rapido cada 5000 ms;
- prioridad de despacho: modo, segundo, LED;
- trabajo de fondo activo mientras no hay eventos.

## Referencia

Compilar con `build-debug` y observar flags, snapshots, publicaciones,
segundos, cambios de modo y toggles.

## Assembly

Integrar `main.S` sin C de aplicacion. Verificar extension A, operaciones AMO,
ISR de 1 ms, coalescencia, claim atomico y contadores globales.

## FreeRTOS

Integrar kernel, port, heap, tick y configuracion. Verificar set/wait/clear de
bits, prioridades y bloqueo eficiente del consumidor.

Las tres rutas fueron compiladas, programadas y verificadas fisicamente con
WCH-Link/CMSIS-DAP v2, JTAG a 50 kHz.

## Resultado visible esperado

1. El LED PC13 conmuta lentamente, cada 500 ms, durante 5 segundos.
2. Despues conmuta rapidamente, cada 250 ms, durante 5 segundos.
3. El ciclo lento/rapido se repite indefinidamente.
4. OpenOCD debe terminar con `Programming Finished` y `Verified OK`.

- [x] Original C validado en placa
- [x] Ensamblador RISC-V puro validado en placa
- [x] FreeRTOS Event Groups validado en placa
