#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include "motorlib/util.h"

Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0 (sensores);
		break;
	case 1:
		// accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoAuxiliarNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	Action accion;

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(sensores.superficie[0] == 'X'){
		accion = IDLE;
	}
	else if(giro45izq != 0){
		accion = TURN_SR;
		giro45izq--;
	}
	else {
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1]-sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2]-sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3]-sensores.cota[0]);


		int pos = VeoCasillaInteresanteA(i, c, d);
		switch (pos){
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45izq = 6;
				accion = TURN_SR;
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 0:
				giro45izq = 5;
				accion = TURN_SR;
				break;
		}
	}

	last_action = accion;
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
}


int ComportamientoAuxiliar::VeoCasillaInteresanteA(char i, char c, char d){
	if (c == 'X') return 2;
	else if (i == 'X') return 1;
	else if (d == 'X') return 3;
	else if (c == 'C') return 2;
	else if (i == 'C') return 1;
	else if (d == 'C') return 3;
	else return 0;
}

char ComportamientoAuxiliar::ViablePorAlturaA(char casilla, int dif){
	if (abs(dif)<=1)
		return casilla;
	else 
		return 'P';
}

void ComportamientoAuxiliar::SituarSensorenMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores){
	//cout << "estoy en situarsensor en matriz de mapa\n";

	int pos = 1;

	switch(sensores.rumbo){
		case norte:
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

			break;

		case noreste:
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

			break;

		case este:
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[4];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[5];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[6];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[7];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[8];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[9];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[10];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[11];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[12];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[13];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[14];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[15];

			break;

		case sureste:
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[4];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[5];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[6];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[7];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[8];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[9];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[10];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[11];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[12];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[13];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[14];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[15];
			break;

		case sur:
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[4];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[5];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[6];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[7];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[8];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[9];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[10];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[11];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[12];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[13];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[14];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[15];
			break;

		case suroeste:
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[4];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[5];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[6];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[7];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[8];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[9];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[10];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[11];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[12];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[13];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[14];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[15];
			break;

		case oeste:
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[4];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[5];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[6];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[7];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[8];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[9];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[10];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[11];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[12];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[13];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[14];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[15];
			break;

		case noroeste:
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[1];
			m[sensores.posF][sensores.posC+1] = sensores.superficie[2];
			m[sensores.posF+1][sensores.posC+1] = sensores.superficie[3];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[4];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[5];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[6];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[7];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[8];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[9];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[10];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[11];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[12];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[13];
			m[sensores.posF-1][sensores.posC-1] = sensores.superficie[14];
			m[sensores.posF-1][sensores.posC+1] = sensores.superficie[15];
			break;
	}
}