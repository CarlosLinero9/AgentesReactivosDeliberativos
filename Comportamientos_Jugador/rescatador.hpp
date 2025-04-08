#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <chrono>
#include <time.h>
#include <thread>
#include <list>
#include <set>
#include <iostream>

#include "comportamientos/comportamiento.hpp"

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
  list<Action> secuencia;

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

class ComportamientoRescatador : public Comportamiento
{

public:
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    // Inicializar Variables de Estado Niveles 0,1,4
    last_action = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
    frecuencia_visita = vector<vector<int>>(mapaResultado.size(), vector<int>(mapaResultado[0].size(), 0));
    pasos = 0;
  }

  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
    hayPlan = false;
  }
  ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}
  ~ComportamientoRescatador() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  Action ComportamientoRescatadorNivel_2(Sensores sensores);
  Action ComportamientoRescatadorNivel_3(Sensores sensores);
  Action ComportamientoRescatadorNivel_4(Sensores sensores);


  /*Parte 0*/
  bool NoVisitaFrecuente(int f, int c);
  int VeoCasillaInteresanteR(Sensores & sensores, bool zap);
  char ViablePorAlturaR (char casilla, int dif, bool zap);
  bool CasillaLibreR(char casilla);
  void SituarSensorenMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores);
  void RefrescarmatrizR(vector<vector<int>> &m);
  


  /*Parte 1*/
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
  

private:

  //Constantes
  const int MAX_PASOS = (mapaResultado.size()*mapaResultado[0].size())/10;  //Maximo de pasos antes de refrescar la matriz
  const int LIMITE_VISITAS = (mapaResultado.size()+mapaResultado[0].size())/4;  //Limite de visitas que puede tener una casilla
  const int SUMA_AL_VISITAR = 2; //Suma que se le añade a la casilla que se va a visitar
  const int SUMA_AL_VER = 1; //Suma que se le añade a la casilla que se ve en el cono de vision

  // Variables de Estado
  Action last_action;
  bool tiene_zapatillas;
  int giro45izq;
  vector<vector<int>> frecuencia_visita;
  int pasos;

  //Variables de estado para el nivel E
  list<Action> plan;
  bool hayPlan;
};

#endif
