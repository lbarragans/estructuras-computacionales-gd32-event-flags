# 5. Solucion de problemas

## No compila el builtin atomico

Confirme que CMake utiliza `-march=rv32imafdc`. La letra `a` habilita la
extension atomica de RISC-V.

## CMake no encuentra el compilador

Ejecute `tools/configure.ps1`, que carga las rutas desde
`tools/local_config.ps1`. Si cambio rutas, elimine `build/` y configure de
nuevo.

## `ninja: no work to do`

No es un error: las fuentes no cambiaron desde la ultima compilacion.

## El LED no cambia de velocidad

1. confirme que programo el ELF del proyecto 05;
2. revise `g_isr_count` y `g_mode_changes`;
3. confirme `g_last_event_snapshot = 7` en el limite de cinco segundos;
4. pulse Reset despues de programar.

## Depuracion no inicia con F5

Use `Run and Debug` y el boton verde. Si no aparece la configuracion, ejecute
`tools/create_debug_config.ps1` y recargue VS Code.

## Archivos que Git debe ignorar

```powershell
git check-ignore .\build
git check-ignore .\tools\local_config.ps1
git check-ignore .\.vscode\launch.json
```
