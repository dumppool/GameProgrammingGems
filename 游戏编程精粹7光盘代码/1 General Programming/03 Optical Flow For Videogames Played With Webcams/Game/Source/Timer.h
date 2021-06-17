 // Contiene toda la informacion relacionada con el tiempo, es decir:
 //- FPS: los frames por segundo a que va la aplicacion
 //- ElapsedTime: el tiempor que ha pasado dsd la ultima llamad a Update
 // Aparte manipula timers-> relojes que se ponen en marcha y no se eliminana hasta 
 // pasado un cierto tiempo. Hay dos tipos de Timers, su diferencia se basa unicamente
 // en como se utilizan sus funciones.
 //- Timer normales: manipula los timers normales con IsEndNormalTimer y SetNormalTimer
 //- Timer de Input: manipula los timers de Input con la funcion IsEndInputTimer
 //  este timer solo necesita una funcion para ponerse en marcha y preguntar si finalizo


#ifndef INC_TIMERMANAGER_H_
#define INC_TIMERMANAGER_H_

#include <map>

//------------------Definicion de nuevos tipos---------------------//
typedef std::pair<float, float>		Pair_Tinit_Dt; //(Tiempo de inicio, duracion del timer)

typedef std::pair< std::string, Pair_Tinit_Dt >				Pair_InputTimer;
typedef std::map< std::string, Pair_Tinit_Dt >				MapInputTimer;
typedef std::map< std::string, Pair_Tinit_Dt >::iterator	It_MapInputTimer;

typedef std::pair< int, Pair_Tinit_Dt >				Pair_NormalTimer;
typedef std::map< int, Pair_Tinit_Dt >				MapNormalTimer;
typedef std::map< int, Pair_Tinit_Dt >::iterator	It_MapNormalTimer;
//--------------------------------------------------------------------//


class CTimer
{

public:

	CTimer(unsigned int avgSamples);
	~CTimer();
	
	// Aactualiza los timers, el valor de FPS y ElapsedTime
	void	Update			( void );
	
	//-- TIMER FUNCTIONS --
	void	SetNormalTimer		( int id, float time );
	bool	IsEndNormalTimer	( int id );
	bool	IsEndInputTimer		( std::string name, float time );
	

	//-- GET FUNCTIONS --
	float	GetElapsedTime	( void ) const { return m_fElpasedTime; }
	float	GetFPS			( void ) const { return m_fps; }
	
private:
	
	MapInputTimer			m_MapInputTimer; //Map con los InputTimers
	It_MapInputTimer		it_MapInputTimer;
	MapNormalTimer			m_MapNormalTimer; //Map con los NormalTimers
	It_MapNormalTimer		it_MapNormalTimer;

	float					m_fElpasedTime; // ElapsedTime
	float					m_fps;			// FPS
	float					m_LastFps;
	float					m_fpsTime;
	double					m_dLastTime;
	int						m_fpsCount;
	float*					mDeltas;			// array of instant delta times
	unsigned int			mSamples;			// deltas arrays length
	unsigned int			mIndex;				// current array position
};

#endif