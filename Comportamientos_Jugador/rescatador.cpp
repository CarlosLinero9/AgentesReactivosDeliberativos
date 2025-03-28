#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0 (sensores);
		break;
	case 1:
		// accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoRescatadorNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoRescatadorNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
{
	return 0;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.

	//Primera versión
	// Action accion;

	// if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	// if(sensores.superficie[0] == 'X'){
	// 	accion = IDLE;
	// }
	// else if(giro45izq != 0){
	// 	accion = TURN_SR;
	// 	giro45izq--;
	// }
	// else if(sensores.superficie[2] == 'C'){
	// 	accion = WALK;
	// }
	// else if(sensores.superficie[1] == 'C'){
	// 	giro45izq = 1;
	// 	accion = TURN_L;
	// }
	// else if(sensores.superficie[3] == 'C'){
	// 	accion = TURN_SR;
	// }
	// else{
	// 	accion = TURN_L;
	// }

	// last_action = accion;
	// return accion;


	//Segunda versión
	// Action accion;

	// if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	// if(sensores.superficie[0] == 'X'){
	// 	accion = IDLE;
	// }
	// else if(giro45izq != 0){
	// 	accion = TURN_SR;
	// 	giro45izq--;
	// }
	// else {
	// 	int pos = VeoCasillaInteresanteR(sensores.superficie[1], sensores.superficie[2], sensores.superficie[3]);
	// 	switch (pos){
	// 		case 2:
	// 			accion = WALK;
	// 			break;
	// 		case 1:
	// 			giro45izq = 1;
	// 			accion = TURN_L;
	// 			break;
	// 		case 3:
	// 			accion = TURN_SR;
	// 			break;
	// 		case 0:
	// 			accion = TURN_L;
	// 			break;
	// 	}
	// }

	// last_action = accion;
	// return accion;


	// //Tercera version
	// Action accion;

	// if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	// if(sensores.superficie[0] == 'X'){
	// 	accion = IDLE;
	// }
	// else if(giro45izq != 0){
	// 	accion = TURN_SR;
	// 	giro45izq--;
	// }
	// else {
	// 	int pos = VeoCasillaInteresanteR(sensores.superficie[1], sensores.superficie[2], sensores.superficie[3], tiene_zapatillas);
	// 	switch (pos){
	// 		case 2:
	// 			accion = WALK;
	// 			break;
	// 		case 1:
	// 			giro45izq = 1;
	// 			accion = TURN_L;
	// 			break;
	// 		case 3:
	// 			accion = TURN_SR;
	// 			break;
	// 		case 0:
	// 			accion = TURN_L;
	// 			break;
	// 	}
	// }

	// last_action = accion;
	// return accion;

	//Cuarta version
	Action accion;
	SituarSensorenMapaR(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(sensores.superficie[0] == 'X'){
		accion = IDLE;
	}
	else if(giro45izq != 0){
		accion = TURN_SR;
		giro45izq--;
	}
	else {
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1]-sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2]-sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3]-sensores.cota[0], tiene_zapatillas);


		int pos = VeoCasillaInteresanteR(i, c, d, tiene_zapatillas);
		switch (pos){
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45izq = 1;
				accion = TURN_L;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				accion = TURN_L;
				break;
		}
	}

	last_action = accion;
	return accion;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}

int ComportamientoRescatador::VeoCasillaInteresanteR(char i, char c, char d){
	if (c == 'X') return 2;
	else if (i == 'X') return 1;
	else if (d == 'X') return 3;
	else if (c == 'C') return 2;
	else if (i == 'C') return 1;
	else if (d == 'C') return 3;
	else return 0;
}

int ComportamientoRescatador::VeoCasillaInteresanteR(char i, char c, char d, bool zap){
	if (c == 'X') return 2;
	else if (i == 'X') return 1;
	else if (d == 'X') return 3;
	else if(!zap) {
		if(c == 'D') return 2;
		else if (i == 'D') return 1;
		else if (d == 'D') return 3;
	}
	if (c == 'C') return 2;
	else if (i == 'C') return 1;
	else if (d == 'C') return 3;
	else return 0;
}

char ComportamientoRescatador::ViablePorAlturaR(char casilla, int dif, bool zap){
	if (abs(dif)<=1 or (zap and abs(dif)<=2))
		return casilla;
	else 
		return 'P';
}

void ComportamientoRescatador::SituarSensorenMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores){
	//cout << "estoy en situarsensor en matriz de mapa\n";

	int pos = 1;

	switch(sensores.rumbo){
		case norte:
			m[sensores.posF][sensores.posC] = sensores.superficie[0];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[1];
			m[sensores.posF-1][sensores.posC] = sensores.superficie[2];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-2][sensores.posC-2] = sensores.superficie[4];
			m[sensores.posF-2][sensores.posC-1] = sensores.superficie[5];
			m[sensores.posF-2][sensores.posC] = sensores.superficie[6];
			m[sensores.posF-2][sensores.posC+1] = sensores.superficie[7];
			m[sensores.posF-2][sensores.posC+2] = sensores.superficie[8];
			m[sensores.posF-3][sensores.posC-3] = sensores.superficie[9];
			m[sensores.posF-3][sensores.posC-2] = sensores.superficie[10];
			m[sensores.posF-3][sensores.posC-1] = sensores.superficie[11];
			m[sensores.posF-3][sensores.posC] = sensores.superficie[12];
			m[sensores.posF-3][sensores.posC+1] = sensores.superficie[13];
			m[sensores.posF-3][sensores.posC+2] = sensores.superficie[14];
			m[sensores.posF-3][sensores.posC+3] = sensores.superficie[15];

			a[sensores.posF][sensores.posC] = sensores.cota[0];
			a[sensores.posF-1][sensores.posC-1] = sensores.cota[1];
			a[sensores.posF-1][sensores.posC] = sensores.cota[2];
			a[sensores.posF-1][sensores.posC+1] = sensores.cota[3];
			a[sensores.posF-2][sensores.posC-2] = sensores.cota[4];
			a[sensores.posF-2][sensores.posC-1] = sensores.cota[5];
			a[sensores.posF-2][sensores.posC] = sensores.cota[6];
			a[sensores.posF-2][sensores.posC+1] = sensores.cota[7];
			a[sensores.posF-2][sensores.posC+2] = sensores.cota[8];
			a[sensores.posF-3][sensores.posC-3] = sensores.cota[9];
			a[sensores.posF-3][sensores.posC-2] = sensores.cota[10];
			a[sensores.posF-3][sensores.posC-1] = sensores.cota[11];
			a[sensores.posF-3][sensores.posC] = sensores.cota[12];
			a[sensores.posF-3][sensores.posC+1] = sensores.cota[13];
			a[sensores.posF-3][sensores.posC+2] = sensores.cota[14];
			a[sensores.posF-3][sensores.posC+3] = sensores.cota[15];

			break;

		case noroeste:
			m[sensores.posF][sensores.posC] = sensores.superficie[0];
			m[sensores.posF][sensores.posC-1] = sensores.superficie[1];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[2];
			m[sensores.posF-1][sensores.posC] = sensores.superficie[3];
			m[sensores.posF][sensores.posC-2] = sensores.superficie[4];
			m[sensores.posF-1][sensores.posC-2] = sensores.superficie[5];
			m[sensores.posF-2][sensores.posC-2] = sensores.superficie[6];
			m[sensores.posF-2][sensores.posC-1] = sensores.superficie[7];
			m[sensores.posF-2][sensores.posC] = sensores.superficie[8];
			m[sensores.posF][sensores.posC-3] = sensores.superficie[9];
			m[sensores.posF-1][sensores.posC-3] = sensores.superficie[10];
			m[sensores.posF-2][sensores.posC-3] = sensores.superficie[11];
			m[sensores.posF-3][sensores.posC-3] = sensores.superficie[12];
			m[sensores.posF-3][sensores.posC-2] = sensores.superficie[13];
			m[sensores.posF-3][sensores.posC-1] = sensores.superficie[14];
			m[sensores.posF-3][sensores.posC] = sensores.superficie[15];

			a[sensores.posF][sensores.posC] = sensores.cota[0];
			a[sensores.posF][sensores.posC-1] = sensores.cota[1];
			a[sensores.posF-1][sensores.posC-1] = sensores.cota[2];
			a[sensores.posF-1][sensores.posC] = sensores.cota[3];
			a[sensores.posF][sensores.posC-2] = sensores.cota[4];
			a[sensores.posF-1][sensores.posC-2] = sensores.cota[5];
			a[sensores.posF-2][sensores.posC-2] = sensores.cota[6];
			a[sensores.posF-2][sensores.posC-1] = sensores.cota[7];
			a[sensores.posF-2][sensores.posC] = sensores.cota[8];
			a[sensores.posF][sensores.posC-3] = sensores.cota[9];
			a[sensores.posF-1][sensores.posC-3] = sensores.cota[10];
			a[sensores.posF-2][sensores.posC-3] = sensores.cota[11];
			a[sensores.posF-3][sensores.posC-3] = sensores.cota[12];
			a[sensores.posF-3][sensores.posC-2] = sensores.cota[13];
			a[sensores.posF-3][sensores.posC-1] = sensores.cota[14];
			a[sensores.posF-3][sensores.posC] = sensores.cota[15];


			break;

		case oeste:
			m[sensores.posF][sensores.posC] = sensores.superficie[0];
			m[sensores.posF+1][sensores.posC-1] = sensores.superficie[1];
			m[sensores.posF][sensores.posC-1] = sensores.superficie[2];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[3];
			m[sensores.posF+2][sensores.posC-2] = sensores.superficie[4];
			m[sensores.posF+1][sensores.posC-2] = sensores.superficie[5];
			m[sensores.posF][sensores.posC-2] = sensores.superficie[6];
			m[sensores.posF-1][sensores.posC-2] = sensores.superficie[7];
			m[sensores.posF-2][sensores.posC-2] = sensores.superficie[8];
			m[sensores.posF+3][sensores.posC-3] = sensores.superficie[9];
			m[sensores.posF+2][sensores.posC-3] = sensores.superficie[10];
			m[sensores.posF+1][sensores.posC-3] = sensores.superficie[11];
			m[sensores.posF][sensores.posC-3] = sensores.superficie[12];
			m[sensores.posF-1][sensores.posC-3] = sensores.superficie[13];
			m[sensores.posF-2][sensores.posC-3] = sensores.superficie[14];
			m[sensores.posF-3][sensores.posC-3] = sensores.superficie[15];

			a[sensores.posF][sensores.posC] = sensores.cota[0];
			a[sensores.posF+1][sensores.posC-1] = sensores.cota[1];
			a[sensores.posF][sensores.posC-1] = sensores.cota[2];
			a[sensores.posF-1][sensores.posC-1] = sensores.cota[3];
			a[sensores.posF+2][sensores.posC-2] = sensores.cota[4];
			a[sensores.posF+1][sensores.posC-2] = sensores.cota[5];
			a[sensores.posF][sensores.posC-2] = sensores.cota[6];
			a[sensores.posF-1][sensores.posC-2] = sensores.cota[7];
			a[sensores.posF-2][sensores.posC-2] = sensores.cota[8];
			a[sensores.posF+3][sensores.posC-3] = sensores.cota[9];
			a[sensores.posF+2][sensores.posC-3] = sensores.cota[10];
			a[sensores.posF+1][sensores.posC-3] = sensores.cota[11];
			a[sensores.posF][sensores.posC-3] = sensores.cota[12];
			a[sensores.posF-1][sensores.posC-3] = sensores.cota[13];
			a[sensores.posF-2][sensores.posC-3] = sensores.cota[14];
			a[sensores.posF-3][sensores.posC-3] = sensores.cota[15];


			break;

		case sureste:
			m[sensores.posF][sensores.posC] = sensores.superficie[0];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC] = sensores.superficie[3];
			m[sensores.posF][sensores.posC+2] = sensores.superficie[4];
			m[sensores.posF+1][sensores.posC+2] = sensores.superficie[5];
			m[sensores.posF+2][sensores.posC+2] = sensores.superficie[6];
			m[sensores.posF+2][sensores.posC+1] = sensores.superficie[7];
			m[sensores.posF+2][sensores.posC] = sensores.superficie[8];
			m[sensores.posF][sensores.posC+3] = sensores.superficie[9];
			m[sensores.posF+1][sensores.posC+3] = sensores.superficie[10];
			m[sensores.posF+2][sensores.posC+3] = sensores.superficie[11];
			m[sensores.posF+3][sensores.posC+3] = sensores.superficie[12];
			m[sensores.posF+3][sensores.posC+2] = sensores.superficie[13];
			m[sensores.posF+3][sensores.posC+1] = sensores.superficie[14];
			m[sensores.posF+3][sensores.posC] = sensores.superficie[15];

			a[sensores.posF][sensores.posC] = sensores.cota[0];
			a[sensores.posF][sensores.posC+1] = sensores.cota[1];
			a[sensores.posF+1][sensores.posC+1] = sensores.cota[2];
			a[sensores.posF+1][sensores.posC] = sensores.cota[3];
			a[sensores.posF][sensores.posC+2] = sensores.cota[4];
			a[sensores.posF+1][sensores.posC+2] = sensores.cota[5];
			a[sensores.posF+2][sensores.posC+2] = sensores.cota[6];
			a[sensores.posF+2][sensores.posC+1] = sensores.cota[7];
			a[sensores.posF+2][sensores.posC] = sensores.cota[8];
			a[sensores.posF][sensores.posC+3] = sensores.cota[9];
			a[sensores.posF+1][sensores.posC+3] = sensores.cota[10];
			a[sensores.posF+2][sensores.posC+3] = sensores.cota[11];
			a[sensores.posF+3][sensores.posC+3] = sensores.cota[12];
			a[sensores.posF+3][sensores.posC+2] = sensores.cota[13];
			a[sensores.posF+3][sensores.posC+1] = sensores.cota[14];
			a[sensores.posF+3][sensores.posC] = sensores.cota[15];

			break;

		case sur:
			m[sensores.posF][sensores.posC] = sensores.superficie[0];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF+1][sensores.posC] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC-1] = sensores.superficie[3];
			m[sensores.posF+2][sensores.posC+2] = sensores.superficie[4];
			m[sensores.posF+2][sensores.posC+1] = sensores.superficie[5];
			m[sensores.posF+2][sensores.posC] = sensores.superficie[6];
			m[sensores.posF+2][sensores.posC-1] = sensores.superficie[7];
			m[sensores.posF+2][sensores.posC-2] = sensores.superficie[8];
			m[sensores.posF+3][sensores.posC+3] = sensores.superficie[9];
			m[sensores.posF+3][sensores.posC+2] = sensores.superficie[10];
			m[sensores.posF+3][sensores.posC+1] = sensores.superficie[11];
			m[sensores.posF+3][sensores.posC] = sensores.superficie[12];
			m[sensores.posF+3][sensores.posC-1] = sensores.superficie[13];
			m[sensores.posF+3][sensores.posC-2] = sensores.superficie[14];
			m[sensores.posF+3][sensores.posC-3] = sensores.superficie[15];

			a[sensores.posF][sensores.posC] = sensores.cota[0];
			a[sensores.posF+1][sensores.posC+1] = sensores.cota[1];
			a[sensores.posF+1][sensores.posC] = sensores.cota[2];
			a[sensores.posF+1][sensores.posC-1] = sensores.cota[3];
			a[sensores.posF+2][sensores.posC+2] = sensores.cota[4];
			a[sensores.posF+2][sensores.posC+1] = sensores.cota[5];
			a[sensores.posF+2][sensores.posC] = sensores.cota[6];
			a[sensores.posF+2][sensores.posC-1] = sensores.cota[7];
			a[sensores.posF+2][sensores.posC-2] = sensores.cota[8];
			a[sensores.posF+3][sensores.posC+3] = sensores.cota[9];
			a[sensores.posF+3][sensores.posC+2] = sensores.cota[10];
			a[sensores.posF+3][sensores.posC+1] = sensores.cota[11];
			a[sensores.posF+3][sensores.posC] = sensores.cota[12];
			a[sensores.posF+3][sensores.posC-1] = sensores.cota[13];
			a[sensores.posF+3][sensores.posC-2] = sensores.cota[14];
			a[sensores.posF+3][sensores.posC-3] = sensores.cota[15];

			break;

		case suroeste:
			m[sensores.posF][sensores.posC] = sensores.superficie[0];
			m[sensores.posF+1][sensores.posC] = sensores.superficie[1];
			m[sensores.posF+1][sensores.posC-1] = sensores.superficie[2];
			m[sensores.posF][sensores.posC-1] = sensores.superficie[3];
			m[sensores.posF+2][sensores.posC] = sensores.superficie[4];
			m[sensores.posF+2][sensores.posC-1] = sensores.superficie[5];
			m[sensores.posF+2][sensores.posC-2] = sensores.superficie[6];
			m[sensores.posF+1][sensores.posC-2] = sensores.superficie[7];
			m[sensores.posF][sensores.posC-2] = sensores.superficie[8];
			m[sensores.posF+3][sensores.posC] = sensores.superficie[9];
			m[sensores.posF+3][sensores.posC-1] = sensores.superficie[10];
			m[sensores.posF+3][sensores.posC-2] = sensores.superficie[11];
			m[sensores.posF+3][sensores.posC-3] = sensores.superficie[12];
			m[sensores.posF+2][sensores.posC-3] = sensores.superficie[13];
			m[sensores.posF+1][sensores.posC-3] = sensores.superficie[14];
			m[sensores.posF][sensores.posC-3] = sensores.superficie[15];

			a[sensores.posF][sensores.posC] = sensores.cota[0];
			a[sensores.posF+1][sensores.posC] = sensores.cota[1];
			a[sensores.posF+1][sensores.posC-1] = sensores.cota[2];
			a[sensores.posF][sensores.posC-1] = sensores.cota[3];
			a[sensores.posF+2][sensores.posC] = sensores.cota[4];
			a[sensores.posF+2][sensores.posC-1] = sensores.cota[5];
			a[sensores.posF+2][sensores.posC-2] = sensores.cota[6];
			a[sensores.posF+1][sensores.posC-2] = sensores.cota[7];
			a[sensores.posF][sensores.posC-2] = sensores.cota[8];
			a[sensores.posF+3][sensores.posC] = sensores.cota[9];
			a[sensores.posF+3][sensores.posC-1] = sensores.cota[10];
			a[sensores.posF+3][sensores.posC-2] = sensores.cota[11];
			a[sensores.posF+3][sensores.posC-3] = sensores.cota[12];
			a[sensores.posF+2][sensores.posC-3] = sensores.cota[13];
			a[sensores.posF+1][sensores.posC-3] = sensores.cota[14];
			a[sensores.posF][sensores.posC-3] = sensores.cota[15];

			break;

		case este:
			m[sensores.posF][sensores.posC] = sensores.superficie[0];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-2][sensores.posC+2] = sensores.superficie[4];
			m[sensores.posF-1][sensores.posC+2] = sensores.superficie[5];
			m[sensores.posF][sensores.posC+2] = sensores.superficie[6];
			m[sensores.posF+1][sensores.posC+2] = sensores.superficie[7];
			m[sensores.posF+2][sensores.posC+2] = sensores.superficie[8];
			m[sensores.posF-3][sensores.posC+3] = sensores.superficie[9];
			m[sensores.posF-2][sensores.posC+3] = sensores.superficie[10];
			m[sensores.posF-1][sensores.posC+3] = sensores.superficie[11];
			m[sensores.posF][sensores.posC+3] = sensores.superficie[12];
			m[sensores.posF+1][sensores.posC+3] = sensores.superficie[13];
			m[sensores.posF+2][sensores.posC+3] = sensores.superficie[14];
			m[sensores.posF+3][sensores.posC+3] = sensores.superficie[15];

			a[sensores.posF][sensores.posC] = sensores.cota[0];
			a[sensores.posF-1][sensores.posC+1] = sensores.cota[1];
			a[sensores.posF][sensores.posC+1] = sensores.cota[2];
			a[sensores.posF+1][sensores.posC+1] = sensores.cota[3];
			a[sensores.posF-2][sensores.posC+2] = sensores.cota[4];
			a[sensores.posF-1][sensores.posC+2] = sensores.cota[5];
			a[sensores.posF][sensores.posC+2] = sensores.cota[6];
			a[sensores.posF+1][sensores.posC+2] = sensores.cota[7];
			a[sensores.posF+2][sensores.posC+2] = sensores.cota[8];
			a[sensores.posF-3][sensores.posC+3] = sensores.cota[9];
			a[sensores.posF-2][sensores.posC+3] = sensores.cota[10];
			a[sensores.posF-1][sensores.posC+3] = sensores.cota[11];
			a[sensores.posF][sensores.posC+3] = sensores.cota[12];
			a[sensores.posF+1][sensores.posC+3] = sensores.cota[13];
			a[sensores.posF+2][sensores.posC+3] = sensores.cota[14];
			a[sensores.posF+3][sensores.posC+3] = sensores.cota[15];

			break;

		case noreste:
			m[sensores.posF][sensores.posC] = sensores.superficie[0];
			m[sensores.posF-1][sensores.posC] = sensores.superficie[1];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-2][sensores.posC] = sensores.superficie[4];
			m[sensores.posF-2][sensores.posC+1] = sensores.superficie[5];
			m[sensores.posF-2][sensores.posC+2] = sensores.superficie[6];
			m[sensores.posF-1][sensores.posC+2] = sensores.superficie[7];
			m[sensores.posF][sensores.posC+2] = sensores.superficie[8];
			m[sensores.posF-3][sensores.posC] = sensores.superficie[9];
			m[sensores.posF-3][sensores.posC+1] = sensores.superficie[10];
			m[sensores.posF-3][sensores.posC+2] = sensores.superficie[11];
			m[sensores.posF-3][sensores.posC+3] = sensores.superficie[12];
			m[sensores.posF-2][sensores.posC+3] = sensores.superficie[13];
			m[sensores.posF-1][sensores.posC+3] = sensores.superficie[14];
			m[sensores.posF][sensores.posC+3] = sensores.superficie[15];

			a[sensores.posF][sensores.posC] = sensores.cota[0];
			a[sensores.posF-1][sensores.posC] = sensores.cota[1];
			a[sensores.posF-1][sensores.posC+1] = sensores.cota[2];
			a[sensores.posF][sensores.posC+1] = sensores.cota[3];
			a[sensores.posF-2][sensores.posC] = sensores.cota[4];
			a[sensores.posF-2][sensores.posC+1] = sensores.cota[5];
			a[sensores.posF-2][sensores.posC+2] = sensores.cota[6];
			a[sensores.posF-1][sensores.posC+2] = sensores.cota[7];
			a[sensores.posF][sensores.posC+2] = sensores.cota[8];
			a[sensores.posF-3][sensores.posC] = sensores.cota[9];
			a[sensores.posF-3][sensores.posC+1] = sensores.cota[10];
			a[sensores.posF-3][sensores.posC+2] = sensores.cota[11];
			a[sensores.posF-3][sensores.posC+3] = sensores.cota[12];
			a[sensores.posF-2][sensores.posC+3] = sensores.cota[13];
			a[sensores.posF-1][sensores.posC+3] = sensores.cota[14];
			a[sensores.posF][sensores.posC+3] = sensores.cota[15];

			break;
	}
}

