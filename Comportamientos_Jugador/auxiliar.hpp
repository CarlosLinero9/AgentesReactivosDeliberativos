#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>

#include "comportamientos/comportamiento.hpp"


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
};

class ComportamientoAuxiliar : public Comportamiento
{

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
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

  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);


  /*Parte 0*/
  int VeoCasillaInteresanteA(char i, char c, char d);
  int VeoCasillaInteresanteA(char i, char c, char d, bool i_libre, bool c_libre, bool d_libre);
  char ViablePorAlturaA (char casilla, int dif);
  bool CasillaLibreA(char casilla);
  void SituarSensorenMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores);

  /*Parte 1*/
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

  
private:
  // Definir Variables de Estado
  Action last_action;
  bool tiene_zapatillas;
  int giro45izq;

  //Variables de estado para el nivel E
  list<Action> plan;
  bool hayPlan;
};

#endif
