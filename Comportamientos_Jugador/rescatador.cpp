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
		accion = ComportamientoRescatadorNivel_4 (sensores);
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

		int pos = VeoCasillaInteresanteR_N0(sensores, tiene_zapatillas);
		
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

	last_action = accion;
	return accion;
}

//Supongo que el argumento i es bueno
bool ComportamientoRescatador::PuedeCorrer_N0(int i, Sensores &sensores, bool zap) {

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

	if (transitable and libre_intermedia and libre_final) {
		return true;
	}

	return false;
}


int ComportamientoRescatador::DetectarCasillaInteresanteR(Sensores &sensores, bool zap) {
    for (int i = 1; i < sensores.superficie.size(); ++i) {
        char casilla = sensores.superficie[i];
        if (((casilla == 'D' and !zap) or casilla == 'X') and CasillaLibreR(sensores.agentes[i])) {
            return i; // Retorna el índice de la casilla interesante
		}    
    }
    return -1; // No se encontró ninguna casilla interesante
}

/*Una primera idea para resolver puede ser esta. 
Tengo que ir perfeccionando cosas.*/
int ComportamientoRescatador::VeoCasillaInteresanteR_N0(Sensores &sensores, bool zap){
	char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1]-sensores.cota[0], zap);
	char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2]-sensores.cota[0], zap);
	char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3]-sensores.cota[0], zap);

	bool puedeCorrer_i = PuedeCorrer_N0(4, sensores, tiene_zapatillas);
	bool puedeCorrer_d = PuedeCorrer_N0(8, sensores, tiene_zapatillas);
	bool puedeCorrer_c = PuedeCorrer_N0(6, sensores, tiene_zapatillas);
	

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
				case 1: // TURN_L
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
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			
			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];

			break;

		case noroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC];

			break;

		case oeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			
			break;

		case suroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC];
			
			break;

		case sur:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			
			break;

		case sureste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC];
			
			break;

		case este:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			
			break;

		case noreste:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC + 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC];
			
			break;
	}
	frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, 
		frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};


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

	if(mapaResultado[sensores.posF][sensores.posC] == 'X'){
		while(sensores.energia <= 2000){
			return IDLE;
		}
	}

	if(!cola.empty()){
		accion = cola.front();
		cola.pop();
	}
	else {
		int pos = VeoCasillaInteresanteR_N1(sensores, tiene_zapatillas);
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
			case 0:
				if(accion_defecto){
					accion = TURN_SR;
				}else{
					accion_defecto = true;
					cola.push(TURN_L);
					cola.push(TURN_SR);
				}
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

			case 7:
				cola.push(TURN_L);
				cola.push(TURN_L);
				
				break;
		}
	}

	
	last_action = accion;
	return accion;
}

int ComportamientoRescatador::DetectarCasillaZapatillasR(Sensores &sensores, bool zap) {
    for (int i = 1; i < sensores.superficie.size(); ++i) {
        char casilla = sensores.superficie[i];
        if (casilla == 'D' and !zap) {
            return i; // Retorna el índice de la casilla interesante
		}    
    }
    return -1; // No se encontró ninguna casilla interesante
}

bool ComportamientoRescatador::DetectarAgenteR(Sensores &sensores) {
	for (int i = 1; i < sensores.agentes.size(); ++i) {
		
		if (sensores.agentes[i] == 'a') {
			return true; // Se encontró un agente
		}    
	}
	return false; // No se encontró ningún agente
}

bool ComportamientoRescatador::EsTransitableR(char casilla){
	
	if(casilla == 'X' or casilla == 'C' or casilla == 'D'
		or casilla == 'S') return true;
	else return false;
	
}

//Supongo que el argumento i es bueno
bool ComportamientoRescatador::PuedeCorrer_N1(int i, Sensores &sensores, bool zap) {

	int altura_actual = sensores.cota[0];
	int altura_dos_pasos = sensores.cota[i];
	int diferencia_altura = abs(altura_dos_pasos - altura_actual);

	bool libre_intermedia = false;
	bool libre_final = false;

	
	libre_final = ((sensores.superficie[i] == 'C' or sensores.superficie[i] == 'D' or sensores.superficie[i] == 'X'
	or sensores.superficie[i] == 'S') and sensores.agentes[i] == '_');

	switch(i){
		case 4:
			libre_intermedia = ((sensores.superficie[1] == 'C' or sensores.superficie[1] == 'D' or sensores.superficie[1] == 'X' 
				or sensores.superficie[1] == 'S' or sensores.superficie[1] == 'T' or sensores.superficie[1] == 'A') and sensores.agentes[1] == '_');
			break;

		case 6:
			libre_intermedia = ((sensores.superficie[2] == 'C' or sensores.superficie[2] == 'D' or sensores.superficie[2] == 'X' 
				or sensores.superficie[2] == 'S' or sensores.superficie[2] == 'T' or sensores.superficie[2] == 'A') and sensores.agentes[2] == '_');
		break;

		case 8:
			libre_intermedia = ((sensores.superficie[3] == 'C' or sensores.superficie[3] == 'D' or sensores.superficie[3] == 'X' 
				or sensores.superficie[3] == 'S' or sensores.superficie[3] == 'T' or sensores.superficie[3] == 'A') and sensores.agentes[3] == '_');
		break;
	}
	
	bool transitable = (diferencia_altura <= 1 || (zap && diferencia_altura <= 2));

	
	if (transitable and libre_intermedia and libre_final) {
		return true;
	}

	return false;
}

int ComportamientoRescatador::VeoCasillaInteresanteR_N1(Sensores &sensores, bool zap){

	char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1]-sensores.cota[0], zap);
	char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2]-sensores.cota[0], zap);
	char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3]-sensores.cota[0], zap);

	bool i_libre = CasillaLibreR(sensores.agentes[1]);
	bool c_libre = CasillaLibreR(sensores.agentes[2]);
	bool d_libre = CasillaLibreR(sensores.agentes[3]);

	bool puedeCorrer_i = PuedeCorrer_N1(4, sensores, tiene_zapatillas);
	bool puedeCorrer_d = PuedeCorrer_N1(8, sensores, tiene_zapatillas);
	bool puedeCorrer_c = PuedeCorrer_N1(6, sensores, tiene_zapatillas);
	

	char correr_c = sensores.superficie[6];
	char correr_i = sensores.superficie[4];
	char correr_d = sensores.superficie[8];

	if(!zap) {
		if(c == 'D' and c_libre) return 2;
		else if (i == 'D' and i_libre) return 1;
		else if (d == 'D' and d_libre) return 3;
		else if(correr_c == 'D' and puedeCorrer_c) return 4;
		else if(correr_i == 'D' and puedeCorrer_i) return 6;
		else if(correr_d == 'D' and puedeCorrer_d) return 5;
	}
	
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

	if(DetectarAgenteR(sensores) == true){
		return 7;
	} 

	int frecuencia_i=0;
	int frecuencia_c=0;
	int frecuencia_d=0;	
	int frecuencia_run_c = 0;
	int frecuencia_run_d = 0;
	int frecuencia_run_i = 0;

	vector<int> frecuencia_casillas;

	switch(sensores.rumbo){
		case norte:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			
			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];

			break;

		case noroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 1][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC];

			break;

		case oeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF - 1][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			
			break;

		case suroeste:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 1][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC];
			
			break;

		case sur:
			
			frecuencia_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC - 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 2][sensores.posC - 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			
			break;

		case sureste:
			
			frecuencia_i = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC];

			frecuencia_run_i = frecuencia_visita[sensores.posF + 1][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC];
			
			break;

		case este:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_c = frecuencia_visita[sensores.posF][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF + 1][sensores.posC + 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF + 2][sensores.posC + 2];
			
			break;

		case noreste:
			
			frecuencia_i = frecuencia_visita[sensores.posF - 1][sensores.posC];
			frecuencia_c = frecuencia_visita[sensores.posF - 1][sensores.posC + 1];
			frecuencia_d = frecuencia_visita[sensores.posF][sensores.posC + 1];

			frecuencia_run_i = frecuencia_visita[sensores.posF - 1][sensores.posC + 2];
			frecuencia_run_c = frecuencia_visita[sensores.posF - 2][sensores.posC + 2];
			frecuencia_run_d = frecuencia_visita[sensores.posF - 2][sensores.posC];
			
			break;
	}
	frecuencia_casillas = {frecuencia_i, frecuencia_c, frecuencia_d, 
		frecuencia_run_i, frecuencia_run_c, frecuencia_run_d};
	

	std::sort(frecuencia_casillas.begin(), frecuencia_casillas.end());  // Ordena de menor a mayor

	for (int freq : frecuencia_casillas) {
		if (freq == frecuencia_c and c_libre and EsTransitableR(c)) return 2;
		else if (freq == frecuencia_i and i_libre and EsTransitableR(i)) return 1;
		else if (freq == frecuencia_d and d_libre and EsTransitableR(d)) return 3;
		else if (freq == frecuencia_run_c and puedeCorrer_c and EsTransitableR(correr_c)) return 4;
		else if (freq == frecuencia_run_i and puedeCorrer_i and EsTransitableR(correr_i)) return 6;
		else if (freq == frecuencia_run_d and puedeCorrer_d and EsTransitableR(correr_d)) return 5;
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
			EstadoR_N2 siguiente = NextCasillaRescatador(st);
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
	
		case RUN: {
			EstadoR_N2 siguiente = NextCasillaRescatador(st);
			EstadoR_N2 siguiente_2 = NextCasillaRescatador(siguiente);
			
			int dif_altura = (altura[siguiente_2.f][siguiente_2.c] - altura[st.f][st.c]);

			int signo;
			if(dif_altura < 0) signo = -1;
			else if(dif_altura > 0) signo = 1;
			else signo = 0;

			switch(terreno[st.f][st.c]){
				case 'A':
					coste = 150 + signo * 15;
					break;
				case 'T':
					coste = 35 + signo * 5;
					break;
				case 'S':
					coste = 3 + signo * 2;
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
					next.brujula = (Orientacion)((next.brujula + 1) % 8);
					break;
				}
			
				case TURN_L: {
					next.brujula = (Orientacion)((next.brujula + 6) % 8);
					break;
				}
			}
			

			if(terreno[next.f][next.c] == 'D'){
				next.zapatillas = true;
			}
		
			return next;
	}

bool ComportamientoRescatador::CasillaAccesibleRescatador(const EstadoR_N2 &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){
		EstadoR_N2 next = NextCasillaRescatador(st);
		bool check1 = false;
		bool check2 = false;
		bool check3 = false;

		check1 = terreno[next.f][next.c] != 'P' and terreno[next.f][next.c] != 'M' and terreno[next.f][next.c] != 'B';
		check2 = (abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1);
		check3 = ((abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 2) and st.zapatillas);

		return check1 and (check2 or check3);
}

void ComportamientoRescatador::VisualizaPlan(const EstadoR_N2 &st, const list<Action> &plan){
	AnularMatrizR(mapaConPlan);
	EstadoR_N2 cst = st;

	auto it = plan.begin();
	while(it != plan.end()){
		switch(*it){
			case RUN:
				for (int i = 0; i < 2; ++i) { 
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
				cst.brujula = (Orientacion)((cst.brujula + 1) % 8);
				break;

			case TURN_L:
				cst.brujula = (Orientacion)((cst.brujula + 6) % 8);
				break;
		}
		it++;
	}
}




list<Action> ComportamientoRescatador::DijsktraR(const EstadoR_N2 &inicio, const EstadoR_N2 &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura){

		NodoR_N2 current_node;
		priority_queue<NodoR_N2, vector<NodoR_N2>, Compare> frontier;
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

			iteraciones++;
			frontier.pop();

			explored.insert(current_node.estado);

			if(current_node.estado.f == final.f and current_node.estado.c == final.c){
				SolutionFound = true;
			}


			
			NodoR_N2 child_WALK = current_node;
			child_WALK.estado = applyR(WALK, current_node.estado, terreno, altura);
			child_WALK.secuencia.push_back(WALK);
			child_WALK.energia += FuncionCoste(WALK, current_node.estado, terreno, altura);
			
			if(explored.find(child_WALK.estado) == explored.end()){
				frontier.push(child_WALK);
			}

			if(!SolutionFound){
				NodoR_N2 child_RUN = current_node;
				child_RUN.estado = applyR(RUN, current_node.estado, terreno, altura);
				child_RUN.secuencia.push_back(RUN);
				child_RUN.energia += FuncionCoste(RUN, current_node.estado, terreno, altura);
				
				if(explored.find(child_RUN.estado) == explored.end()){
					frontier.push(child_RUN);
				}
			}

			if(!SolutionFound){
				NodoR_N2 child_TURN_SR = current_node;
				child_TURN_SR.estado = applyR(TURN_SR, current_node.estado, terreno, altura);
				child_TURN_SR.secuencia.push_back(TURN_SR);
				child_TURN_SR.energia += FuncionCoste(TURN_SR, current_node.estado, terreno, altura);
				
				if(explored.find(child_TURN_SR.estado) == explored.end()){
					frontier.push(child_TURN_SR);
				}
			
				NodoR_N2 child_TURN_L = current_node;
				child_TURN_L.estado = applyR(TURN_L, current_node.estado, terreno, altura);
				child_TURN_L.secuencia.push_back(TURN_L);
				child_TURN_L.energia += FuncionCoste(TURN_L, current_node.estado, terreno, altura);
				
				if(explored.find(child_TURN_L.estado) == explored.end()){
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

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores){
	Action accion = IDLE;
	iteraciones--;
	
	ModificarMapaR(sensores, mapaResultado, mapaCotas, mapaEntidades);
	

	if(mapaResultado[sensores.posF][sensores.posC] == 'D') tiene_zapatillas = true;
	
	if(sensores.choque){
		// Si el rescatador choca, se reinicia el plan
		plan_N4.clear();
		hayPlan = false;
	}

	if (sensores.posF == sensores.destinoF && sensores.posC == sensores.destinoC) {
		if (!sensores.gravedad) {
			// El accidentado no requiere ayuda urgente, misión completada
			return IDLE; // Esperar nueva misión
		} else {
			if(iteraciones < 200){
				return CALL_OFF; // Renunciar a la misión si la energía es baja o el tiempo es alto
			}else{
				if (!auxiliarAvisado) {
					auxiliarAvisado = true;
					return CALL_ON; // Avisar al auxiliar
				} else {
					if(fallos_auxiliares == MAXIMOS_FALLOS_N4){
						return CALL_OFF; // Renunciar a la misión si el auxiliar no responde
					}else{
						if(esperas == MAXIMAS_ESPERAS_N4){
							fallos_auxiliares++;
							esperas = 0;
							auxiliarAvisado = false; // Reiniciar el estado del aviso al auxiliar
							return CALL_OFF; // Renunciar a la misión si el auxiliar no responde
						}
						esperas++;
						return IDLE; // Esperar al auxiliar
					}
				}
			}
		}
	}else{
		auxiliarAvisado = false; // Reiniciar el estado del aviso al auxiliar
	}

	
	if(mapaResultado[sensores.posF][sensores.posC] == 'X'){
		while(sensores.energia < 2500){
			return IDLE;
		}
	}
	if(sensores.energia < 700){
		if(!hayPlanEnergia){
			int distancia = 5000;
			int f = -1;
			int c = -1;
			for(int i = 0; i < mapaResultado.size(); i++){
				for(int j = 0; j < mapaResultado[0].size(); j++){
					if(mapaResultado[i][j] == 'X' and abs(i - sensores.posF) + abs(j - sensores.posC) < distancia){
						distancia = abs(i - sensores.posF) + abs(j - sensores.posC);
						f = i;
						c = j;
					}
				}
			}

			if(f != -1 and c != -1){
				EstadoR_N4 inicio, fin;
				inicio.f = sensores.posF;
				inicio.c = sensores.posC;
				inicio.brujula = sensores.rumbo;
				inicio.zapatillas = tiene_zapatillas;
				fin.f = f;
				fin.c = c;
				current_state = inicio;
				plan_N4  = AlgoritmoAE(inicio, fin, mapaResultado, mapaCotas, mapaEntidades);
				VisualizaPlan(inicio, plan_N4);
				hayPlanEnergia = plan_N4.size() != 0;
			}
		}
		if(hayPlanEnergia and plan_N4.size()>0){
			accion = plan_N4.front();
			last_state = current_state;
			current_state = applyR(accion, current_state, mapaResultado, mapaCotas, mapaEntidades);
			if(current_state == last_state){
				accionesProhibidas[last_state].insert(accion);
				hayPlanEnergia = false;
				plan_N4.clear();
				return IDLE;
			}else if(accion == WALK and (sensores.agentes[2] == 'v' or sensores.agentes[2] == 'e')){
				plan_N4.clear();
				return IDLE;
			}else if(accion == RUN and ((sensores.agentes[2] == 'v' or sensores.agentes[2] == 'e') or (sensores.agentes[6] == 'v' or sensores.agentes[6] == 'e'))){
				plan_N4.clear();
				return IDLE;
			}else{
				accion = plan_N4.front();
				auto it = plan_N4.begin();
				it = plan_N4.erase(it);
			}
			return accion;
		}
		if(plan_N4.size()==0 and hayPlanEnergia){
			hayPlanEnergia=false;
		}
	}
	

	if(!hayPlan){
		EstadoR_N4 inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.brujula = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		current_state = inicio;
		plan_N4  = AlgoritmoAE(inicio, fin, mapaResultado, mapaCotas, mapaEntidades);
		VisualizaPlan(inicio, plan_N4);
		hayPlan = plan_N4.size() != 0;
	}
	if(hayPlan and plan_N4.size()>0){
		accion = plan_N4.front();

		last_state = current_state;
		current_state = applyR(accion, current_state, mapaResultado, mapaCotas, mapaEntidades);
		
		if(current_state == last_state){
			accionesProhibidas[last_state].insert(accion);
			hayPlan = false;
			plan_N4.clear();
			return IDLE;
		}else if(accion == WALK and (sensores.agentes[2] == 'v' or sensores.agentes[2] == 'e')){
			plan_N4.clear();
			return IDLE;
		}else if(accion == RUN and ((sensores.agentes[2] == 'v' or sensores.agentes[2] == 'e') or (sensores.agentes[6] == 'v' or sensores.agentes[6] == 'e'))){
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

	if(accion == IDLE){
		if(!tiene_zapatillas and sensores.energia > 1000){
			return BuscaZapatillas(sensores);
		}
	}


	return accion;
}

vector<Action> ComportamientoRescatador::AlgoritmoAE(const EstadoR_N4 &inicio, const EstadoR_N4 &final,
	const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura,
	 const vector<vector<unsigned char>> &entidades){
		NodoR_N4 current_node;
		priority_queue<NodoR_N4> frontier;
		set<EstadoR_N4> explored;
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
			if(EsAccionValidaR(WALK, current_node.estado)){
				NodoR_N4 child_WALK = current_node;
				child_WALK.estado = applyR(WALK, current_node.estado, terreno, altura, entidades);
				child_WALK.secuencia.push_back(WALK);
				child_WALK.energia += FuncionCoste_R(WALK, current_node.estado, terreno, altura);
				child_WALK.energia_heuristica = Heuristica(child_WALK.estado, final);

				if(explored.find(child_WALK.estado) == explored.end()){
					// Se mete en la lista frontier después de añadir a secuencia la acción
					frontier.push(child_WALK);
				}
			}
			

			if(!SolutionFound){
				if(EsAccionValidaR(RUN, current_node.estado)){
					// Genero el hijo resultante de aplicar la acción RUN
					NodoR_N4 child_RUN = current_node;
					child_RUN.estado = applyR(RUN, current_node.estado, terreno, altura, entidades);
					child_RUN.secuencia.push_back(RUN);
					child_RUN.energia += FuncionCoste_R(RUN, current_node.estado, terreno, altura);
					child_RUN.energia_heuristica = Heuristica(child_RUN.estado, final);
					 
					if(explored.find(child_RUN.estado) == explored.end()){
						frontier.push(child_RUN);
					}
				}
			}

			// Genero el hijo resultante de aplicar la acción TURN_SR
			if(!SolutionFound){
				if(EsAccionValidaR(TURN_SR, current_node.estado)){
					// Genero el hijo resultante de aplicar la acción TURN_SR
					NodoR_N4 child_TURN_SR = current_node;
					child_TURN_SR.estado = applyR(TURN_SR, current_node.estado, terreno, altura, entidades);
					child_TURN_SR.secuencia.push_back(TURN_SR);
					child_TURN_SR.energia += FuncionCoste_R(TURN_SR, current_node.estado, terreno, altura);
					child_TURN_SR.energia_heuristica = Heuristica(child_TURN_SR.estado, final);
					 
					if(explored.find(child_TURN_SR.estado) == explored.end()){
						frontier.push(child_TURN_SR);
					}
				}
		
				
				if(EsAccionValidaR(TURN_L, current_node.estado)){
					NodoR_N4 child_TURN_L = current_node;
					child_TURN_L.estado = applyR(TURN_L, current_node.estado, terreno, altura, entidades);
					child_TURN_L.secuencia.push_back(TURN_L);
					child_TURN_L.energia += FuncionCoste_R(TURN_L, current_node.estado, terreno, altura);
					child_TURN_L.energia_heuristica = Heuristica(child_TURN_L.estado, final);
					
					if(explored.find(child_TURN_L.estado) == explored.end()){
						frontier.push(child_TURN_L);
					}
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

int ComportamientoRescatador::Heuristica(const EstadoR_N4 &a, const EstadoR_N4 &b){
	int dx = std::abs(a.f - b.f);   
	int dy = std::abs(a.c - b.c); 

	if(dx < dy) return dy;
	else return dx;
}

void ComportamientoRescatador::VisualizaPlan(const EstadoR_N4 &st, const vector<Action> &plan){
	AnularMatrizR(mapaConPlan);
	EstadoR_N4 cst = st;

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
				cst.brujula = (Orientacion)((cst.brujula + 1) % 8);
				break;

			case TURN_L:
				cst.brujula = (Orientacion)((cst.brujula + 6) % 8);
				break;
		}
		it++;
	}
}

int ComportamientoRescatador::FuncionCoste_R(const Action &accion, const EstadoR_N4 &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
		int coste = 0;

		switch(accion){
			case WALK: {
				EstadoR_N4 siguiente = NextCasillaRescatador(st);
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
		
			case RUN: {
				EstadoR_N4 siguiente = NextCasillaRescatador(st);
				EstadoR_N4 siguiente_2 = NextCasillaRescatador(siguiente);
				if(siguiente_2.f < 3 or siguiente_2.f >= terreno.size()-3 or siguiente_2.c < 3 or siguiente_2.c >= terreno[0].size()-3){
					return 100000;
				}
				if(siguiente.f < 0 or siguiente.f >= terreno.size() or siguiente.c < 0 or siguiente.c >= terreno[0].size()){
					return 100000;
				}
				
				int dif_altura = (altura[siguiente_2.f][siguiente_2.c] - altura[st.f][st.c]);
	
				int signo;
				if(dif_altura < 0) signo = -1;
				else if(dif_altura > 0) signo = 1;
				else signo = 0;
	
				switch(terreno[st.f][st.c]){
					case 'A':
						coste = 150 + signo * 15;
						break;
					case 'T':
						coste = 35 + signo * 5;
						break;
					case 'S':
						coste = 3 + signo * 2;
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
					
			case TURN_L: {
				if(st.f < 3 or st.f >= terreno.size()-3 or st.c < 3 or st.c >= terreno[0].size()-3){
					return 100000;
				}
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

EstadoR_N4 ComportamientoRescatador::NextCasillaRescatador(const EstadoR_N4 &st){
	EstadoR_N4 siguiente = st;
	
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

EstadoR_N4 ComportamientoRescatador::applyR(Action accion, const EstadoR_N4 &st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura, const vector<vector<unsigned char>> &entidades){
		EstadoR_N4 next = st;

		switch(accion){
			case WALK: {
				EstadoR_N4 auxiliar = NextCasillaRescatador(st);
				if(auxiliar.f < 3 or auxiliar.f >= terreno.size()-3 or auxiliar.c < 3 or auxiliar.c >= terreno[0].size()-3){
					return st;
				}

				if(terreno[auxiliar.f][auxiliar.c] == '?'){
					next = auxiliar;
				}else{
					bool check2 = abs(altura[auxiliar.f][auxiliar.c] - altura[st.f][st.c]) <= 1;
					bool check3 = ((abs(altura[auxiliar.f][auxiliar.c] - altura[st.f][st.c]) <= 2) and st.zapatillas == true);
					bool check1 = terreno[auxiliar.f][auxiliar.c] != 'P' and terreno[auxiliar.f][auxiliar.c] != 'M' and terreno[auxiliar.f][auxiliar.c] != 'B';
					bool check4 = entidades[auxiliar.f][auxiliar.c] != 'a';

					if(check1 and (check2 or check3) and check4){
						next = auxiliar;
					}
				}
				
				break;
			}
		
			case RUN: {
				EstadoR_N4 intermedia = NextCasillaRescatador(st);
				EstadoR_N4 fin = NextCasillaRescatador(intermedia);
				
				if(fin.f < 3 or fin.f >= terreno.size()-3 or fin.c < 3 or fin.c >= terreno[0].size()-3){
					return st;
				}
				if(intermedia.f < 3 or intermedia.f >= terreno.size()-3 or intermedia.c < 3 or intermedia.c >= terreno[0].size()-3){
					return st;
				}

				if(terreno[fin.f][fin.c] == '?' or terreno[intermedia.f][intermedia.c] == '?'){
					next = fin;
				}else{
					bool check1 = terreno[fin.f][fin.c] != 'P' and terreno[fin.f][fin.c] != 'M' and terreno[fin.f][fin.c] != 'B';
					bool check2 = abs(altura[fin.f][fin.c] - altura[st.f][st.c]) <= 1;
					bool check3 = ((abs(altura[fin.f][fin.c] - altura[st.f][st.c]) <= 2) and st.zapatillas == true);
					bool check4 = terreno[intermedia.f][intermedia.c] != 'P' and terreno[intermedia.f][intermedia.c] != 'M' and terreno[intermedia.f][intermedia.c] != 'B';
					bool check5 = entidades[intermedia.f][intermedia.c] != 'a';
					bool check6 = entidades[fin.f][fin.c] != 'a';
					if(check1 and check4 and (check2 or check3) and check5 and check6){
						next = fin;
					}
				}

				break;
			}
		
			case TURN_SR: {
				next.brujula = (Orientacion)((next.brujula + 1) % 8);
				break;
			}
		
			case TURN_L: {
				next.brujula = (Orientacion)((next.brujula + 6) % 8);
				break;
			}
		}
		

		if(terreno[next.f][next.c] == 'D'){
			next.zapatillas = true;
		}
	
		return next;
}

void ComportamientoRescatador::ModificarMapaR(const Sensores &sensores, vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a,
	vector<vector<unsigned char>> &e){

	for(int i = 0; i < sensores.superficie.size(); i++){
		if(sensores.superficie[i] == 'D'){
			zapatillas_vistas = true;
		}
	}
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

			e[sensores.posF][sensores.posC] = sensores.agentes[0];
			e[sensores.posF-1][sensores.posC-1] = sensores.agentes[1];
			e[sensores.posF-1][sensores.posC] = sensores.agentes[2];
			e[sensores.posF-1][sensores.posC+1] = sensores.agentes[3];
			e[sensores.posF-2][sensores.posC-2] = sensores.agentes[4];
			e[sensores.posF-2][sensores.posC-1] = sensores.agentes[5];
			e[sensores.posF-2][sensores.posC] = sensores.agentes[6];
			e[sensores.posF-2][sensores.posC+1] = sensores.agentes[7];
			e[sensores.posF-2][sensores.posC+2] = sensores.agentes[8];
			e[sensores.posF-3][sensores.posC-3] = sensores.agentes[9];
			e[sensores.posF-3][sensores.posC-2] = sensores.agentes[10];
			e[sensores.posF-3][sensores.posC-1] = sensores.agentes[11];
			e[sensores.posF-3][sensores.posC] = sensores.agentes[12];
			e[sensores.posF-3][sensores.posC+1] = sensores.agentes[13];
			e[sensores.posF-3][sensores.posC+2] = sensores.agentes[14];
			e[sensores.posF-3][sensores.posC+3] = sensores.agentes[15];

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

			e[sensores.posF][sensores.posC] = sensores.agentes[0];
			e[sensores.posF][sensores.posC-1] = sensores.agentes[1];
			e[sensores.posF-1][sensores.posC-1] = sensores.agentes[2];
			e[sensores.posF-1][sensores.posC] = sensores.agentes[3];
			e[sensores.posF][sensores.posC-2] = sensores.agentes[4];
			e[sensores.posF-1][sensores.posC-2] = sensores.agentes[5];
			e[sensores.posF-2][sensores.posC-2] = sensores.agentes[6];
			e[sensores.posF-2][sensores.posC-1] = sensores.agentes[7];
			e[sensores.posF-2][sensores.posC] = sensores.agentes[8];
			e[sensores.posF][sensores.posC-3] = sensores.agentes[9];
			e[sensores.posF-1][sensores.posC-3] = sensores.agentes[10];
			e[sensores.posF-2][sensores.posC-3] = sensores.agentes[11];
			e[sensores.posF-3][sensores.posC-3] = sensores.agentes[12];
			e[sensores.posF-3][sensores.posC-2] = sensores.agentes[13];
			e[sensores.posF-3][sensores.posC-1] = sensores.agentes[14];
			e[sensores.posF-3][sensores.posC] = sensores.agentes[15];

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

			e[sensores.posF][sensores.posC] = sensores.agentes[0];
			e[sensores.posF+1][sensores.posC-1] = sensores.agentes[1];
			e[sensores.posF][sensores.posC-1] = sensores.agentes[2];
			e[sensores.posF-1][sensores.posC-1] = sensores.agentes[3];
			e[sensores.posF+2][sensores.posC-2] = sensores.agentes[4];
			e[sensores.posF+1][sensores.posC-2] = sensores.agentes[5];
			e[sensores.posF][sensores.posC-2] = sensores.agentes[6];
			e[sensores.posF-1][sensores.posC-2] = sensores.agentes[7];
			e[sensores.posF-2][sensores.posC-2] = sensores.agentes[8];
			e[sensores.posF+3][sensores.posC-3] = sensores.agentes[9];
			e[sensores.posF+2][sensores.posC-3] = sensores.agentes[10];
			e[sensores.posF+1][sensores.posC-3] = sensores.agentes[11];
			e[sensores.posF][sensores.posC-3] = sensores.agentes[12];
			e[sensores.posF-1][sensores.posC-3] = sensores.agentes[13];
			e[sensores.posF-2][sensores.posC-3] = sensores.agentes[14];
			e[sensores.posF-3][sensores.posC-3] = sensores.agentes[15];
	
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

			e[sensores.posF][sensores.posC] = sensores.agentes[0];
			e[sensores.posF][sensores.posC+1] = sensores.agentes[1];
			e[sensores.posF+1][sensores.posC+1] = sensores.agentes[2];
			e[sensores.posF+1][sensores.posC] = sensores.agentes[3];
			e[sensores.posF][sensores.posC+2] = sensores.agentes[4];
			e[sensores.posF+1][sensores.posC+2] = sensores.agentes[5];
			e[sensores.posF+2][sensores.posC+2] = sensores.agentes[6];
			e[sensores.posF+2][sensores.posC+1] = sensores.agentes[7];
			e[sensores.posF+2][sensores.posC] = sensores.agentes[8];
			e[sensores.posF][sensores.posC+3] = sensores.agentes[9];
			e[sensores.posF+1][sensores.posC+3] = sensores.agentes[10];
			e[sensores.posF+2][sensores.posC+3] = sensores.agentes[11];
			e[sensores.posF+3][sensores.posC+3] = sensores.agentes[12];
			e[sensores.posF+3][sensores.posC+2] = sensores.agentes[13];
			e[sensores.posF+3][sensores.posC+1] = sensores.agentes[14];
			e[sensores.posF+3][sensores.posC] = sensores.agentes[15];

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

			e[sensores.posF][sensores.posC] = sensores.agentes[0];
			e[sensores.posF+1][sensores.posC+1] = sensores.agentes[1];
			e[sensores.posF+1][sensores.posC] = sensores.agentes[2];
			e[sensores.posF+1][sensores.posC-1] = sensores.agentes[3];
			e[sensores.posF+2][sensores.posC+2] = sensores.agentes[4];
			e[sensores.posF+2][sensores.posC+1] = sensores.agentes[5];
			e[sensores.posF+2][sensores.posC] = sensores.agentes[6];
			e[sensores.posF+2][sensores.posC-1] = sensores.agentes[7];
			e[sensores.posF+2][sensores.posC-2] = sensores.agentes[8];
			e[sensores.posF+3][sensores.posC+3] = sensores.agentes[9];
			e[sensores.posF+3][sensores.posC+2] = sensores.agentes[10];
			e[sensores.posF+3][sensores.posC+1] = sensores.agentes[11];
			e[sensores.posF+3][sensores.posC] = sensores.agentes[12];
			e[sensores.posF+3][sensores.posC-1] = sensores.agentes[13];
			e[sensores.posF+3][sensores.posC-2] = sensores.agentes[14];
			e[sensores.posF+3][sensores.posC-3] = sensores.agentes[15];

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

			e[sensores.posF][sensores.posC] = sensores.agentes[0];
			e[sensores.posF+1][sensores.posC] = sensores.agentes[1];
			e[sensores.posF+1][sensores.posC-1] = sensores.agentes[2];
			e[sensores.posF][sensores.posC-1] = sensores.agentes[3];
			e[sensores.posF+2][sensores.posC] = sensores.agentes[4];
			e[sensores.posF+2][sensores.posC-1] = sensores.agentes[5];
			e[sensores.posF+2][sensores.posC-2] = sensores.agentes[6];
			e[sensores.posF+1][sensores.posC-2] = sensores.agentes[7];
			e[sensores.posF][sensores.posC-2] = sensores.agentes[8];
			e[sensores.posF+3][sensores.posC] = sensores.agentes[9];
			e[sensores.posF+3][sensores.posC-1] = sensores.agentes[10];
			e[sensores.posF+3][sensores.posC-2] = sensores.agentes[11];
			e[sensores.posF+3][sensores.posC-3] = sensores.agentes[12];
			e[sensores.posF+2][sensores.posC-3] = sensores.agentes[13];
			e[sensores.posF+1][sensores.posC-3] = sensores.agentes[14];
			e[sensores.posF][sensores.posC-3] = sensores.agentes[15];

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

			e[sensores.posF][sensores.posC] = sensores.agentes[0];
			e[sensores.posF-1][sensores.posC+1] = sensores.agentes[1];
			e[sensores.posF][sensores.posC+1] = sensores.agentes[2];
			e[sensores.posF+1][sensores.posC+1] = sensores.agentes[3];
			e[sensores.posF-2][sensores.posC+2] = sensores.agentes[4];
			e[sensores.posF-1][sensores.posC+2] = sensores.agentes[5];
			e[sensores.posF][sensores.posC+2] = sensores.agentes[6];
			e[sensores.posF+1][sensores.posC+2] = sensores.agentes[7];
			e[sensores.posF+2][sensores.posC+2] = sensores.agentes[8];
			e[sensores.posF-3][sensores.posC+3] = sensores.agentes[9];
			e[sensores.posF-2][sensores.posC+3] = sensores.agentes[10];
			e[sensores.posF-1][sensores.posC+3] = sensores.agentes[11];
			e[sensores.posF][sensores.posC+3] = sensores.agentes[12];
			e[sensores.posF+1][sensores.posC+3] = sensores.agentes[13];
			e[sensores.posF+2][sensores.posC+3] = sensores.agentes[14];
			e[sensores.posF+3][sensores.posC+3] = sensores.agentes[15];

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

			e[sensores.posF][sensores.posC] = sensores.agentes[0];
			e[sensores.posF-1][sensores.posC] = sensores.agentes[1];
			e[sensores.posF-1][sensores.posC+1] = sensores.agentes[2];
			e[sensores.posF][sensores.posC+1] = sensores.agentes[3];
			e[sensores.posF-2][sensores.posC] = sensores.agentes[4];
			e[sensores.posF-2][sensores.posC+1] = sensores.agentes[5];
			e[sensores.posF-2][sensores.posC+2] = sensores.agentes[6];
			e[sensores.posF-1][sensores.posC+2] = sensores.agentes[7];
			e[sensores.posF][sensores.posC+2] = sensores.agentes[8];
			e[sensores.posF-3][sensores.posC] = sensores.agentes[9];
			e[sensores.posF-3][sensores.posC+1] = sensores.agentes[10];
			e[sensores.posF-3][sensores.posC+2] = sensores.agentes[11];
			e[sensores.posF-3][sensores.posC+3] = sensores.agentes[12];
			e[sensores.posF-2][sensores.posC+3] = sensores.agentes[13];
			e[sensores.posF-1][sensores.posC+3] = sensores.agentes[14];
			e[sensores.posF][sensores.posC+3] = sensores.agentes[15];

			break;
	}
}


bool ComportamientoRescatador::EsAccionValidaR(const Action &accion, const EstadoR_N4 &estado) {
	if (accionesProhibidas[estado].count(accion) > 0) {
        return false; // La acción está prohibida
    }
	return true; // La acción es válida
}

Action ComportamientoRescatador::BuscaZapatillas(Sensores &sensores) {

	Action accion = IDLE;
	ModificarMapaR(sensores, mapaResultado, mapaCotas, mapaEntidades);
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(!hayPlanZapatillas and zapatillas_vistas){
		int distancia = 5000;
		int f = -1;
		int c = -1;
		for(int i = 0; i < mapaResultado.size(); i++){
			for(int j = 0; j < mapaResultado[0].size(); j++){
				if(mapaResultado[i][j] == 'D' and abs(i - sensores.posF) + abs(j - sensores.posC) < distancia){
					distancia = abs(i - sensores.posF) + abs(j - sensores.posC);
					f = i;
					c = j;
				}
			}
		}
		if(f != -1 and c != -1){
			EstadoR_N4 inicio, fin;
			inicio.f = sensores.posF;
			inicio.c = sensores.posC;
			inicio.brujula = sensores.rumbo;
			inicio.zapatillas = tiene_zapatillas;
			fin.f = f;
			fin.c = c;
			current_state = inicio;
			plan_N4  = AlgoritmoAE(inicio, fin, mapaResultado, mapaCotas, mapaEntidades);
			VisualizaPlan(inicio, plan_N4);
			hayPlanZapatillas = plan_N4.size() != 0;
		}
	}
	if(hayPlanZapatillas and plan_N4.size()>0){

		accion = plan_N4.front();

		last_state = current_state;
		current_state = applyR(accion, current_state, mapaResultado, mapaCotas, mapaEntidades);
		
		if(current_state == last_state){
			accionesProhibidas[last_state].insert(accion);
			hayPlanZapatillas = false;
			plan_N4.clear();
			return IDLE;
		}else if(accion == WALK and (sensores.agentes[2] != '_')){
			plan_N4.clear();
			return IDLE;
		}else{
			accion = plan_N4.front();
			auto it = plan_N4.begin();
			it = plan_N4.erase(it);
		}
	}
	if(plan_N4.size()==0 and hayPlanZapatillas){
		hayPlanZapatillas=false;
	}

	if(accion == IDLE){
		if(!cola.empty()){
			accion = cola.front();
			cola.pop();
		}
		else {
			int pos = VeoCasillaInteresanteR_N1(sensores, tiene_zapatillas);
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
				case 0:
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
	
				case 7:
					cola.push(TURN_L);
					cola.push(TURN_L);
					
					break;
			}
		}
	
		
		last_action = accion;
	}
	return accion;
}
