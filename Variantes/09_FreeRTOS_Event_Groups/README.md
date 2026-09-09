# 09 - FreeRTOS Event Groups

## Lenguaje / tecnologia

C + FreeRTOS.

## Correspondencia

Bitmask manual:

```text
publish OR bits
claim snapshot
```

Event Group:

```text
xEventGroupSetBits
xEventGroupWaitBits
```

## Ventajas

- tareas pueden bloquearse eficientemente;
- espera por uno o varios bits;
- kernel administra sincronizacion.

## Limite conceptual

Los Event Groups siguen teniendo semantica de bits; no son una cola de N
ocurrencias.

## Estado

**Integracion pendiente.**
