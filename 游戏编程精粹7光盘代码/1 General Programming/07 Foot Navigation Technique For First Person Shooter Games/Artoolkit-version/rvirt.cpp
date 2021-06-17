/***********************************************************************/
/* Foot navigation technique for first person shooting games           */
/* with ARToolkit version  (http://sourceforge.net/projects/artoolkit/)*/
/* This archive was assembled by:                                      */
/*  Marcus A. C. Farias, Daniela G. Trevisan and Luciana P.Nedel       */
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

#include <GL/gl.h>
#include <GL/glu.h>
/* If gl.h will be very old, does not have GL_BGR… But what it matters is the version of the library, and not it version of gl.h! Therefore GL_BGR was defined here.*/
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif


#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/gsub_lite.h>

#include <boost/format.hpp>

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

namespace
{
  class Mapa
  {
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
    string config_name, map_file_name;
    bool fullscreen;

    void load(const string& file_name);
    ConfigFile():
      config_name("Configuração sem nome"), map_file_name("rvirt.map"),
      fullscreen(false)
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

  inline void display();
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

  const unsigned int HALF_JUMP_DURATION = 1000;
  TickTimer jump_timer;
  bool is_jumping = false;
  void jump();
  inline void jump_update();

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
    ptr_caminha_direita,
	ptr_jump;
  VoidDoubleFunc
    ptr_caminha_frente,
    ptr_caminha_tras,
    ptr_vira_esquerda,
    ptr_vira_direita;

  SDL_Surface *surface;
  const double PI_OVER_180 = 0.017453292519943296, PASSO = 2.0, PASSO_ROT = 0.25, START_POS_Y = 0.25;
  double secsperframe, cima_baixo, rotacao, pos_x, pos_y = START_POS_Y, pos_z;
  float luz_posicao[] = {0.0f, 1.5f, 0.0f, 1.0f};
  bool b_grab_input = false, a_janela_estah_ativa = true;
}

namespace artoolkit
{
  double patt_centre[2] = {0.0, 0.0};
  double patt_width = 80.0; 
  double patt_trans[3][4];
  int patt_id;
  int art_threshhold = 100;

  //bool patt_found = true;
  ARParam artcparam;
  ARGL_CONTEXT_SETTINGS_REF argl_settings = NULL;

  void setup_camera(const char *cparam_name, char *vconf, ARParam *cparam)
  {
    ARParam wparam;
    int xsize, ysize;

    // Open the video path.
    if(arVideoOpen(vconf) < 0) {
      throw runtime_error("setupCamera(): Unable to open connection to camera.\n");
    }

    // Find the size of the window.
    if(arVideoInqSize(&xsize, &ysize) < 0) throw runtime_error("Unable to set up AR camera.");
    fprintf(stdout, "Camera image size (x,y) = (%d,%d)\n", xsize, ysize);

    // Load the camera parameters, resize for the window and init.
    if (arParamLoad(cparam_name, 1, &wparam) < 0) {
      throw runtime_error((boost::format("setupCamera(): Error loading parameter file %s for camera.\n") % cparam_name).str());
    }
    arParamChangeSize(&wparam, xsize, ysize, cparam);
    fprintf(stdout, "*** Camera Parameter ***\n");
    arParamDisp(cparam);

    arInitCparam(cparam);

    if (arVideoCapStart() != 0) {
      throw runtime_error("setupCamera(): Unable to begin camera data capture.\n");
    }
  }

  void setup_marker(const char * patt_name, int * patt_id)
  {
    if((*patt_id = arLoadPatt(patt_name)) < 0){
      throw runtime_error("setup_marker: pattern load error!!");
    }
  }

  void quit()
  {
    arglCleanup(argl_settings);
    arVideoCapStop();
    arVideoClose();
  }

  /* This function inicialize ARToolKit. */
  void setup()
  {
    const char * cparam_name = "Data/camera_para.dat";
    const char * patt_name  = "Data/patt.hiro";
    char vconf[] = "Data/WDM_camera.xml";
    setup_camera(cparam_name, vconf, &artcparam);
    // Setup argl library for current context.
    if((argl_settings = arglSetupForCurrentContext()) == NULL) {
      throw runtime_error("main(): arglSetupForCurrentContext() returned error.\n");
    }
    setup_marker(patt_name, &patt_id);
    atexit(quit);
  }

  void matrix_to_euler(double mat[16], double & angle_y)
  {
    angle_y = asin(mat[8]); // making convertion for columns...
#if 0
    /* From: GameDev.net - The Matrix and Quaternions FAQ
    http://www.gamedev.net/reference/articles/article1691.asp#Q37 */
    angle_y = D =  asin(mat[2]);         /* Calculate Y-axis angle */
    C           =  cos(angle_y);
    angle_y    *=  RADIANS;
    if(fabs( C ) > 0.005)                /* Gimbal lock? */
    {
      tr_x     =  mat[10] / C;           /* No, so get X-axis angle */
      tr_y     = -mat[6]  / C;
      angle_x  = atan2(tr_y, tr_x) * RADIANS;
      tr_x     =  mat[0] / C;            /* Get Z-axis angle */
      tr_y     = -mat[1] / C;
      angle_z  = atan2(tr_y, tr_x) * RADIANS;
    }
    else                                 /* Gimball lock has occurred */
    {
      angle_x  = 0;                      /* Set X-axis angle to zero */
      tr_x     =  mat[5];                /* And calculate Z-axis angle */
      tr_y     =  mat[4];
      angle_z  = atan2(try, trx) * RADIANS;
    }

    /* return only positive angles in [0,360] */
    if(angle_x < 0) angle_x += 360;
    if(angle_y < 0) angle_y += 360;
    if(angle_z < 0) angle_z += 360;
#endif
  }

  void check()
  {
    static Uint32 ms_prev = 0;
    ARUint8 *image;

    Uint32 ms = SDL_GetTicks();
    float s_elapsed = (float)(ms - ms_prev) * 0.001f;
    if (s_elapsed < 0.01f) return; // Don't update more often than 100 Hz.
    ms_prev = ms;

    // Update drawing.
    //DrawCubeUpdate(s_elapsed);

    ARMarkerInfo    *marker_info; // Pointer to array holding the details of detected markers.
    int             marker_num; // Count of number of markers detected.
    // Grab a video frame.
    if ((image = arVideoGetImage()) != NULL) {
      //gARTImage = image;	// Save the fetched image.

      // Detect the markers in the video frame.
      if (arDetectMarker(image, art_threshhold, &marker_info, &marker_num) < 0) {
        throw runtime_error("Error in arDetectMarker.");
      }

      // Check through the marker_info array for highest confidence
      // visible marker matching our preferred pattern.
      int k = -1;
      for(int j = 0; j < marker_num; j++) {
        if (marker_info[j].id == patt_id) {
          if (k == -1) k = j; // First marker detected.
          else if(marker_info[j].cf > marker_info[k].cf) k = j; // Higher confidence marker detected.
        }
      }

      double m[16];
      if(k != -1) {
        // Get the transformation between the marker and the real camera into gPatt_trans.
        arGetTransMat(&marker_info[k], patt_centre, patt_width, patt_trans);
        arglCameraViewRH(patt_trans, m, 1.0);
        /*
        boost::format fmter("%f%|20t|%f%|40t|%f%|60t|%f\n");
        for(int mi = 0; mi < 4; ++mi){
          for(int mj = 0; mj < 4; ++mj){
            fmter % m[mj*4 + mi]; // OpenGL keeps matrix in columns...
          }
          cout << fmter.str();
        }
        cout << '\n';
        */
        static double start_position_x = m[12], start_position_z = m[14];
        const double mov_eps = 10.0; // Acertar este valor
        const double mov_mult = 0.0625;
        cout << m[12] << '\n';

        /* Webcam should be placed at right of the foot player.In the of using the left foot 
        "caminha_frente" e "caminha_tras" will need be inverted.*/
        if(m[12] > start_position_x + mov_eps) ptr_caminha_frente(m[12] * mov_mult);
        else if(m[12] < start_position_x - mov_eps) ptr_caminha_tras(m[12] * mov_mult);

        if(m[13] > start_position_z + mov_eps) ptr_jump();

        /* Disactivated to simplify the controls :
        if(m[14] > start_position_z + mov_eps) ptr_caminha_direita();
        else if(m[14] < start_position_z - mov_eps) ptr_caminha_esquerda();
        */

        double angle_y;
        matrix_to_euler(m, angle_y);
        //cout << angle_y << '\n';
        const double rot_eps = 0.3;
        if(angle_y > rot_eps){
          ptr_vira_esquerda(secsperframe * 100.0);
        }else if(angle_y < -rot_eps){
          ptr_vira_direita(secsperframe * 100.0);
        }

        //patt_found = true;
      } else {
        //patt_found = false;
      }

      /* I removed the part to show the image captured for the ARToolkit. Now I can capture the next one. */
      arVideoCapNext();
    }
  }
}

int main(int argc, char** argv)
{
  try{
    config_file.load("rvirt.cfg");

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
      throw runtime_error("It is not possibel initialize the SDL.");
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

    artoolkit::setup();

    glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

    reshape(LARGURA, ALTURA);

    // /*
    const float luz_ambiente[] = {0.45f, 0.45f, 0.45f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);

    const float luz_difusa[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    // */

    /* light default 0:
    const float luz_especular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
    */

    // /*
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

    grab_input(false);
    log_file.file << "Configuração: " << config_file.config_name << "\n\n";

    for(;;){
      eventos();
      if(a_janela_estah_ativa){

        artoolkit::check();

        ptr_key_hold();
        ptr_mouse_button_hold();
        jump_update();
        display();

        GLenum errogl = glGetError();
        if(errogl != GL_NO_ERROR){
          
          throw runtime_error(string("Error OpenGL: ") + reinterpret_cast<const char*>(gluErrorString(errogl)));
        }
        adjveloc();
      }
    }
  }catch(exception& ex){
    log_file.file << ex.what() << endl;
  }catch(...){
    log_file.file << "Unknown exception." << endl;
  }
  return 1;
}

namespace
{

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glRotated(cima_baixo, 1.0, 0.0, 0.0);
  glRotated(rotacao, 0.0, 1.0, 0.0);
  glTranslated(-pos_x, -pos_y, -pos_z);

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
  /* Force parameter does the function to execute operations even the previous state is the same. Used in the inicialization. */
  if(!force && new_state == state) return;
  switch(new_state){
    case INIT_SCREEN:
      pos_x = mapa.inic_x;
      pos_z = mapa.inic_z;
      rotacao = mapa.inic_rotacao;
      cima_baixo = 0.0;

      mapa.clear_stats();
      log_file.clear_stats();

      ptr_key_hold = void_void_null_func;
      ptr_mouse_button_hold = void_void_null_func;
      ptr_caminha_frente = void_double_null_func;
      ptr_caminha_tras = void_double_null_func;
      ptr_caminha_esquerda = void_void_null_func;
      ptr_caminha_direita = void_void_null_func;
      ptr_jump = void_void_null_func;
      ptr_vira_esquerda = void_double_null_func;
      ptr_vira_direita = void_double_null_func;
      break;

    case PLAYING:
      log_file.timer.start();
      log_file.event("início", "início", false);
      if(state == TESTING)
        break;
      // else fall-thru
    case TESTING:

      ptr_key_hold = key_hold;
      ptr_mouse_button_hold = mouse_button_hold;
      ptr_caminha_frente = caminha_frente;
      ptr_caminha_tras = caminha_tras;
      ptr_caminha_esquerda = caminha_esquerda;
      ptr_caminha_direita = caminha_direita;
      //ptr_jump = jump; // It is not working now... Disactivated.
      ptr_vira_esquerda = vira_esquerda;
      ptr_vira_direita = vira_direita;
      break;

    case END_SCREEN:

      ptr_key_hold = void_void_null_func;
      ptr_mouse_button_hold = void_void_null_func;
      ptr_caminha_frente = void_double_null_func;
      ptr_caminha_tras = void_double_null_func;
      ptr_caminha_esquerda = void_void_null_func;
      ptr_caminha_direita = void_void_null_func;
      ptr_jump = void_void_null_func;
      ptr_vira_esquerda = void_double_null_func;
      ptr_vira_direita = void_double_null_func;

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

// === } CONTROLS ===

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

void jump(){
  if(!is_jumping && jump_timer.ja_passou(4*HALF_JUMP_DURATION)){
    is_jumping = true;
    jump_timer.start();
  }
}

void jump_update(){
  if(is_jumping){
    const double max_hite = 1.0, jump_step = max_hite / HALF_JUMP_DURATION;
    Uint32 passed = jump_timer.quanto_passou();
    if(passed < HALF_JUMP_DURATION){
      pos_y += jump_step;
    }else if(passed > HALF_JUMP_DURATION){
      if(passed > 2*HALF_JUMP_DURATION){
        pos_y = START_POS_Y;
        is_jumping = false;
      }else
        pos_y -= jump_step;
    }
  }
}

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
    /* not return, test others posibilities of collision*/ \
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
          glBindTexture(GL_TEXTURE_2D, 0); // Disactivate textures
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

            // side 2

            if(x+1 >= colunas || (*this)[z][x+1] != C_PAREDE){
              glNormal3f(1.0f, 0.0f, 0.0f);
              glTexCoord2f(0.0f, 1.0f); glVertex3d(x+1, 0.0, z+1);
              glTexCoord2f(2.0f, 1.0f); glVertex3d(x+1, 0.0, z);
              glTexCoord2f(2.0f, 0.0f); glVertex3d(x+1, 0.5, z);
              glTexCoord2f(0.0f, 0.0f); glVertex3d(x+1, 0.5, z+1);
            }

            // side 3

            if(z+1 >= linhas || (*this)[z+1][x] != C_PAREDE){
              glNormal3f(0.0f, 0.0f, 1.0f);
              glTexCoord2f(0.0f, 1.0f); glVertex3d(x, 0.0, z+1);
              glTexCoord2f(2.0f, 1.0f); glVertex3d(x+1, 0.0, z+1);
              glTexCoord2f(2.0f, 0.0f); glVertex3d(x+1, 0.5, z+1);
              glTexCoord2f(0.0f, 0.0f); glVertex3d(x, 0.5, z+1);
            }

            // side 4

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
  
            // side 1

            glNormal3f(0.0f, 0.0f, -1.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3d(x+0.9, 0.45, z+0.1);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(x+0.9, 0.0, z+0.1);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(x+0.1, 0.0, z+0.1);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(x+0.1, 0.45, z+0.1);

            // side 2

            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(x+0.9, 0.0, z+0.9);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(x+0.9, 0.0, z+0.1);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(x+0.9, 0.45, z+0.1);
            glTexCoord2f(0.0f, 0.0f); glVertex3d(x+0.9, 0.45, z+0.9);

            // side 3

            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3d(x+0.1, 0.0, z+0.9);
            glTexCoord2f(1.0f, 1.0f); glVertex3d(x+0.9, 0.0, z+0.9);
            glTexCoord2f(1.0f, 0.0f); glVertex3d(x+0.9, 0.45, z+0.9);
            glTexCoord2f(0.0f, 0.0f); glVertex3d(x+0.1, 0.45, z+0.9);

            // side 4

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
    throw runtime_error("Can't load texture \"" caixa_file_name "\".");

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

      case 'f': case 'F':
        fullscreen = true;
        break;

      case '#':
        // Comnent
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
    throw runtime_error("Can't load the final screen  \"" file_name "\".");
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

} // namespace
