#include "leaderboardmanager.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QDir>

LeaderboardManager::LeaderboardManager(const QString& filename) {
    // Save the file in a temporary location or user's home path for persistence
    filename_ = QDir::tempPath() + "/" + filename;
}

QVector<ScoreEntry> LeaderboardManager::loadScores() {
    QFile file(filename_);
    QVector<ScoreEntry> scores;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split(',');
            if (parts.size() == 2) {
                bool ok;
                int score = parts[0].toInt(&ok);
                if (ok) {
                    scores.push_back({score, parts[1]});
                }
            }
        }
        file.close();
    }

    // Sort the scores and keep only the top 5
    std::sort(scores.begin(), scores.end(), std::greater<ScoreEntry>());
    if (scores.size() > 5) {
        scores.resize(5);
    }
    return scores;
}

void LeaderboardManager::addScore(const QString& name, int newScore) {
    QVector<ScoreEntry> scores = loadScores();
    scores.push_back({newScore, name});

    // Sort and keep the top 5
    std::sort(scores.begin(), scores.end(), std::greater<ScoreEntry>());
    if (scores.size() > 5) {
        scores.resize(5);
    }

    // Write scores back to file
    QFile file(filename_);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QTextStream out(&file);
        for (const auto& entry : scores) {
            out << entry.score << "," << entry.name << "\n";
        }
        file.close();
    } else {
        qWarning() << "Could not open leaderboard file for writing:" << file.errorString();
    }
}