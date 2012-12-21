

# Global variables:
#

BIN = latexpreview
OBJ = window.o app.o

CC = g++
OUT_DIR = bin

CFLAGS = -Wall -O3 -I.
LFLAGS = -Wall -O3 -lboost_system -lboost_iostreams -lboost_filesystem

WXCFLAGS = `wx-config --cxxflags`
WXLFLAGS = `wx-config --libs`


# Build targets:
#

default: all
all:     prepare $(OUT_DIR)/$(BIN)

$(OUT_DIR)/$(BIN): $(addprefix $(OUT_DIR)/, $(OBJ))
	$(CC) $(LFLAGS) -o $@ $^ $(WXLFLAGS)

clean:
	rm -f $(OUT_DIR)/*.o *~
	@rmdir --ignore-fail-on-non-empty $(OUT_DIR)

prepare:
	@mkdir -p $(OUT_DIR)


# Build rules:
#

$(OUT_DIR)/%.o: %.cpp %.h
	$(CC) $(CFLAGS) -o $@ -c $< $(WXCFLAGS)

