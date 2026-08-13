# 1. Preparacion del entorno

## Requisitos

- GD32VW553HMQ6/HMQ7 y LED PC13;
- WCH-Link o CMSIS-DAP conectado por JTAG;
- VS Code normal, CMake y Ninja;
- Nuclei RISC-V GCC/GDB;
- OpenOCD y `GD32VW55x_Firmware_Library_V1.6.0`.

La instalacion completa esta en `gd32vw553-vscode-cmake-guide`.

## Abrir el proyecto

1. Inicie Visual Studio Code normal.
2. Seleccione `File > Open Folder`.
3. Elija la carpeta `05_Interrupt_Event_Flags`.
4. Acepte `Trust the authors` si aparece el modo restringido.

La carpeta mostrada en el Explorador de VS Code debe contener directamente
`CMakeLists.txt`, `Src`, `Inc`, `tools` y `.vscode`.

## Configurar las rutas locales

Desde el Explorador de VS Code:

1. abra `tools`;
2. copie `local_config.example.ps1` y pegue la copia en la misma carpeta;
3. renombre la copia como `local_config.ps1`;
4. abra el archivo y reemplace las tres rutas de ejemplo.

Las variables representan:

| Variable | Ubicacion requerida |
| --- | --- |
| `GD32_SDK_ROOT` | Raiz de `GD32VW55x_Firmware_Library_V1.6.0` |
| `NUCLEI_TOOLCHAIN_DIR` | Carpeta `NucleiRISCVGCC/bin` |
| `OPENOCD_ROOT` | Carpeta que contiene `bin` y `scripts` de OpenOCD |

No agregue `tools/local_config.ps1` a Git: contiene rutas particulares de cada
computador y ya esta incluido en `.gitignore`.

## Verificar sin usar la consola

1. Abra `Terminal > Run Task`.
2. Seleccione `1. Verificar entorno GD32`.
3. Compruebe que cada componente termine con `[OK]`.

La terminal integrada solo muestra el resultado de la tarea; el estudiante no
necesita copiar ni escribir comandos.

## Extensiones

- `marus25.cortex-debug`;
- `ms-vscode.cpptools`;
- `ms-vscode.cmake-tools`.
