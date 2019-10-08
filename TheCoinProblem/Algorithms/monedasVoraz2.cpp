/*
	Problema de las monedas

	Implementación con método voraz

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
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <unordered_map>
#include <vector>

using namespace std;

// Constantes que indican el intervalo del número de iteraciones del bucle
int const IT_MIN = 10;
int const IT_MAX = 15;

// De cuánto en cuánto se va iterando el bucle
int const SUMA = 1;

// Constantes que indican el intervalo en el que queremos que se mueva el random
int const RA_MIN = 0;
int const RA_MAX = 3;

/*
  Función que resuelve el problema de las monedas con el método voraz,
  teniendo en cuenta las hipótesis necesarias:
  1. Los tipos de monedas están ordenados en orden estrictamente creciente
  2. Cantidad de monedas disponible de cada tipo ilimitada
  3. La moneda de valor más bajo es la de valor 1, y siempre está
  4. Cada tipo de moneda es múltiplo del anterior
  5. El número de tipos de monedas sea finito

  Recibe como parámetros:
  M, que es el vector que contiene los tipos de monedas
  C, que es la cantidad que queremos alcanzar
  totalutilizadas, que es el número total de monedas que utilizamos
  w, que es un vector de pares que almacena cuántas monedas de cada tipo utilizamos

  El algoritmo consiste en recorrer desde el final el vector con los tipos de monedas ordenado crecientemente,
  de forma que primero miramos los tipos de monedas más grandes, y nos vamos quedando con todas las más grandes
  que nos caben hasta llegar a alcanzar la cantidad C
*/
vector<int> monedasVoraz2(vector<int> const &M, int C, int &totalutilizadas, vector<pair<int, int>> &w) {
	// Vector de soluciones (tipos de monedas que usamos)
	vector<int> sol(M.size());
	// Lo que nos falta para alcanzar C
	int falta = C;
	// Variable iteradora para recorrer el bucle que mira los tipos de monedas
	int i = M.size() - 1;

	// Mientras no hayamos alcanzado la cantidad y no hayamos terminado de mirar todos los tipos de monedas
	while (falta != 0 && i > -1) {
		// En sol[i] guardamos el número de monedas que vamos a utilizar del tipo que se encuentra en M[i]
		sol[i] = falta / M[i];

		// Actualizamos el número total de monedas que llevamos hasta el momento
		totalutilizadas += sol[i];

		// Si hemos metido más de 0 monedas de un tipo, lo incluímos en nuestro vector de cantidades y tipos
		if (sol[i] > 0)
			w.push_back({ sol[i], M[i] });

		// Actualizamos la cantidad que nos falta
		falta = falta % M[i];

		// Para ir recorriendo el bucle
		i--;
	}

	// Finalmente devolvemos el vector con las soluciones
	return sol;
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

	// Creamos nuestra moneda, que inicializamos a 1 (pues es una de las condiciones iniciales)
	//int valorMoneda = 1;
	int valorMoneda;

	// Creamos el número por el que vamos a multiplicar para conseguir el siguiente tipo 
	int multiplicador;

	// Creamos el vector que va a almacenar los tipos de monedas
	vector<int> tiposMonedas;

	// Para cuando queremos insertar la entrada cumpliendo las hipótesis
	// Recorremos el vector insertando los tipos de monedas
	/*for (int i = 0; i < N; i++) {
		tiposMonedas.push_back(valorMoneda);

		// Al multiplicador le damos un valor aleatorio entre 0 y 2
		multiplicador = 2 + rand() % 3;

		// Le damos valor a la siguiente moneda que vamos a insertar, de forma que sea múltiplo del anterior
		valorMoneda *= multiplicador;
	}*/

	// Para cuando queremos insertar la entrada sin cumplir las hipótesis
	// Recorremos el vector insertando los tipos de monedas
	for (int i = 0; i < N; ++i) {

		// Le damos un valor aleatorio (entre 1 y 4 * N) a la moneda que vamos a insertar
		valorMoneda = 1 + rand() % (4 * N);

		// Cuando ya sabemos seguro que la moneda no está entre las que teníamos, la insertamos
		// en el vector que almacena los tipos de moneda
		tiposMonedas.push_back(valorMoneda);
	}

	// Creamos el vector en el que vamos a devolver cuántas monedas de cada tipo hemos utilizado
	vector <pair<int, int>> w;

	// Le damos un valor a C, que es la cantidad que queremos alcanzar para resolver el problema
	// En este caso el valor elegido es 2^N
	//int C = pow(2, N);
	int C = 2 * N;

	// Inicializamos el número total de monedas utilizadas a 0
	int totalutilizadas = 0;

	// Nos guardamos el tiempo justo antes de realizar la prueba
	int t0 = clock();

	// Realizamos la prueba llamando a la función que calcula la solución
	monedasVoraz2(tiposMonedas, C, totalutilizadas, w);

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
	/*for (auto m : w) {
		if (m.first > 0)
			salida << m.first << " monedas del tipo " << m.second << '\n';
	}*/

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
	/*for (auto m : w) {
		if (m.first > 0)
			cout << m.first << " " << m.second << '\n';
	}*/

	// Y el tiempo requerido
	cout << "Tiempo requerido = " << tiempo << " segundos" << '\n';
}

/*
  Función que escribe en los ficheros

  - El fichero "SalidaVoraz2" contiene la salida con toda la información
  - El fichero "GraficaXVoraz2" contiene tan solo información de los tamaños de cara a pintar la gráfica
  - El fichero "GraficaYVoraz2" contiene tan solo información de los tiempos de cara a pintar la gráfica
*/
void fichero() {

	// Creamos los ficheros
	ofstream salida("SalidaVoraz2.txt");
	ofstream salidaGraficarX("GraficaXVoraz2.txt");
	ofstream salidaGraficarY("GraficaYVoraz2.txt");

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