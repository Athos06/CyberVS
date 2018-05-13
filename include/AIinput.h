//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef AI_INPUT_H
#define AI_INPUT_H
//|||||||||||||||||||||||||||||||||||||||||||||||

class Player;
class AIplayer;
//|||||||||||||||||||||||||||||||||||||||||||||||

class AIinput {
public:
	AIinput();
	~AIinput();
	
	void initAI(int);

	float calcForce(AIplayer*, int);
	void getInput(AIplayer*, double);
	void getAngle();
	void resetInput();
private:
	//variables para control de posicion
	//y disparo
	float m_goalRotation;
	float m_goalPower;

	float m_readyRotation;
	float m_readyPower;
	bool m_readyToShoot;

	int	m_initialError;
	int	m_currentError;

	float m_initialAngle;

	bool m_getAngle;
	bool m_getPower;

	bool m_rotDirection;
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif
//|||||||||||||||||||||||||||||||||||||||||||||||