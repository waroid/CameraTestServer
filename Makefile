DEPEND_FILE = depend_file
BINDIR = /usr/local/bin

CC = gcc
CXX = g++
CFLAGS = -D__RASPBERRY__
CXXFLAGS = -D__RASPBERRY__

INCPATH = 
LINK = g++
LIBS = -lm -lpthread

# FILE
CSOURCES = 

CPPSOURCES = main.cpp \
	core/Logger.cpp

OBJECTS = $(CSOURCES:%.c=%.o)
OBJECTS += $(CPPSOURCES:%.cpp=%.o)

TARGET = CameraTestServer

# Implict rules
.SUFFIXES : .cpp .c

.cpp.o: $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.c.o: $(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

.PHONY: all clean depend install uninstall kill

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPEND_FILE)

depend:
	#$(CC) -MM $(CSOURCES) > $(DEPEND_FILE)
	$(CXX) -MM $(CPPSOURCES) > $(DEPEND_FILE)

rebuild: clean depend all

install:
	cp -f $(TARGET) $(BINDIR)

uninstall:
	- killall $(TARGET)
	rm -f $(BINDIR)/$(TARGET)

kill:
	- killall $(TARGET)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(MAKECMDGOALS), install)
ifneq ($(MAKECMDGOALS), uninstall)
ifneq ($(CSOURCES), )
ifneq ($(CPPSOURCE), )
-include $(DEPEND_FILE)
endif
endif
endif
endif
endif
endif
