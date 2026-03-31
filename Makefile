all:
	nasm -f bin src/stage1.s -o bin/stage1.bin
	nasm -f bin src/stage2.s -o bin/stage2.bin
	nasm -f elf src/kernel_entry.s -o bin/kernel_entry.o

	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/kernel.o src/kernel.c
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -T src/linker.ld -o bin/kernel.elf bin/kernel_entry.o bin/kernel.o
	i686-elf-objcopy -O binary bin/kernel.elf bin/kernel.bin

	cat bin/stage1.bin bin/stage2.bin bin/kernel.bin > bin/os.img
	truncate -s 1048576 bin/os.img

	qemu-system-i386 -drive id=disk,file=bin/os.img,format=raw,if=ide -boot order=c

clean:
	rm bin/*