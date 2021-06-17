/***********************************************************************/
/* Foot navigation technique for first person shooting games           */
/* with Flock of birds (magnetic device for motion capture) version    */
/* This archive was assembled by:                                      */
/*  Marcus A. C. Farias, Daniela G. Trevisan and Luciana P.Nedel       */
/*  Colaborators: Fabio Dapper and Alexandre L. Azevedo                */
/*  Universidade Federal do Rio Grande do Sul - RS - Brasil            */
/*  Instituto de Informática                                           */
/*  http://www.inf.ufrgs.br/                                           */
/*  contact: nedel@inf.ufrgs.br                                        */ 
/*  2007-07-07                                                         */
/***********************************************************************/
#include "tick_timer.hpp"
#include <SDL/SDL.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

//#define USAR_BIRDS
#ifdef USAR_BIRDS
#include <bird.h>
#endif

//#define USAR_CAPACETE
#ifdef USAR_CAPACETE
#include "vfxsdk.h"
#endif

//#define USAR_LUVA
#ifdef USAR_LUVA
#include <fglove.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip> // setw
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
using namespace std;

namespace{

  class Mapa{
    // indexing the texture vetor:
    static const int TEX_OBSTACULO = 0, TEX_CHAO = 1, TEX_PAREDE = 2;

    // keeping the target state. Itens of pair are the line and the column 
    set< pair<int, int> > alvos_atingidos;
    set< pair<int, int> > alvos_testes_atingidos;
    char *chars;
    unsigned int display_list, textura[3];
    void cria_labirinto();
    void cria_texturas();
  public:
    static const char
      // empty space with ground
      C_CHAO = ' ',

      // Idicates the begin (intern use of the program)
      C_INICIO = '\x80',

      /* Indicates the begin and the direction that the player starts looking
      (use in the map file). */
      C_INICIO_CIMA = '^',
      C_INICIO_BAIXO = 'V',
      C_INICIO_ESQUERDA = '<',
      C_INICIO_DIREITA = '>',

     /* When the player crosses thil cell he is ready for begin the evaluation */
      C_INICIO_PLAYING = 'i',

      C_FIM = 'f',

      C_PAREDE = '#',
      C_OBSTACULO = '@',

      C_ALVO = 'x',
      C_ALVO_TESTE = 'y';
    int linhas, colunas;
    size_t total_alvos;
    double inic_x, inic_z, inic_rotacao;
    void load_from_file(const string& file_name);
    char* operator[](int idx) { return &chars[idx * colunas]; }
    void desenha();
    void marcar_alvo(int linha, int coluna);
    size_t num_alvos_atingidos() const { return alvos_atingidos.size(); }
    void clear_stats() { alvos_atingidos.clear(); alvos_testes_atingidos.clear(); }
    Mapa():
      chars(NULL), linhas(0), colunas(0), total_alvos(0),
      inic_x(0.0), inic_z(0.0), inic_rotacao(0.0)
    {}
    Mapa::~Mapa() { delete chars; }
  }mapa;

  class LogFile{
    map<pair<string, string>, int> event_counter;
  public:
    TickTimer timer;
    ofstream file;
    LogFile();
    void event(const string& evt_name, const string& evt_obj, bool totalizar = true);
    void write_total();
    void clear_stats() { event_counter.clear(); }
  }log_file;

  class ConfigFile{
  public:
    string config_name, map_file_name, luva_port;
    bool birds, capacete, luva, fullscreen;

    void load(const string& file_name);
    ConfigFile():
      config_name("Configuração sem nome"), map_file_name("rvirt.map"), luva_port(""),
      birds(false), capacete(false), luva(false), fullscreen(false)
    {}
  }config_file;

  class InitScreen{
    unsigned int display_list, init_textura;
  public:
    InitScreen();
    void desenha();
  }*init_screen;

  class EndScreen{
    unsigned int display_list, end_textura;
  public:
    EndScreen();
    void desenha();
  }*end_screen;

  enum AppState{
    INIT_SCREEN, TESTING, PLAYING, END_SCREEN
  }state;

  class MostraColisao{
    TickTimer timer;
  public:
    void enable();
    void desenha();
  }mostra_colisao;

  class InputDevice{
    /* Activate and desactivate the device checking.
    If device_enabled is false, device is never checked.
    If device_enabled if true, it will depend of enable_checks() and disable_checks() functions.
    The derivative classes constructors will receive a parameter to know if device_enabled should be true or
    false. On the other hand, check() function is completely treated here and the derivative classes 
    just need to do override of do_check(). */
  protected:
    typedef void (InputDevice::*FnCheckType)();
    FnCheckType fn_check;
    InputDevice(): fn_check(do_check), device_enabled(true) {}
    virtual void do_check() = 0;
    virtual void dont_check() {}
  public:
    bool device_enabled;
    void enable_checks() { fn_check = &InputDevice::do_check; }
    void enable_checks(bool enab) { if(enab) enable_checks(); else disable_checks(); }
    void disable_checks() { fn_check = &InputDevice::dont_check; }
    bool checks_are_enabled() { return fn_check == &InputDevice::do_check; }
    void check() { if(device_enabled) (this->*fn_check)(); }
    virtual ~InputDevice() {}
  };

#ifdef USAR_BIRDS

  class Birds : public InputDevice{
    BIRDDEVICECONFIG devconfig;
    double pos1_inicial, ang2_inicial, pos_ant;
    int andando;
  protected:
    virtual void do_check();
  public:
    Birds(bool dev_enab);
    virtual ~Birds();
  }*birds;

#else

  class Birds : public InputDevice{
  protected:
    virtual void do_check() {}
  public:
    Birds(bool) {}
    virtual ~Birds() {}
  }*birds;

#endif

#ifdef USAR_CAPACETE

  class Capacete : public InputDevice{
    double rotacao_anterior;
  protected:
    virtual void do_check();
  public:
    Capacete(bool dev_enab);
    virtual ~Capacete();
  }*capacete;

#else

  class Capacete : public InputDevice{
  protected:
    virtual void do_check() {}
  public:
    Capacete(bool) {}
    virtual ~Capacete() {}
  }*capacete;

#endif

#ifdef USAR_LUVA

  class Luva : public InputDevice{
    fdGlove *pGlove;
    float roll, pitch;
    int gesture;
    float luva_zero;
    unsigned short roll_esq, roll_dir;
  protected:
    virtual void do_check();
  public:
    Luva(const char* port);
    void set_roll_esq();
    void set_roll_dir();
    void calibra();
    virtual ~Luva();
  }*luva;

#else

  class Luva : public InputDevice{
  protected:
    virtual void do_check() {}
  public:
    Luva(const char*) {}
    void set_roll_esq() {}
    void set_roll_dir() {}
    void calibra() {}
    virtual ~Luva() {}
  }*luva;

#endif

  void display();
  void change_state(AppState new_state, bool force = false);
  void eventos();
  void key_hold();
  void key_down(const SDL_KeyboardEvent* key);
  void mouse_motion(const SDL_MouseMotionEvent* motion);
  void mouse_button_hold();
  void adjveloc();
  void reshape(int largura, int altura);
  void grab_input(bool b);
  void caminha(double passo = 1.0);
  void caminha_frente(double passo);
  void caminha_tras(double passo);
  void caminha_esquerda();
  void caminha_direita();
  void vira_direita(double rot);
  void vira_esquerda(double rot);
  void olha_pra_cima();
  void olha_pra_baixo();
  bool colidiu();

  /* pointers for functions actuating according to the current state.
  see change_state(). */
  void void_void_null_func() {}
  void void_double_null_func(double) {}
  typedef void (*VoidVoidFunc)();
  typedef void (*VoidDoubleFunc)(double);
  VoidVoidFunc
    ptr_key_hold,
    ptr_mouse_button_hold,
    ptr_caminha_esquerda,
    ptr_caminha_direita;
  VoidDoubleFunc
    ptr_caminha_frente,
    ptr_caminha_tras;

  SDL_Surface *surface;
  double secsperframe, cima_baixo, rotacao, pos_x, pos_z;
  const double PI_OVER_180 = 0.017453292519943296, PASSO = 2.0, PASSO_ROT = 0.25;
  float luz_posicao[] = {0.0f, 1.5f, 0.0f, 1.0f};
  bool b_grab_input = false, a_janela_estah_ativa = true;
}

int main(int argc, char** argv){
  try{
    config_file.load("rvirt.cfg");

    Birds l_birds(config_file.birds);
    birds = &l_birds;

    Capacete l_capacete(config_file.capacete);
    capacete = &l_capacete;

    Luva l_luva(config_file.luva ? config_file.luva_port.c_str() : "");
    luva = &l_luva;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
      throw runtime_error("It is not possible inicialize the SDL.");
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    const int LARGURA = 640, ALTURA = 480;

    surface = SDL_SetVideoMode(
      LARGURA, ALTURA, SDL_GetVideoInfo()->vfmt->BitsPerPixel,
      SDL_OPENGL | SDL_HWSURFACE |
      (config_file.fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE));
    if(!surface)
      throw runtime_error("Video mode error.");

    SDL_WM_SetCaption("RV", 0);

    glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

    reshape(LARGURA, ALTURA);

    // /*
    const float luz_ambiente[] = {0.45f, 0.45f, 0.45f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);

    const float luz_difusa[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    // */

    glMaterialf(GL_FRONT, GL_SHININESS, 70.0f);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // */

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    mapa.load_from_file(config_file.map_file_name);

    // /*
    luz_posicao[0] = mapa.inic_x;
    luz_posicao[2] = mapa.inic_z;
    // */

    InitScreen l_init_screen;
    init_screen = &l_init_screen;

    EndScreen l_end_screen;
    end_screen = &l_end_screen;

    change_state(INIT_SCREEN, true);

    grab_input(true);
    log_file.file << "Configuração: " << config_file.config_name << "\n\n";

    for(;;){
      eventos();
      if(a_janela_estah_ativa){
        l_capacete.check();
        l_birds.check();
        l_luva.check();

        ptr_key_hold();
        ptr_mouse_button_hold();
        display();

        GLenum errogl = glGetError();
        if(errogl != GL_NO_ERROR){
          throw runtime_error(string("OpenGL error: ") + reinterpret_cast<const char*>(gluErrorString(errogl)));
        }
        adjveloc();
      }
    }
  }catch(exception& ex){
    log_file.file << ex.what() << endl;
  }catch(...){
    log_file.file << "Exception unknow." << endl;
  }
  return 1;
}

namespace{

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glRotated(cima_baixo, 1.0, 0.0, 0.0);
  glRotated(rotacao, 0.0, 1.0, 0.0);
  glTranslated(-pos_x, -0.25, -pos_z);

  /* Find a better position for light (maybe do it mobile).
  Could be needed others lights setting too */
  glLightfv(GL_LIGHT0, GL_POSITION, luz_posicao);

  /*
  glPointSize(30.0f);
  glColor3f(1.0f, 1.0f, 0.0f);
  glBegin(GL_POINTS);
    glVertex3fv(luz_posicao);
  glEnd();
  glBegin(GL_LINES);
    glVertex3fv(luz_posicao);
    glVertex3f(luz_posicao[0], 0.0f, luz_posicao[2]);
  glEnd();
  */

  mapa.desenha();

  switch(state){
    case INIT_SCREEN:
      init_screen->desenha();
      break;

    case END_SCREEN:
      end_screen->desenha();
      break;

    default: ;
  }
  mostra_colisao.desenha();
  SDL_GL_SwapBuffers();
}

void change_state(AppState new_state, bool force){
  /*  force parameter makes the function  executes operations even the previous state 
  is the same.  Used for the initialization. */
  if(!force && new_state == state) return;
  switch(new_state){
    case INIT_SCREEN:
      pos_x = mapa.inic_x;
      pos_z = mapa.inic_z;
      rotacao = mapa.inic_rotacao;
      cima_baixo = 0.0;

      mapa.clear_stats();
      log_file.clear_stats();

      birds->disable_checks();
      capacete->disable_checks();
      luva->disable_checks();

      ptr_key_hold = void_void_null_func;
      ptr_mouse_button_hold = void_void_null_func;
      ptr_caminha_frente = void_double_null_func;
      ptr_caminha_tras = void_double_null_func;
      ptr_caminha_esquerda = void_void_null_func;
      ptr_caminha_direita = void_void_null_func;
      break;

    case PLAYING:
      log_file.timer.start();
      log_file.event("início", "início", false);
      if(state == TESTING)
        break;
      // else fall-thru
    case TESTING:
      birds->enable_checks();
      capacete->enable_checks();
      luva->enable_checks();

      ptr_key_hold = key_hold;
      ptr_mouse_button_hold = mouse_button_hold;
      ptr_caminha_frente = caminha_frente;
      ptr_caminha_tras = caminha_tras;
      ptr_caminha_esquerda = caminha_esquerda;
      ptr_caminha_direita = caminha_direita;
      break;

    case END_SCREEN:
      birds->disable_checks();
      capacete->disable_checks();
      luva->disable_checks();

      ptr_key_hold = void_void_null_func;
      ptr_mouse_button_hold = void_void_null_func;
      ptr_caminha_frente = void_double_null_func;
      ptr_caminha_tras = void_double_null_func;
      ptr_caminha_esquerda = void_void_null_func;
      ptr_caminha_direita = void_void_null_func;

      log_file.event("fim", "fim", false);
      log_file.write_total();
      break;
  }
  state = new_state;
}

void eventos(){
  /* If the window is not activate, use SDL_WaitEvent.
  After use SDL_PollEvent to get all pendant events.*/

  SDL_Event event;
  if(a_janela_estah_ativa){
    if(!SDL_PollEvent(&event)) return;
  }else{
    if(!SDL_WaitEvent(&event)) return;
  }

  do{
    switch(event.type){
      case SDL_KEYDOWN:
        key_down(&event.key);
        break;

      case SDL_MOUSEMOTION:
        mouse_motion(&event.motion);
        break;

      /*
      case SDL_MOUSEBUTTONDOWN:
        mouse_button(&event.button);
        break;
      */

      case SDL_ACTIVEEVENT:
        a_janela_estah_ativa = event.active.gain;
        break;

      case SDL_VIDEORESIZE:
        reshape(event.resize.w, event.resize.h);
        display();
        break;

      case SDL_QUIT:
        exit(0);
    }
  }while(SDL_PollEvent(&event));
}

// ===  CONTROLES { ===

void key_hold(){
  static Uint8 * teclas = SDL_GetKeyState(NULL);
  if(teclas[SDLK_w]) ptr_caminha_frente(1.0);
  if(teclas[SDLK_s]) ptr_caminha_tras(1.0);
  if(teclas[SDLK_a]) ptr_caminha_esquerda();
  if(teclas[SDLK_d]) ptr_caminha_direita();
  if(teclas[SDLK_UP]) olha_pra_cima();
  if(teclas[SDLK_DOWN]) olha_pra_baixo();
  if(teclas[SDLK_LEFT]) vira_esquerda(secsperframe * 100.0);
  if(teclas[SDLK_RIGHT]) vira_direita(secsperframe * 100.0);
}

void key_down(const SDL_KeyboardEvent* key){
  switch(key->keysym.sym){

    case SDLK_PAUSE:
      /* Pause: "Handle or returns" the mouse arrow.
      Now it is not a true pause but it can be implemented generating one more state of AppState. */
      grab_input(!b_grab_input);
      break;

    case SDLK_ESCAPE:
      /* Esc: for exit program */
      exit(0);

    case SDLK_SPACE:
      /* Espace: Starts or re-starts. */
      switch(state){
        case INIT_SCREEN:
          change_state(TESTING);
          break;

        case END_SCREEN:
          change_state(INIT_SCREEN);
          break;
      }
      break;

    case SDLK_e:
      if(state == TESTING)
        luva->set_roll_esq();
      break;

    case SDLK_d:
      if(state == TESTING)
        luva->set_roll_dir();
      break;

    case SDLK_l:
      if(state == TESTING)
        luva->calibra();
      break;

    default:;
  }
}

void mouse_motion(const SDL_MouseMotionEvent* motion){
  if(!b_grab_input || (state != TESTING && state != PLAYING))
    return;

  if(motion->xrel < 0)
    vira_esquerda(-motion->xrel * PASSO_ROT);
  else if(motion->xrel > 0)
    vira_direita(motion->xrel * PASSO_ROT);

  if(motion->yrel > 0)
    olha_pra_baixo();
  else if (motion->yrel < 0)
    olha_pra_cima();
}

void mouse_button_hold(){
  Uint8 mouse_btn = SDL_GetMouseState(NULL, NULL);
  if(mouse_btn & SDL_BUTTON_LMASK) ptr_caminha_frente(1.0);
  if(mouse_btn & SDL_BUTTON_RMASK) ptr_caminha_tras(1.0);
}

// === } CONTROLES ===

void grab_input(bool b){
  b_grab_input = b;
  if(b){
    SDL_ShowCursor(0);
    SDL_WM_GrabInput(SDL_GRAB_ON);
  }else{
    SDL_ShowCursor(1);
    SDL_WM_GrabInput(SDL_GRAB_OFF);
  }
}

void reshape(int largura, int altura){
  if(altura == 0) altura = 1;
  glViewport(0, 0, largura, altura);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, static_cast<double>(largura) / altura, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void adjveloc(){
  static TickTimer esp(true);
  secsperframe = esp.quanto_passou() / 1000.0;
}

#define UPD_POS(opx, opz, ang, a_passo)                                               \
  double old_pos_x = pos_x, old_pos_z = pos_z;                                        \
  pos_x opx##= sin((rotacao + (ang)) * PI_OVER_180) * a_passo * PASSO * secsperframe; \
  if(colidiu())                                                                       \
    pos_x = old_pos_x;                                                                \
  pos_z opz##= cos((rotacao + (ang)) * PI_OVER_180) * a_passo * PASSO * secsperframe; \
  if(colidiu())                                                                       \
    pos_z = old_pos_z;

void caminha(double passo){
  if(passo == 0.0)
    return;
  if(passo < 0.0)
    caminha_frente(-passo);
  else
    caminha_tras(passo);
}

void caminha_frente(double passo){
  UPD_POS(+, -, 0.0, passo);
}

void caminha_tras(double passo){
  UPD_POS(-, +, 0.0, passo);
}

void caminha_esquerda(){
  UPD_POS(-, +, 90.0, 1.0);
}

void caminha_direita(){
  UPD_POS(-, +, -90.0, 1.0);
}

#undef UPD_POS

void vira_direita(double rot){
  rotacao += rot;
  if(rotacao >= 360.0) rotacao -= 360.0;
}

void vira_esquerda(double rot){
  rotacao -= rot;
  if(rotacao <= 0.0) rotacao += 360.0;
}

void olha_pra_baixo(){
  cima_baixo += 0.5;
  if(cima_baixo >= 80.0)
    cima_baixo = 80.0;
}

void olha_pra_cima(){
  cima_baixo -= 0.5;
  if(cima_baixo <= -80.0)
    cima_baixo = -80.0;
}

bool colidiu(){
  // It returns true if it has collided with something (it may to cancel the movement) 

  // Reminder if pos_x and pos_z are integers,
  // the player is in the center of the cell
  const int
    x = static_cast<int>(pos_x + 0.5),
    z = static_cast<int>(pos_z + 0.5);
  char obj; // object that the player collides 

#define TESTACOLISAO(x, z)                                      \
  if((x) >= mapa.colunas || (z) >= mapa.linhas ||               \
    (x) < 0 || (z) < 0 || (obj = mapa[z][x]) == Mapa::C_PAREDE) \
  {                                                             \
    mostra_colisao.enable();                                    \
    if(state == PLAYING)                                        \
      log_file.event("colisão", "parede");                      \
    return true;                                                \
  }else if(obj == Mapa::C_OBSTACULO){                           \
    mostra_colisao.enable();                                    \
    if(state == PLAYING)                                        \
      log_file.event("colisão", "obstáculo");                   \
    return true;                                                \
  }else if(obj == Mapa::C_ALVO || obj == Mapa::C_ALVO_TESTE){   \
    mapa.marcar_alvo(z, x);                                     \
    /* not return, test others posibilities of collision*/       \
  }else if(obj == Mapa::C_FIM){                                 \
    if(mapa.num_alvos_atingidos() == mapa.total_alvos)          \
      change_state(END_SCREEN);                                 \
  }else if(obj == Mapa::C_INICIO_PLAYING){                      \
    change_state(PLAYING);                                      \
  }else                                                         \
    (void)0; /* avoid that a potential else outside of the macro be linked to this if */

  const double epsilon = 0.375; //  it should be between 0 e 0.5

 // Colision test in the square that m is.
  TESTACOLISAO(x, z);

  // It tests the proximity with neighboring cells,
  // in four directions and the diagonal lines.
  if(pos_z - z > epsilon){
    TESTACOLISAO(x, z + 1);
    if(pos_x - x > epsilon)
      TESTACOLISAO(x + 1, z + 1);
    if(x - pos_x > epsilon)
      TESTACOLISAO(x - 1, z + 1);
  }
  if(z - pos_z > epsilon){
    TESTACOLISAO(x, z - 1);
    if(pos_x - x > epsilon)
      TESTACOLISAO(x + 1, z - 1);
    if(x - pos_x > epsilon)
      TESTACOLISAO(x - 1, z - 1);
  }
  if(pos_x - x > epsilon){
    TESTACOLISAO(x + 1, z);
    if(pos_z - z > epsilon)
      TESTACOLISAO(x + 1, z + 1);
    if(z - pos_z > epsilon)
      TESTACOLISAO(x + 1, z - 1);
  }
  if(x - pos_x > epsilon){
    TESTACOLISAO(x - 1, z);
    if(pos_z - z > epsilon)
      TESTACOLISAO(x - 1, z + 1);
    if(z - pos_z > epsilon)
      TESTACOLISAO(x - 1, z - 1);
  }
  return false;
#undef TESTACOLISAO
}

// === ShowCollision { ===

void MostraColisao::enable(){
  timer.start();
}

void MostraColisao::desenha(){
  if(timer.ja_passou_desde_start(100)){
    timer.undef();
    return;
  }
  glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0.0, 100.0, 0.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
    glPushAttrib(GL_ENABLE_BIT);
      glDisable(GL_DEPTH_TEST);
      glEnable(GL_BLEND);
      glPushMatrix();
        glLoadIdentity();
        glColor4ub(255, 0, 0, 70);
        glRasterPos2i(0, 0);
        glRecti(0, 0, 100, 100);
      glPopMatrix();
    glPopAttrib();
  glMatrixMode(GL_PROJECTION);
    glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

// === } ShowCollision ===

// === LogFile { ===

LogFile::LogFile():
  file("labrv_log.txt")
{
  file.setf(ios::fixed, ios::floatfield);
  file.precision(5);
}

void LogFile::event(const string& evt_name, const string& evt_obj, bool totalizar){
  file <<
    setw(14) << timer.quanto_passou_desde_start() / 1000.0 << 's' <<
    setw(16) << evt_name <<
    setw(16) << evt_obj <<
    setw( 8) << '(' <<
    setw(14) << pos_x << ';' <<
    setw(14) << pos_z << ")\n";
  if(totalizar)
    ++event_counter[make_pair(evt_name, evt_obj)];
}

void LogFile::write_total(){
  file << "\nTotal dos eventos:\n";
  for(map<pair<string, string>, int>::iterator it(event_counter.begin()), end(event_counter.end());
    it != end; ++it)
  {
    file << it->first.first << ' ' << it->first.second << ": " << it->second << '\n';
  }
}

// === } LogFile ===

// === Map { ===

void Mapa::load_from_file(const string& file_name){
  ifstream file(file_name.c_str());
  file >> linhas >> colunas;
  if(!file)
    throw runtime_error("Error loading file map \"" + file_name + "\".");
  chars = new char[linhas * colunas];
  for(int i = 0; i < linhas; ++i){
    for(int j = 0; j < colunas; ++j){
      int c = file.get();
      if(!file)
        throw runtime_error("Error reading file map \"" + file_name + "\".");
      if(c == '\n'){
        if(j == 0){
          // '\n' waited, to ignore
          --j;
          continue;
        }else{
              // '\n' not waited. Fill the line with spaces.
          for(int k = j; k < colunas; ++k)
            chars[i * colunas + k] = C_CHAO;
          break;
        }
      }
      switch(c){
        case C_INICIO_CIMA:
          c = C_INICIO;
          // fall thru;
        case C_INICIO:
         // inic_rotacao = 0.0; // It already is inicialized with 0.0
          inic_x = j;
          inic_z = i;
          break;

        case C_INICIO_DIREITA:
          c = C_INICIO;
          inic_rotacao = 90.0;
          inic_x = j;
          inic_z = i;
          break;

        case C_INICIO_BAIXO:
          c = C_INICIO;
          inic_rotacao = 180.0;
          inic_x = j;
          inic_z = i;
          break;

        case C_INICIO_ESQUERDA:
          c = C_INICIO;
          inic_rotacao = 270.0;
          inic_x = j;
          inic_z = i;
          break;
      }
      chars[i * colunas + j] = c;
    }
  }
  cria_texturas();
  cria_labirinto();
}

void Mapa::cria_labirinto(){
  display_list = glGenLists(1);
  glNewList(display_list, GL_COMPILE);
  glPushMatrix();
  glTranslatef(-0.5f, 0.0f, -0.5f);
  for(int z = 0; z < linhas; ++z){
    for(int x = 0; x < colunas; ++x){
      switch((*this)[z][x]){
        case C_INICIO:
          glBindTexture(GL_TEXTURE_2D, 0); // Disactivate textures
          glBegin(GL_QUADS);
            glColor3ub(0, 0, 255);
            glNormal3f(0.0f, 1.0f, 0.0f);

            glVertex3d(x, 0.0, z);
            glVertex3d(x, 0.0, z+1);
            glVertex3d(x+1, 0.0, z+1);
            glVertex3d(x+1, 0.0, z);
          glEnd();
          break;

        case C_INICIO_PLAYING:
          glBindTexture(GL_TEXTURE_2D, 0);
          glBegin(GL_QUADS);
            glColor3ub(102, 224, 255);
            glNormal3f(0.0f, 1.0f, 0.0f);

            glVertex3d(x, 0.0, z);
            glVertex3d(x, 0.0, z+1);
            glVertex3d(x+1, 0.0, z+1);
            glVertex3d(x+1, 0.0, z);
          glEnd();
          break;

        case C_FIM:
          glBindTexture(GL_TEXTURE_2D, 0);// Disactivate textures
          glBegin(GL_QUADS);
            glColor3f(0.977f, 1.0f, 0.111f);
            glNormal3f(0.0f, 1.0f, 0.0f);

            glVertex3d(x, 0.0, z);
            glVertex3d(x, 0.0, z+1);
            glVertex3d(x+1, 0.0, z+1);
            glVertex3d(x+1, 0.0, z);
          glEnd();
          break;

        case C_PAREDE:
          glBindTexture(GL_TEXTURE_2D, textura[TEX_PAREDE]);
          glBegin(GL_QUADS);
            glColor3ub(255, 255, 255);

            /* "if"s before each face are to draw hollow walls (they prevent the drawing of walls that are leaned in other walls). */

            // side 1

            if(z-1 < 0 || (*this)[z-1][x] != C_PAREDE){
              glNormal3f(0.0f, 0.0f, -1.0f);
              glTexCoord2f(0.0f, 0.0f); glVertex3d(x+1, 0.5, z);
              glTexCoord2f(0.0f, 1.0f); glVertex3d(x+1, 0.0, z);
              glTexCoord2f(2.0f, 1.0f); glVertex3d(x, 0.0, z);
              glTexCoord2f(2.0f, 0.0f); glVertex3d(x, 0.5, z);
            }

            // Side 2

            if(x+1 >= colunas || (*this)[z][x+1] != C_PAREDE){
              glNormal3f(1.0f, 0.0f, 0.0f);
              glTexCoord2f(0.0f, 1.0f); glVertex3d(x+1, 0.0, z+1);
              glTexCoord2f(2.0f, 1.0f); glVertex3d(x+1, 0.0, z);
              glTexCoord2f(2.0f, 0.0f); glVertex3d(x+1, 0.5, z);
              glTexCoord2f(0.0f, 0.0f); glVertex3d(x+1, 0.5, z+1);
            }

            // Side 3

            if(z+1 >= linhas || (*this)[z+1][x] != C_PAREDE){
              glNormal3f(0.0f, 0.0f, 1.0f);
              glTexCoord2f(0.0f, 1.0f); glVertex3d(x, 0.0, z+1);
              glTexCoord2f(2.0f, 1.0f); glVertex3d(x+1, 0.0, z+1);
              glTexCoord2f(2.0f, 0.0f); glVertex3d(x+1, 0.5, z+1);
              glTexCoord2f(0.0f, 0.0f); glVertex3d(x, 0.5, z+1);
            }

            // Side 4

            if(x-1 < 0 || (*this)[z][x-1] != C_PAREDE){
              glNormal3f(-1.0f, 0.0f, 0.0f);
              glTexCoord2f(0.0f, 1.0f); glVertex3d(x, 0.0, z);
              glTexCoord2f(2.0f, 1.0f); glVertex3d(x, 0.0, z+1);
              glTexCoord2f(2.0f, 0.0f); glVertex3d(x, 0.5, z+1);
              glTexCoord2f(0.0f, 0.0f); glVertex3d(x, 0.5, z);
            }
          glEnd();
          break;

        case C_OBSTACULO:
          glBindTexture(GL_TEXTURE_2D, textura[TEX_OBSTACULO]);
          glBegin(GL_QUADS);
            glColor3ub(255, 255, 255);
  
            // Side 1

            glNormal3f(0.0f, 0.0f, -1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3d(x+0.9, 0.45, z+0.1);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(x+0.9, 0.0, z+0.1);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(x+0.1, 0.0, z+0.1);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(x+0.1, 0.45, z+0.1);

            // Side 2

            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(x+0.9, 0.0, z+0.9);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(x+0.9, 0.0, z+0.1);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(x+0.9, 0.45, z+0.1);
            glTexCoord2f(0.0f, 0.0f); glVertex3d(x+0.9, 0.45, z+0.9);

            // Side 3

            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(x+0.1, 0.0, z+0.9);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(x+0.9, 0.0, z+0.9);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(x+0.9, 0.45, z+0.9);
            glTexCoord2f(0.0f, 0.0f); glVertex3d(x+0.1, 0.45, z+0.9);

            // Side 4

            glNormal3f(-1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(x+0.1, 0.0, z+0.1);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(x+0.1, 0.0, z+0.9);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(x+0.1, 0.45, z+0.9);
            glTexCoord2f(0.0f, 0.0f); glVertex3d(x+0.1, 0.45, z+0.1);

          glEnd();
         // fall-thru: The obstacle also has a ground.

        case C_CHAO:
          glBindTexture(GL_TEXTURE_2D, textura[TEX_CHAO]);
          glBegin(GL_QUADS);
            glColor3ub(255, 255, 255);
            glNormal3f(0.0f, 1.0f, 0.0f);

            glTexCoord2f(0.0f, 0.0f); glVertex3d(x, 0.0, z);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(x, 0.0, z+1);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(x+1, 0.0, z+1);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(x+1, 0.0, z);
          glEnd();
          break;

        case C_ALVO:
          ++total_alvos;
          break;
      } // switch
    } // for
  } // for
  glPopMatrix();
  glEndList();
}

void Mapa::cria_texturas(){
#define caixa_file_name "imagens/crate.bmp"
#define chao_file_name "imagens/chao.bmp"
#define parede_file_name "imagens/parede.bmp"
  SDL_Surface * imagem_caixa = SDL_LoadBMP(caixa_file_name);
  if(imagem_caixa == NULL)
    throw runtime_error("Can't load texture\"" caixa_file_name "\".");

  SDL_Surface * imagem_chao = SDL_LoadBMP(chao_file_name);
  if(imagem_chao == NULL){
    SDL_FreeSurface(imagem_caixa);
    throw runtime_error("Can't load texture \"" chao_file_name "\".");
  }

  SDL_Surface * imagem_parede = SDL_LoadBMP(parede_file_name);
  if(imagem_parede == NULL){
    SDL_FreeSurface(imagem_caixa);
    SDL_FreeSurface(imagem_chao);
    throw runtime_error("Can't load texture \"" parede_file_name "\".");
  }

#undef caixa_file_name
#undef chao_file_name
#undef parede_file_name

  glGenTextures(3, textura);

  glBindTexture(GL_TEXTURE_2D, textura[TEX_OBSTACULO]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagem_caixa->w, imagem_caixa->h,
    0, GL_BGR, GL_UNSIGNED_BYTE, imagem_caixa->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, textura[TEX_CHAO]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagem_chao->w, imagem_chao->h,
    0, GL_BGR, GL_UNSIGNED_BYTE, imagem_chao->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glBindTexture(GL_TEXTURE_2D, textura[TEX_PAREDE]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagem_parede->w, imagem_parede->h,
    0, GL_BGR, GL_UNSIGNED_BYTE, imagem_parede->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  /* Free imagens! It is not exception-safe*/
  SDL_FreeSurface(imagem_caixa);
  SDL_FreeSurface(imagem_chao);
  SDL_FreeSurface(imagem_parede);
}

void Mapa::desenha(){
  // To draw what it is fixed in the map 
  glCallList(display_list);

  //To draw what changes (targets)
  glPushMatrix();
  glTranslatef(-0.5f, 0.0f, -0.5f);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBegin(GL_QUADS);
  for(int z = 0; z < linhas; ++z){
    for(int x = 0; x < colunas; ++x){

      if((*this)[z][x] == C_ALVO){
        if(alvos_atingidos.find(make_pair(z, x)) != alvos_atingidos.end())
          glColor3ub(0, 255, 0);
        else
          glColor3ub(255, 0, 0);
      }else if((*this)[z][x] == C_ALVO_TESTE){
        if(alvos_testes_atingidos.find(make_pair(z, x)) != alvos_testes_atingidos.end())
          glColor3f(0, 255, 0);
        else
          glColor3f(255, 0, 0);
      }else
        continue;

      glNormal3f(0.0f, 1.0f, 0.0f);

      glVertex3d(x, 0.0, z);
      glVertex3d(x, 0.0, z+1);
      glVertex3d(x+1, 0.0, z+1);
      glVertex3d(x+1, 0.0, z);
    } // for
  } // for
  glEnd();
  glPopMatrix();
}

void Mapa::marcar_alvo(int linha, int coluna){
  switch((*this)[linha][coluna]){
    case C_ALVO_TESTE:
      alvos_testes_atingidos.insert(make_pair(linha, coluna));
      break;

    case C_ALVO: {
      pair<set< pair<int, int> >::iterator, bool> i = alvos_atingidos.insert(make_pair(linha, coluna));
      if(i.second) // It is a target (true) not yet found?
        log_file.event("alvo", "alvo");
      break;
    }

    default:
      /* error program */;
  }
}

// === } Map ===

// === ConfigFile { ===

void ConfigFile::load(const string& file_name){
  ifstream file(file_name.c_str());
  if(!file)
    throw runtime_error("Can't open configuration file \"" + file_name + "\".");
 // Firt line: configuration name (it will appear in the log file)
  getline(file, config_name);
  string line;
  while(getline(file, line)){
    if(line.size() < 1) continue;
    switch(line[0]){
      case 'm': case 'M':
        /*
        The remaining line is the name of the map file. Ex.:
        M rvirt.map
        */
        map_file_name = line.substr(2);
        break;

      case 'l': case 'L':
        /* Ativates the data glove and indicates which port to use. Ex.:
        L COM1
        */
        luva = true;
        luva_port = line.substr(2);
        break;

      case 'c': case 'C':
        capacete = true;
        break;

      case 'b': case 'B':
        birds = true;
        break;

      case 'f': case 'F':
        fullscreen = true;
        break;

      case '#':
        // Comments
      default:
        break;
    }
  }
}

// === } ConfigFile ===

// === InitScreen { ===

InitScreen::InitScreen():
  display_list(0), init_textura(0)
{
#define file_name "imagens/init.bmp"
  SDL_Surface * imagem = SDL_LoadBMP(file_name);
  if(imagem != NULL)
  {
    glGenTextures(1, &init_textura);

    glBindTexture(GL_TEXTURE_2D, init_textura);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagem->w, imagem->h,
      0, GL_BGR, GL_UNSIGNED_BYTE, imagem->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Free the imagem! It is not exception-safe */
    SDL_FreeSurface(imagem);
  }else
    throw runtime_error("Can't load the initial screen \"" file_name "\".");
#undef file_name
  display_list = glGenLists(1);
  glNewList(display_list, GL_COMPILE);
    glMatrixMode(GL_PROJECTION);
      glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, 100.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
      glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_DEPTH_TEST);
        glPushMatrix();
          glLoadIdentity();
          glBindTexture(GL_TEXTURE_2D, init_textura);
          glBegin(GL_QUADS);
            glColor3ub(255, 255, 255);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(10.0f, 10.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(90.0f, 10.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(90.0f, 16.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(10.0f, 16.0f);
          glEnd();
        glPopMatrix();
      glPopAttrib();
    glMatrixMode(GL_PROJECTION);
      glPopMatrix(); // it restores the projection 
    glMatrixMode(GL_MODELVIEW);
  glEndList();
}

void InitScreen::desenha(){
  glCallList(display_list);
}

// === } InitScreen ===

// === EndScreen { ===

EndScreen::EndScreen():
  display_list(0), end_textura(0)
{
#define file_name "imagens/end.bmp"
  SDL_Surface * imagem = SDL_LoadBMP(file_name);
  if(imagem != NULL)
  {
    glGenTextures(1, &end_textura);

    glBindTexture(GL_TEXTURE_2D, end_textura);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagem->w, imagem->h,
      0, GL_BGR, GL_UNSIGNED_BYTE, imagem->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Free the imagem! It is not exception-safe */
    SDL_FreeSurface(imagem);
  }else
    throw runtime_error("Can't load the final screen   \"" file_name "\".");
#undef file_name
  display_list = glGenLists(1);
  glNewList(display_list, GL_COMPILE);
    glMatrixMode(GL_PROJECTION);
      glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0., 100., 0., 100.);
    glMatrixMode(GL_MODELVIEW);
      glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_DEPTH_TEST);
        glPushMatrix();
          glLoadIdentity();
          glBindTexture(GL_TEXTURE_2D, end_textura);
          glBegin(GL_QUADS);
            glColor3ub(255, 255, 255);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(10.0f, 10.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(90.0f, 10.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(90.0f, 16.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(10.0f, 16.0f);
          glEnd();
        glPopMatrix();
      glPopAttrib();
    glMatrixMode(GL_PROJECTION);
      glPopMatrix(); // it restores the projection
    glMatrixMode(GL_MODELVIEW);
  glEndList();
}

void EndScreen::desenha(){
  glCallList(display_list);
}

// === } EndScreen ===

// === Birds { ===

#ifdef USAR_BIRDS

Birds::Birds(bool dev_enab):
  pos_ant(0.0), andando(0)
{
  device_enabled = dev_enab;
  if(!dev_enab)
    return;
  int devcnt;
  int i;
  WORD comport;

  // wake up a flock of birds using the RS232 interface

  // tell driver to look on comport 1 for master bird

  comport = 1;
  if(!birdRS232WakeUp(1, TRUE, 1, &comport, 9600, 2000, 2000)){
    /*printf("Unable to connect with Bird at Com%d",comport);
    getchar();
    exit(1);*/
    throw runtime_error("Unable to connect with Bird");
  }
  birdGetDeviceConfig(1, 1, &devconfig);
  devconfig.byDataFormat = BDF_POSITIONANGLES;
  //devconfig.byDataFormat = BDF_POSITIONMATRIX; // changes the data format
  birdSetDeviceConfig(1, 1, &devconfig);

  birdStartFrameStream(1);
  if(birdFrameReady(1)){
    BIRDFRAME frame;

    birdGetMostRecentFrame(1,&frame);

    /*double pos[3];
    double ang[3];*/
    double pos_scale;
    //double mat[3][3];

    BIRDREADING *preading;
    // reading array is indexed by device address (1 relative)
    preading = &frame.reading[0]; 
    pos_scale = devconfig.wScaling;

    // convert position and angle data
    //pos[0] = preading->position.nX * pos_scale / 32767.;
    pos1_inicial = preading->position.nY * pos_scale / 32767.;
    /*pos[2] = preading->position.nZ * pos_scale / 32767.;
    ang[0] = preading->angles.nAzimuth * 180. / 32767.;
    ang[1] = preading->angles.nElevation * 180. / 32767.;*/
    ang2_inicial = preading->angles.nRoll * 180. / 32767.;
  }
}

void Birds::do_check(){
  if (birdFrameReady(1)){
    BIRDFRAME frame;

    birdGetMostRecentFrame(1,&frame);

    double pos[3];
    double ang[3];
    double pos_scale;
    double mat[3][3];

    BIRDREADING *preading;
    // reading array is indexed by device address (1 relative)
    preading = &frame.reading[0]; 
    pos_scale = devconfig.wScaling;

     // convert position and angle data
    //pos[0] = preading->position.nX * pos_scale / 32767.;
    pos[1] = preading->position.nY * pos_scale / 32767.;
    //pos[2] = preading->position.nZ * pos_scale / 32767.;
    //ang[0] = preading->angles.nAzimuth * 180. / 32767.;
    //ang[1] = preading->angles.nElevation * 180. / 32767.;
    ang[2] = preading->angles.nRoll * 180. / 32767.;


    const double rot_min = 10.0, rot_div = 25.0;// rot_min = 6.0
    //bool tah_girando = false;
    if(ang[2] > ang2_inicial + rot_min){
      vira_direita(ang[2] / rot_div);
      pos[1] -= pos_ant;//0.3;
      //tah_girando = true;
    }else if(ang[2] < ang2_inicial - rot_min){
      vira_esquerda(-ang[2] / rot_div);
      pos[1] += pos_ant;//0.3;
      //tah_girando = true;
    }
    const double reduc_pos = 4.0;
    const double mov_min = 0.9;//0.7
    /*
    if(pos[1] > pos1_inicial + mov_min){
      ++andando;
    }else if(pos[1] < pos1_inicial - mov_min){
      --andando;
    }
    if(andando)
      ptr_caminha_frente(andando);
    */


    // CONTÍNUO

    //cout << pos[1] << ' ' << pos1_inicial << '\n';
    if(pos[1] < pos1_inicial - mov_min){
      //cout << pos[1] << '\n';
      caminha(pos[1] / reduc_pos);
    }else if(pos[1] > pos1_inicial + mov_min){
      caminha(pos[1] / reduc_pos);
    }

    /*
    // by ranges
    const int faixas_lims = 3;
    const double faixas_veloc[faixas_lims] = {0.7, 2.8, 3.5};
    static double veloc;
    if(!tah_girando){
      for(int i = 0; i < faixas_lims - 1; ++i){
        if(pos[1] > pos1_inicial + faixas_veloc[i] &&
          pos[1] < pos1_inicial + faixas_veloc[i + 1])
        {
          veloc = -(i + 1);
        }else if(pos[1] < pos1_inicial - faixas_veloc[i] &&
          pos[1] < pos1_inicial - faixas_veloc[i + 1])
        {
          veloc = i + 1;
        }else
          veloc = 0.0;
      }
    //}
    ptr_caminha(veloc);
    */
    /*
    if(pos[1] > pos1_inicial + mov_min){
      ptr_caminha_tras();
    }else if(pos[1] < pos1_inicial - mov_min){
      ptr_caminha_frente();
    }
    */

    //cout << "ang2_inicial = " << ang2_inicial << '\n';
    //cout << "ang[2] = " << ang[2] << '\n';
    pos_ant=pos[1]/reduc_pos;
  }
}

Birds::~Birds(){
  if(device_enabled){
    birdStopFrameStream(1);
    birdShutDown(1);
  }
}

#endif

// === } Birds ===

// === Head mounted display { ===

#ifdef USAR_CAPACETE

Capacete::Capacete(bool dev_enab):
  rotacao_anterior(0.0)
{
  device_enabled = dev_enab;
  if(!dev_enab)
    return;
  /*
  while(1){
    if(VFXLoadDll()==VFX_OK){
      VFXSetVideoMode(VFX_OVERRIDE);
      break;
    }
  }
  // */

  // /*
  if(VFXLoadDll() != VFX_OK)
    throw runtime_error("Where is the Head mounted display?");
  VFXSetVideoMode(VFX_OVERRIDE);
  // */
}

void Capacete::do_check(){
  //observador.rotaciona(0,-pitch,0);
  long lyaw, lpitch, lroll;
  VFXGetTracking(&lyaw, &lpitch, &lroll);
  //cout << lyaw << ' ' << lpitch << ' ' << lroll << '\n';
  double yaw, pitch;
  yaw = 360 - ((lyaw * 360.0) / 65535) - 180;
  pitch = -(360 - ((lpitch * 360.0) / 65535) - 180);

  //cout << yaw << ' ' << pitch << '\n';
  double rotyaw = yaw - rotacao_anterior;
  //cout << yaw << ' ' << pitch <<
    //" rotyaw: " << rotyaw << " rotacao_anterior: " << rotacao_anterior <<'\n';
  rotacao_anterior = yaw;
  if(rotyaw < 0)
    vira_esquerda(-rotyaw);
  else
    vira_direita(rotyaw);
}

Capacete::~Capacete(){
  if(device_enabled)
    VFXFreeDll();
}

#endif

// === } Head mounted display ===

// === Data glove { ===

#ifdef USAR_LUVA

Luva::Luva(const char* port):
  pGlove(NULL), luva_zero(0.5f)
{
  if(port == NULL || port[0] == '\0'){
    device_enabled = false;
    return;
  }
  pGlove = fdOpen(const_cast<char*>(port));
  if(pGlove == NULL)
    throw runtime_error("Data glove not found.");
  device_enabled = true;
  /*roll = fdGetSensorScaled(pGlove, FD_ROLL);
  roll = fdGetSensorScaled(pGlove, FDROLL);
  roll = fdGetSensorScaled(pGlove, FDROLL);*/
}

void Luva::set_roll_esq(){
  if(device_enabled){
    roll_esq = fdGetSensorRaw(pGlove, FD_ROLL);
    //cout << roll_esq << "    esquerda\n";
  }
}

void Luva::set_roll_dir(){
  if(device_enabled){
    roll_dir = fdGetSensorRaw(pGlove, FD_ROLL);
    //cout << roll_esq << "  direita\n";
  }
}


void Luva::calibra(){
  if(device_enabled){
    fdSetCalibration(pGlove, FD_ROLL, roll_dir, roll_esq);
  }

  /*if(device_enabled)
    luva_zero = fdGetSensorScaled(pGlove, FD_ROLL);*/
}

void Luva::do_check(){
  pitch = fdGetSensorScaled(pGlove, FD_PITCH);
  roll = fdGetSensorScaled(pGlove, FD_ROLL);
  //roll = fdGetSensorRaw(pGlove,FD_ROLL);

  gesture = fdGetGesture(pGlove);

  //cout << luva_zero  << '\t' << roll << '\t' << pitch << '\n';

  const double escala = 2.5;
  const float lim = 0.15f;
  // --- turn with data glove:
  if(roll < luva_zero - lim){
    vira_esquerda((luva_zero - roll ) * escala);
  }else if(roll > luva_zero + lim){
    vira_direita((roll - luva_zero ) * escala);
  }

  if(gesture == 0 || gesture == 8){ // close hand
    //forward and back movements
    if(pitch < 0.9f){ //if data glove has been rotated up
      caminha_tras(1.0);
    }else {//if data glove has been rotated down
      caminha_frente(1.0);
    }
    /*
    cout << roll<< ' ' << pitch << '\n';
    if(roll < 0.7f){ //if data glove has been rotated up
       vira_esquerda(-(roll - 0.8));
    }else if(roll > 0.9f){//if data glove has been rotated down
       vira_direita(roll - 0.8);
    }*/
  }
}

/*
void Luva::do_check(){
  if(!checks_enabled) return;
  pitch = fdGetSensorScaled(pGlove, FD_PITCH);
  roll = fdGetSensorScaled(pGlove, FD_ROLL);
  gesture = fdGetGesture(pGlove);
  if(gesture==0){ // mão fechada
    //deslocamento do observador para frente e para trás
    if(pitch < 0.2){ //se tiver rotacionado a luva para cima
      caminha_tras(1.0);
    }else if(pitch > 0.7){//se tiver rotacionado a luva para baixo
      caminha_frente(1.0);
    }
  }
}*/

Luva::~Luva(){
  if(device_enabled)
    fdClose(pGlove);
}

#endif

// === } data glove ===

} // namespace
