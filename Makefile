

# Global variables:
#

BIN = latexpreview
OBJ = latexpreviewwindow.o latexpreviewapp.o

CC = g++
OUT_DIR = release

CFLAGS = -Wall -O3 -I.
LFLAGS = -Wall -O3 -lboost_system -lboost_iostreams

WXCFLAGS = `wx-config --cxxflags`
WXLFLAGS = `wx-config --libs`


# Build targets:
#

default: all
all:     prepare $(OUT_DIR)/$(BIN)

$(OUT_DIR)/$(BIN): $(addprefix $(OUT_DIR)/, $(OBJ))
	$(CC) $(LFLAGS) -o $(OUT_DIR)/$(BIN) $^ $(WXLFLAGS)

clean:
	rm -f $(OUT_DIR)/*.o *~
	@rmdir --ignore-fail-on-non-empty $(OUT_DIR)

prepare:
	@mkdir -p $(OUT_DIR)


# Build rules:
#

$(OUT_DIR)/%.o: %.cpp %.h
	$(CC) $(CFLAGS) -o $@ -c $< $(WXCFLAGS)



