# Eventos atomicos en RISC-V puro

main.S configura GPIOC, SysTimer y ECLIC, publica eventos desde la ISR con
amoor.w.aqrl y los reclama desde main con amoswap.w.aqrl. Toda la logica de
aplicacion, despacho, modos y contadores esta escrita en Assembly.
