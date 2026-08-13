# 1. Preparacion del entorno

## Requisitos

- GD32VW553HMQ6/HMQ7 y LED PC13;
- WCH-Link o CMSIS-DAP conectado por JTAG;
- VS Code normal, CMake y Ninja;
- Nuclei RISC-V GCC/GDB;
- OpenOCD y `GD32VW55x_Firmware_Library_V1.6.0`.

La instalacion completa esta en `gd32vw553-vscode-cmake-guide`.

## Abrir y configurar

```powershell
cd C:\ruta\Estructuras_Computacionales_GD32\05_Interrupt_Event_Flags
code .
Copy-Item `
  ..\GD32VW553_VSCode_CMake_Guide\tools\local_config.ps1 `
  .\tools\local_config.ps1
```

Si no existe la guia contigua, copie y edite el ejemplo:

```powershell
Copy-Item .\tools\local_config.example.ps1 .\tools\local_config.ps1
notepad .\tools\local_config.ps1
```

No publique `tools/local_config.ps1`.

## Verificar

```powershell
powershell -NoProfile -ExecutionPolicy Bypass `
  -File .\tools\verify_environment.ps1
```

Todos los componentes deben aparecer como `[OK]`.

## Extensiones

- `marus25.cortex-debug`;
- `ms-vscode.cpptools`;
- `ms-vscode.cmake-tools`.
