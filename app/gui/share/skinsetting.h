#ifndef TSKINSETTING_H
#define TSKINSETTING_H

#include <QSettings>

class TSkinSetting
{
public:
    explicit TSkinSetting(QString skinName);
    ~TSkinSetting();

    void writeMainWindowState(const QByteArray &geometry, const QByteArray &state);
    void writeEqualizerWindowState(const QByteArray &geometry, const QByteArray &state);
    void writeDesktopLyricWindowState(const QByteArray &geometry, const QByteArray &state);
    void writeLyricWindowState(const QByteArray &geometry, const QByteArray &state);
    void writePlaylistWindowState(const QByteArray &geometry, const QByteArray &state);
    void writeBrowserWindowState(const QByteArray &geometry, const QByteArray &state);

    void mainWindowState(QByteArray *geometry, QByteArray *state);
    void equalizerWindowState(QByteArray *geometry, QByteArray *state);
    void desktopLyricWindowState(QByteArray *geometry, QByteArray *state);
    void lyricWindowState(QByteArray *geometry, QByteArray *state);
    void playlistWindowState(QByteArray *geometry, QByteArray *state);
    void browserWindowState(QByteArray *geometry, QByteArray *state);

    bool exists() { return mExists; }

private:
    bool mExists;
    QSettings *mSettings;
};

#endif // TSKINSETTING_H
