SOURCES = sample.cpp motion.cpp objectdetector.cpp
TARGET = Run

OBJECTS = ${SOURCES:.cpp=.o}

INCLUDE_DIRS = -I/robotis/Linux/include -I/robotis/Framework/include

CXX = g++
CXXFLAGS += -O2 -DLINUX -Wall $(INCLUDE_DIRS)
#CXXFLAGS += -O2 -DDEBUG -DLINUX -Wall $(INCLUDE_DIRS)
#CXXFLAGS += `pkg-config opencv --cflags --libs`
LFLAGS += -lpthread -ljpeg -lrt

all: $(TARGET)

clean:
	rm -f *.a *.o $(TARGET) core *~ *.so *.lo *.swp

libclean:
	make -C /robotis/Linux/build clean

distclean: clean libclean

darwin.a:
	make -C /robotis/Linux/build

$(TARGET): darwin.a $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) /robotis/Linux/lib/darwin.a -o $(TARGET) $(LFLAGS) `pkg-config --cflags --libs opencv`
	chmod 755 $(TARGET)

