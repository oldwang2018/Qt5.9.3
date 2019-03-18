#ifndef QPERSON_H
#define QPERSON_H

#include <QObject>

class QPerson : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("author","Wang")
    Q_CLASSINFO("company","UPC")
    Q_CLASSINFO("version","1.0.0")

    Q_PROPERTY(int ageTEST  READ readAge  WRITE setAge NOTIFY ageChanged)
    Q_PROPERTY(QString nameTEST MEMBER m_name)
    Q_PROPERTY(int score MEMBER m_score)

private:
    int  m_age=10;
    QString m_name;
    int     m_score=79;
public:
    explicit QPerson(QString fName, QObject *parent = nullptr);

    int     readAge();
    void    setAge(int value);

    void    incAge();
signals:
    void    ageChanged( int  value);

public slots:
};

#endif // QPERSON_H
