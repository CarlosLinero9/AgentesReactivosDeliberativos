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
		// accion = ComportamientoAuxiliarNivel_4 (sensores);
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

	if(cola){
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

	bool no_visita_frecuente_i = false;
	bool no_visita_frecuente_c = false;
	bool no_visita_frecuente_d = false;
	

	int frecuencia_i=0;
	int frecuencia_c=0;
	int frecuencia_d=0;	

	vector<int> frecuencia_casillas;

	switch(sensores.rumbo){
		case norte:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			// no_visita_frecuente_i = NoVisitaFrecuente(frecuencia_i);
			// no_visita_frecuente_c = NoVisitaFrecuente(frecuencia_c);
			// no_visita_frecuente_d = NoVisitaFrecuente(frecuencia_d);
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case noroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC];
			// no_visita_frecuente_i = NoVisitaFrecuente(frecuencia_i);
			// no_visita_frecuente_c = NoVisitaFrecuente(frecuencia_c);
			// no_visita_frecuente_d = NoVisitaFrecuente(frecuencia_d);
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case oeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			// no_visita_frecuente_i = NoVisitaFrecuente(frecuencia_i);
			// no_visita_frecuente_c = NoVisitaFrecuente(frecuencia_c);
			// no_visita_frecuente_d = NoVisitaFrecuente(frecuencia_d);
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case suroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC - 1];
			// no_visita_frecuente_i = NoVisitaFrecuente(frecuencia_i);
			// no_visita_frecuente_c = NoVisitaFrecuente(frecuencia_c);
			// no_visita_frecuente_d = NoVisitaFrecuente(frecuencia_d);
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case sur:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			// no_visita_frecuente_i = NoVisitaFrecuente(frecuencia_i);
			// no_visita_frecuente_c = NoVisitaFrecuente(frecuencia_c);
			// no_visita_frecuente_d = NoVisitaFrecuente(frecuencia_d);
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case sureste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC];
			// no_visita_frecuente_i = NoVisitaFrecuente(frecuencia_i);
			// no_visita_frecuente_c = NoVisitaFrecuente(frecuencia_c);
			// no_visita_frecuente_d = NoVisitaFrecuente(frecuencia_d);
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case este:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			// no_visita_frecuente_i = NoVisitaFrecuente(frecuencia_i);
			// no_visita_frecuente_c = NoVisitaFrecuente(frecuencia_c);
			// no_visita_frecuente_d = NoVisitaFrecuente(frecuencia_d);
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;

		case noreste:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC + 1];
			// no_visita_frecuente_i = NoVisitaFrecuente(frecuencia_i);
			// no_visita_frecuente_c = NoVisitaFrecuente(frecuencia_c);
			// no_visita_frecuente_d = NoVisitaFrecuente(frecuencia_d);
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d};
			break;
	}

	std::sort(frecuencia_casillas.begin(), frecuencia_casillas.end());  // Ordena de menor a mayor

		
	if (c == 'X' and c_libre) return 2;
	else if (i == 'X' and i_libre) return 1;
	else if (d == 'X' and d_libre) return 3;
	else if(!zap) {
		if(c == 'D' and c_libre) return 2;
		else if (i == 'D' and i_libre) return 1;
		else if (d == 'D' and d_libre) return 3;
	}

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

		if(sensores.posF == objetivo.first and sensores.posC == objetivo.second){
			objetivo.first = -1;
			objetivo.second = -1;
			RecuperarFrecuenciasA();

			int distancia = 5000;

			for(int i = 0; i < mapaResultado.size() -3; i++){
				for(int j = 0; j < mapaResultado[i].size()-3; j++){
					int dist = abs(i - sensores.posF) + abs(j - sensores.posC);
					if(mapaResultado[i][j] == '?' and dist < distancia){
			
						distancia = dist;
						objetivo.first = i;
						objetivo.second = j;
						
					}
				}
			}

			GuardarFrecuenciasA();
			CalcularObjetivoA();
		}

		if(pasos == MAX_PASOS){
			objetivo.first = -1;
			objetivo.second = -1;

			int distancia = 5000;

			for(int i = 0; i < mapaResultado.size() -3; i++){
				for(int j = 0; j < mapaResultado[i].size()-3; j++){
					int dist = abs(i - sensores.posF) + abs(j - sensores.posC);
					if(mapaResultado[i][j] == '?' and dist < distancia){
			
						distancia = dist;
						objetivo.first = i;
						objetivo.second = j;
						
					}
				}
			}
			if(objetivo.first != -1 and objetivo.second != -1){
				GuardarFrecuenciasA();
				CalcularObjetivoA();

				if(sensores.posF == objetivo.first and sensores.posC == objetivo.second){
					objetivo.first = -1;
					objetivo.second = -1;
					RecuperarFrecuenciasA();
				}
			}
			pasos = 0;
		}

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

void ComportamientoAuxiliar::GuardarFrecuenciasA(){
	for(int i = 0; i < frecuencia_visita.size(); i++){
		for(int j = 0; j < frecuencia_visita[i].size(); j++){
			frecuencia_visita_aux[i][j] = frecuencia_visita[i][j];
		}
	}
}

void ComportamientoAuxiliar::RecuperarFrecuenciasA(){
	for(int i = 0; i < frecuencia_visita_aux.size(); i++){
		for(int j = 0; j < frecuencia_visita_aux[i].size(); j++){
			frecuencia_visita[i][j] = frecuencia_visita_aux[i][j];
		}
	}
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

	if (c == 'X' and c_libre) return 2;
	else if (i == 'X' and i_libre) return 1;
	else if (d == 'X' and d_libre) return 3;
	if(!zap) {
		if(c == 'D' and c_libre) return 2;
		else if (i == 'D' and i_libre) return 1;
		else if (d == 'D' and d_libre) return 3;
	}

	for (int freq : frecuencia_casillas) {
		if (freq == frecuencia_c && c_libre and EsTransitableA(sensores.superficie[2]) and c!='P') return 2;
		else if (freq == frecuencia_i && i_libre and EsTransitableA(sensores.superficie[1]) and i!='P') return 1;
		else if (freq == frecuencia_d && d_libre and EsTransitableA(sensores.superficie[3]) and d!='P') return 3;
	}

	return 0;
	
}

void ComportamientoAuxiliar::CalcularObjetivoA(){
	
	// Si se encuentra un objetivo, recalcular las frecuencias
    if (objetivo.first != -1) {
        for (int i = 0; i < frecuencia_visita.size(); i++) {
            for (int j = 0; j < frecuencia_visita[0].size(); j++) {
                if (i == objetivo.first && j == objetivo.second) {
                    frecuencia_visita[i][j] = 0; // El objetivo tiene prioridad máxima
                } else {
                    frecuencia_visita[i][j] = abs(i - objetivo.first) + abs(j - objetivo.second);
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*PARTE DELIBERATIVA*////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*TUTORIAL*/

list<Action> AvanzaSaltosDeCaballoA(){
	list<Action> secuencia;
	secuencia.push_back(WALK);
	secuencia.push_back(WALK);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(WALK);
	return secuencia;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_E(Sensores sensores){
	Action accion = IDLE;
	if(!hayPlan){
		//Invocar al método de busqueda
		EstadoA inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		//plan = AnchuraAuxiliar(inicio, fin, mapaResultado, mapaCotas);
		plan  = AnchuraAuxiliar_V2(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}
	if(hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}
	if(plan.size()==0){
		hayPlan=false;
	}
	return accion;
}

list<Action> ComportamientoAuxiliar::AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final,
    			const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){

		NodoA current_node;
		list<NodoA> frontier;
		list<NodoA> explored;
		list<Action> path;

		current_node.estado = inicio;
		frontier.push_back(current_node);
		bool SolutionFound = (current_node.estado.f == final.f and current_node.estado.c == final.c);

		while(!SolutionFound and !frontier.empty()){
			frontier.pop_front();
			explored.push_back(current_node);

			//Compruebbo si estoy en una casilla de las zapatillas
			if(terreno[current_node.estado.f][current_node.estado.c] == 'D'){
				current_node.estado.zapatillas = true;
			}

			//Genero el hijo resultante de aplicar la accion WALK
			NodoA child_WALK = current_node;
			child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
			if(child_WALK.estado.f == final.f and child_WALK.estado.c == final.c){
				//El hijo generado es solucion
				child_WALK.secuencia.push_back(WALK);
				current_node = child_WALK;
				SolutionFound = true;
			}
			else if(!Find(child_WALK, frontier) and !Find(child_WALK, explored)){
				//Se mete en la lista frontier después de añadir a secuencia la acción
				child_WALK.secuencia.push_back(WALK);
				frontier.push_back(child_WALK);
			}

			//Genero el hijo resultante de aplicar la accion TURN_SR
			if(!SolutionFound){
				NodoA child_TURN_SR = current_node;
				child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
				if(!Find(child_TURN_SR, frontier) and !Find(child_TURN_SR, explored)){
					child_TURN_SR.secuencia.push_back(TURN_SR);
					frontier.push_back(child_TURN_SR);
				}
			}

			//Paso a evaluar el siguiente nodo en la lista "frontier"
			if(!SolutionFound and !frontier.empty()){
				current_node = frontier.front();
				SolutionFound = (current_node.estado.f == final.f and current_node.estado.c == final.c);
			}
		}

		if(SolutionFound) path = current_node.secuencia;
		
		return path;
}

bool ComportamientoAuxiliar::CasillaAccesibleAuxiliar(const EstadoA &st, const vector<vector<unsigned char>> &terreno, 
    const vector<vector<unsigned char>> &altura){
		EstadoA next = NextCasillaAuxiliar(st);
		bool check1 = false;
		bool check2 = false;
		bool check3 = false;

		check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
		check2 = terreno[next.f][next.c] != 'B' or (terreno[next.f][next.c] == 'B' and st.zapatillas);
		check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;

		return check1 and check2 and check3;
}

EstadoA ComportamientoAuxiliar::NextCasillaAuxiliar(const EstadoA &st){
	EstadoA siguiente = st;
	
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

EstadoA ComportamientoAuxiliar::applyA(Action accion, const EstadoA &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoA next = st;
	switch(accion){
		case WALK:
			if(CasillaAccesibleAuxiliar(st, terreno, altura)){
				next = NextCasillaAuxiliar(st);
			}
			break;
		case TURN_SR:
			next.brujula = (next.brujula + 1) % 8;
			break;
	}

	return next;
}

bool ComportamientoAuxiliar::Find(const NodoA &st,const list<NodoA> &lista){
	auto it = lista.begin();
	while(it != lista.end() and !((*it) == st)){
		it++;
	}

	return (it != lista.end());
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoA &st, const list<Action> &plan){
	AnularMatrizA(mapaConPlan);
	EstadoA cst = st;

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

void ComportamientoAuxiliar::AnularMatrizA(vector<vector<unsigned char>> &m){
	for(int i=0; i<m.size(); i++){
		for(int j=0; j<m.size(); j++){
			m[i][j] = 0;
		}
	}
}

list<Action> ComportamientoAuxiliar::AnchuraAuxiliar_V2(const EstadoA &inicio, const EstadoA &final,
	const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){

		NodoA current_node;
		list<NodoA> frontier;
		set<NodoA> explored;
		list<Action> path;

		current_node.estado = inicio;
		frontier.push_back(current_node);
		bool SolutionFound = (current_node.estado.f == final.f and current_node.estado.c == final.c);

		while(!SolutionFound and !frontier.empty()){
			frontier.pop_front();
			explored.insert(current_node);

			//Compruebbo si estoy en una casilla de las zapatillas
			if(terreno[current_node.estado.f][current_node.estado.c] == 'D'){
				current_node.estado.zapatillas = true;
			}

			//Genero el hijo resultante de aplicar la accion WALK
			NodoA child_WALK = current_node;
			child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
			if(child_WALK.estado.f == final.f and child_WALK.estado.c == final.c){
				//El hijo generado es solucion
				child_WALK.secuencia.push_back(WALK);
				current_node = child_WALK;
				SolutionFound = true;
			}
			else if(explored.find(child_WALK) == explored.end()){
				//Se mete en la lista frontier después de añadir a secuencia la acción
				child_WALK.secuencia.push_back(WALK);
				frontier.push_back(child_WALK);
			}

			//Genero el hijo resultante de aplicar la accion TURN_SR
			if(!SolutionFound){
				NodoA child_TURN_SR = current_node;
				child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
				if(explored.find(child_TURN_SR) == explored.end()){
					child_TURN_SR.secuencia.push_back(TURN_SR);
					frontier.push_back(child_TURN_SR);
				}
			}

			//Paso a evaluar el siguiente nodo en la lista "frontier"
			if(!SolutionFound and !frontier.empty()){
				current_node = frontier.front();
				while(explored.find(current_node) != explored.end() and !frontier.empty()){
					frontier.pop_front();
					current_node = frontier.front();
				}
			}
		}

		if(SolutionFound) path = current_node.secuencia;
		
		return path;
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
int ComportamientoAuxiliar::Heuristica(const ubicacion &a, const ubicacion &b){
	int dx = std::abs(a.f - b.f);   
	int dy = std::abs(a.c - b.c); 

	if(dx < dy) return dy;
	else return dx;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores){
	Action accion = IDLE;
	if(!hayPlan){
		//Invocar al método de busqueda
		EstadoA_N3 inicio, fin;
		inicio.auxiliar.f = sensores.posF;
		inicio.auxiliar.c = sensores.posC;
		inicio.auxiliar.brujula = sensores.rumbo;
		inicio.zapatillas_auxiliar = tiene_zapatillas;
		fin.auxiliar.f = sensores.destinoF;
		fin.auxiliar.c = sensores.destinoC;
		plan  = AlgoritmoAE(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}
	if(hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
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
			int dif_altura = (altura[siguiente.auxiliar.f][siguiente.auxiliar.c] - altura[st.auxiliar.f][st.auxiliar.c]);
			switch(terreno[st.auxiliar.f][st.auxiliar.c]){
				case 'A':
					coste = 100 + dif_altura * 10;
					break;
				case 'T':
					coste = 20 + dif_altura * 5;
					break;
				case 'S':
					coste = 2 + dif_altura;
					break;
				case 'B':
					coste = 20 + dif_altura * 5;			
					break;
				default:
					coste = 1;
					break;
			}
			break;
		}
	
	
		case TURN_SR: {
			switch(terreno[st.auxiliar.f][st.auxiliar.c]){
				case 'A':
					coste = 16;
					break;
				case 'T':
					coste = 3;
					break;
				case 'B':
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
	
	switch(st.auxiliar.brujula){
		case norte:
			siguiente.auxiliar.f = st.auxiliar.f - 1;
			break;
		case noreste:
			siguiente.auxiliar.f = st.auxiliar.f - 1;
			siguiente.auxiliar.c = st.auxiliar.c + 1;
			break;
		case este:
			siguiente.auxiliar.c = st.auxiliar.c + 1;
			break;
		case sureste:
			siguiente.auxiliar.f = st.auxiliar.f + 1;
			siguiente.auxiliar.c = st.auxiliar.c + 1;
			break;
		case sur:
			siguiente.auxiliar.f = st.auxiliar.f + 1;
			break;
		case suroeste:
			siguiente.auxiliar.f = st.auxiliar.f + 1;
			siguiente.auxiliar.c = st.auxiliar.c - 1;
			break;
		case oeste:
			siguiente.auxiliar.c = st.auxiliar.c - 1;
			break;
		case noroeste:
			siguiente.auxiliar.f = st.auxiliar.f - 1;
			siguiente.auxiliar.c = st.auxiliar.c - 1;
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
			next.auxiliar.brujula = (Orientacion)((next.auxiliar.brujula + 1) % 8);
			break;
		}
	}

	if(terreno[next.auxiliar.f][next.auxiliar.c] == 'D'){
		next.zapatillas_auxiliar = true;
	} else {
		next.zapatillas_auxiliar = st.zapatillas_auxiliar;
	}

	return next;
}

bool ComportamientoAuxiliar::CasillaAccesibleAuxiliar(const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
	EstadoA_N3 next = NextCasillaAuxiliar(st);
	bool check1 = false;
	bool check2 = false;
	bool check3 = false;
	bool check4 = false;

	check1 = terreno[next.auxiliar.f][next.auxiliar.c] != 'P' and terreno[next.auxiliar.f][next.auxiliar.c] != 'M';
	check2 = terreno[next.auxiliar.f][next.auxiliar.c] != 'B' or (terreno[next.auxiliar.f][next.auxiliar.c] == 'B' and st.zapatillas_auxiliar);
	check3 = abs(altura[next.auxiliar.f][next.auxiliar.c] - altura[st.auxiliar.f][st.auxiliar.c]) <= 1;
	check4 = next.auxiliar.f == st.rescatador.f and next.auxiliar.c == st.rescatador.c;

	return check1 and check2 and check3 and !check4;
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoA_N3 &st, const list<Action> &plan){
	AnularMatrizA(mapaConPlan);
	EstadoA_N3 cst = st;

	auto it = plan.begin();
	while(it != plan.end()){
		switch(*it){
			case WALK:
				switch(cst.auxiliar.brujula){
					case 0:
						cst.auxiliar.f--;
						break;
					case 1:
						cst.auxiliar.f--;
						cst.auxiliar.c++;
						break;
					case 2:
						cst.auxiliar.c++;
						break;
					case 3:
						cst.auxiliar.f++;
						cst.auxiliar.c++;
						break;
					case 4:
						cst.auxiliar.f++;
						break;
					case 5:
						cst.auxiliar.f++;
						cst.auxiliar.c--;
						break;
					case 6:
						cst.auxiliar.c--;
						break;
					case 7:
						cst.auxiliar.f--;
						cst.auxiliar.c--;
						break;
				}
				mapaConPlan[cst.auxiliar.f][cst.auxiliar.c] = 2;
				break;

			case TURN_SR:
				cst.auxiliar.brujula = (Orientacion)((cst.auxiliar.brujula + 1) % 8);
				break;
		}
		it++;
	}
}


list<Action> ComportamientoAuxiliar::AlgoritmoAE(const EstadoA_N3 &inicio, const EstadoA_N3 &final,
	const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
	
		NodoA_N3 current_node;
		priority_queue<NodoA_N3, vector<NodoA_N3>, Compara_N3> frontier;
		set<EstadoA_N3> explored;
		list<Action> path;

		current_node.estado = inicio;
		current_node.energia = 0;
		frontier.push(current_node);
		bool SolutionFound = (current_node.estado.auxiliar.f == final.auxiliar.f
			and current_node.estado.auxiliar.c == final.auxiliar.c);

		while(!SolutionFound and !frontier.empty()){
			frontier.pop();
			explored.insert(current_node.estado);

			// Compruebo si estoy en una casilla de las zapatillas
			if(terreno[current_node.estado.auxiliar.f][current_node.estado.auxiliar.c] == 'D'){
				current_node.estado.zapatillas_auxiliar = true;
			}

			if(current_node.estado.auxiliar.f == final.auxiliar.f and current_node.estado.auxiliar.c == final.auxiliar.c){
				SolutionFound = true;
			}

			// Genero el hijo resultante de aplicar la acción WALK
			NodoA_N3 child_WALK = current_node;
			child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
			child_WALK.secuencia.push_back(WALK);
			child_WALK.energia += FuncionCoste_A(WALK, current_node.estado, terreno, altura);
			child_WALK.energia_heuristica = Heuristica(child_WALK.estado.auxiliar, final.auxiliar);
			if(child_WALK.estado.auxiliar.f == final.auxiliar.f and child_WALK.estado.auxiliar.c == final.auxiliar.c){
				// El hijo generado es solución
				current_node = child_WALK;
				SolutionFound = true;
			}
			else if(explored.find(child_WALK.estado) == explored.end()){
				// Se mete en la lista frontier después de añadir a secuencia la acción
				frontier.push(child_WALK);
			}

			// Genero el hijo resultante de aplicar la acción TURN_SR
			if(!SolutionFound){
				NodoA_N3 child_TURN_SR = current_node;
				child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
				child_TURN_SR.secuencia.push_back(TURN_SR);
				child_TURN_SR.energia += FuncionCoste_A(TURN_SR, current_node.estado, terreno, altura);
				child_TURN_SR.energia_heuristica = Heuristica(child_WALK.estado.auxiliar, final.auxiliar);
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
		return path;
}

/*NIVEL 4*/
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores){
}