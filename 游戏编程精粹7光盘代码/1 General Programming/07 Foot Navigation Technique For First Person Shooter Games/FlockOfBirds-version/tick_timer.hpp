#ifndef TICK_TIMER_HPP
#define TICK_TIMER_HPP

#include <SDL/SDL.h>

// Classe que marca intervalos de tempo em milissegundos.
// Pode ser usada pra testar se já está na hora de efetuar uma alteração no
// estado de um objeto, por exemplo.
// Não serve como 'alarm'.
class TickTimer{
  Uint32 starttick, prevtick;
public:
  // Construtor.
  // Parâmetro: init = true -> timer marca o momento da construção
  //            init = false (default) -> timer inicia indefinido
  TickTimer(bool init = false){
    if(init) start();
    else undef();
  }

  // Inicia o timer (salva o tick atual)
  void start() { starttick = prevtick = SDL_GetTicks(); }

  /* Salva o tick atual, mas não altera o starttick
  (a menos que o timer esteja indefinido) */
  void mark() { if(!is_started()) start(); else prevtick = SDL_GetTicks(); }

  // Põe o timer no estado "indefinido"
  void undef() { starttick = prevtick = 0; }

  // Está no estado "iniciado"? (ou está no "indefinido"?)
  bool is_started() const { return starttick != 0; }

  // Funções que operam sobre o tick prévio:
  /* Compara o tick de agora com o salvo e diz se já se passaram os 'milisec'
  milissegundos. Se já passou, salva o tick de agora como referência pra
  futuras chamadas a esta função. */
  bool ja_passou(Uint32 milisec){
    Uint32 now = SDL_GetTicks();
    bool passou = now - prevtick >= milisec;
    if(passou) prevtick = now;
    return passou;
  }

  /* Retorna quantos milissegundos passaram desde o start() ou desde a chamada
  anterior a esta função. */
  Uint32 quanto_passou(){
    Uint32 now = SDL_GetTicks(), quanto = now - prevtick;
    prevtick = now;
    return quanto;
  }

  // Funções que operam sempre sobre o tick inicial, em vez de sobre o tick prévio:
  Uint32 quanto_passou_desde_start() const {
    return SDL_GetTicks() - starttick;
  }

  bool ja_passou_desde_start(Uint32 milisec) const {
    return SDL_GetTicks() - starttick >= milisec;
  }
};

#endif
