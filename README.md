# OS

A hobby x86 operating system written from scratch in C and x86 assembly — no HAL, no libc, just a custom bootloader and kernel talking directly to the hardware.

This is a learning project: the goal is to work through the classic stages of OS development (boot, protected mode, interrupts, memory, drivers) and build things up from bare metal.

## Features

- **Custom two-stage bootloader** (`src/boot/bootloader.s`) — loads the kernel from disk and switches the CPU into 32-bit protected mode
- **GDT** (Global Descriptor Table) setup for protected mode segmentation
- **IDT/ISR handling** — interrupt descriptor table with handlers for CPU exceptions (divide-by-zero, double fault, page fault, general protection fault, etc.)
- **PIC remapping** and hardware interrupt support (timer + keyboard IRQs)
- **PIT timer driver** with a callback system — kernel code can register functions to run on a timer tick
- **Physical memory manager (PMM)** — frame allocation/deallocation over a memory map
- **VGA text-mode screen driver** with a scrolling terminal and live keyboard input/cursor display
- **Freestanding C standard library subset** (string/stdlib helpers) for use without libc

## Tech stack

- **C** (freestanding, no libc) for the kernel and drivers
- **x86 assembly** (NASM syntax) for the bootloader, mode switching, and ISR stubs
- **i686-elf** GCC cross-compiler toolchain
- **QEMU** (`qemu-system-i386`) for running/debugging the OS image
- **GNU Make** for the build

## Project layout

```
src/
  boot/          bootloader + kernel entry (assembly)
  kernel/         kernel_main and IDT setup
  linker.ld       kernel linker script
lib/
  gdt/            Global Descriptor Table
  idt/            Interrupt Descriptor Table + handler stubs
  io/             port I/O + PIC (8259) driver
  pmm/            physical memory manager
  screen/         VGA text-mode + terminal
  timer/          PIT driver + timer callbacks
  string/         freestanding string functions
  stdlib/         freestanding stdlib helpers
```

## Building and running

### Prerequisites

- An [i686-elf cross-compiler](https://wiki.osdev.org/GCC_Cross-Compiler) (`i686-elf-gcc`, `i686-elf-objcopy`) on your `PATH`
- [NASM](https://www.nasm.us/)
- [QEMU](https://www.qemu.org/) (`qemu-system-i386`)

### Build and boot in QEMU

```sh
make        # builds bootloader + kernel, links, and produces bin/os.img
make all    # build, then boot the image in QEMU
```

### Debug with GDB

```sh
make debug  # boots QEMU paused, waiting for a GDB connection on :1234
```

Then in another terminal:

```sh
gdb bin/kernel.elf -ex "target remote :1234"
```

### Clean build artifacts

```sh
make clean
```

## Status

Early-stage. Currently boots to protected mode, brings up interrupts, and runs a live scrolling terminal with keyboard input. Not yet implemented: paging/virtual memory, a heap allocator, a filesystem, user mode, or multitasking.
