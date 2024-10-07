CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm $(shell sdl2-config --cflags --libs)

# Règles pour les fichiers exécutables
kh_sdl: kh_sdl.c
	$(CC) kh_sdl.c -o kh_sdl $(CFLAGS) $(LDFLAGS)

exo2: exo2.c
	$(CC) exo2.c -o exo2 $(CFLAGS) $(LDFLAGS)

# Règle par défaut pour compiler tous les fichiers
all: kh_sdl exo2

# Nettoyer les fichiers exécutables
clean:
	rm -f kh_sdl exo2
