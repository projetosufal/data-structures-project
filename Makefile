CC = gcc # Compiler to use
OPTIONS = -lm # -g for debug, -O2 for optimise and -Wall additional messages
OBJS = main.c # List of objects to be build
DIR =./src/
BIN = huffman-p2
all:
	${CC} ${OPTIONS} ${INCLUDES} ${DIR}${OBJS} -o ${BIN}
clean:
	rm ${BIN}
