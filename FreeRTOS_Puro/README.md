# Eventos con FreeRTOS puro

La aplicacion usa Event Groups: una tarea publica bits cada 250 ms y otra se
bloquea en xEventGroupWaitBits, reclama y limpia los eventos. Conserva los
periodos de 250, 1000 y 5000 ms sin llamar la referencia original.
