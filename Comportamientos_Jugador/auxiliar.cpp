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

	last_action = accion;
	return accion;
}

int ComportamientoAuxiliar::DetectarCasillaInteresanteA(Sensores &sensores, bool zap) {
    for (int i = 0; i < sensores.superficie.size(); ++i) {
        char casilla = sensores.superficie[i];
        if ((casilla == 'D' and !zap) or casilla == 'X') {
            return i; // Retorna el índice de la casilla interesante
		}    
    }
    return -1; // No se encontró ninguna casilla interesante
}

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

	if(indice_interes != -1 and CasillaLibreA(sensores.agentes[indice_interes])){
		switch(indice_interes){
			case 1:
			case 4:
			case 9:
				if(((i == 'X') or (i == 'C') or (i == 'D')) and i_libre) return 1;
				break;

			case 3:
			case 8:
			case 15:
				if(((d == 'X') or (d == 'C') or (d == 'D')) and d_libre) return 3;
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
				if(((c == 'X') or (c == 'C') or (c == 'D')) and c_libre) return 2;
				break;
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
		if (freq == frecuencia_i && i_libre && i == 'C') return 1;
		else if (freq == frecuencia_c && c_libre && c == 'C') return 2;
		else if (freq == frecuencia_d && d_libre && d == 'C') return 3;
	}

	return 0;
	
}


char ComportamientoAuxiliar::ViablePorAlturaA(char casilla, int dif, bool zap){
	if (abs(dif)<=1 or (zap and abs(dif)<=2))
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

/*NIVEL 1*/
Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
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
int Heuristica(const std::pair<int, int>& a, const std::pair<int, int>& b){
	int dx = std::abs(a.first - b.first);   
	int dy = std::abs(a.second - b.second); 
	return std::max(dx, dy);
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores){
	Action accion = IDLE;
	if(!hayPlan){
		//Invocar al método de busqueda
		EstadoA_N3 inicio, fin;
		inicio.f_auxiliar = sensores.posF;
		inicio.c_auxiliar = sensores.posC;
		inicio.brujula_auxiliar = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f_auxiliar = sensores.destinoF;
		fin.c_auxiliar = sensores.destinoC;
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



int ComportamientoAuxiliar::FuncionCoste(const Action &accion, const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	int coste = 0;

	switch(accion){
		case WALK: {
			EstadoA_N3 siguiente = NextCasillaAuxiliar(st);
			int dif_altura = (altura[siguiente.f_auxiliar][siguiente.c_auxiliar] - altura[st.f_auxiliar][st.c_auxiliar]);
			switch(terreno[st.f_auxiliar][st.c_auxiliar]){
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
			switch(terreno[st.f_auxiliar][st.c_auxiliar]){
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
	
	switch(st.brujula_auxiliar){
		case norte:
			siguiente.f_auxiliar = st.f_auxiliar - 1;
			break;
		case noreste:
			siguiente.f_auxiliar = st.f_auxiliar - 1;
			siguiente.c_auxiliar = st.c_auxiliar + 1;
			break;
		case este:
			siguiente.c_auxiliar = st.c_auxiliar + 1;
			break;
		case sureste:
			siguiente.f_auxiliar = st.f_auxiliar + 1;
			siguiente.c_auxiliar = st.c_auxiliar + 1;
			break;
		case sur:
			siguiente.f_auxiliar = st.f_auxiliar + 1;
			break;
		case suroeste:
			siguiente.f_auxiliar = st.f_auxiliar + 1;
			siguiente.c_auxiliar = st.c_auxiliar - 1;
			break;
		case oeste:
			siguiente.c_auxiliar = st.c_auxiliar - 1;
			break;
		case noroeste:
			siguiente.f_auxiliar = st.f_auxiliar - 1;
			siguiente.c_auxiliar = st.c_auxiliar - 1;
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
			next.brujula_auxiliar = (next.brujula_auxiliar + 1) % 8;
			break;
		}
	}

	if(terreno[next.f_auxiliar][next.c_auxiliar] == 'D'){
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
	bool check4 = false;

	check1 = terreno[next.f_auxiliar][next.c_auxiliar] != 'P' and terreno[next.f_auxiliar][next.c_auxiliar] != 'M';
	check2 = terreno[next.f_auxiliar][next.c_auxiliar] != 'B' or (terreno[next.f_auxiliar][next.c_auxiliar] == 'B' and st.zapatillas);
	check3 = abs(altura[next.f_auxiliar][next.c_auxiliar] - altura[st.f_auxiliar][st.c_auxiliar]) <= 1;
	check4 = next.f_auxiliar == st.f_rescatador and next.c_auxiliar == st.c_rescatador;

	return check1 and check2 and check3 and !check4;
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoA_N3 &st, const list<Action> &plan){
	AnularMatrizA(mapaConPlan);
	EstadoA_N3 cst = st;

	auto it = plan.begin();
	while(it != plan.end()){
		switch(*it){
			case WALK:
				switch(cst.brujula_auxiliar){
					case 0:
						cst.f_auxiliar--;
						break;
					case 1:
						cst.f_auxiliar--;
						cst.c_auxiliar++;
						break;
					case 2:
						cst.c_auxiliar++;
						break;
					case 3:
						cst.f_auxiliar++;
						cst.c_auxiliar++;
						break;
					case 4:
						cst.f_auxiliar++;
						break;
					case 5:
						cst.f_auxiliar++;
						cst.c_auxiliar--;
						break;
					case 6:
						cst.c_auxiliar--;
						break;
					case 7:
						cst.f_auxiliar--;
						cst.c_auxiliar--;
						break;
				}
				mapaConPlan[cst.f_auxiliar][cst.c_auxiliar] = 2;
				break;

			case TURN_SR:
				cst.brujula_auxiliar = (cst.brujula_auxiliar + 1) % 8;
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
		bool SolutionFound = (current_node.estado.f_auxiliar == final.f_auxiliar
			and current_node.estado.c_auxiliar == final.c_auxiliar);

		while(!SolutionFound and !frontier.empty()){
			frontier.pop();
			explored.insert(current_node.estado);

			// Compruebo si estoy en una casilla de las zapatillas
			if(terreno[current_node.estado.f_auxiliar][current_node.estado.c_auxiliar] == 'D'){
				current_node.estado.zapatillas = true;
			}

			if(current_node.estado.f_auxiliar == final.f_auxiliar and current_node.estado.c_auxiliar == final.c_auxiliar){
				SolutionFound = true;
			}

			// Genero el hijo resultante de aplicar la acción WALK
			NodoA_N3 child_WALK = current_node;
			child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
			child_WALK.secuencia.push_back(WALK);
			child_WALK.energia += FuncionCoste(WALK, current_node.estado, terreno, altura);
			child_WALK.energia_heuristica = Heuristica({child_WALK.estado.f_auxiliar, child_WALK.estado.c_auxiliar},
				{final.f_auxiliar, final.c_auxiliar});
			if(child_WALK.estado.f_auxiliar == final.f_auxiliar and child_WALK.estado.c_auxiliar == final.c_auxiliar){
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
				child_TURN_SR.energia += FuncionCoste(TURN_SR, current_node.estado, terreno, altura);
				child_TURN_SR.energia_heuristica = Heuristica({child_TURN_SR.estado.f_auxiliar, child_TURN_SR.estado.c_auxiliar},
					{final.f_auxiliar, final.c_auxiliar});
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