/*
	Problema de las monedas

	Implementaci�n con m�todo de ramificaci�n y poda (esquema pesimista-optimista)

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
#include <math.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <unordered_map>
#include <vector>

using namespace std;

// Constantes que indican el intervalo del n�mero de iteraciones del bucle
int const IT_MIN = 10;
int const IT_MAX = 15;

// De cu�nto en cu�nto se va iterando el bucle
int const SUMA = 1;

// Constantes que indican el intervalo en el que queremos que se mueva el random
int const RA_MIN = 1;
int const RA_MAX = 2;

// Nodo con toda la informaci�n necesaria
struct nodo {
	vector<int> sol; // Array de soluciones parciales
	int k;  // Indica el nivel del �rbol explorado
	int cantidad; // Indica la cantidad pagada hasta el momento
	int monedas; // Recuento de monedas utilizadas hasta el momento
	int costeEstimado; // Cantidad estimada, cota optimista, de coste del nodo como soluci�n (prioridad)
};

// Comparador para la cola
class comparador {
public:
	bool operator()(nodo& n1, nodo& n2) {
		return n1.costeEstimado > n2.costeEstimado;
	}
};

/*
	Funci�n que calcula la cota optimista para una cantidad acumulada dada la cantidad total a cubrir
	Recibe como par�metros:
	cantidadTotal, cantidad total a pagar para resolver el problema del cambio
	cantidadAcumulada, cantidad que ya se ha pagado
	monedas, n�mero de monedas ya usado
*/
int calculo_pesimista(int cantidadTotal, int cantidadAcumulada, int monedas) {
	// assert(cantidadTotal >= cantidadRestante);
	return (cantidadTotal - cantidadAcumulada) + monedas;
}

/*
	Funci�n que calcula la cota optimista para una cantidad acumulada dada la cantidad total a cubrir
	Recibe como par�metros:
	cantidadTotal, cantidad total a pagar para resolver el problema del cambio
	cantidadAcumulada, cantidad que ya se ha pagado
	monedas, n�mero de monedas ya usado
	monedaMax, valor de la moneda de mayor valor en el sistema
*/
int calculo_optimista(int cantidadTotal, int cantidadAcumulada, int monedas, int monedaMax) {
	return ((cantidadTotal - cantidadAcumulada) / monedaMax) + monedas;
}

/*
  Funci�n que resuelve el problema de las monedas con el m�todo de ramificaci�n y poda

  Recibe como par�metros:
  M, que es el vector que contiene los tipos de monedas
  C, que es la cantidad que queremos alcanzar
  N, que es el n�mero de tipos de monedas que tenemos
  solucion, que es el array de soluciones parciales
  numNodos, que es el n�mero de nodos explorados

  Devuelve el m�nimo n�mero de monedas necesario para alcanzar la cantidad C

  La poda consiste en descartar la soluci�n que llevamos si esta es peor que una ya encontrada
*/
int monedasRPMinimo(vector<int> const& M, int const & C, int N, vector<int>& solucion, int& numNodos) {
	// Coste mejor, al principio es tomar C monedas unidad
	int coste_mejor = C;
	// Generamos la ra�z
	nodo Y,X; Y.k = N; Y.cantidad = 0; Y.monedas = 0; Y.sol.resize(N, 0);
	Y.costeEstimado = calculo_optimista(C, Y.cantidad, Y.monedas, M[Y.k-1]);
	// Creamos la cola
	priority_queue<nodo, vector<nodo>, comparador> cola;
	cola.push(Y); numNodos = 0;
	while (!cola.empty() && cola.top().costeEstimado <= coste_mejor) {
		numNodos = max( (int)cola.size(), numNodos);
		Y = cola.top(); cola.pop();
		X.k = Y.k - 1; X.sol = Y.sol; // pasamos a considerar el siguiente tipo de moneda
		X.cantidad = Y.cantidad; X.monedas = Y.monedas;
		if (X.k < 0) continue;
		X.costeEstimado = calculo_optimista(C, X.cantidad, X.monedas, M[X.k]);
		// Para cada nivel probamos a usar desde cero monedas hasta el m�ximo posible
		// No tomamos monedas de ese tipo
		if(X.k > 0 && X.costeEstimado <= coste_mejor)
			cola.push(X);
		
		// Consideramos tomar monedas de ese tipo
		for (int i = 0; i < (C-X.cantidad)/ M[X.k]; i++) {
			X.sol[X.k] += 1;
			X.cantidad += M[X.k];
			X.monedas += 1;
			X.costeEstimado = calculo_optimista(C, X.cantidad, X.monedas, M[X.k]);
			if (X.costeEstimado <= coste_mejor) {
				if (X.cantidad == C) {
					coste_mejor = X.monedas;
					solucion = X.sol;
				}
				else {
					int pes = calculo_pesimista(C, X.cantidad, X.monedas);
					coste_mejor = min(pes, coste_mejor);
					cola.push(X);
				}
			}
		}
		
	}

	return coste_mejor;
}


/*
  Funci�n que realiza las pruebas

  Recibe como par�metros:
  N, que es el n�mero total de tipos de monedas que tenemos
  salida, flujo de salida para escribir por fichero toda la informaci�n necesaria sobre la prueba
  salidaGraficaX, flujo de salida para escribir por fichero la informaci�n sobre el tama�o de la prueba
  salidaGraficaY, flujo de salida para escribir por fichero la informaci�n sobre el tiempo que ha durado la prueba
*/
void prueba(int const N, ofstream & salida, ofstream & salidaGraficarX, ofstream & salidaGraficarY) {

	// Generamos la semilla del random en funci�n del tiempo
	srand(time(NULL));

	// Creamos nuestra moneda
	int valorMoneda;

	// Creamos el vector que va a almacenar los tipos de monedas
	// int* tiposMonedas = new int[N];
	vector<int> tiposMonedas(N, 0);

	// A�adimos la moneda unidad
	tiposMonedas[0] = 1;

	// Recorremos el vector insertando los tipos de monedas
	for (int i = 1; i < N; ++i) {


		// Le damos un valor aleatorio (entre 1 y 4 * N) a la moneda que vamos a insertar
		// Cuando ya sabemos seguro que la moneda no est� entre las que ten�amos, la insertamos
		// en el vector que almacena los tipos de moneda
		bool found = false;
		while (!found) {
			valorMoneda = 1 + rand() % (4 * N);
			found = true;
			for (int j = 0; j <= i && found; j++) {
				found = valorMoneda != tiposMonedas[j];
			}
		}
		
		tiposMonedas[i] = valorMoneda;
	}

	sort(tiposMonedas.begin(), tiposMonedas.end());



	// Le damos un valor a C, que es la cantidad que queremos alcanzar para resolver el problema
	// En este caso el valor elegido es 2 * N
	int C = 2 * N;

	int numNodos = 0;
	// int* solucion = new int[N];
	vector<int> solucion(N + 1);

	// Inicializamos el n�mero total de monedas utilizadas a 0
	int totalutilizadas = 0;

	// Nos guardamos el tiempo justo antes de realizar la prueba
	int t0 = clock();

	// Realizamos la prueba llamando a la funci�n que calcula la soluci�n y guard�ndola en el par sol
	int sol = monedasRPMinimo(tiposMonedas, C, N, solucion, numNodos);
	totalutilizadas = sol;
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

	/*salida << "Hemos cogido ";
	for (int i = 0; i < N; i++) {
		if (solucion[i] > 0)
			salida << solucion[i] << " monedas del tipo " << tiposMonedas[i - 1] << '\n';
	}*/

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
	/*for (int i = 0; i < N; i++) {
		if (solucion[i] > 0)
			salida << solucion[i] << " " << tiposMonedas[i - 1] << '\n';
	}
	*/
	// Y el tiempo requerido
	cout << "Tiempo requerido = " << tiempo << " segundos" << '\n';
	cout << "N�mero de nodos: " << numNodos << '\n';
}

/*
  Funci�n que escribe en los ficheros

  - El fichero "SalidaRYP" contiene la salida con toda la informaci�n
  - El fichero "GraficaXRYP" contiene tan solo informaci�n de los tama�os de cara a pintar la gr�fica
  - El fichero "GraficaYRYP" contiene tan solo informaci�n de los tiempos de cara a pintar la gr�fica
*/
void fichero() {

	// Creamos los ficheros
	ofstream salida("SalidaRYP.txt");
	ofstream salidaGraficarX("GraficaXRYP.txt");
	ofstream salidaGraficarY("GraficaYRYP.txt");

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