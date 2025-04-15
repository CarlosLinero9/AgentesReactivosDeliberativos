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
  int f_rescatador;
  int c_rescatador;
  int brujula_rescatador;
  int f_auxiliar;
  int c_auxiliar;
  int brujula_auxiliar;
  bool zapatillas;

  bool operator==(const EstadoA_N3 &st) const
  {
    return (f_auxiliar == st.f_auxiliar and c_auxiliar == st.c_auxiliar and
      brujula_auxiliar == st.brujula_auxiliar and zapatillas == st.zapatillas);
  }

  bool operator<(const EstadoA_N3 &st) const
  {
    return (f_auxiliar < st.f_auxiliar) or (f_auxiliar==st.f_auxiliar and c_auxiliar < st.c_auxiliar) or
      (f_auxiliar==st.f_auxiliar and c_auxiliar==st.c_auxiliar and
      brujula_auxiliar < st.brujula_auxiliar) or (f_auxiliar==st.f_auxiliar and c_auxiliar==st.c_auxiliar and
      brujula_auxiliar==st.brujula_auxiliar and zapatillas < st.zapatillas);
  }
};

struct NodoA_N3{
  EstadoA_N3 estado;
  list<Action> secuencia;
  int energia;
  int energia_heuristica;

  bool operator==(const NodoA_N3 &nodo) const
  {
    return (estado == nodo.estado);
  }

  bool operator<(const NodoA_N3 &node) const
  {
    return (estado<node.estado);
  }

  bool operator>(const NodoA_N3 &node) const
  {
    return (energia+energia_heuristica>node.energia+node.energia_heuristica);
  }

};

class Compara_N3{
  public:
    bool operator()(const NodoA_N3 &nodo1, const NodoA_N3 &nodo2) const
    {
      return nodo1>nodo2;
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
  }
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan = false;
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
  bool EsTransitableA(char casilla);
  void CalcularObjetivoA();
  void GuardarFrecuenciasA();
  void RecuperarFrecuenciasA();
  

  ///////////////////////////////////////////////////////////////////////////////////////////
  /*PARTE DELIBERATIVA*//////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////


  /*TUTORIAL*/
  Action ComportamientoAuxiliarNivel_E(Sensores sensores);
  list<Action> AnchuraAuxiliar(const EstadoA &inicio, const EstadoA &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  void VisualizaPlan(const EstadoA &st, const list<Action> &plan);
  bool CasillaAccesibleAuxiliar(const EstadoA &st, const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  EstadoA NextCasillaAuxiliar(const EstadoA &st);
  EstadoA applyA(Action accion, const EstadoA &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  bool Find(const NodoA &st,const list<NodoA> &lista);
  void AnularMatrizA(vector<vector<unsigned char>> &m);
  list<Action> AnchuraAuxiliar_V2(const EstadoA &inicio, const EstadoA &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);

  /*NIVEL 2*/
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  
  /*NIVEL 3*/
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  int FuncionCoste(const Action &accion, const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  EstadoA_N3 NextCasillaAuxiliar(const EstadoA_N3 &st);
  EstadoA_N3 applyA(Action accion, const EstadoA_N3 &st, const vector<vector<unsigned char>> &terreno,
    const vector<vector<unsigned char>> &altura);
  bool CasillaAccesibleAuxiliar(const EstadoA_N3 &st, 
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);
  void VisualizaPlan(const EstadoA_N3 &st, const list<Action> &plan);
  list<Action> AlgoritmoAE(const EstadoA_N3 &inicio, const EstadoA_N3 &final,
    const vector<vector<unsigned char>> &terreno, const vector<vector<unsigned char>> &altura);


  /*NIVEL 4*/
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);
  

  ///////////////////////////////////////////////////////////////////////////////////////////
  /*VARIABLES*//////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
private:
  // Definir Variables de Estado

  /*NIVELES 0, 1 y 4*/
  //Constantes
  const int MAX_PASOS = 125;  //Maximo de pasos antes de refrescar la matriz
  const int SUMA_AL_VISITAR = 3; //Suma que se le añade a la casilla que se va a visitar
  const int SUMA_AL_VER = 1; //Suma que se le añade a la casilla que se ve en el cono de vision

  // Variables de Estado
  bool accion_defecto;
  Action last_action;
  bool tiene_zapatillas;
  int giro45izq;
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
