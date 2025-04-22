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
		accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		accion = ComportamientoRescatadorNivel_3 (sensores);
		//accion = ComportamientoRescatadorNivel_E(sensores);
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*PARTE REACTIVA*///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*NIVEL 0*/

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	Action accion;
	SituarSensorenMapaR(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(sensores.superficie[0] == 'X'){
		accion = IDLE;
	}else if(!cola.empty()){
		accion = cola.front();
		cola.pop();
	}
	
	else {

		int pos = VeoCasillaInteresanteR(sensores, tiene_zapatillas);
		
		switch (pos){
			case 2:
				accion = WALK;
				break;
			case 1:
				cola.push(TURN_L);
				cola.push(TURN_SR);
				break;
			case 3:
				accion = TURN_SR;
				break;
			case 4:
				accion = RUN;
				break;
			case 5:
				cola.push(TURN_SR);
				cola.push(RUN);
				break;
			case 6:
				cola.push(TURN_L);
				cola.push(TURN_SR);
				cola.push(RUN);
				
				break;
			case 0:
				if(accion_defecto){
					accion = TURN_SR;
				}else{
					accion_defecto = true;
					cola.push(TURN_L);
					cola.push(TURN_SR);
				}
				
				break;
		}
	}
	//pasos++;


	// if(pasos == MAX_PASOS){
	// 	RefrescarmatrizR(frecuencia_visita);
	// 	pasos = 0;
	// }

	last_action = accion;
	return accion;
}

//Supongo que el argumento i es bueno
bool ComportamientoRescatador::PuedeCorrer(int i, Sensores &sensores, bool zap) {

	int altura_actual = sensores.cota[0];
	int altura_dos_pasos = sensores.cota[i];
	int diferencia_altura = abs(altura_dos_pasos - altura_actual);

	bool libre_intermedia = false;
	bool libre_final = false;

	
	libre_final = ((sensores.superficie[i] == 'C' or sensores.superficie[i] == 'D' or sensores.superficie[i] == 'X') and sensores.agentes[i] == '_');

	switch(i){
		case 4:
			libre_intermedia = ((sensores.superficie[1] == 'C' or sensores.superficie[1] == 'D' or sensores.superficie[1] == 'X') and sensores.agentes[1] == '_');
			break;

		case 6:
			libre_intermedia = ((sensores.superficie[2] == 'C' or sensores.superficie[2] == 'D' or sensores.superficie[2] == 'X') and sensores.agentes[2] == '_');
		break;

		case 8:
			libre_intermedia = ((sensores.superficie[3] == 'C' or sensores.superficie[3] == 'D' or sensores.superficie[3] == 'X') and sensores.agentes[3] == '_');
		break;
	}
	
	bool transitable = (diferencia_altura <= 1 || (zap && diferencia_altura <= 2));

	// cout << "Transitable: " << transitable << endl;
	// cout << "Libre Intermedia: " << libre_intermedia << endl;
	// cout << "Libre Final: " << libre_final << endl;
	if (transitable and libre_intermedia and libre_final) {
		//cout << "TRUE" << endl;
		return true;
	}

	return false;
}


int ComportamientoRescatador::DetectarCasillaInteresanteR(Sensores &sensores, bool zap) {
    for (int i = 0; i < sensores.superficie.size(); ++i) {
        char casilla = sensores.superficie[i];
        if (((casilla == 'D' and !zap) or casilla == 'X') and CasillaLibreR(sensores.agentes[i])) {
            return i; // Retorna el índice de la casilla interesante
		}    
    }
    return -1; // No se encontró ninguna casilla interesante
}

/*Una primera idea para resolver puede ser esta. 
Tengo que ir perfeccionando cosas.*/
int ComportamientoRescatador::VeoCasillaInteresanteR(Sensores &sensores, bool zap){
	char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1]-sensores.cota[0], zap);
	char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2]-sensores.cota[0], zap);
	char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3]-sensores.cota[0], zap);

	bool puedeCorrer_i = PuedeCorrer(4, sensores, tiene_zapatillas);
	bool puedeCorrer_d = PuedeCorrer(8, sensores, tiene_zapatillas);
	bool puedeCorrer_c = PuedeCorrer(6, sensores, tiene_zapatillas);
	

	char correr_c = sensores.superficie[6];
	char correr_i = sensores.superficie[4];
	char correr_d = sensores.superficie[8];

	bool i_libre = CasillaLibreR(sensores.agentes[1]);
	bool c_libre = CasillaLibreR(sensores.agentes[2]);
	bool d_libre = CasillaLibreR(sensores.agentes[3]);
	

	int frecuencia_i=0;
	int frecuencia_c=0;
	int frecuencia_d=0;	
	int frecuencia_run_i = 0;
	int frecuencia_run_c = 0;
	int frecuencia_run_d = 0;


	vector<int> frecuencia_casillas;
	
	if (c == 'X' and c_libre) return 2;
	else if (i == 'X' and i_libre) return 1;
	else if (d == 'X' and d_libre) return 3;
	else if(correr_c == 'X' and puedeCorrer_c) return 4;
	else if(correr_i == 'X' and puedeCorrer_i) return 6;
	else if(correr_d == 'X' and puedeCorrer_d) return 5;
	else if(!zap) {
		if(c == 'D' and c_libre) return 2;
		else if (i == 'D' and i_libre) return 1;
		else if (d == 'D' and d_libre) return 3;
		else if(correr_c == 'D' and puedeCorrer_c) return 4;
		else if(correr_i == 'D' and puedeCorrer_i) return 6;
		else if(correr_d == 'D' and puedeCorrer_d) return 5;
	}

	/*Primero me aseguro de que se observa alguna casilla interesante*/
	
	if(!sigo_plan){
		int indice_interes = DetectarCasillaInteresanteR(sensores, zap);

		if(indice_interes != -1){
			sigo_plan = true;
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
					sigo_plan = false;
					cola_acciones = queue<int>();
					return 0; // No se encontró ninguna acción válida
					break;
			}
		}
		else{
			sigo_plan = false;
			return 0;
		}
	}

	switch(sensores.rumbo){
		case norte:
			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
			break;

		case noroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC];

			
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
			break;

		case oeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
			break;

		case suroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC];
			
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
			break;

		case sur:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
			break;

		case sureste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC];
			
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
			break;

		case este:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
			break;

		case noreste:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC + 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC];
			
			frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
			break;
	}

	std::sort(frecuencia_casillas.begin(), frecuencia_casillas.end());  // Ordena de menor a mayor

	for (int freq : frecuencia_casillas) {
		if (freq == frecuencia_c and c_libre and (c == 'C' or c == 'D')) return 2;
		else if (freq == frecuencia_i and i_libre and (i == 'C' or i == 'D')) return 1;
		else if (freq == frecuencia_d and d_libre and (d == 'C' or d == 'D')) return 3;
		else if (freq == frecuencia_run_c and puedeCorrer_c and (correr_c == 'C' or correr_c == 'D')) return 4;
		else if (freq == frecuencia_run_i and puedeCorrer_i and (correr_i == 'C' or correr_i == 'D')) return 6;
		else if (freq == frecuencia_run_d and puedeCorrer_d and (correr_d == 'C' or correr_d == 'D')) return 5;
	}

	return 0;
	
}

char ComportamientoRescatador::ViablePorAlturaR(char casilla, int dif, bool zap){
	if (abs(dif)<=1 or (zap and abs(dif)<=2))
		return casilla;
	else 
		return 'P';
}

bool ComportamientoRescatador::CasillaLibreR(char casilla){
	if(casilla == '_') return true;
	else return false;
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

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
	Action accion;
	SituarSensorenMapaR(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(giro45izq != 0){
		accion = TURN_SR;
		giro45izq--;
	}
	else {

		if(sensores.posF == objetivo.first and sensores.posC == objetivo.second){
			objetivo.first = -1;
			objetivo.second = -1;
			RecuperarFrecuenciasR();

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

			GuardarFrecuenciasR();
			CalcularObjetivoR();
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
				GuardarFrecuenciasR();
				CalcularObjetivoR();

				if(sensores.posF == objetivo.first and sensores.posC == objetivo.second){
					objetivo.first = -1;
					objetivo.second = -1;
					RecuperarFrecuenciasR();
				}
			}
			pasos = 0;
		}

		int pos = VeoCasillaInteresanteR_N1(sensores, tiene_zapatillas);
		switch (pos){
			case 2:
				accion = WALK;
				//cout << "Avanzo" << endl;
				break;
			case 1:
				giro45izq = 1;
				accion = TURN_L;
				//cout << "Izqda" << endl;
				break;
			case 3:
				accion = TURN_SR;
				//cout << "Dcha" << endl;
				break;
			case 0:
				if(accion_defecto){
					accion = TURN_SR;
				}else{
					accion_defecto = true;
					giro45izq = 1;
					accion = TURN_L;
				}
				//cout << "Defecto" << endl;
				break;
		}
	}

	
	pasos++;
	last_action = accion;
	return accion;
}

void ComportamientoRescatador::GuardarFrecuenciasR(){
	for(int i = 0; i < frecuencia_visita.size(); i++){
		for(int j = 0; j < frecuencia_visita[i].size(); j++){
			frecuencia_visita_aux[i][j] = frecuencia_visita[i][j];
		}
	}
}

void ComportamientoRescatador::RecuperarFrecuenciasR(){
	for(int i = 0; i < frecuencia_visita_aux.size(); i++){
		for(int j = 0; j < frecuencia_visita_aux[i].size(); j++){
			frecuencia_visita[i][j] = frecuencia_visita_aux[i][j];
		}
	}
}

int ComportamientoRescatador::DetectarCasillaZapatillasR(Sensores &sensores, bool zap) {
    for (int i = 0; i < sensores.superficie.size(); ++i) {
        char casilla = sensores.superficie[i];
        if (casilla == 'D' and !zap) {
            return i; // Retorna el índice de la casilla interesante
		}    
    }
    return -1; // No se encontró ninguna casilla interesante
}

bool ComportamientoRescatador::EsTransitableR(char casilla){
	
	if(casilla == 'X' or casilla == 'C' or casilla == 'D'
		or casilla == 'S') return true;
	else return false;
	
}

int ComportamientoRescatador::VeoCasillaInteresanteR_N1(Sensores &sensores, bool zap){

	char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1]-sensores.cota[0], zap);
	char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2]-sensores.cota[0], zap);
	char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3]-sensores.cota[0], zap);

	bool i_libre = CasillaLibreR(sensores.agentes[1]);
	bool c_libre = CasillaLibreR(sensores.agentes[2]);
	bool d_libre = CasillaLibreR(sensores.agentes[3]);
	//cout << "i: " << i << " c: " << c << " d: " << d << endl;
	if(!zap){
		int indice_interes = DetectarCasillaZapatillasR(sensores, zap);

		if(indice_interes != -1){
			switch(indice_interes){
				case 1:
				case 4:
				case 9:
					//Aquí debería de meter algo de si es transitable o no
					if(EsTransitableR(sensores.superficie[1]) and i_libre and i != 'P') return 1;
					break;

				case 3:
				case 8:
				case 15:
					if(EsTransitableR(sensores.superficie[3]) and d_libre and d != 'P') return 3;
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
					if(EsTransitableR(sensores.superficie[2]) and c_libre and c != 'P') return 2;
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

		
	if(!zap) {
		if(c == 'D' and c_libre) return 2;
		else if (i == 'D' and i_libre) return 1;
		else if (d == 'D' and d_libre) return 3;
	}

	for (int freq : frecuencia_casillas) {
		if (freq == frecuencia_c && c_libre and EsTransitableR(sensores.superficie[2]) and c!='P') return 2;
		else if (freq == frecuencia_i && i_libre and EsTransitableR(sensores.superficie[1]) and i!='P') return 1;
		else if (freq == frecuencia_d && d_libre and EsTransitableR(sensores.superficie[3]) and d!='P') return 3;
	}

	return 0;
	
}

void ComportamientoRescatador::CalcularObjetivoR(){
	
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

list<Action> AvanzaSaltosDeCaballoR(){
	list<Action> secuencia;
	secuencia.push_back(WALK);
	secuencia.push_back(WALK);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(TURN_SR);
	secuencia.push_back(WALK);
	return secuencia;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_E(Sensores sensores){
	Action accion = IDLE;
	if(!hayPlan){
		//Invocar al método de busqueda
		EstadoR inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		//plan = AnchuraRescatador(inicio, fin, mapaResultado, mapaCotas);
		plan  = AnchuraRescatador_V2(inicio, fin, mapaResultado, mapaCotas);
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

list<Action> ComportamientoRescatador::AnchuraRescatador(const EstadoR &inicio, const EstadoR &final,
    			const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){

		NodoR current_node;
		list<NodoR> frontier;
		list<NodoR> explored;
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
			NodoR child_WALK = current_node;
			child_WALK.estado = applyR(WALK, current_node.estado, terreno, altura);
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
				NodoR child_TURN_SR = current_node;
				child_TURN_SR.estado = applyR(TURN_SR, current_node.estado, terreno, altura);
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

bool ComportamientoRescatador::CasillaAccesibleRescatador(const EstadoR &st, const vector<vector<unsigned char>> &terreno, 
    const vector<vector<unsigned char>> &altura){
		EstadoR next = NextCasillaRescatador(st);
		bool check1 = false;
		bool check2 = false;
		bool check3 = false;

		check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M';
		check2 = terreno[next.f][next.c] != 'B' or (terreno[next.f][next.c] == 'B' and st.zapatillas);
		check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;

		return check1 and check2 and check3;
}

EstadoR ComportamientoRescatador::NextCasillaRescatador(const EstadoR &st){
	EstadoR siguiente = st;
	
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

EstadoR ComportamientoRescatador::applyR(Action accion, const EstadoR &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoR next = st;
	switch(accion){
		case WALK:
			if(CasillaAccesibleRescatador(st, terreno, altura)){
				next = NextCasillaRescatador(st);
			}
			break;
		case TURN_SR:
			next.brujula = (next.brujula + 1) % 8;
			break;
	}

	return next;
}

bool ComportamientoRescatador::Find(const NodoR &st,const list<NodoR> &lista){
	auto it = lista.begin();
	while(it != lista.end() and !((*it) == st)){
		it++;
	}

	return (it != lista.end());
}

void ComportamientoRescatador::VisualizaPlan(const EstadoR &st, const list<Action> &plan){
	AnularMatrizR(mapaConPlan);
	EstadoR cst = st;

	auto it = plan.begin();
	while(it != plan.end()){
		switch(*it){
			case RUN:
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
				mapaConPlan[cst.f][cst.c] = 3;
				break;

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
				mapaConPlan[cst.f][cst.c] = 1;
				break;

			case TURN_SR:
				cst.brujula = (cst.brujula + 1) % 8;
				break;

			case TURN_L:
				cst.brujula = (cst.brujula + 6) % 8;
				break;
		}
		it++;
	}
}

void ComportamientoRescatador::AnularMatrizR(vector<vector<unsigned char>> &m){
	for(int i=0; i<m.size(); i++){
		for(int j=0; j<m.size(); j++){
			m[i][j] = 0;
		}
	}
}

list<Action> ComportamientoRescatador::AnchuraRescatador_V2(const EstadoR &inicio, const EstadoR &final,
	const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){

		NodoR current_node;
		list<NodoR> frontier;
		set<NodoR> explored;
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
			NodoR child_WALK = current_node;
			child_WALK.estado = applyR(WALK, current_node.estado, terreno, altura);
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
				NodoR child_TURN_SR = current_node;
				child_TURN_SR.estado = applyR(TURN_SR, current_node.estado, terreno, altura);
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

void ComportamientoRescatador::PintaPlan(const list<Action> &plan, bool zap){

	auto it = plan.begin();
	while(it != plan.end()){
		if(*it == WALK){
			cout << "W ";
		}else if(*it == RUN){
			cout << "R ";
		}else if(*it == TURN_SR){
			cout << "r ";
		}else if(*it == TURN_L){
			cout << "L ";
		}else if(*it == CALL_ON){
			cout << "C ";
		}else if(*it == CALL_OFF){
			cout << "c ";
		}else if(*it == IDLE){
			cout << "I ";
		}else{
			cout << "-_";
		}
		it++;	
	}
	cout << "( longitud " << plan.size();
	if(zap) cout << "[Z]";
	cout << ")\n";
}



/*NIVEL 2*/

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
	Action accion = IDLE;
	if(!hayPlan){
		//Invocar al método de busqueda
		EstadoR_N2 inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		cout << "Inicio: (" << inicio.f << ", " << inicio.c << ")" << endl; 
		cout << "Fin: (" << fin.f << ", " << fin.c << ")" << endl; 
		plan  = DijsktraR(inicio, fin, mapaResultado, mapaCotas);
		VisualizaPlan(inicio, plan);
		hayPlan = plan.size() != 0;
	}
	//PintaPlan(plan, true);
	if(hayPlan and plan.size()>0){
		accion = plan.front();
		plan.pop_front();
	}
	if(plan.size()==0){
		hayPlan=false;
	}
	return accion;
}

int ComportamientoRescatador::FuncionCoste(const Action &accion, const EstadoR_N2 &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	int coste = 0;

	switch(accion){
		case WALK: {
			EstadoR_N2 siguiente =  applyR(WALK, st, terreno, altura);
			int dif_altura = (altura[siguiente.f][siguiente.c] - altura[st.f][st.c]);

			if(st.zapatillas == true) dif_altura = 0;

			switch(terreno[st.f][st.c]){
				case 'A':
					coste = 100 + dif_altura * 10;
					break;
				case 'T':
					coste = 20 + dif_altura * 5;
					break;
				case 'S':
					coste = 2 + dif_altura;
					break;
				default:
					coste = 1;
					break;
			}
			break;
		}
	
		case RUN: {
			
			EstadoR_N2 siguiente_2 = applyR(RUN, st, terreno, altura);
			
			int dif_altura = (altura[siguiente_2.f][siguiente_2.c] - altura[st.f][st.c]);

			//Gasto es independiente si hay zapatillas
			if(st.zapatillas == true) dif_altura = 0;
			switch (terreno[st.f][st.c]) {
				case 'A':
					coste = 150 + dif_altura * 15;
					break;
				case 'T':
					coste = 35 + dif_altura * 5;
					break;
				case 'S':
					coste = 3 + dif_altura * 2;
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
				
		case TURN_L: {
			switch(terreno[st.f][st.c]){
				case 'A':
					coste = 30;
					break;
				case 'T':
					coste = 5;
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

EstadoR_N2 ComportamientoRescatador::NextCasillaRescatador(const EstadoR_N2 &st){
	EstadoR_N2 siguiente = st;
	
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

EstadoR_N2 ComportamientoRescatador::applyR(Action accion, const EstadoR_N2 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura){
			EstadoR_N2 next = st;

			switch(accion){
				case WALK: {
					if(CasillaAccesibleRescatador(st, terreno, altura)){
						next = NextCasillaRescatador(st);
					}
					break;
				}
			
				case RUN: {
					EstadoR_N2 intermedia = NextCasillaRescatador(st);
					EstadoR_N2 fin = NextCasillaRescatador(intermedia);
					

					bool check1 = terreno[fin.f][fin.c] != 'P' and terreno[fin.f][fin.c] != 'M' and terreno[fin.f][fin.c] != 'B';
					bool check2 = abs(altura[fin.f][fin.c] - altura[st.f][st.c]) <= 1;
					bool check3 = ((abs(altura[fin.f][fin.c] - altura[st.f][st.c]) <= 2) and st.zapatillas == true);
					bool check4 = terreno[intermedia.f][intermedia.c] != 'P' and terreno[intermedia.f][intermedia.c] != 'M' and terreno[intermedia.f][intermedia.c] != 'B';



					if(check1 and check4 and (check2 or check3)){
						next = fin;
					}
					
					break;
				}
			
				case TURN_SR: {
					next.brujula = (next.brujula + 1) % 8;
					break;
				}
			
				case TURN_L: {
					next.brujula = (next.brujula + 6) % 8;
					break;
				}
			}
			

			if(terreno[next.f][next.c] == 'D'){
				next.zapatillas = true;
			}
			else{
				next.zapatillas = st.zapatillas;
			}
		
			return next;
	}

bool ComportamientoRescatador::CasillaAccesibleRescatador(const EstadoR_N2 &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
		EstadoR_N2 next = NextCasillaRescatador(st);
		bool check1 = false;
		bool check2 = false;
		bool check3 = false;

		check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M' and terreno[next.f][next.c] != 'B';
		check2 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;
		check3 = ((abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 2) and st.zapatillas == true);

		return check1 and (check2 or check3);
}

void ComportamientoRescatador::VisualizaPlan(const EstadoR_N2 &st, const list<Action> &plan){
	AnularMatrizR(mapaConPlan);
	EstadoR_N2 cst = st;

	auto it = plan.begin();
	while(it != plan.end()){
		switch(*it){
			case RUN:
				for (int i = 0; i < 2; ++i) { // RUN covers two steps
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
					mapaConPlan[cst.f][cst.c] = 3;
				}
				break;
			
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
				mapaConPlan[cst.f][cst.c] = 1;
				break;

			case TURN_SR:
				cst.brujula = (cst.brujula + 1) % 8;
				break;

			case TURN_L:
				cst.brujula = (cst.brujula + 6) % 8;
				break;
		}
		it++;
	}
}




list<Action> ComportamientoRescatador::DijsktraR(const EstadoR_N2 &inicio, const EstadoR_N2 &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){

		NodoR_N2 current_node;
		priority_queue<NodoR_N2> frontier;
		set<EstadoR_N2> explored;
		list<Action> path;

		int iteraciones = 0;

		current_node.estado = inicio;
		current_node.energia = 0;
		if(terreno[current_node.estado.f][current_node.estado.c] == 'D'){
			current_node.estado.zapatillas = true;
		}

		frontier.push(current_node);
		bool SolutionFound = (current_node.estado.f == final.f
			and current_node.estado.c == final.c);

		while(!SolutionFound and !frontier.empty()){
			//cout << current_node.energia << endl;

			iteraciones++;
			frontier.pop();
			//if(frontier.empty()) cout << "Vacio" << endl;

			explored.insert(current_node.estado);

			if(current_node.estado.f == final.f and current_node.estado.c == final.c){
				SolutionFound = true;
			}


			
			//Genero el hijo resultante de aplicar la accion WALK
			NodoR_N2 child_WALK = current_node;
			child_WALK.estado = applyR(WALK, current_node.estado, terreno, altura);
			child_WALK.energia += FuncionCoste(WALK, current_node.estado, terreno, altura);
			// if(child_WALK.estado.f == final.f and child_WALK.estado.c == final.c){
			// 	// El hijo generado es solución
			// 	child_WALK.secuencia.push_back(WALK);
			// 	current_node = child_WALK;
			// 	SolutionFound = true;
			// } else
			if(explored.find(child_WALK.estado) == explored.end()){
				// Se mete en la lista frontier después de añadir a secuencia la acción
				child_WALK.secuencia.push_back(WALK);
				frontier.push(child_WALK);
			}

			if(!SolutionFound){
				NodoR_N2 child_RUN = current_node;
				child_RUN.estado = applyR(RUN, current_node.estado, terreno, altura);
				child_RUN.energia += FuncionCoste(RUN, current_node.estado, terreno, altura);
				// if(child_RUN.estado.f == final.f and child_RUN.estado.c == final.c){
				// 	// El hijo generado es solución
				// 	child_RUN.secuencia.push_back(RUN);
				// 	current_node = child_RUN;
				// 	SolutionFound = true;
				// } else
				if(explored.find(child_RUN.estado) == explored.end()){
					// Se mete en la lista frontier después de añadir a secuencia la acción
					child_RUN.secuencia.push_back(RUN);
					frontier.push(child_RUN);
				}
			}

			if(!SolutionFound){
				NodoR_N2 child_TURN_SR = current_node;
				child_TURN_SR.estado = applyR(TURN_SR, current_node.estado, terreno, altura);
				child_TURN_SR.energia += FuncionCoste(TURN_SR, current_node.estado, terreno, altura);
				// if(child_TURN_SR.estado.f == final.f and child_TURN_SR.estado.c == final.c){
				// 	// El hijo generado es solución
				// 	child_TURN_SR.secuencia.push_back(TURN_SR);
				// 	current_node = child_TURN_SR;
				// 	SolutionFound = true;
				// } else
				if(explored.find(child_TURN_SR.estado) == explored.end()){
					// Se mete en la lista frontier después de añadir a secuencia la acción
					child_TURN_SR.secuencia.push_back(TURN_SR);
					frontier.push(child_TURN_SR);
				}
			

			
				NodoR_N2 child_TURN_L = current_node;
				child_TURN_L.estado = applyR(TURN_L, current_node.estado, terreno, altura);
				child_TURN_L.energia += FuncionCoste(TURN_L, current_node.estado, terreno, altura);
				// if(child_TURN_L.estado.f == final.f and child_TURN_L.estado.c == final.c){
				// 	// El hijo generado es solución
				// 	child_TURN_L.secuencia.push_back(TURN_L);
				// 	current_node = child_TURN_L;
				// 	SolutionFound = true;
				// } else
				if(explored.find(child_TURN_L.estado) == explored.end()){
					// Se mete en la lista frontier después de añadir a secuencia la acción
					child_TURN_L.secuencia.push_back(TURN_L);
					frontier.push(child_TURN_L);
				}
			}
			

			//Paso a evaluar el siguiente nodo en la lista "frontier"
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

/*NIVEL 3*/

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
	//Debería de definir algún comportamiento para apartarse y no molestar al auxiliar
	//De todas formas, el auxiliar lo evita en su comportamiento
	Action accion = IDLE;
	return accion;
}


/*NIVEL 4*/

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}