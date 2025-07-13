#ifndef MJHAND
#define MJHAND
/*每個玩家手中都會有牌, 此為手牌的物件*/
#include <iostream> 
using namespace std;

#include "MJtile.h" 
#include "MJcollection.h" 
class MJhand{
	friend ostream& operator << (ostream&, const MJhand&);
public:
	MJtile& operator [] (int);
	
	MJhand();
	MJhand(MJtile*, int);
	~MJhand();
	
	int faceup_len() const;
	void set_faceup_len(int f); //能出的牌的頭

	int total_len() const; 
	void set_total_len(int f); //能出的牌的尾
	
	int stage() const;
	void set_stage(int s);
	
	/*可以採取的動作*/
	int caneat(const MJtile&);
	bool canpong(const MJtile&);
	bool canminggone(const MJtile&);
	bool canangone(const MJtile&);
	bool canbugone(const MJtile&);
	bool canhu(const MJtile&);

	void arrange(); //排列手中的牌
	void draw(MJcollection&); //抽牌
	void draw(MJtile&);
	MJtile play(int);
	void faceup(int);
	void applique(int, MJcollection&); //補花
	void initial(MJcollection&);
	void showhandtoothers();

       /*執行可以採取的動作*/
	void eat(const MJtile&, int);
	void pong(const MJtile&);
	void minggone(const MJtile&, MJcollection&);
	void angone(int, MJcollection&);
	void bugone(int, MJcollection&);
	void huother(const MJtile&);
	void huown();
    MJhand* gethuhand();

	int getTotalLen(void);
	int getFaceupLen(void);
	MJtile getLastTile(void);
		
private:
	int _total_len; //玩家手裡總共能出的牌的尾（rear)
	int _faceup_len; //玩家手裡總共能出的牌的頭 (front) //丟到faceup外面的都是吃過或碰過的牌
	int _stage;//玩家的狀態：0是正常、1是抽牌後的狀態、2是胡
	MJtile _tiles[30];//胡的標準是拿17張牌來判斷, 一個玩家手裡最多能有的牌數為 30 = 17+8+5
    bool noreveal[30]; //沒有牌
}; 
#endif
