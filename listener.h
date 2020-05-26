#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>

class listener : public QObject
{
    Q_OBJECT
public:
    explicit listener(QObject *parent = nullptr);

signals:

};

#endif // LISTENER_H
