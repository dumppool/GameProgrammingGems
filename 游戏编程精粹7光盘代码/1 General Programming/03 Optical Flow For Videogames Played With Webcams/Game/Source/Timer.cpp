#include <string>
#include <Windows.h>
#include "Timer.h"

//Inicializamos todas las variables
CTimer::CTimer( unsigned int avgSamples = 30 )
{
	mSamples		= avgSamples;
	m_fElpasedTime	= 0.f;
	m_dLastTime		= 0;
	m_fps			= 0.f;
	m_fpsTime		= 0.f;
	m_fpsCount		= 0;
	m_LastFps		= 0.f;
	mIndex			= 0;
	
	mDeltas = new float[mSamples];
	for(unsigned int j=0;j<mSamples;j++)
	{
		mDeltas[j] = 0;
	}

}

CTimer::~CTimer( void )
{
	delete mDeltas;
}

// Actualiza los valores FPS, ElapsedTime y los timers (los normales y los inputs)
void CTimer::Update( void )
{
	
	//-----Actualizacion de los valores FPS y ElpasedTime-----
	//Calculo de la diferencia de tiempo (m_fElpasedTime)
	double	l_dCurTime     = timeGetTime();

	if( m_dLastTime == 0 ) 
		m_dLastTime = l_dCurTime;

	mDeltas[mIndex] = (float)((l_dCurTime - m_dLastTime) * 0.001);
	m_dLastTime    = l_dCurTime;

	float d = 0;
	for(unsigned int j=0; j<mSamples; j++)
	{
		d += mDeltas[j];
	}
	d /= (float)mSamples;
	m_fElpasedTime = d;
	mIndex = (++mIndex) % mSamples;
	

	//Calculo de los frames por segundo (m_fps)
	m_fpsCount++;
	m_fpsTime += m_fElpasedTime;
	
	m_fps = m_fpsCount/m_fpsTime;
	
	if( l_dCurTime - m_LastFps > 1000 )	//Solo mostramos los fps cada segundo
	{
		m_fps = m_fpsCount/m_fpsTime;
		m_fpsCount = 0;
		m_fpsTime = 0;
		m_LastFps = (float)l_dCurTime;
	}
	//---------------------------------------------//



	//-----------Actualizacion de los timers ------//

	//Primero actualizamos los InputTimer: Si la cuenta atas de algun InputTimer llego
	//a 0, se elimina de la estructura de datos que la contiene (m_MapInputTimer)
	for(it_MapInputTimer = m_MapInputTimer.begin(); it_MapInputTimer != m_MapInputTimer.end(); it_MapInputTimer++)
	{
		Pair_Tinit_Dt time = it_MapInputTimer->second;
		//Si ya paso el tiempo => eliminamos el timer
		if( (float)(l_dCurTime - time.first) * 0.001 > time.second )
		{
			m_MapInputTimer.erase( it_MapInputTimer );
			break;
		}
	}
	//A continuacion actualizamos los NormalTimer: Si la cuenta atas de algun NormalTimer llego
	//a 0, se elimina de la estructura de datos que la contiene (m_MapNormalTimer)
	for(it_MapNormalTimer = m_MapNormalTimer.begin(); it_MapNormalTimer != m_MapNormalTimer.end(); it_MapNormalTimer++)
	{
		Pair_Tinit_Dt time = it_MapNormalTimer->second;
		//Si ya paso el tiempo => eliminamos el timer
		if( (float)(l_dCurTime - time.first) * 0.001 > time.second )
		{
			m_MapNormalTimer.erase( it_MapNormalTimer );
			break;
		}
	}
	//----------------------------------------------//
}



//------------------- NORMAL TIMER---------------------------//

// Inserta en el map de los NormalTimer un nuevo timer.
//	int id -> el identificador del nuevo timer a insertar
//	float time -> tiempos de duracion del timer a insertar
void CTimer::SetNormalTimer(int id, float time)
{
	//Se inserta un nuevo NormalTimer con la informacion de:
	//	- indentificador (id)
	//	- tiempo de duracion del timer (time)
	//	- el tiempo en que empezo su cuenta atras (l_dCurTime)
	double	l_dCurTime     = timeGetTime();
	m_MapNormalTimer.insert( Pair_NormalTimer(id, Pair_Tinit_Dt((float)l_dCurTime, time) ) );
}

//Comprueba si la cuenta atras del NormalTime con identificador id llego a 0
//	int id -> el identificador del timer.
bool CTimer::IsEndNormalTimer(int id)
{
	//Si se encuentra el timer quiere decir que la cuenta atras no llego a 0
	//De haber llegado a 0, en la funcion Update se hubiera eliminado el timer id
	
	bool IsEndNormalTimer = false;

	it_MapNormalTimer = m_MapNormalTimer.find( id );
	IsEndNormalTimer = ( it_MapNormalTimer != m_MapNormalTimer.end() );
		
	return IsEndNormalTimer;
}



//-------------------INPUT TIMER---------------------------//

// Si el timer "name" no existe o aun no llego a 0 crea uno de nuevo, lo inserta y
// retorna true. Si existe (su cuentra atras aun no llego a 0) retorna false.
//	std::string name -> identificador del timer.
//	float time -> tiempo de duracion del timer a insertar.
bool CTimer::IsEndInputTimer(std::string name, float time)
{
	double	l_dCurTime     = timeGetTime();
	bool IsEndInputTimer = false;
	//Si existe ==> aun no ha transcurrido el tiempo dt.
	//La funcion Update se encarga de eliminarlo si su tiempo transcurrio
	it_MapInputTimer = m_MapInputTimer.find( name );
	if( it_MapInputTimer != m_MapInputTimer.end() )
	{
		IsEndInputTimer =  false;
	}
	else
	{
		//No existe ==> Lo insertamos al mapa de timers con su informacion
		//La informacion es su nombre, el tiempo de duracion y el tiempo en que fue insertado.
		m_MapInputTimer.insert( Pair_InputTimer( name, Pair_Tinit_Dt((float)l_dCurTime, time) ) );
		IsEndInputTimer =  true;
	}

	return IsEndInputTimer;
}