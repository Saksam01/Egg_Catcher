#ifndef LEADERBOARDMANAGER_H
#define LEADERBOARDMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QVector>

struct ScoreEntry {
    QString name;
    int score;
};

class LeaderboardManager : public QObject
{
    Q_OBJECT

public:
    explicit LeaderboardManager(QObject *parent = nullptr);

    // Push score
    void addScore(const QString &name, int score);

    // Load sorted scores
    QVector<ScoreEntry> loadScores();

private:
    void fetchScores();

private:
    QNetworkAccessManager net;
    QString firebaseUrl;
    QVector<ScoreEntry> cachedScores;
};

#endif
