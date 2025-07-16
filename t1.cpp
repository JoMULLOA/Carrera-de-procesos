#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include <random>
#include <algorithm>

#define RESET "\033[0m"
#define GREEN "\033[32m" //Definir el color verde para este texto en la consola
using namespace std;// Para evitar escribir std:: en cada función

mutex mtx; // Para sincronizar el acceso a la consola
vector<pair<string, int>> resultados; // Guardar resultados de la carrera en un estilo de par de valores (auto, lugar)
int lugar = 1; // Para registrar el lugar de llegada de cada auto

// Función que simula la carrera de un auto
void correrAuto(string id, int distanciaTotal) {
    random_device rd; // Generador de números aleatorios
    mt19937 gen(rd()); // mt19937 es una función de generación de números aleatorios
    uniform_int_distribution<> distAvance(1, 10);   // Avanza entre 1 y 10 metros
    uniform_int_distribution<> distTiempo(100, 500); // Tiempo entre 100ms y 500ms

    int distanciaRecorrida = 0;//Inicializar la distancia recorrida por el auto

    while (distanciaRecorrida < distanciaTotal) {
        int avance = distAvance(gen); //Generar un número aleatorio para el avance del auto, que arriba fue definido entre 1 y 10 metros
        distanciaRecorrida += avance;

        // Verificar si el auto ya terminó la carrera
        if (distanciaRecorrida >= distanciaTotal) {
            mtx.lock();//Bloquear el acceso a la consola
            cout << GREEN << id  << " avanza " << avance << " metros y termina la carrera en el lugar " << lugar << "!\n" << RESET;
            resultados.push_back({id, lugar});//Guardar el resultado de la carrera
            lugar++;//Incrementar el lugar para el siguiente auto
            mtx.unlock();
            break; // Sale del bucle porque ya terminó la carrera
        }

        // Imprimir el progreso de cada auto
        mtx.lock();
        cout << id << " avanza " << avance << " metros (total: " << distanciaRecorrida << " metros)\n";
        mtx.unlock();

        // Dormir por un tiempo aleatorio
        this_thread::sleep_for(chrono::milliseconds(distTiempo(gen)));
    }
}


int main(int argc, char* argv[]) {
    // Verificar si los argumentos son suficientes
    if (argc != 3) {
        cout << "El ingreso debe ser ./t1.exe distanciaCarrera numeroAutos\n";
        return 1;
    }

    // Castear los argumentos a enteros
    int M = atoi(argv[1]); // Distancia total de la carrera
    int N = atoi(argv[2]); // Número de autos

    cout << "Distancia total carrera: " << M << " metros\n";
    cout << "----------------------------\n";

    vector<thread> autos;//Para crear los hilos de los autos
    vector<string> ids;//Para guardar los ids de los autos

    // Crear N autos e iniciarlos
    for (int i = 1; i <= N; ++i) {
        string id = "Auto" + to_string(i);//Crear el id del auto
        ids.push_back(id); //Guardar el id del auto
        autos.emplace_back(correrAuto, id, M); //Crear un hilo para el auto, emplace_back sirve para agregar los argumentos al hilo
    }

    // Esperar a que todos los autos/hilos terminen
    for (auto& autoThread : autos) {
        autoThread.join();
    }

    // Mostrar resultados finales
    mtx.lock();
    cout << "\nLugar  Auto\n";
    cout << "---------------\n";
    for (const auto& resultado : resultados) {
        cout << resultado.second << "      " << resultado.first << "\n";
    }
    mtx.unlock();

    return 0;
}
