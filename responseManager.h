#ifndef RESPONSEMANAGER_H
#define RESPONSEMANAGER_H

#include <iostream>

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QTextCodec>


class ResponseManager : public QObject
{
    Q_OBJECT
public:
    explicit ResponseManager(QObject *parent = 0);

    QString MakeResponse(QString inputData);

signals:

public slots:

private:

};

#endif // RESPONSEMANAGER_H
