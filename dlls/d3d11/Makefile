+SRCDIR?=build_from_build_dllsXX_YYY
DESTDIR?=../..
CURRENT_DLL=DLL_D3D11
CFLAGS+=-I$(SRCDIR)/../include -I$(SRCDIR)/../../qemu/include -DQEMU_CURRENT_DLL=$(CURRENT_DLL) -O2 -DCONST_VTABLE -DGUEST_BIT=$(GUEST_BIT) -DHOST_BIT=$(HOST_BIT)
WINEGCC=$(DESTDIR)/build/wine-host/tools/winegcc/winegcc -B$(DESTDIR)/build/wine-host/tools/winebuild -I$(DESTDIR)/build/wine-host/include -I$(DESTDIR)/wine/include

VPATH=$(SRCDIR)

all: d3d11.dll qemu_d3d11.dll.so

d3d11.dll: d3d11.spec main_g.o
	$(WINEGCC) -shared --sysroot=../.. -b $(GUEST_CC) $(SRCDIR)/d3d11.spec *_g.o -o d3d11.dll -nodefaultlibs -nostdlib -lwine -lmsvcrt -luuid -ldxguid -lkernel32 -L../../$(WINE_DIR)/libs/wine

qemu_d3d11.dll.so: main_h.o qemu_d3d11.spec
	$(WINEGCC) -shared $(HOST_CC) $(SRCDIR)/qemu_d3d11.spec *_h.o -o qemu_d3d11.dll.so -ld3d11

%_g.o: %.c qemu_d3d11.h Makefile $(SRCDIR)/Makefile
	$(GUEST_CC)-gcc $< -c -o $@ $(CFLAGS) -DQEMU_DLL_GUEST -I../../../wine/include/wine -Wno-format

%_h.o: %.c qemu_d3d11.h Makefile $(SRCDIR)/Makefile
	$(WINEGCC) $(HOST_CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -rf *o *.dll *.so