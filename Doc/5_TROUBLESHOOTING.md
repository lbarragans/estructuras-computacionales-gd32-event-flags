# 5. Solucion de problemas

## No compila el builtin atomico

Confirme que CMake utiliza `-march=rv32imafdc`. La letra `a` habilita la
extension atomica de RISC-V.

## CMake no encuentra el compilador

Revise `tools/local_config.ps1` y ejecute `1. Verificar entorno GD32`. Si
cambio una ruta, elimine la carpeta `build` desde el Explorador y ejecute
`2. Configurar CMake (Debug)` nuevamente.

## `ninja: no work to do`

No es un error: las fuentes no cambiaron desde la ultima compilacion.

## El LED no cambia de velocidad

1. confirme que programo el ELF del proyecto 05;
2. revise `g_isr_count` y `g_mode_changes`;
3. confirme `g_last_event_snapshot = 7` en el limite de cinco segundos;
4. pulse Reset despues de programar.

## Depuracion no inicia con F5

Use `Run and Debug` y el boton verde. Si no aparece la configuracion, ejecute
la tarea `6. Preparar depuracion` y recargue VS Code.

## Archivos que Git debe ignorar

```text
git check-ignore .\build
git check-ignore .\tools\local_config.ps1
git check-ignore .\.vscode\launch.json
```

Estos tres comandos son solo una comprobacion previa para quien vaya a
publicar el repositorio; no forman parte de la practica del estudiante.
