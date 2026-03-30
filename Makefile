all:
	nasm -f bin src/stage1.s -o bin/stage1.bin
	nasm -f bin src/stage2.s -o bin/stage2.bin

	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -T src/linker.ld -o bin/kernel.elf src/kernel.c
	i686-elf-objcopy -O binary bin/kernel.elf bin/kernel.bin

	cat bin/stage1.bin bin/stage2.bin bin/kernel.bin > bin/os.img
	truncate -s 20480 bin/os.img   # pad to 40 sectors (20KB)

	qemu-system-i386 -drive id=disk,file=bin/os.img,format=raw,if=ide -boot order=c