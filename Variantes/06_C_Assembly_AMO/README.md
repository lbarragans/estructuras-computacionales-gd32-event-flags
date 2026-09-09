# 06 - C + Assembly: atomicos RISC-V AMO

## Lenguajes

C + Assembly RISC-V.

## Publicacion

```asm
amoor.w.aqrl
```

Realiza atomicamente:

```text
memory = old_memory OR mask
```

## Claim

```asm
amoswap.w.aqrl
```

intercambia atomicamente las flags por cero y devuelve el valor anterior.

## Que ensena

- extension A;
- ABI;
- read-modify-write atomico;
- acquire/release;
- relacion entre intrinsecos C e ISA.

## Estado

**Fuente lista; pendiente de integracion/validacion con el toolchain.**
