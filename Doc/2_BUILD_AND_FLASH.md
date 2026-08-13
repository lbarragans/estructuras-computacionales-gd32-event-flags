# 2. Compilacion y programacion

## Configurar CMake

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\configure.ps1 -BuildType Debug
```

Debe terminar con `Configuring done` y `Generating done`.

## Compilar

```powershell
cmake --build --preset build-debug
```

Se compilan `Src/main.c`, `Src/events.c` y el SDK. CMake genera ELF, HEX, BIN,
MAP y LST dentro de `build/debug`.

## Programar

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\flash.ps1 -BuildType Debug
```

El resultado correcto contiene:

```text
** Programming Finished **
** Verified OK **
** Resetting Target **
```

## Tarea de VS Code

En `Terminal > Run Task`, ejecute:

```text
Build + Flash GD32 Events
```

El LED debe permanecer cinco segundos lento y cinco segundos rapido.
