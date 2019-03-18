#include "qdicethread.h"
#include<QTime>
void QDiceThread::diceBegin()
{
    m_Paused=false;
}

void QDiceThread::dicePause()
{
    m_Paused=true;
}

void QDiceThread::stopThread()
{
    m_stop=true;
}

QDiceThread::QDiceThread()
{
//构造函数
}

void QDiceThread::run()
{
    m_stop=false;
    m_seq=0;
    qsrand(QTime::currentTime().msec());
    while(!m_stop)
    {
        if(!m_Paused)
        {
            m_diceValue=qrand()%6+1;
            m_seq++;
            emit newValue(m_seq,m_diceValue);
        }
        msleep(500);
    }
   // quit();
}
