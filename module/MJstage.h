#ifndef MJSTAGE
#define MJSTAGE


#include <memory>
#include "MJcollection.h"
#include "Shuffler.h"
#include "MJplayer.h"
#include "MJAIplayer.h"
#include "Debug.h"

using namespace std;

class MJstage {
public:
    MJstage(void);				
    MJstage(int, int, int);
    //MJstage(MJstage&);    // copy constructor
    ~MJstage(void);
    void clear(void);
    void pickSeat(void);		
    void pickBookmaker(void);	
    void nextBookmaker(void);   
    int getBookmaker(void);     
    void getTiles(void);		
    void initiate(void);	
    // return pair<winner, loser> index
    pair<int, int> mainGame(int&);
    void countTai(void);
    int get_money(int);ss
    int get_seed(void);
    void writeInfo(void);
    MJstage& operator=(MJstage&&);

private:
	vector<unique_ptr<MJplayer> > _players;
        // It's a vector by TA originally
	int _bookmaker;
	int seed;
	// player index (0 - 3), not position (1 - 4)
	MJcollection mjcol;
	map<int, int> playerToPos;
	map<int, int> posToPlayer;
};
#endif
