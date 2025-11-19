#ifndef LEADERBOARDMANAGER_H
#define LEADERBOARDMANAGER_H

#include <QVector>
#include <QString>
#include <algorithm>

// Structure to hold a single score entry
struct ScoreEntry {
    int score;
    QString name;

    // Operator for sorting (highest score first)
    bool operator>(const ScoreEntry& other) const {
        return score > other.score;
    }
};

// Class to handle loading and saving the top scores
class LeaderboardManager {
public:
    LeaderboardManager(const QString& filename = "eggcatcher_leaderboard.txt");

    // Load top 5 scores from the file
    QVector<ScoreEntry> loadScores();

    // Add a new score and save the updated list (maintains top 5)
    void addScore(const QString& name, int newScore);

private:
    QString filename_;
};

#endif // LEADERBOARDMANAGER_H