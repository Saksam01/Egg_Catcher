#include "LeaderboardManager.h"
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>

LeaderboardManager::LeaderboardManager(QObject *parent)
    : QObject(parent)
{
    firebaseUrl =
        "https://eggcatcher-7e326-default-rtdb.firebaseio.com/leaderboard.json";
}

/* -------------------------------------------------------------
   ADD OR UPDATE PLAYER SCORE
--------------------------------------------------------------*/
void LeaderboardManager::addScore(const QString &name, int score)
{
    fetchScores();

    QString existingKey;
    int oldScore = -1;

    // 1) Find if player exists
    {
        QNetworkRequest getReq(firebaseUrl);
        QEventLoop loop;
        QNetworkReply *rep = net.get(getReq);

        connect(rep, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (rep->error() == QNetworkReply::NoError) {
            QJsonObject root =
                QJsonDocument::fromJson(rep->readAll()).object();

            for (auto it = root.begin(); it != root.end(); ++it) {
                QJsonObject obj = it.value().toObject();
                if (obj["name"].toString() == name) {
                    existingKey = it.key();
                    oldScore = obj["score"].toInt();
                    break;
                }
            }
        }
        rep->deleteLater();
    }

    // 2) If exists → update only if score improved
    if (!existingKey.isEmpty()) {

        if (score <= oldScore)
            return;     // No update required

        QString updateUrl =
            QString("https://eggcatcher-7e326-default-rtdb.firebaseio.com/leaderboard/%1.json")
                .arg(existingKey);

        QJsonObject data;
        data["name"] = name;
        data["score"] = score;

        QNetworkRequest req(updateUrl);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply *rep = net.put(req, QJsonDocument(data).toJson());

        QEventLoop loop;
        connect(rep, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        rep->deleteLater();
        return;
    }

    // 3) If new → push new record
    QJsonObject obj;
    obj["name"] = name;
    obj["score"] = score;

    QNetworkRequest req(firebaseUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *rep = net.post(req, QJsonDocument(obj).toJson());

    QEventLoop loop;
    connect(rep, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    rep->deleteLater();
}


/* -------------------------------------------------------------
   LOAD scores sorted
--------------------------------------------------------------*/
QVector<ScoreEntry> LeaderboardManager::loadScores()
{
    cachedScores.clear();
    fetchScores();
    return cachedScores;
}


/* -------------------------------------------------------------
   Internal: fetch scores from firebase REST
--------------------------------------------------------------*/
void LeaderboardManager::fetchScores()
{
    QNetworkRequest req(firebaseUrl);
    QEventLoop loop;

    QNetworkReply *rep = net.get(req);
    connect(rep, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (rep->error() != QNetworkReply::NoError) {
        rep->deleteLater();
        return;
    }

    QByteArray raw = rep->readAll();
    rep->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (!doc.isObject()) return;

    QJsonObject root = doc.object();

    cachedScores.clear();
    for (auto it = root.begin(); it != root.end(); ++it) {
        QJsonObject obj = it.value().toObject();

        ScoreEntry e;
        e.name = obj["name"].toString();
        e.score = obj["score"].toInt();
        cachedScores.push_back(e);
    }

    // Sort in descending order
    std::sort(cachedScores.begin(), cachedScores.end(),
              [](auto &a, auto &b) { return a.score > b.score; });
}
