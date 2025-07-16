# Regla por defecto: compilar el ejecutable t1.exe
all: t1.exe

# Regla para compilar t1.cpp en el ejecutable t1.exe
t1.exe: t1.cpp
	g++ -std=c++11 -pthread -Wall -o t1.exe t1.cpp

# Regla para limpiar los archivos generados (el ejecutable)
clean:
	rm -f t1.exe
