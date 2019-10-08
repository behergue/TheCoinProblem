/*
	Problema de las monedas

	Implementaci�n con programaci�n din�mica (matriz)

	M�todos algor�tmicos en Resoluci�n de Problemas
	Facultad de Inform�tica
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

#include "Matriz.h"

using namespace std;

// Constantes que indican el intervalo del n�mero de iteraciones del bucle
int const IT_MIN = 1000;
int const IT_MAX = 4000;

// De cu�nto en cu�nto se va iterando el bucle
int const SUMA = 200;

// Constantes que indican el intervalo en el que queremos que se mueva el random
int const RA_MIN = 1;
int const RA_MAX = 2;

/*
  Funci�n que resuelve el problema de las monedas con programaci�n din�mica,
  teniendo en cuenta las hip�tesis necesarias:
  1. Cantidad de monedas disponible de cada tipo ilimitada
  2. El n�mero de tipos de monedas sea finito

  Recibe como par�metros:
  v, que es el vector que contiene los tipos de monedas
  C, que es la cantidad que queremos alcanzar
  totalutilizadas, que es el n�mero total de monedas que utilizamos

  El algoritmo consiste en, partiendo de unos casos base y bas�ndonos en los casos anteriores, calcular
  la soluci�n �ptima, utilizando para ello la recursi�n

  La funci�n devuelve un par con el n�mero m�nimo de monedas necesario para resolver el problema
  y el vector que contiene cu�ntas monedas del tipo v[i] hemos utilizado para ello
*/
pair<int, vector<int>> monedas(vector<int> const &v, int C, int &totalutilizados){

	// Creamos la variable que almacena el m�nimo n�mero de monedas y lo inicializamos a infinito
	int num = INT_MAX;

	// Guardamos en N el n�mero de tipos de moneda que tenemos
	int N = v.size();

	// Creamos la matriz que utilizaremos para almacenar los datos en la recursi�n
	Matriz <int> M(N + 1, C + 1);

	// CASOS BASE
	// Inicializamos los casos base: la primera fila a infinito
	// y la primera columna a 0 (esto se inicializa directamente al crear la matriz)
	for (int j = 1; j < C + 1; ++j) {
		M[0][j] = INT_MAX;
	}

	// RECURSI�N
	// Recorremos toda la matriz desde la pos (0, 0) hasta la (N, C)
	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < C + 1; j++) {

			// Si la moneda que quiero usar es m�s grande que la cantidad que quiero sumar,
			// no la sumo y me quedo con la soluci�n �ptima que tenia antes
			if (v[i - 1] > j || M[i][j - v[i - 1]] == INT_MAX)
				M[i][j] = M[i - 1][j];

			// Si s� que puedo sumar la moneda, me quedo con el m�nimo de monedas que necesito
			// entre las que necesito si la cojo y las que ten�a antes (sin cogerla)
			else
				M[i][j] = min(M[i - 1][j], M[i][j - v[i - 1]] + 1);
		}
	}

	// Al final, la soluci�n se encuentra en la �ltima pos de la matriz, por lo que la guardo en mi variable num
	num = M[N][C];

	/////////////////////////////////////////////////////////////////////////////
	// Ahora vamos a reconstruir la soluci�n

	// Creamos el vector que va a almacenar cu�ntas monedas del tipo v[i] hemos utilizado
	vector<int> cuantas(N + 1);

	// Si hemos encontrado soluci�n (hay problemas que no la tienen)
	if (num != INT_MAX) {

		// Inicializamos las variables al m�ximo para recorrer la matriz al rev�s
		int i = N;
		int j = C;

		// Mientras no hayamos terminado de recorrer las columnas
		while (j > 0) {

			// Si hemos cogido la moneda que est� en M[i][j], moneda del tipo v[i]
			if (v[i - 1] < j + 1 && M[i][j] != M[i - 1][j]) {

				// Sumamos 1 al n�mero de monedas de dicho tipo que hemos utilizado
				cuantas[i]++;

				// Sumamos 1 al n�mero total de monedas utilizado
				totalutilizados++;

				// Voy al sitio "del que vengo" al haber cogido la moneda en M[i][j]
				j -= v[i - 1];
			}

			// Si ya no cogemos m�s monedas del tipo v[i]
			else {
				i--;
			}
		}
	}

	// Devolvemos el n�mero total de monedas utilizadas y cu�ntas del tipo v[i] hemos usado
	return { num, cuantas };
}

void prueba(int N, ofstream &salida, ofstream &salidaGraficarX, ofstream &salidaGraficarY) {

	// Generamos la semilla del random en funci�n del tiempo
	srand(time(NULL));

	// Creamos nuestra moneda
	int valorMoneda;

	// Creamos el vector que va a almacenar los tipos de monedas
	vector<int> tiposMonedas;

	// Creamos un mapa para llevar la cuenta de los tipos de moneda que introducimos (para no admitir repetidos)
	unordered_map <int, bool> norepes;

	// Recorremos el vector insertando los tipos de monedas
	for (int i = 1; i <= N; ++i) {

		// Le damos un valor aleatorio (entre 1 y 4 * N) a la moneda que vamos a insertar
		valorMoneda = 1 + rand() % (4 * N);

		// Mientras la moneda que hemos generado ya est� entre las que ten�amos, seguimos generando otra de manera aleatoria
		while (norepes.count(valorMoneda))
			valorMoneda = 1 + rand() % (4 * N);

		// Cuando ya sabemos seguro que la moneda no est� entre las que ten�amos, la insertamos
		// tanto en el diccionario donde llevamos las insertadas como en el vector que almacena los tipos de moneda
		norepes.insert({valorMoneda, true});
		tiposMonedas.push_back(valorMoneda);
	}

	// Le damos un valor a C, que es la cantidad que queremos alcanzar para resolver el problema
	// En este caso el valor elegido es 2 * N
	int C = 2 * N;

	// Inicializamos el n�mero total de monedas utilizadas a 0
	int totalutilizadas = 0;

	// Nos guardamos el tiempo justo antes de realizar la prueba
	int t0 = clock();

	// Realizamos la prueba llamando a la funci�n que calcula la soluci�n y guard�ndola en el par sol
	auto sol = monedas(tiposMonedas, C, totalutilizadas);

	// Nos guardamos el tiempo justo al terminar la prueba
	int t1 = clock();

	// Calculamos el tiempo que ha durado la prueba restando el inicial al final
	double tiempo = double(t1 - t0) / CLOCKS_PER_SEC;

	/////////////////////////////////////////////////////////////////////////////
	// Ahora escribimos en los archivos

	// Primero en salida
	salida << "N�mero de tipos de monedas que tenemos: " << N << '\n';
	salida << "Queremos alcanzar la cantidad " << C << '\n';
	salida << "Cantidad total de monedas utilizadas: " << totalutilizadas << '\n';

	salida << "Hemos cogido ";
	for (auto i : sol.second) {
		if (sol.second[i] > 0)
			salida << sol.second[i] << " monedas del tipo " << tiposMonedas[i - 1] << '\n';
	}

	salida << "Tiempo requerido = " << tiempo << " segundos" << '\n';
	salida << "--------------------------\n";

	// Despu�s en gr�ficaX
	salidaGraficarX << N << '\n';

	// Y por �ltimo en gr�ficaY
	salidaGraficarY << fixed;
	salidaGraficarY << setprecision(3) << tiempo << '\n';

	/////////////////////////////////////////////////////////////////////////////

	// Sacamos por pantalla el n�mero de tipos
	cout << "N = " << N << '\n';

	// Cu�ntas de cada tipo
	for (auto i : sol.second) {
		if (sol.second[i] > 0)
			salida << sol.second[i] << " " << tiposMonedas[i - 1] << '\n';
	}

	// Y el tiempo requerido
	cout << "Tiempo requerido = " << tiempo << " segundos" << '\n';
}

/*
  Funci�n que escribe en los ficheros

  - El fichero "SalidaPDMatriz" contiene la salida con toda la informaci�n
  - El fichero "GraficaXPDMatriz" contiene tan solo informaci�n de los tama�os de cara a pintar la gr�fica
  - El fichero "GraficaYPDMatriz" contiene tan solo informaci�n de los tiempos de cara a pintar la gr�fica
*/
void fichero() {

	// Creamos los ficheros
	ofstream salida("SalidaPDMatriz.txt");
	ofstream salidaGraficarX("GraficaXPDMatriz.txt");
	ofstream salidaGraficarY("GraficaYPDMatriz.txt");

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