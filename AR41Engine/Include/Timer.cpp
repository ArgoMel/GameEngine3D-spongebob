#include "Timer.h"

CTimer::CTimer()
	: m_Second{}
	, m_Time{}
	, m_DeltaTime(0.f)
	, m_FPS(0.f)
	, m_FPSTime(0.f)
	, m_Tick(0)
{
}

CTimer::~CTimer()
{
}

void CTimer::Init()
{
	QueryPerformanceFrequency(&m_Second);	// ���ػ� Ÿ�̸Ӱ� 1�ʰ� �귶������ ���� ���´�.
	QueryPerformanceCounter(&m_Time);	// ���� ���ػ� Ÿ�̸��� ���� ���´�.
}

void CTimer::Update()
{
	LARGE_INTEGER	time;
	QueryPerformanceCounter(&time);
	// (���� ���ػ� Ÿ�̸Ӱ� - ���� �������� ���ػ� Ÿ�̸Ӱ�) / �ʴ� ���ػ� Ÿ�̸Ӱ�
	m_DeltaTime = (time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;
	m_Time = time;	// ���� Ÿ�̸Ӱ��� ���� Ÿ�̸Ӱ����� ��ü�Ѵ�.
	m_FPSTime += m_DeltaTime;
	++m_Tick;
	if (m_Tick == 60)
	{
		m_FPS = m_Tick / m_FPSTime;
		m_FPSTime = 0.f;
		m_Tick = 0;
	}
}
