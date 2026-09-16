# 6. Implementaciones definitivas

La referencia conserva el bitmask atomico C y separa ISR, publicacion, claim y
despacho.

La ruta Assembly pura implementa GPIO, SysTimer/ECLIC, publicacion mediante
`amoor.w.aqrl`, claim mediante `amoswap.w.aqrl`, prioridades y contadores.

La ruta FreeRTOS usa una tarea productora, una consumidora y Event Groups. No
llama la implementacion original.
