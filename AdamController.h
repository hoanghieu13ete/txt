#pragma once
#ifdef USING_ADAM

#using "lib\Advantech\Advantech.Adam.dll"


#endif
ref class AdamController
{
public:
	AdamController();
	~AdamController();
	bool connectToAdam();
	void openBarrier();
	void closeBarrier();
	void refreshAdam();
private:
	Advantech::Adam::AdamSocket^ m_adamModbus;
	Advantech::Adam::Adam6000Type m_Adam6000Type;
	//bool m_isConnected;
	bool m_bStart;
	int m_iPort;
	bool m_signalOpen;
};

