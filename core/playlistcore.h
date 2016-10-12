#ifndef TPLAYLISTCORE_H
#define TPLAYLISTCORE_H

#include <string>
#include <list>
using namespace std;

class TPlaylistCore
{
public:
    TPlaylistCore();

    void addPlaylist(string name);
    void removePlaylist(int index);
    void savePlaylist(int index);
    void renamePlaylist(int index, string newName);
    void sort();
    void saveAll();

private:
    int mCurrentPlaylistIndex;
};

#endif // TPLAYLISTCORE_H
