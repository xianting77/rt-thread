start qemu-system-arm.exe -M mini2440 -kernel ..\bsp\mini2440\rtthread-mini2440.axf -show-cursor -serial telnet:127.0.0.1:1200,server -net nic -net tap,ifname=virtual -sd SDCARD -serial file:virtualkbd
start putty.exe telnet://127.0.0.1:1200/
