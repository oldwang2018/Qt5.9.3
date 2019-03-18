#ifndef QDICETHREAD_H
#define QDICETHREAD_H
#include<QThread>

class QDiceThread:public QThread
{
    Q_OBJECT
private:
    int m_seq=0;
    int m_diceValue=0;
    bool m_Paused=true;
    bool m_stop=false;
protected:
    void run() Q_DECL_OVERRIDE;
public:
    void diceBegin();
    void dicePause();
    void stopThread();
signals:
    void newValue(int seq,int diceValue);

public:
    QDiceThread();
};

#endif // QDICETHREAD_H
