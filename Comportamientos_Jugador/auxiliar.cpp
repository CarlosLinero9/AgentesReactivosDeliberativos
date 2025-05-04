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
		accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		accion = ComportamientoAuxiliarNivel_3 (sensores);
		//accion = ComportamientoAuxiliarNivel_E(sensores);
		break;
	case 4:
		accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*PARTE REACTIVA*///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*NIVEL 0*/

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	Action accion;
	SituarSensorenMapaA(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(sensores.superficie[0] == 'X'){
		accion = IDLE;
	}
	else if(giro45izq != 0){
		accion = TURN_SR;
		giro45izq--;
	}
	else {

		int pos = VeoCasillaInteresanteA(sensores, tiene_zapatillas);

		switch (pos){
			case 2:
				accion = WALK;
				//cout << "Avanzo" << endl;
				break;
			case 1:
				giro45izq = 6;
				accion = TURN_SR;
				//cout << "Izqda" << endl;
				break;
			case 3:
				accion = TURN_SR;
				//cout << "Der" << endl;
				break;
			case 0:
				if(accion_defecto){
					accion = TURN_SR;
				}else{
					accion_defecto = true;
					giro45izq = 6;
					accion = TURN_SR;
				}
				//cout << "Defecto" << endl;
				break;
		}
	}

	last_action = accion;
	return accion;
}

int ComportamientoAuxiliar::DetectarCasillaInteresanteA(Sensores &sensores, bool zap) {
    for (int i = 0; i < sensores.superficie.size(); ++i) {
        char casilla = sensores.superficie[i];
        if (((casilla == 'D' and !zap) or casilla == 'X')  and CasillaLibreA(sensores.agentes[i])) {
            return i; // Retorna el índice de la casilla interesante
		}    
    }
    return -1; // No se encontró ninguna casilla interesante
}

// bool ComportamientoAuxiliar::NoVisitaFrecuente(int frecuencia){
// 	// if(frecuencia <= 20) return true;
// 	// else return false;
// }

/*Una primera idea para resolver puede ser esta. 
Tengo que ir perfeccionando cosas.*/
int ComportamientoAuxiliar::VeoCasillaInteresanteA(Sensores &sensores, bool zap){

	char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1]-sensores.cota[0], zap);
	char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2]-sensores.cota[0], zap);
	char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3]-sensores.cota[0], zap);

	bool i_libre = CasillaLibreA(sensores.agentes[1]);
	bool c_libre = CasillaLibreA(sensores.agentes[2]);
	bool d_libre = CasillaLibreA(sensores.agentes[3]);
	/*Primero me aseguro de que se observa alguna casilla interesante*/

	if (c == 'X' and c_libre) return 2;
	else if (i == 'X' and i_libre) return 1;
	else if (d == 'X' and d_libre) return 3;
	else if(!zap) {
		if(c == 'D' and c_libre) return 2;
		else if (i == 'D' and i_libre) return 1;
		else if (d == 'D' and d_libre) return 3;
	}

	if(!cola){
		int indice_interes = DetectarCasillaInteresanteA(sensores, zap);
	
	if(indice_interes != -1){
			cola = true;
			switch(indice_interes){
				case 1:
					cola_acciones.push(1);
					cola_acciones.push(3);
					cola_acciones.push(2);
					break;
				case 2:
					cola_acciones.push(2);
					break;
				case 3:
					cola_acciones.push(3);
					cola_acciones.push(2);
					break;
				case 4:
					cola_acciones.push(1);
					cola_acciones.push(3);
					cola_acciones.push(2);
					cola_acciones.push(2);
					break;
				case 5:
					cola_acciones.push(2);
					cola_acciones.push(1);
					cola_acciones.push(3);
					cola_acciones.push(2);
					break;
				case 6:
					cola_acciones.push(2);
					cola_acciones.push(2);
					break;
				case 7:
					cola_acciones.push(2);
					cola_acciones.push(3);
					cola_acciones.push(2);
					break;
				case 8:
					cola_acciones.push(3);
					cola_acciones.push(2);
					cola_acciones.push(2);
					break;
				case 9:
					cola_acciones.push(1);
					cola_acciones.push(3);
					cola_acciones.push(2);
					cola_acciones.push(2);
					cola_acciones.push(2);
					break;
				case 10:
					cola_acciones.push(2);
					cola_acciones.push(1);
					cola_acciones.push(3);
					cola_acciones.push(2);
					cola_acciones.push(2);
					break;
				case 11:
					cola_acciones.push(2);
					cola_acciones.push(2);
					cola_acciones.push(1);
					cola_acciones.push(3);
					cola_acciones.push(2);
					break;
				case 12:
					cola_acciones.push(2);
					cola_acciones.push(2);
					cola_acciones.push(2);
					break;
				case 13:
					cola_acciones.push(2);
					cola_acciones.push(2);
					cola_acciones.push(3);
					cola_acciones.push(2);
					break;
				case 14:
					cola_acciones.push(2);
					cola_acciones.push(3);
					cola_acciones.push(2);
					cola_acciones.push(2);
					break;
				case 15:
					cola_acciones.push(3);
					cola_acciones.push(2);
					cola_acciones.push(2);
					cola_acciones.push(2);
					break;
			}
		}
	}else{
		if(!cola_acciones.empty()){
			int accion = cola_acciones.front();
			cola_acciones.pop();

			switch (accion) {
				case 1: // TURN_SL
					if (i_libre and (i == 'C' or i == 'D' or i == 'X')) {
						return accion;
					}
					break;
				case 2: // WALK
					if (c_libre and (c == 'C' or c == 'D' or c == 'X')) {
						return accion;
					}
					break;
				case 3: // TURN_SR
					if (d_libre and (d == 'C' or d == 'D' or d == 'X')) {
						return accion;
					}
					break;
				default:
					cola = false;
					cola_acciones = queue<int>();
					return 0; // No se encontró ninguna acción válida
					break;
			}
		}
		else{
			cola = false;
			return 0;
		}
	}


	int frecuencia_i=0;
	int frecuencia_c=0;
	int frecuencia_d=0;	

	vector<int> frecuencia_casillas;

	switch(sensores.rumbo){
		case norte:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case noroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case oeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case suroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case sur:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case sureste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case este:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case noreste:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;
	}

	std::sort(frecuencia_casillas.begin(), frecuencia_casillas.end());  // Ordena de menor a mayor

	for (int freq : frecuencia_casillas) {
		if (freq == frecuencia_c and c_libre and (c == 'C' or c == 'D')) return 2;
		else if (freq == frecuencia_i and i_libre and (i == 'C' or i == 'D')) return 1;
		else if (freq == frecuencia_d and d_libre and (d == 'C' or d == 'D')) return 3;
	}

	return 0;
	
}


char ComportamientoAuxiliar::ViablePorAlturaA(char casilla, int dif, bool zap){
	if (abs(dif)<=1)
		return casilla;
	else 
		return 'P';
}

bool ComportamientoAuxiliar::CasillaLibreA(char casilla){
	if(casilla == '_') return true;
	else return false;
}

void ComportamientoAuxiliar::SituarSensorenMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores){
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

			frecuencia_visita[sensores.posF][sensores.posC]+=SUMA_AL_VISITAR;
			frecuencia_visita[sensores.posF-1][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC+3]+=SUMA_AL_VER;

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

			frecuencia_visita[sensores.posF][sensores.posC]+=SUMA_AL_VISITAR;
			frecuencia_visita[sensores.posF][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC]+=SUMA_AL_VER;


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

			frecuencia_visita[sensores.posF][sensores.posC]+=SUMA_AL_VISITAR;
			frecuencia_visita[sensores.posF+1][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC-3]+=SUMA_AL_VER;


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

			frecuencia_visita[sensores.posF][sensores.posC]+=SUMA_AL_VISITAR;
			frecuencia_visita[sensores.posF][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC]+=SUMA_AL_VER;

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

			frecuencia_visita[sensores.posF][sensores.posC]+=SUMA_AL_VISITAR;
			frecuencia_visita[sensores.posF+1][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC-3]+=SUMA_AL_VER;

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

			frecuencia_visita[sensores.posF][sensores.posC]+=SUMA_AL_VISITAR;
			frecuencia_visita[sensores.posF+1][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC-1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC-2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC-3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC-3]+=SUMA_AL_VER;

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

			frecuencia_visita[sensores.posF][sensores.posC]+=SUMA_AL_VISITAR;
			frecuencia_visita[sensores.posF-1][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+1][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+2][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF+3][sensores.posC+3]+=SUMA_AL_VER;

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

			frecuencia_visita[sensores.posF][sensores.posC]+=SUMA_AL_VISITAR;
			frecuencia_visita[sensores.posF-1][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC+1]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC+2]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-3][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-2][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF-1][sensores.posC+3]+=SUMA_AL_VER;
			frecuencia_visita[sensores.posF][sensores.posC+3]+=SUMA_AL_VER;

			break;
	}
}

/*NIVEL 1*/

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
	Action accion;
	SituarSensorenMapaA(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(giro45izq != 0){
		accion = TURN_SR;
		giro45izq--;
	}
	else {
		
		int pos = VeoCasillaInteresanteA_N1(sensores, tiene_zapatillas);
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
				if(accion_defecto){
					accion = TURN_SR;
				}else{
					accion_defecto = true;
					giro45izq = 6;
					accion = TURN_SR;
				}
				
				break;
		}
	}

	
	pasos++;
	last_action = accion;
	return accion;
}

int ComportamientoAuxiliar::DetectarCasillaZapatillasA(Sensores &sensores, bool zap) {
    for (int i = 0; i < sensores.superficie.size(); ++i) {
        char casilla = sensores.superficie[i];
        if (casilla == 'D' and !zap) {
            return i; // Retorna el índice de la casilla interesante
		}    
    }
    return -1; // No se encontró ninguna casilla interesante
}

bool ComportamientoAuxiliar::EsTransitableA(char casilla){
	
	if(casilla == 'X' or casilla == 'C' or casilla == 'D'
		or casilla == 'S') return true;
	else return false;
	
}

int ComportamientoAuxiliar::VeoCasillaInteresanteA_N1(Sensores &sensores, bool zap){

	char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1]-sensores.cota[0], zap);
	char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2]-sensores.cota[0], zap);
	char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3]-sensores.cota[0], zap);

	bool i_libre = CasillaLibreA(sensores.agentes[1]);
	bool c_libre = CasillaLibreA(sensores.agentes[2]);
	bool d_libre = CasillaLibreA(sensores.agentes[3]);
	//cout << "i: " << i << " c: " << c << " d: " << d << endl;

	if(!zap) {
		if(c == 'D' and c_libre) return 2;
		else if (i == 'D' and i_libre) return 1;
		else if (d == 'D' and d_libre) return 3;
	}

	if(!zap){
		int indice_interes = DetectarCasillaZapatillasA(sensores, zap);

		if(indice_interes != -1){
			switch(indice_interes){
				case 1:
				case 4:
				case 9:
					//Aquí debería de meter algo de si es transitable o no
					if(EsTransitableA(sensores.superficie[1]) and i_libre and i != 'P') return 1;
					break;

				case 3:
				case 8:
				case 15:
					if(EsTransitableA(sensores.superficie[3]) and d_libre and d != 'P') return 3;
					break;

				case 2:
				case 5:
				case 6:
				case 7:
				case 10:
				case 11:
				case 12:
				case 13:	
				case 14:
					if(EsTransitableA(sensores.superficie[2]) and c_libre and c != 'P') return 2;
					break;
			}
		}
	}

	int frecuencia_i=0;
	int frecuencia_c=0;
	int frecuencia_d=0;	

	vector<int> frecuencia_casillas;

	//Debería jugar tmb con los consumos para menearme lo maximo posible.
	//Lo ideal será establecer una jerarquía en funcion del gasto al moverme
	//y así ir a las que gasten menos.

	switch(sensores.rumbo){
		case norte:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			
			break;

		case noroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC];
			
			break;

		case oeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			
			break;

		case suroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC - 1];
			
			break;

		case sur:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			
			break;

		case sureste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC];
			
			break;

		case este:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			
			break;

		case noreste:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC + 1];
			
			
			break;
	}
	frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};

	std::sort(frecuencia_casillas.begin(), frecuencia_casillas.end());  // Ordena de menor a mayor

	for (int freq : frecuencia_casillas) {
		if (freq == frecuencia_c && c_libre and EsTransitableA(sensores.superficie[2]) and c!='P') return 2;
		else if (freq == frecuencia_i && i_libre and EsTransitableA(sensores.superficie[1]) and i!='P') return 1;
		else if (freq == frecuencia_d && d_libre and EsTransitableA(sensores.superficie[3]) and d!='P') return 3;
	}

	return 0;
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*PARTE DELIBERATIVA*////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// /*TUTORIAL*/

void ComportamientoAuxiliar::AnularMatrizA(vector<vector<unsigned char>> &m){
	for(int i=0; i<m.size(); i++){
		for(int j=0; j<m.size(); j++){
			m[i][j] = 0;
		}
	}
}





/*NIVEL 2*/
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores){
	//Debería de definir algún comportamiento para apartarse y no molestar al rescatador
	//De todas formas, el rescatador lo evita en su comportamiento
	Action accion = IDLE;
	return accion;
}



/*NIVEL 3*/

/*Un ejemplo de heurística es la de Chebysev*/
int ComportamientoAuxiliar::Heuristica(const EstadoA_N3 &a, const EstadoA_N3 &b){
	int dx = std::abs(a.f - b.f);   
	int dy = std::abs(a.c - b.c); 

	//return 0;
	if(dx < dy) return dy;
	else return dx;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores){
	Action accion = IDLE;
	if(!hayPlan){
		//Invocar al método de busqueda
		EstadoA_N3 inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan  = AlgoritmoAE(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}
	if(hayPlan and plan.size()>0){
		accion = plan.front();
		auto it = plan.begin();
		it = plan.erase(it);
	}
	if(plan.size()==0){
		hayPlan=false;
	}
	return accion;
}



int ComportamientoAuxiliar::FuncionCoste_A(const Action &accion, const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	int coste = 0;

	switch(accion){
		case WALK: {
			EstadoA_N3 siguiente = NextCasillaAuxiliar(st);
			int dif_altura = (altura[siguiente.f][siguiente.c] - altura[st.f][st.c]);

			int signo;
			if(dif_altura < 0) signo = -1;
			else if(dif_altura > 0) signo = 1;
			else signo = 0;
			switch(terreno[st.f][st.c]){
				case 'A':
					coste = 100 + signo * 10;
					break;
				case 'T':
					coste = 20 + signo * 5;
					break;
				case 'S':
					coste = 2 + signo;
					break;
				default:
					coste = 1;
					break;
			}
			break;
		}
	
	
		case TURN_SR: {
			switch(terreno[st.f][st.c]){
				case 'A':
					coste = 16;
					break;
				case 'T':
					coste = 3;
					break;
				default:
					coste = 1;
					break;
			}
			break;
		}
	}

	return coste;
}

EstadoA_N3 ComportamientoAuxiliar::NextCasillaAuxiliar(const EstadoA_N3 &st){
	EstadoA_N3 siguiente = st;
	
	switch(st.brujula){
		case norte:
			siguiente.f = st.f - 1;
			break;
		case noreste:
			siguiente.f = st.f - 1;
			siguiente.c = st.c + 1;
			break;
		case este:
			siguiente.c = st.c + 1;
			break;
		case sureste:
			siguiente.f = st.f + 1;
			siguiente.c = st.c + 1;
			break;
		case sur:
			siguiente.f = st.f + 1;
			break;
		case suroeste:
			siguiente.f = st.f + 1;
			siguiente.c = st.c - 1;
			break;
		case oeste:
			siguiente.c = st.c - 1;
			break;
		case noroeste:
			siguiente.f = st.f - 1;
			siguiente.c = st.c - 1;
			break;
	}
	return siguiente;
}

EstadoA_N3 ComportamientoAuxiliar::applyA(Action accion, const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoA_N3 next = st;

	switch(accion){
		case WALK: {
			if(CasillaAccesibleAuxiliar(st, terreno, altura)){
				next = NextCasillaAuxiliar(st);
			}
			break;
		}
	
		case TURN_SR: {
			next.brujula = (next.brujula + 1) % 8;
			break;
		}
	}

	if(terreno[next.f][next.c] == 'D'){
		next.zapatillas = true;
	} else {
		next.zapatillas = st.zapatillas;
	}

	return next;
}

bool ComportamientoAuxiliar::CasillaAccesibleAuxiliar(const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
	EstadoA_N3 next = NextCasillaAuxiliar(st);
	bool check1 = false;
	bool check2 = false;
	bool check3 = false;

	check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
	check2 = terreno[next.f][next.c] != 'B' or (terreno[next.f][next.c] == 'B' and st.zapatillas == true);
	check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;

	return check1 and check2 and check3;
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoA_N3 &st, const vector<Action> &plan){
	AnularMatrizA(mapaConPlan);
	EstadoA_N3 cst = st;

	auto it = plan.begin();
	while(it != plan.end()){
		switch(*it){
			case WALK:
				switch(cst.brujula){
					case 0:
						cst.f--;
						break;
					case 1:
						cst.f--;
						cst.c++;
						break;
					case 2:
						cst.c++;
						break;
					case 3:
						cst.f++;
						cst.c++;
						break;
					case 4:
						cst.f++;
						break;
					case 5:
						cst.f++;
						cst.c--;
						break;
					case 6:
						cst.c--;
						break;
					case 7:
						cst.f--;
						cst.c--;
						break;
				}
				mapaConPlan[cst.f][cst.c] = 2;
				break;

			case TURN_SR:
				cst.brujula = (cst.brujula + 1) % 8;
				break;
		}
		it++;
	}
}


vector<Action> ComportamientoAuxiliar::AlgoritmoAE(const EstadoA_N3 &inicio, const EstadoA_N3 &final,
	const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
	
		NodoA_N3 current_node;
		priority_queue<NodoA_N3> frontier;
		set<EstadoA_N3> explored;
		vector<Action> path;
		int iteraciones = 0;

		current_node.estado = inicio;
		current_node.energia = 0;
		current_node.energia_heuristica = 0;
		if(terreno[current_node.estado.f][current_node.estado.c] == 'D'){
			current_node.estado.zapatillas = true;
		}

		frontier.push(current_node);
		bool SolutionFound = (current_node.estado.f == final.f
			and current_node.estado.c == final.c);

		while(!SolutionFound and !frontier.empty()){
			iteraciones++;
			frontier.pop();
			explored.insert(current_node.estado);

			if(current_node.estado.f == final.f and current_node.estado.c == final.c){
				SolutionFound = true;
			}

			// Genero el hijo resultante de aplicar la acción WALK
			NodoA_N3 child_WALK = current_node;
			child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
			child_WALK.secuencia.push_back(WALK);
			child_WALK.energia += FuncionCoste_A(WALK, current_node.estado, terreno, altura);
			child_WALK.energia_heuristica = Heuristica(child_WALK.estado, final);

			if(explored.find(child_WALK.estado) == explored.end()){
				// Se mete en la lista frontier después de añadir a secuencia la acción
				frontier.push(child_WALK);
			}

			// Genero el hijo resultante de aplicar la acción TURN_SR
			if(!SolutionFound){
				NodoA_N3 child_TURN_SR = current_node;
				child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
				child_TURN_SR.secuencia.push_back(TURN_SR);
				child_TURN_SR.energia += FuncionCoste_A(TURN_SR, current_node.estado, terreno, altura);
				child_TURN_SR.energia_heuristica = Heuristica(child_TURN_SR.estado, final);
				 
				if(explored.find(child_TURN_SR.estado) == explored.end()){
					frontier.push(child_TURN_SR);
				}
			}

			// Paso a evaluar el siguiente nodo en la lista "frontier"
			if(!SolutionFound and !frontier.empty()){
				current_node = frontier.top();
				while(explored.find(current_node.estado) != explored.end() and !frontier.empty()){
					frontier.pop();

					if(!frontier.empty())
						current_node = frontier.top();
				}
			}
		}

		if(SolutionFound) path = current_node.secuencia;
		cout << "En abierto hay " << frontier.size() << " nodos\n";
		cout << "En cerrado hay " << explored.size() << " nodos\n";
		cout << "Se han realizado " << iteraciones << " iteraciones\n";
		return path;
}




/*NIVEL 4*/

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores){
	Action accion = IDLE;

	ModificarMapaA(sensores, mapaResultado, mapaCotas);

	if(mapaResultado[sensores.posF][sensores.posC] == 'D') tiene_zapatillas = true;

	if(f_dest != -1 and c_dest != -1){
		if(DestinoEnConoVision(sensores, f_dest, c_dest)){
			hayPlanEnergia = false;
			hayPlan = false;
			plan_N4.clear();
			f_dest = -1;
			c_dest = -1;
			return IDLE;
		}
	}
	
	
	if(sensores.energia < 500){
		cout << "Me quedo sin energia\n";
		if(mapaResultado[sensores.posF][sensores.posC] == 'X'){
			return IDLE;
		}else{
			if(!hayPlanEnergia){
				int distancia = 5000;
				int f;
				int c;
				for(int i = 0; i < mapaResultado.size(); i++){
					for(int j = 0; j < mapaResultado[0].size(); j++){
						if(mapaResultado[i][j] == 'X' and abs(i - sensores.posF) + abs(j - sensores.posC) < distancia){
							distancia = abs(i - sensores.posF) + abs(j - sensores.posC);
							f = i;
							c = j;
						}
					}
				}
				EstadoA_N4 inicio, fin;
				inicio.f = sensores.posF;
				inicio.c = sensores.posC;
				inicio.brujula = sensores.rumbo;
				inicio.zapatillas = tiene_zapatillas;
				fin.f = f;
				fin.c = c;
				f_dest = f;
				c_dest = c;
				current_state = inicio;
				plan_N4  = AlgoritmoAE_N4(inicio, fin, mapaResultado, mapaCotas);
				VisualizaPlan(inicio, plan_N4);
				hayPlanEnergia = plan_N4.size() != 0;

			}
			if(hayPlanEnergia and plan_N4.size()>0){
				accion = plan_N4.front();

				last_state = current_state;
				current_state = applyA(accion, current_state, mapaResultado, mapaCotas);
				if(current_state == last_state){
					accionesProhibidas[last_state].insert(accion);
					hayPlanEnergia = false;
					plan_N4.clear();
					return IDLE;
				}else{
					accion = plan_N4.front();
					auto it = plan_N4.begin();
					it = plan_N4.erase(it);
				}
			}
			if(plan_N4.size()==0 and hayPlanEnergia){
				hayPlanEnergia=false;
			}
			return accion;
		}
	}


	if (sensores.venpaca) {
		//cout << "Venpaca: " << sensores.venpaca << endl;
		if(!hayPlan){
			//cout << "Planeo de Inicio\n";
			EstadoA_N4 inicio, fin;
			inicio.f = sensores.posF;
			inicio.c = sensores.posC;
			inicio.brujula = sensores.rumbo;
			inicio.zapatillas = tiene_zapatillas;
			fin.f = sensores.destinoF;
			fin.c = sensores.destinoC;
			f_dest = sensores.destinoF;
			c_dest = sensores.destinoC;
			//cout << "A" << endl;
			current_state = inicio;
			//cout << "B"	<< endl;
			plan_N4  = AlgoritmoAE_N4(inicio, fin, mapaResultado, mapaCotas);
			//cout << "C" << endl;
			VisualizaPlan(inicio, plan_N4);
			//cout << "D" << endl;
			hayPlan = plan_N4.size() != 0;
			//cout << "E" << endl;
		}
		if(hayPlan and plan_N4.size()>0){

			accion = plan_N4.front();
	
			last_state = current_state;
			current_state = applyA(accion, current_state, mapaResultado, mapaCotas);
			//cout << ((current_state == last_state) ? "No me muevo\n" : "Me muevo\n");
			//cout << current_state.f << " " << current_state.c << " " << current_state.brujula << endl;
			//cout << last_state.f << " " << last_state.c << " " << last_state.brujula << endl;
			if(current_state == last_state){
				//cout << "Cambio de planes\n";
				accionesProhibidas[last_state].insert(accion);
				hayPlan = false;
				plan_N4.clear();
				return IDLE;
			}else{
				accion = plan_N4.front();
				auto it = plan_N4.begin();
				it = plan_N4.erase(it);
			}
		}
		if(plan_N4.size()==0 and hayPlan){
			hayPlan=false;
		}
	}

	
	return accion;
}

vector<Action> ComportamientoAuxiliar::AlgoritmoAE_N4(const EstadoA_N4 &inicio, const EstadoA_N4 &final,
	const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
		NodoA_N4 current_node;
		priority_queue<NodoA_N4> frontier;
		set<EstadoA_N4> explored;
		vector<Action> path;
		int iteraciones = 0;
		//cout << "B_1" << endl;

		current_node.estado = inicio;
		current_node.energia = 0;
		current_node.energia_heuristica = 0;
		if(terreno[current_node.estado.f][current_node.estado.c] == 'D'){
			current_node.estado.zapatillas = true;
		}
		//cout << "B_2" << endl;

		frontier.push(current_node);
		bool SolutionFound = (current_node.estado.f == final.f
			and current_node.estado.c == final.c);
		//cout << "B_3" << endl;

		while(!SolutionFound and !frontier.empty()){

			iteraciones++;
			frontier.pop();
			explored.insert(current_node.estado);

			// if(iteraciones){
			// 	cout << "B_4" << endl;
			// }
			

			if(current_node.estado.f == final.f and current_node.estado.c == final.c){
				SolutionFound = true;
			}
			// if(iteraciones){
			// 	cout << "B_5" << endl;
			// }
			// Genero el hijo resultante de aplicar la acción WALK
			if(EsAccionValidaA(WALK, current_node.estado)){
				//cout << "B_5.1" << endl;
				NodoA_N4 child_WALK = current_node;
				child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
				child_WALK.secuencia.push_back(WALK);
				child_WALK.energia += FuncionCoste_A(WALK, current_node.estado, terreno, altura);
				child_WALK.energia_heuristica = Heuristica(child_WALK.estado, final);

				if(explored.find(child_WALK.estado) == explored.end()){
					// Se mete en la lista frontier después de añadir a secuencia la acción
					frontier.push(child_WALK);
				}
			}
			// if(iteraciones){
			// 	cout << "B_6" << endl;
			// }
			
			// if(iteraciones){
			// 	cout << "B_7" << endl;
			// }

			// Genero el hijo resultante de aplicar la acción TURN_SR
			if(!SolutionFound){
				if(EsAccionValidaA(TURN_SR, current_node.estado)){
					//cout << "B_7.1" << endl;
					// Genero el hijo resultante de aplicar la acción TURN_SR
					NodoA_N4 child_TURN_SR = current_node;
					child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
					child_TURN_SR.secuencia.push_back(TURN_SR);
					child_TURN_SR.energia += FuncionCoste_A(TURN_SR, current_node.estado, terreno, altura);
					child_TURN_SR.energia_heuristica = Heuristica(child_TURN_SR.estado, final);
					 
					if(explored.find(child_TURN_SR.estado) == explored.end()){
						frontier.push(child_TURN_SR);
					}
				}
				// if(iteraciones){
				// 	cout << "B_8" << endl;
				// }
			}
			// if(iteraciones){
			// 	cout << "B_9" << endl;
			// }

			// Paso a evaluar el siguiente nodo en la lista "frontier"
			if(!SolutionFound and !frontier.empty()){
				// if(iteraciones){
				// 	cout << "B_10" << endl;
				// }
				current_node = frontier.top();
				// if(iteraciones == 1){
				// 	cout << "B_11" << endl;
				// }
				while(explored.find(current_node.estado) != explored.end() and !frontier.empty()){
					// if(iteraciones){
					// 	cout << "B_12" << endl;
					// }
					frontier.pop();
					// if(iteraciones == 1){
					// 	cout << "B_13" << endl;
					// }

					if(!frontier.empty())
						current_node = frontier.top();

					// if(iteraciones){
					// 	cout << "B_14" << endl;
					// }
				}
				// if(iteraciones){
				// 	cout << "B_15" << endl;
				// }
			}
			// if(iteraciones){
			// 	cout << "B_16" << endl;
			// }
		}
		
		//cout << "B_17" << endl;
		
		
		if(SolutionFound) path = current_node.secuencia;
		// cout << "En abierto hay " << frontier.size() << " nodos\n";
		// cout << "En cerrado hay " << explored.size() << " nodos\n";
		// cout << "Se han realizado " << iteraciones << " iteraciones\n";
		return path;
}

int ComportamientoAuxiliar::Heuristica(const EstadoA_N4 &a, const EstadoA_N4 &b){
	int dx = std::abs(a.f - b.f);   
	int dy = std::abs(a.c - b.c); 

	//return 0;
	if(dx < dy) return dy;
	else return dx;
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoA_N4 &st, const vector<Action> &plan){
	AnularMatrizA(mapaConPlan);
	EstadoA_N4 cst = st;

	auto it = plan.begin();
	while(it != plan.end()){
		switch(*it){
			
			case WALK:
				switch(cst.brujula){
					case 0:
						cst.f--;
						break;
					case 1:
						cst.f--;
						cst.c++;
						break;
					case 2:
						cst.c++;
						break;
					case 3:
						cst.f++;
						cst.c++;
						break;
					case 4:
						cst.f++;
						break;
					case 5:
						cst.f++;
						cst.c--;
						break;
					case 6:
						cst.c--;
						break;
					case 7:
						cst.f--;
						cst.c--;
						break;
				}
				mapaConPlan[cst.f][cst.c] = 2;
				break;

			case TURN_SR:
				cst.brujula = (Orientacion)((cst.brujula + 1) % 8);
				break;
		}
		it++;
	}
}

int ComportamientoAuxiliar::FuncionCoste_A(const Action &accion, const EstadoA_N4 &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
		int coste = 0;

		switch(accion){
			case WALK: {
				EstadoA_N4 siguiente = NextCasillaAuxiliar(st);
				if(siguiente.f < 3 or siguiente.f >= terreno.size()-3 or siguiente.c < 3 or siguiente.c >= terreno[0].size()-3){
					return 100000;
				}

				int dif_altura = (altura[siguiente.f][siguiente.c] - altura[st.f][st.c]);
				int signo;
				if(dif_altura < 0) signo = -1;
				else if(dif_altura > 0) signo = 1;
				else signo = 0;
	
				
				switch(terreno[st.f][st.c]){
					case 'A':
						coste = 100 + signo * 10;
						break;
					case 'T':
						coste = 20 + signo * 5;
						break;
					case 'S':
						coste = 2 + signo;
						break;
					default:
						coste = 1;
						break;
					}
				
				break;
			}
		
			case TURN_SR: {
				if(st.f < 3 or st.f >= terreno.size()-3 or st.c < 3 or st.c >= terreno[0].size()-3){
					return 100000;
				}

				switch(terreno[st.f][st.c]){
					case 'A':
						coste = 16;
						break;
					case 'T':
						coste = 3;
						break;
					default:
						coste = 1;
						break;
				}
				break;
			}
		}
		
		return coste;
}

EstadoA_N4 ComportamientoAuxiliar::NextCasillaAuxiliar(const EstadoA_N4 &st){
	EstadoA_N4 siguiente = st;
	
	switch(st.brujula){
		case norte:
			siguiente.f = st.f - 1;
			break;
		case noreste:
			siguiente.f = st.f - 1;
			siguiente.c = st.c + 1;
			break;
		case este:
			siguiente.c = st.c + 1;
			break;
		case sureste:
			siguiente.f = st.f + 1;
			siguiente.c = st.c + 1;
			break;
		case sur:
			siguiente.f = st.f + 1;
			break;
		case suroeste:
			siguiente.f = st.f + 1;
			siguiente.c = st.c - 1;
			break;
		case oeste:
			siguiente.c = st.c - 1;
			break;
		case noroeste:
			siguiente.f = st.f - 1;
			siguiente.c = st.c - 1;
			break;
	}
	return siguiente;
}

EstadoA_N4 ComportamientoAuxiliar::applyA(Action accion, const EstadoA_N4 &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
		EstadoA_N4 next = st;

		switch(accion){
			case WALK: {
				EstadoA_N4 auxiliar = NextCasillaAuxiliar(st);
				if(auxiliar.f < 3 or auxiliar.f >= terreno.size()-3 or auxiliar.c < 3 or auxiliar.c >= terreno[0].size()-3){
					return st;
				}

				if(terreno[auxiliar.f][auxiliar.c] == '?'){
					next = auxiliar;
				}else{
					bool check2 = abs(altura[auxiliar.f][auxiliar.c] - altura[st.f][st.c]) <= 1;
					bool check3 = ((terreno[auxiliar.f][auxiliar.c] == 'B') and st.zapatillas == true);
					bool check1 = terreno[auxiliar.f][auxiliar.c] != 'P' and terreno[auxiliar.f][auxiliar.c] != 'M' and terreno[auxiliar.f][auxiliar.c] != 'B';

					if((check1 or check3) and check2){
						next = auxiliar;
					}
				}
				
				break;
			}
		
		
			case TURN_SR: {
				next.brujula = (Orientacion)((next.brujula + 1) % 8);
				break;
			}
		}
		

		if(terreno[next.f][next.c] == 'D'){
			next.zapatillas = true;
		}
	
		return next;
}

void ComportamientoAuxiliar::ModificarMapaA(const Sensores &sensores, vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a){
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

bool ComportamientoAuxiliar::EsAccionValidaA(const Action &accion, const EstadoA_N4 &estado) {
	if (accionesProhibidas[estado].count(accion) > 0) {
        return false; // La acción está prohibida
    }
	return true; // La acción es válida
}

bool ComportamientoAuxiliar::DestinoEnConoVision(const Sensores &sensores, int f_destino, int c_destino) {
    // Obtener las coordenadas del cono de visión
    vector<pair<int, int>> conoVision = ObtenerConoVision(sensores);
    // Coordenadas del destino
    int destinoF = f_destino;
    int destinoC = c_destino;

	//cout << "Destino: " << destinoF << " " << destinoC << endl;

    // Buscar si el destino está en el vector
    for (const auto &coordenada : conoVision) {
		//cout << coordenada.first << " " << coordenada.second << endl;
        if (coordenada.first == destinoF && coordenada.second == destinoC) {
			//cout << "True" << endl;
            return true; // El destino está en el cono de visión
        }
    }

	//cout << "False" << endl;
    return false; // El destino no está en el cono de visión
}

vector<pair<int, int>> ComportamientoAuxiliar::ObtenerConoVision(const Sensores &sensores) {
    vector<pair<int, int>> coordenadasCono;

    // Coordenadas del auxiliar
    int f = sensores.posF;
    int c = sensores.posC;

    // Dependiendo de la orientación (rumbo), calcula las casillas visibles
	switch (sensores.rumbo) {
		case norte:
			coordenadasCono.push_back({f, c});
			coordenadasCono.push_back({f - 1, c - 1});
			coordenadasCono.push_back({f - 1, c});
			coordenadasCono.push_back({f - 1, c + 1});
			coordenadasCono.push_back({f - 2, c - 2});
			coordenadasCono.push_back({f - 2, c - 1});
			coordenadasCono.push_back({f - 2, c});
			coordenadasCono.push_back({f - 2, c + 1});
			coordenadasCono.push_back({f - 2, c + 2});
			coordenadasCono.push_back({f - 3, c - 3});
			coordenadasCono.push_back({f - 3, c - 2});
			coordenadasCono.push_back({f - 3, c - 1});
			coordenadasCono.push_back({f - 3, c});
			coordenadasCono.push_back({f - 3, c + 1});
			coordenadasCono.push_back({f - 3, c + 2});
			coordenadasCono.push_back({f - 3, c + 3});
			break;

		case este:
			coordenadasCono.push_back({f, c});
			coordenadasCono.push_back({f - 1, c + 1});
			coordenadasCono.push_back({f, c + 1});
			coordenadasCono.push_back({f + 1, c + 1});
			coordenadasCono.push_back({f - 2, c + 2});
			coordenadasCono.push_back({f - 1, c + 2});
			coordenadasCono.push_back({f, c + 2});
			coordenadasCono.push_back({f + 1, c + 2});
			coordenadasCono.push_back({f + 2, c + 2});
			coordenadasCono.push_back({f - 3, c + 3});
			coordenadasCono.push_back({f - 2, c + 3});
			coordenadasCono.push_back({f - 1, c + 3});
			coordenadasCono.push_back({f, c + 3});
			coordenadasCono.push_back({f + 1, c + 3});
			coordenadasCono.push_back({f + 2, c + 3});
			coordenadasCono.push_back({f + 3, c + 3});
			break;

		case sur:
			coordenadasCono.push_back({f, c});
			coordenadasCono.push_back({f + 1, c + 1});
			coordenadasCono.push_back({f + 1, c});
			coordenadasCono.push_back({f + 1, c - 1});
			coordenadasCono.push_back({f + 2, c + 2});
			coordenadasCono.push_back({f + 2, c + 1});
			coordenadasCono.push_back({f + 2, c});
			coordenadasCono.push_back({f + 2, c - 1});
			coordenadasCono.push_back({f + 2, c - 2});
			coordenadasCono.push_back({f + 3, c + 3});
			coordenadasCono.push_back({f + 3, c + 2});
			coordenadasCono.push_back({f + 3, c + 1});
			coordenadasCono.push_back({f + 3, c});
			coordenadasCono.push_back({f + 3, c - 1});
			coordenadasCono.push_back({f + 3, c - 2});
			coordenadasCono.push_back({f + 3, c - 3});
			break;

		case oeste:
			coordenadasCono.push_back({f, c});
			coordenadasCono.push_back({f + 1, c - 1});
			coordenadasCono.push_back({f, c - 1});
			coordenadasCono.push_back({f - 1, c - 1});
			coordenadasCono.push_back({f + 2, c - 2});
			coordenadasCono.push_back({f + 1, c - 2});
			coordenadasCono.push_back({f, c - 2});
			coordenadasCono.push_back({f - 1, c - 2});
			coordenadasCono.push_back({f - 2, c - 2});
			coordenadasCono.push_back({f + 3, c - 3});
			coordenadasCono.push_back({f + 2, c - 3});
			coordenadasCono.push_back({f + 1, c - 3});
			coordenadasCono.push_back({f, c - 3});
			coordenadasCono.push_back({f - 1, c - 3});
			coordenadasCono.push_back({f - 2, c - 3});
			coordenadasCono.push_back({f - 3, c - 3});
			break;

		case noreste:
			coordenadasCono.push_back({f, c});
			coordenadasCono.push_back({f - 1, c});
			coordenadasCono.push_back({f - 1, c + 1});
			coordenadasCono.push_back({f, c + 1});
			coordenadasCono.push_back({f - 2, c});
			coordenadasCono.push_back({f - 2, c + 1});
			coordenadasCono.push_back({f - 2, c + 2});
			coordenadasCono.push_back({f - 1, c + 2});
			coordenadasCono.push_back({f, c + 2});
			coordenadasCono.push_back({f - 3, c});
			coordenadasCono.push_back({f - 3, c + 1});
			coordenadasCono.push_back({f - 3, c + 2});
			coordenadasCono.push_back({f - 3, c + 3});
			coordenadasCono.push_back({f - 2, c + 3});
			coordenadasCono.push_back({f - 1, c + 3});
			coordenadasCono.push_back({f, c + 3});
			break;

		case sureste:
			coordenadasCono.push_back({f, c});
			coordenadasCono.push_back({f, c + 1});
			coordenadasCono.push_back({f + 1, c + 1});
			coordenadasCono.push_back({f + 1, c});
			coordenadasCono.push_back({f, c + 2});
			coordenadasCono.push_back({f + 1, c + 2});
			coordenadasCono.push_back({f + 2, c + 2});
			coordenadasCono.push_back({f + 2, c + 1});
			coordenadasCono.push_back({f + 2, c});
			coordenadasCono.push_back({f, c + 3});
			coordenadasCono.push_back({f + 1, c + 3});
			coordenadasCono.push_back({f + 2, c + 3});
			coordenadasCono.push_back({f + 3, c + 3});
			coordenadasCono.push_back({f + 3, c + 2});
			coordenadasCono.push_back({f + 3, c + 1});
			coordenadasCono.push_back({f + 3, c});
			break;

		case suroeste:
			coordenadasCono.push_back({f, c});
			coordenadasCono.push_back({f + 1, c});
			coordenadasCono.push_back({f + 1, c - 1});
			coordenadasCono.push_back({f, c - 1});
			coordenadasCono.push_back({f + 2, c});
			coordenadasCono.push_back({f + 2, c - 1});
			coordenadasCono.push_back({f + 2, c - 2});
			coordenadasCono.push_back({f + 1, c - 2});
			coordenadasCono.push_back({f, c - 2});
			coordenadasCono.push_back({f + 3, c});
			coordenadasCono.push_back({f + 3, c - 1});
			coordenadasCono.push_back({f + 3, c - 2});
			coordenadasCono.push_back({f + 3, c - 3});
			coordenadasCono.push_back({f + 2, c - 3});
			coordenadasCono.push_back({f + 1, c - 3});
			coordenadasCono.push_back({f, c - 3});
			break;

		case noroeste:
			coordenadasCono.push_back({f, c});
			coordenadasCono.push_back({f, c - 1});
			coordenadasCono.push_back({f - 1, c - 1});
			coordenadasCono.push_back({f - 1, c});
			coordenadasCono.push_back({f, c - 2});
			coordenadasCono.push_back({f - 1, c - 2});
			coordenadasCono.push_back({f - 2, c - 2});
			coordenadasCono.push_back({f - 2, c - 1});
			coordenadasCono.push_back({f - 2, c});
			coordenadasCono.push_back({f, c - 3});
			coordenadasCono.push_back({f - 1, c - 3});
			coordenadasCono.push_back({f - 2, c - 3});
			coordenadasCono.push_back({f - 3, c - 3});
			coordenadasCono.push_back({f - 3, c - 2});
			coordenadasCono.push_back({f - 3, c - 1});
			coordenadasCono.push_back({f - 3, c});
			break;
	}

    return coordenadasCono;
}