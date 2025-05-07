#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>
#include <queue>
#include <iostream>
#include <utility>
#include <algorithm>
#include <set>
#include <map>

#include "comportamientos/comportamiento.hpp"

////////////////////////////////////////////////////////////////////////////
//ESTRUCTURAS AUXILIARES NIVELES DELIBERATIVOS//////////////////////////////
////////////////////////////////////////////////////////////////////////////

/*TUTORIAL*/
struct EstadoA{
  int f;
  int c;
  int brujula;
  bool zapatillas;

  bool operator==(const EstadoA &st) const
  {
    return (f == st.f and c == st.c and brujula == st.brujula 
           and zapatillas == st.zapatillas);
  }
};

struct NodoA{
  EstadoA estado;
  list<Action> secuencia;

  bool operator==(const NodoA &nodo) const
  {
    return (estado == nodo.estado);
  }

  bool operator<(const NodoA &node) const
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


/*NIVEL 3*/

struct EstadoA_N3{
  int f;
  int c;
  int brujula;
  bool zapatillas;

  bool operator==(const EstadoA_N3 &st) const
  {
    return (f == st.f and c == st.c and brujula == st.brujula 
           and zapatillas == st.zapatillas);
  }

  bool operator<(const EstadoA_N3 &st) const
  {
    if (f < st.f) return true;
    else if (f == st.f && c < st.c) return true;
    else if (f == st.f && c == st.c && brujula < st.brujula) return true;
    else if (f == st.f && c == st.c && brujula == st.brujula && zapatillas < st.zapatillas) return true;
    else return false;
  }
};

struct NodoA_N3{
  EstadoA_N3 estado;
  vector<Action> secuencia;
  int energia;
  int energia_heuristica;

  bool operator==(const NodoA_N3 &nodo) const
  {
    return (estado == nodo.estado);
  }

  bool operator<(const NodoA_N3 &node) const
  {
    return ((node.energia + node.energia_heuristica) < (energia + energia_heuristica));
  }
};

/*NIVEL 4*/
struct EstadoA_N4{
  int f;
  int c;
  Orientacion brujula;
  bool zapatillas;


  bool operator==(const EstadoA_N4 &st) const
  {
    return (f == st.f and c == st.c and brujula == st.brujula 
           and zapatillas == st.zapatillas);
  }

  bool operator<(const EstadoA_N4 &st) const
  {
    if (f < st.f) return true;
    else if (f == st.f && c < st.c) return true;
    else if (f == st.f && c == st.c && brujula < st.brujula) return true;
    else if (f == st.f && c == st.c && brujula == st.brujula && zapatillas < st.zapatillas) return true;
    else return false;
  }
};

struct NodoA_N4{
  EstadoA_N4 estado;
  vector<Action> secuencia;
  int energia;
  int energia_heuristica;

  bool operator==(const NodoA_N4 &nodo) const
  {
    return (estado == nodo.estado);
  }

  bool operator<(const NodoA_N4 &node) const
  {
    return ((node.energia + node.energia_heuristica) < (energia + energia_heuristica));
  }
};

///////////////////////////////////////////////////////////////////////////////////////////
// Definicion de la clase ComportamientoAuxiliar///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class ComportamientoAuxiliar : public Comportamiento
{

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    accion_defecto = false;
    giro45izq = 0;
    frecuencia_visita = vector<vector<int>>(mapaResultado.size(), vector<int>(mapaResultado[0].size(), 0));
    frecuencia_visita_aux = vector<vector<int>>(mapaResultado.size(), vector<int>(mapaResultado[0].size(), 0));
    pasos = 0;
    objetivo = {-1, -1};
    cola = false;
    cola_acciones = queue<int>();
    plan_N4 = vector<Action>();
    hayPlan = false;
    hayPlanEnergia = false;
    iteraciones = 3000;
    iteraciones_busqueda = 0;
  }
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan = false;
    plan = vector<Action>();
  }
  ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
  ~ComportamientoAuxiliar() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  
  ///////////////////////////////////////////////////////////////////////////////////////////
  /*PARTE REACTIVA*//////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////


  /*NIVEL 0*/
  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  int VeoCasillaInteresanteA(Sensores &sensores, bool zap);
  char ViablePorAlturaA (char casilla, int dif, bool zap);
  bool CasillaLibreA(char casilla);
  void SituarSensorenMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores);
  int DetectarCasillaInteresanteA(Sensores &sensores, bool zap);
  bool NoVisitaFrecuente(int frecuencia);

  /*NIVEL 1*/
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);
  int DetectarCasillaZapatillasA(Sensores &sensores, bool zap);
  int VeoCasillaInteresanteA_N1(Sensores & sensores, bool zap);
  bool EsTransitableA(int nivel, char casilla);
  

  ///////////////////////////////////////////////////////////////////////////////////////////
  /*PARTE DELIBERATIVA*//////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////


  /*TUTORIAL*/
   void AnularMatrizA(vector<vector<unsigned char>> &m);
  
  /*NIVEL 2*/
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  
  /*NIVEL 3*/
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  int FuncionCoste_A(const Action &accion, const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  EstadoA_N3 NextCasillaAuxiliar(const EstadoA_N3 &st);
  EstadoA_N3 applyA(Action accion, const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  bool CasillaAccesibleAuxiliar(const EstadoA_N3 &st, 
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  void VisualizaPlan(const EstadoA_N3 &st, const vector<Action> &plan);
  vector<Action> AlgoritmoAE(const EstadoA_N3 &inicio, const EstadoA_N3 &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  int Heuristica(const EstadoA_N3 &a, const EstadoA_N3 &b);


  /*NIVEL 4*/
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);
  vector<Action> AlgoritmoAE(const EstadoA_N4 &inicio, const EstadoA_N4 &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  vector<Action> AlgoritmoAE_N4(const EstadoA_N4 &inicio, const EstadoA_N4 &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  int Heuristica(const EstadoA_N4 &a, const EstadoA_N4 &b);
  void VisualizaPlan(const EstadoA_N4 &st, const vector<Action> &plan);
  int FuncionCoste_A(const Action &accion, const EstadoA_N4 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  EstadoA_N4 NextCasillaAuxiliar(const EstadoA_N4 &st);
  EstadoA_N4 applyA(Action accion, const EstadoA_N4 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  void ModificarMapaA(const Sensores &sensores, vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a);
  bool EsAccionValidaA(const Action &accion, const EstadoA_N4 &estado);
  bool DestinoEnConoVision(const EstadoA_N4 &estado, const EstadoA_N4 &final);
  vector<pair<int, int>> ObtenerConoVision(const EstadoA_N4 &estado);
  Action BuscaZapatillas(Sensores &sensores);




  
  

  ///////////////////////////////////////////////////////////////////////////////////////////
  /*VARIABLES*//////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
private:
  // Definir Variables de Estado

  /*NIVELES 0, 1 y 4*/
  //Constantes
  const int SUMA_AL_VISITAR = 30; //Suma que se le añade a la casilla que se va a visitar
  const int SUMA_AL_VER = 1; //Suma que se le añade a la casilla que se ve en el cono de vision
  const int ITERACIONES_BUSQUEDA_ZAP = 500;


  // Variables de Estado
  bool accion_defecto;
  queue<int> cola_acciones;
  bool cola;
  Action last_action;
  bool tiene_zapatillas;
  int giro45izq;
  vector<vector<int>> frecuencia_visita;
  vector<vector<int>> frecuencia_visita_aux;
  int pasos;
  pair<int,int> objetivo;

  /*NIVELES 2,3 Y 4*/
  //Variables de estado
  vector<Action> plan;
  bool hayPlan;
  vector<Action> plan_N4;
  bool hayPlanEnergia;
  map<EstadoA_N4, set<Action>> accionesProhibidas;

  EstadoA_N4 current_state;
  EstadoA_N4 last_state;
  int iteraciones;
  int iteraciones_busqueda;

};

#endif
