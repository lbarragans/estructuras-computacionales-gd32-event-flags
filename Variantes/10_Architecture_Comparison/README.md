# 10 - Como elegir el mecanismo de eventos

## Flags

Use flags cuando solo importe:

```text
¿ocurrio?
```

Ejemplos:

- refrescar pantalla;
- recalcular estado;
- indicar configuracion pendiente.

## Counters

Use contadores cuando importe:

```text
¿cuantas veces ocurrio?
```

Ejemplo:

- contar pulsos;
- contabilizar muestras no atendidas.

## Queue / ring buffer

Use queue cuando importe:

```text
orden + multiplicidad + payload
```

Ejemplos:

- comandos UART;
- paquetes;
- muestras con timestamp.

## FreeRTOS Event Groups

Utiles cuando varias condiciones booleanas deben compartirse/sincronizar tareas.

## Task Notifications

En un sistema FreeRTOS con un receptor concreto, una task notification puede ser
mas ligera que crear un Event Group o Queue, segun la semantica necesaria.

## Semaforos

Un semaforo no es simplemente "otra bandera".

Se usa principalmente para sincronizacion y, en el caso de mutex, exclusion
mutua/prioridad.

## Regla practica

```text
booleano                 -> flag
conteo                    -> counter
secuencia/datos           -> queue
bits de condicion RTOS    -> Event Group
notificacion a una tarea  -> Task Notification
```

Elegir la primitiva por la informacion que debe conservar, no por popularidad.
