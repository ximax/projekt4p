CC=g++

CFLAGS=--std=c++0x -O999 -Wall

all: Main
	$(CC) $(CFLAGS) Main.cpp -o Main -I SFML-2.1/include MapSystem/StaticTiledMap.hpp MapSystem/ItemManager.hpp -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -L SFML-2.1/lib tinyxml/tinyxml.cpp tinyxml/tinyxmlparser.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinystr.cpp MapSystem/StaticTiledMap.cpp MapSystem/ItemManager.cpp
