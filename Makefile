all:
	mkdir -p bin

	nasm -f bin src/boot/stage1.s -o bin/stage1.bin
	nasm -f bin src/boot/stage2.s -o bin/stage2.bin
	nasm -f elf src/boot/kernel_entry.s -o bin/kernel_entry.o
	nasm -f elf lib/idt/IDT_handler_stubs.s -o bin/IDT_handler_stub.o
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/kernel.o src/kernel/kernel.c
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/screen_services.o lib/screen/screen_services.c
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/stdlib.o lib/stdlib/stdlib.c
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/IDT_handlers.o lib/idt/IDT_handlers.c
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/string.o lib/string/string.c
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/io.o lib/io/io.c
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/pic.o lib/io/pic.c
	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -c -o bin/pmm.o lib/pmm/pmm.c

	i686-elf-gcc -ffreestanding -fno-builtin -nostdlib -T src/linker.ld -o bin/kernel.elf bin/kernel_entry.o bin/kernel.o bin/screen_services.o bin/stdlib.o bin/IDT_handlers.o bin/string.o bin/IDT_handler_stub.o bin/io.o bin/pic.o bin/pmm.o
	i686-elf-objcopy -O binary bin/kernel.elf bin/kernel.bin

	cat bin/stage1.bin bin/stage2.bin bin/kernel.bin > bin/os.img
	truncate -s 1048576 bin/os.img

	qemu-system-i386 -drive id=disk,file=bin/os.img,format=raw,if=ide -boot order=c

clean:
	rm bin/*