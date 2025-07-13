#ifndef MJPLAYER
#define MJPLAYER
#include <map>
#include <vector>
#include <iostream>
#include "MJhand.h"
using namespace std;
class MJplayer {
public :
	MJplayer();
	MJplayer(int); // money
	MJplayer(int, int, MJtile*, int); //每個玩家的目前狀態 position, money, tiles, num_tiles
	~MJplayer();

	void Set_Pos(int); //選位置
	int Get_Pos() const;

	void Set_Mon(int); //東南西北
	int Get_Mon() const;

	int faceup_len() const;

	void Set_Hand(MJhand&); // tiles, num_tiles
	void Set_Hand(MJtile*, int); // tiles, num_tiles
	void Print_Hand() const;

	virtual void initiate(MJcollection&);
	void draw(MJcollection&);
	MJtile play(int);
	void act(int, int, MJtile&, MJcollection&);

	void showhandtoothers(void);
	void clear_info(void);
	// actiontype: hu=-1 nothing=0 eat=1 pong=2 minggone=3 angone=4 bugone=5
	virtual void strategy(int position, MJtile t, int &actiontype, int &actionparameter); 
	// call after every play
	virtual int decidePlay(void);
	virtual void getinfo(int position, int type, MJtile* ts, int tiles_num);
	// type: eat=1 pong=2 minggone=3 angone=4 bugone=5 applique=6
	virtual bool is_human(void);
	virtual string className(void);
	virtual string getFunctionOrder(void);

protected :
	MJhand _hand;
	int _position;//位置east = 1 ,south = 2 , west = 3 , north = 4
	int _money; //台數
	map<int, int> previousPlayer;

	vector<vector<int> > out;//每個玩家出牌或吃碰過的陣列out[suit][rank]
	int count_angone;
	MJtile previousTile;
};
#endif



