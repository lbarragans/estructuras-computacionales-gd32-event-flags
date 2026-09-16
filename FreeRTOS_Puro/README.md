# Eventos con FreeRTOS puro

La aplicacion usa Event Groups: una tarea publica bits cada 250 ms y otra se
bloquea en xEventGroupWaitBits, reclama y limpia los eventos. Conserva los
periodos de 250, 1000 y 5000 ms sin llamar la referencia original.

La tarea consumidora tambien conmuta el LED PC13: modo lento cada 500 ms y
modo rapido cada 250 ms. El modo cambia cada 5 segundos.

En VS Code use **Terminal > Run Task > Build + Flash FreeRTOS**. La ruta
`GD32_MSDK_ROOT` debe estar definida en `tools/local_config.ps1`.
