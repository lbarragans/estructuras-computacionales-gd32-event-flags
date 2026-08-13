# 3. Conceptos y preguntas

## ISR corta

Una rutina de servicio de interrupcion debe completar rapidamente el trabajo
urgente. Aqui limpia la solicitud, actualiza contadores y publica eventos. El
GPIO y la logica de aplicacion permanecen en `main()`.

## Trabajo diferido

Trabajo diferido significa registrar que algo debe hacerse y atenderlo despues
en un contexto menos restrictivo. Es el paso conceptual entre un programa
simple y arquitecturas con colas, planificadores o RTOS.

## Banderas de bits

Cada bit representa un evento. OR permite combinar eventos independientes y
AND permite consultar uno sin afectar los demas.

```c
if ((events & EVENT_ONE_SECOND) != 0U) {
    /* atender evento */
}
```

## Condicion de carrera

Una secuencia corriente de leer, modificar y escribir puede ser interrumpida.
La ISR podria publicar un bit justo entre la lectura y la limpieza de `main`,
y ese bit se perderia. El intercambio atomico elimina esa ventana.

## Banderas frente a contadores o colas

Las banderas indican que un tipo de evento esta pendiente, pero dos eventos
iguales antes de ser atendidos se fusionan en un solo bit. Si cada ocurrencia
importa, debe usarse un contador atomico o una cola.

## Preguntas

1. ¿Por que la ISR no modifica directamente PC13?
2. ¿Que eventos coinciden exactamente a los cinco segundos?
3. ¿Por que el snapshot esperado en ese instante es `0x07`?
4. ¿Que problema evita `__atomic_exchange_n()`?
5. ¿Que significa la extension A de RISC-V?
6. ¿Por que `volatile` por si solo no garantiza atomicidad?
7. ¿Cuando una bandera puede perder informacion cuantitativa?
8. ¿Que diferencia hay entre evento, interrupcion y tarea?
9. ¿Que variable demuestra que `main()` sigue ejecutandose?
10. ¿Como reemplazaria las banderas por una cola circular?

## Actividades

1. Cambie el modo cada tres segundos.
2. Agregue `EVENT_TEN_SECONDS` sin modificar los bits existentes.
3. Abra el archivo LST y localice la instruccion atomica generada.
4. Sustituya temporalmente el intercambio atomico por lectura y limpieza
   separadas y explique la carrera potencial.
