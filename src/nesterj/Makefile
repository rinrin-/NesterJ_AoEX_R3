TARGET = NesterJ

PSP_FW_VERSION=500
PSP_LARGE_MEMORY = 1
#BUILD_PRX = 1

OBJS =	nes/cpu/nes6502.o nes/ppu/nes_ppu.o \
	nes/apu/nes_apu.o nes/apu/nes_apu_wrapper.o nes/apu/nes_exsound.o nes/apu/nes_fme7.o\
	nes/apu/nes_mmc5.o nes/apu/nes_n106.o nes/apu/nes_vrc6.o nes/apu/nes_vrc7.o nes/apu/nes_fds.o\
	nes/nes.o nes/nes_config.o nes/nes_mapper.o nes/nes_rom.o nes/snss.o\
	fileio.o nes/nes_crc32.o nes/libsnss/libsnss.o emu_main.o loadromimage.o\
	soundmanager.o screenmanager.o inputmanager.o\
	rewind.o main.o pg.o menu.o filer.o sound.o debug/debug.o string.o menu_submenu.o\
	image.o zip.o ulunzip.o convfile.o\
	../utility/exception.o pggbk.o fat.o nes/unif.o

INCDIR = lib/
CFLAGS = -Wall -O3 -fomit-frame-pointer -mgp32 -mlong32
# -g
# -fstrict-aliasing
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS += -lpspaudio -lpspgu -lpsppower -lpng -lz lib/unziplib/unziplib.a

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = NesterJ AoEX R3 SP
PSP_EBOOT_ICON = res/icon0.png

all: unziplib

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

clean: unziplib_clean clean

unziplib:
	make -C ./lib/unziplib/

unziplib_clean:
	make clean -C ./lib/unziplib/

disasm:
	psp-objdump -d $(OBJS) > NesterJ_all.asm

