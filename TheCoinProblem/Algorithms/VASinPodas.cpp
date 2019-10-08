/*
	Problema de las monedas

	Implementación con método de vuelta atrás

	Métodos algorítmicos en Resolución de Problemas
	Facultad de Informática
	Universidad Complutense de Madrid

	Beatriz Herguedas Pinedo
*/

#include <algorithm>
#include <climits>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <unordered_map>
#include <vector>

using namespace std;

// Constantes que indican el intervalo del número de iteraciones del bucle
int const IT_MIN = 10;
int const IT_MAX = 25;

// De cuánto en cuánto se va iterando el bucle
int const SUMA = 1;

// Constantes que indican el intervalo en el que queremos que se mueva el random
int const RA_MIN = 1;
int const RA_MAX = 2;

/*
  Función que resuelve el problema de las monedas con el método de vuelta atrás,
  teniendo en cuenta las hipótesis necesarias:
  1. Los tipos de monedas están ordenados en orden estrictamente decreciente
  2. Cantidad de monedas disponible de cada tipo ilimitada

  Recibe como parámetros:
  C, que es la cantidad que queremos alcanzar
  tipo, que es el tipo de moneda que estamos probando en cada momento
  monedasUsadas, que es el número de tipos de monedas que hemos usado
  M, que es el vector que contiene el sistema monetario o monedas a usar
  
  Devuelve el mínimo número de monedas necesario para alcanzar la cantidad C
*/
int vueltaAtras(int C, int tipo, int monedasUsadas, vector<int> const& M) {
	int  minimo = INT_MAX;
	if (C == 0)
		return monedasUsadas;
	if (tipo < 0)
		return minimo;
	// Probamos para el tipo correspondiente a usar desde 0 hasta el número máximo posible de monedas a usar
	for (int i = 0; i <= C / M[tipo]; i++) {
		if (C - M[tipo] * i >= 0)
			minimo = min(minimo, vueltaAtras(C - M[tipo] * i, tipo - 1, i + monedasUsadas, M));
	}
	return minimo;
}

/*
  Función que realiza las pruebas

  Recibe como parámetros:
  N, que es el número total de tipos de monedas que tenemos
  salida, flujo de salida para escribir por fichero toda la información necesaria sobre la prueba
  salidaGraficaX, flujo de salida para escribir por fichero la información sobre el tamaño de la prueba
  salidaGraficaY, flujo de salida para escribir por fichero la información sobre el tiempo que ha durado la prueba
*/
void prueba(int N, ofstream &salida, ofstream &salidaGraficarX, ofstream &salidaGraficarY) {

	// Generamos la semilla del random en función del tiempo
	srand(time(NULL));

	// Creamos la base (queremos que sea siempre 2 porque si no al hacer las potencias nos salimos de rango)
	int base = 2;

	// Creamos nuestra moneda, que inicializamos al valor de la base
	int valorMoneda = base;

	// Creamos el vector que va a almacenar los tipos de monedas e insertamos un 1 (pues la primera potencia es elevar a 0)
	vector<int> tiposMonedas;
	tiposMonedas.push_back(1);

	// Recorremos el vector insertando los tipos de monedas
	for (int i = 1; i < N; i++) {
		tiposMonedas.push_back(valorMoneda);

		// La siguiente moneda a insertar será la sigiente potencia, es decir, volver a multiplicar por la base
		valorMoneda *= base;
	}

	// Creamos el vector en el que vamos a devolver cuántas monedas de cada tipo hemos utilizado
	vector <pair<int, int>> w;

	// Le damos un valor a C, que es la cantidad que queremos alcanzar para resolver el problema
	// En este caso el valor elegido es 2^N
	int C = 2 * N;

	// Inicializamos el número total de monedas utilizadas a 0
	int totalutilizadas = 0;

	// Nos guardamos el tiempo justo antes de realizar la prueba
	int t0 = clock();

	// Realizamos la prueba llamando a la función que calcula la solución
	totalutilizadas = vueltaAtras(C, N-1, 0, tiposMonedas);



	// Nos guardamos el tiempo justo al terminar la prueba
	int t1 = clock();

	// Calculamos el tiempo que ha durado la prueba restando el inicial al final
	double tiempo = double(t1 - t0) / CLOCKS_PER_SEC;

	/////////////////////////////////////////////////////////////////////////////
	// Ahora escribimos en los archivos

	// Primero en salida
	salida << "Número de tipos de monedas que tenemos: " << N << '\n';
	salida << "Queremos alcanzar la cantidad " << C << '\n';
	salida << "Cantidad total de monedas utilizadas: " << totalutilizadas << '\n';

	salida << "Hemos cogido ";
	for (auto m : w) {
		if (m.first > 0)
			salida << m.first << " monedas del tipo " << m.second << '\n';
	}

	salida << "Tiempo requerido = " << tiempo << " segundos" << '\n';
	salida << "--------------------------\n";

	// Después en gráficaX
	salidaGraficarX << N << '\n';

	// Y por último en gráficaY
	salidaGraficarY << fixed;
	salidaGraficarY << setprecision(3) << tiempo << '\n';

	/////////////////////////////////////////////////////////////////////////////

	// Sacamos por pantalla el número de tipos
	cout << "N = " << N << '\n';

	// Cuántas de cada tipo
	for (auto m : w) {
		if (m.first > 0)
			cout << m.first << " " << m.second << '\n';
	}

	// Y el tiempo requerido
	cout << "Tiempo requerido = " << tiempo << " segundos" << '\n';
}

/*
  Función que escribe en los ficheros

  - El fichero "SalidaVA" contiene la salida con toda la información
  - El fichero "GraficaXVA" contiene tan solo información de los tamaños de cara a pintar la gráfica
  - El fichero "GraficaYVA" contiene tan solo información de los tiempos de cara a pintar la gráfica
*/
void fichero() {

	// Creamos los ficheros
	ofstream salida("SalidaVA.txt");
	ofstream salidaGraficarX("GraficaXVA.txt");
	ofstream salidaGraficarY("GraficaYVA.txt");

	// Hacemos las pruebas en el intervalo de iteraciones decidido
	for (int i = IT_MIN; i < IT_MAX + 1; i = i + SUMA)
		prueba(i, salida, salidaGraficarX, salidaGraficarY);

	// Cerramos los ficheros
	salida.close();
	salidaGraficarX.close();
	salidaGraficarY.close();
}

int main() {
	fichero();
	system("pause");
	return 0;
}