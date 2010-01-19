CC = arm-agb-elf-gcc
CXX = arm-agb-elf-g++
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)
ARCH = thumb

TARGET = exercise

CFLAGS = -O2 -m$(ARCH) -Wall -I../../libxrc -I../../../../include -I../libimage
LDFLAGS = -m$(ARCH)  ../../libxrc/libxrc-$(ARCH).a ../../../../lib/libaxeso-$(ARCH).a ../../../../lib/libgba-$(ARCH).a

upload: $(TARGET).bin 
	xpcomm $(TARGET).bin ../logic

$(TARGET).bin: $(TARGET).elf
	arm-agb-elf-objcopy -v -O binary $(TARGET).elf $(TARGET).bin

#libxrc-$(ARCH).a:
#	$(MAKE) -C ../../libxrc libxrc-$(ARCH).a

# Normal link line.  Includes fast LUT code
$(TARGET).elf: $(OBJECTS) 
	$(CXX) -o $(TARGET).elf $(OBJECTS) $(LDFLAGS) 

# Use this instead to test using the slow non-LUT stub
#$(TARGET).elf: $(OBJECTS) libxrc-$(ARCH).a 
#	$(CXX) -o $(TARGET).elf $(OBJECTS) ../../libxrc/stubs/stub_lut_rgb2hsv_ov6620.cxx $(LDFLAGS) 


%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<

clean:
	-rm *.o
	-rm *.a
