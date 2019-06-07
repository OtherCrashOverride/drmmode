all:
	gcc -g main.c -o drmmode -I/usr/include/drm -ldrm
