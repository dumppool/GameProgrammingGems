#ifndef TICK_TIMER_HPP
#define TICK_TIMER_HPP

#include <SDL/SDL.h>

// Classe que marca intervalos de tempo em milissegundos.
// Pode ser usada pra testar se j� est� na hora de efetuar uma altera��o no
// estado de um objeto, por exemplo.
// N�o serve como 'alarm'.
class TickTimer{
  Uint32 starttick, prevtick;
public:
  // Construtor.
  // Par�metro: init = true -> timer marca o momento da constru��o
  //            init = false (default) -> timer inicia indefinido
  TickTimer(bool init = false){
    if(init) start();
    else undef();
  }

  // Inicia o timer (salva o tick atual)
  void start() { starttick = prevtick = SDL_GetTicks(); }

  /* Salva o tick atual, mas n�o altera o starttick
  (a menos que o timer esteja indefinido) */
  void mark() { if(!is_started()) start(); else prevtick = SDL_GetTicks(); }

  // P�e o timer no estado "indefinido"
  void undef() { starttick = prevtick = 0; }

  // Est� no estado "iniciado"? (ou est� no "indefinido"?)
  bool is_started() const { return starttick != 0; }

  // Fun��es que operam sobre o tick pr�vio:
  /* Compara o tick de agora com o salvo e diz se j� se passaram os 'milisec'
  milissegundos. Se j� passou, salva o tick de agora como refer�ncia pra
  futuras chamadas a esta fun��o. */
  bool ja_passou(Uint32 milisec){
    Uint32 now = SDL_GetTicks();
    bool passou = now - prevtick >= milisec;
    if(passou) prevtick = now;
    return passou;
  }

  /* Retorna quantos milissegundos passaram desde o start() ou desde a chamada
  anterior a esta fun��o. */
  Uint32 quanto_passou(){
    Uint32 now = SDL_GetTicks(), quanto = now - prevtick;
    prevtick = now;
    return quanto;
  }

  // Fun��es que operam sempre sobre o tick inicial, em vez de sobre o tick pr�vio:
  Uint32 quanto_passou_desde_start() const {
    return SDL_GetTicks() - starttick;
  }

  bool ja_passou_desde_start(Uint32 milisec) const {
    return SDL_GetTicks() - starttick >= milisec;
  }
};

#endif
