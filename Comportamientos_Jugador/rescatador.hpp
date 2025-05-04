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
#include <map>


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
struct EstadoR_N4{
  int f;
  int c;
  Orientacion brujula;
  bool zapatillas;


  bool operator==(const EstadoR_N4 &st) const
  {
    return (f == st.f and c == st.c and brujula == st.brujula 
           and zapatillas == st.zapatillas);
  }

  bool operator<(const EstadoR_N4 &st) const
  {
    if (f < st.f) return true;
    else if (f == st.f && c < st.c) return true;
    else if (f == st.f && c == st.c && brujula < st.brujula) return true;
    else if (f == st.f && c == st.c && brujula == st.brujula && zapatillas < st.zapatillas) return true;
    else return false;
  }
};

struct NodoR_N4{
  EstadoR_N4 estado;
  vector<Action> secuencia;
  int energia;
  int energia_heuristica;

  bool operator==(const NodoR_N4 &nodo) const
  {
    return (estado == nodo.estado);
  }

  bool operator<(const NodoR_N4 &node) const
  {
    return ((node.energia + node.energia_heuristica) < (energia + energia_heuristica));
  }
};


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
    accion_defecto = false;
    frecuencia_visita = vector<vector<int>>(mapaResultado.size(), vector<int>(mapaResultado[0].size(), 0));
    objetivo = {-1, -1};
    cola_acciones = queue<int>();
    cola = queue<Action>();
    sigo_plan = false;
    plan_N4 = vector<Action>();
    hayPlan = false;
    hayPlanEnergia = false;
    auxiliarAvisado = false;

  }

  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan = false;
    tiene_zapatillas = false;
    plan = list<Action>();
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
  int VeoCasillaInteresanteR_N0(Sensores & sensores, bool zap);
  char ViablePorAlturaR (char casilla, int dif, bool zap);
  bool CasillaLibreR(char casilla);
  void SituarSensorenMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores);
  int DetectarCasillaInteresanteR(Sensores &sensores, bool zap);
  bool PuedeCorrer_N0(int i, Sensores &sensores, bool zap);
  // void RefrescarmatrizR(vector<vector<int>> &m);
  // int minimo(int a, int b, int c);

  /*NIVEL 1*/
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  int DetectarCasillaZapatillasR(Sensores &sensores, bool zap);
  int VeoCasillaInteresanteR_N1(Sensores & sensores, bool zap);
  bool EsTransitableR(char casilla);
  bool PuedeCorrer_N1(int i, Sensores &sensores, bool zap);
  bool DetectarAgenteR(Sensores &sensores);



  
  
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
  vector<Action> AlgoritmoAE(const EstadoR_N4 &inicio, const EstadoR_N4 &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  int Heuristica(const EstadoR_N4 &a, const EstadoR_N4 &b);
  void VisualizaPlan(const EstadoR_N4 &st, const vector<Action> &plan);
  int FuncionCoste_R(const Action &accion, const EstadoR_N4 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  EstadoR_N4 NextCasillaRescatador(const EstadoR_N4 &st);
  EstadoR_N4 applyR(Action accion, const EstadoR_N4 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  void ModificarMapaR(const Sensores &sensores, vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a);
  bool EsAccionValidaR(const Action &accion, const EstadoR_N4 &estado);



  
  


  ///////////////////////////////////////////////////////////////////////////////////////////
  /*VARIABLES*//////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

private:

  //Constantes

  /*NIVEL 0, 1 y 4*/
  const int SUMA_AL_VISITAR = 30; //Suma que se le añade a la casilla que se va a visitar
  const int SUMA_AL_VER = 1; //Suma que se le añade a la casilla que se ve en el cono de vision

  // Variables de Estado
  bool accion_defecto;
  queue<int> cola_acciones;
  queue<Action> cola;
  bool sigo_plan;
  Action last_action;
  bool tiene_zapatillas;
  vector<vector<int>> frecuencia_visita;
  pair<int,int> objetivo;
  
  /*NIVELES 2, 3 Y 4*/
  //Variables de estado
  list<Action> plan;
  vector<Action> plan_N4;
  bool hayPlan;
  bool hayPlanEnergia;
  map<EstadoR_N4, set<Action>> accionesProhibidas;

  EstadoR_N4 current_state;
  EstadoR_N4 last_state;

  bool auxiliarAvisado;
};

#endif