#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>
#include <set>
#include <queue>
#include <iostream>
#include <algorithm>

#include "comportamientos/comportamiento.hpp"

////////////////////////////////////////////////////////////////////////////
//ESTRUCTURAS AUXILIARES NIVELES DELIBERATIVOS//////////////////////////////
////////////////////////////////////////////////////////////////////////////

/*TUTORIAL*/
struct EstadoR{
  int f;
  int c;
  int brujula;
  bool zapatillas;

  bool operator==(const EstadoR &st) const
  {
    return (f == st.f and c == st.c and brujula == st.brujula 
           and zapatillas == st.zapatillas);
  }
};

struct NodoR{
  EstadoR estado;
  list<Action> secuencia; //vector aquí

  bool operator==(const NodoR &nodo) const
  {
    return (estado == nodo.estado);
  }

  bool operator<(const NodoR &node) const
  {
    if(estado.f < node.estado.f) return true;
    else if(estado.f == node.estado.f and estado.c < node.estado.c) return true;
    else if(estado.f == node.estado.f and estado.c == node.estado.c and estado.brujula 
          < node.estado.brujula) return true;
    else if(estado.f == node.estado.f and estado.c == node.estado.c and estado.brujula
         == node.estado.brujula and estado.zapatillas < node.estado.zapatillas) return true;
    else return false;
  }
};

/*NIVEL 2*/
struct EstadoR_N2{
  int f;
  int c;
  Orientacion brujula;
  bool zapatillas;

  bool operator==(const EstadoR_N2 &st) const
  {
    return (f == st.f and c == st.c and brujula == st.brujula 
           and zapatillas == st.zapatillas);
  }

  bool operator<(const EstadoR_N2 &st) const
  {
    if (f < st.f) return true;
    else if (f == st.f && c < st.c) return true;
    else if (f == st.f && c == st.c && brujula < st.brujula) return true;
    else if (f == st.f && c == st.c && brujula == st.brujula && zapatillas < st.zapatillas) return true;
    else return false;
  }
};

struct NodoR_N2{
  EstadoR_N2 estado;
  list<Action> secuencia;
  int energia;

  bool operator==(const NodoR_N2 &nodo) const
  {
    return (estado == nodo.estado);
  }

  bool operator<(const NodoR_N2 &node) const
  {
    return (estado<node.estado);
  }

  bool operator>(const NodoR_N2 &nodo) const
  {
    return (energia>nodo.energia);
  }

};

struct Compare {
  bool operator()(const NodoR_N2& a, const NodoR_N2& b) {
      return (a>b);
  }
};

/*NIVEL 3*/

/*NIVEL 4*/



///////////////////////////////////////////////////////////////////////////////////////////
// Definicion de la clase ComportamientoRescatador/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class ComportamientoRescatador : public Comportamiento
{

public:
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
    accion_defecto = false;
    frecuencia_visita = vector<vector<int>>(mapaResultado.size(), vector<int>(mapaResultado[0].size(), 0));
    frecuencia_visita_aux = vector<vector<int>>(mapaResultado.size(), vector<int>(mapaResultado[0].size(), 0));
    pasos = 0;
    objetivo = {-1, -1};
    cola_acciones = queue<int>();
    cola = queue<Action>();
    sigo_plan = false;
  }

  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan = false;
    tiene_zapatillas = false;
  }
  ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}
  ~ComportamientoRescatador() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  ///////////////////////////////////////////////////////////////////////////////////////////
  /*PARTE REACTIVA*//////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

  /*NIVEL 0*/
  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  //bool NoVisitaFrecuente(int frecuencia);
  int VeoCasillaInteresanteR(Sensores & sensores, bool zap);
  char ViablePorAlturaR (char casilla, int dif, bool zap);
  bool CasillaLibreR(char casilla);
  void SituarSensorenMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores);
  int DetectarCasillaInteresanteR(Sensores &sensores, bool zap);
  bool PuedeCorrer(int i, Sensores &sensores, bool zap);
  // void RefrescarmatrizR(vector<vector<int>> &m);
  // int minimo(int a, int b, int c);

  /*NIVEL 1*/
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  int DetectarCasillaZapatillasR(Sensores &sensores, bool zap);
  int VeoCasillaInteresanteR_N1(Sensores & sensores, bool zap);
  bool EsTransitableR(char casilla);
  void CalcularObjetivoR();
  void GuardarFrecuenciasR();
  void RecuperarFrecuenciasR();
  
  
  ///////////////////////////////////////////////////////////////////////////////////////////
  /*PARTE DELIBERATIVA*//////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

  /*TUTORIAL*/
  Action ComportamientoRescatadorNivel_E(Sensores sensores);
  list<Action> AnchuraRescatador(const EstadoR &inicio, const EstadoR &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  void VisualizaPlan(const EstadoR &st, const list<Action> &plan);
  bool CasillaAccesibleRescatador(const EstadoR &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  EstadoR NextCasillaRescatador(const EstadoR &st);
  EstadoR applyR(Action accion, const EstadoR &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  bool Find(const NodoR &st,const list<NodoR> &lista);
  void AnularMatrizR(vector<vector<unsigned char>> &m);
  list<Action> AnchuraRescatador_V2(const EstadoR &inicio, const EstadoR &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  void PintaPlan(const list<Action> &plan, bool zap);
  

  /*NIVEL 2*/
  Action ComportamientoRescatadorNivel_2(Sensores sensores);
  list<Action> DijsktraR(const EstadoR_N2 &inicio, const EstadoR_N2 &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  int FuncionCoste(const Action &accion, const EstadoR_N2 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  EstadoR_N2 NextCasillaRescatador(const EstadoR_N2 &st);
  EstadoR_N2 applyR(Action accion, const EstadoR_N2 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  bool CasillaAccesibleRescatador(const EstadoR_N2 &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  void VisualizaPlan(const EstadoR_N2 &st, const list<Action> &plan);


  /*NIVEL 3*/
  Action ComportamientoRescatadorNivel_3(Sensores sensores);

  /*NIVEL 4*/
  Action ComportamientoRescatadorNivel_4(Sensores sensores);
  
  


  ///////////////////////////////////////////////////////////////////////////////////////////
  /*VARIABLES*//////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

private:

  //Constantes

  /*NIVEL 0, 1 y 4*/
  const int MAX_PASOS = 75;  //Maximo de pasos antes de refrescar la matriz
  const int SUMA_AL_VISITAR = 4; //Suma que se le añade a la casilla que se va a visitar
  const int SUMA_AL_VER = 1; //Suma que se le añade a la casilla que se ve en el cono de vision

  // Variables de Estado
  bool accion_defecto;
  queue<int> cola_acciones;
  queue<Action> cola;
  bool corriendo;
  bool sigo_plan;
  Action last_action;
  bool tiene_zapatillas;
  int giro45izq;
  int giro45dech;
  vector<vector<int>> frecuencia_visita;
  vector<vector<int>> frecuencia_visita_aux;
  int pasos;
  pair<int,int> objetivo;

  /*NIVELES 2 Y 3*/
  //Variables de estado
  list<Action> plan;
  bool hayPlan;
};

#endif