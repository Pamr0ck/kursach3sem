#ifndef ARCHIVATOR_H
#define ARCHIVATOR_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
#include <QString>
#include <QVector>
#include <QQueue>
#include <wchar.h>


//#define QChar wchar_t
#define QCHARLEN 15   // utf-8
typedef QVector<QChar> code;

struct Node{
    QChar letter = 0;
    code letter_bin;
    int w = 0;
//	int num = 256;

    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
};


class archivator
{
public:
    archivator();
    QString DoCode (QString stream);
};

#endif // ARCHIVATOR_H
