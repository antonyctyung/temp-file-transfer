1) Plug USB cable into PC <--> FPGA
2) Connect over UART to system. 
    Speed: 115200
    Bits: 8-bit
    Stop-bits: 1-bit
    Parity: None
3) Monitor bootup. Once complete (approx 5 minutes), login with credentials debian:debian.
4) Run `ip a` to get IP address of system.
5) ssh debian@IP for a more stable connection

---

Compiling rc6.c (and rc6.h) for amd64:
`gcc rc6.c -o rc6-amd64`

Compiling rc6.c (and rc6.h) for riscv:
`riscv64-linux-gnu-gcc rc6.c -o rc6-riscv`

Running Executables:
* `./rc6-amd64`
* `./rc6-riscv`
* `./rc6.py`
