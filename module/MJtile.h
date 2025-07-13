#ifndef MJTILE
#define MJTILE
/*麻將的物件, 有花色、數字、ID、和比較的形式*/
#include <iostream> 
#include <cassert> 
using namespace std; 
class MJtile{
	friend ostream& operator << (ostream&, const MJtile&);
public:
	MJtile();
	MJtile(int id); //
	~MJtile();
	void setfromId(int);
	/*設置每敦麻將的編號 */
	int suit() const;
	/*麻將的花樣,suit=1為條,suit=1為筒,suit=1為萬,suit=1為風牌或花牌*/
	int rank() const;
	/*條統萬的數字*/
	int flower() const; 
	/*get flower number, if not flower return 0*/
	int getTileId() const;
	/*get tile id*/ 
	bool operator == (const MJtile&) const;
	/*compare same kind of tile, don't need to worry of flower tile*/ 
	bool operator != (const MJtile&) const;
	/*compare not the same kind of tile, don't need to worry of flower tile*/ 
	bool fromsuitrank(int, int) const;
	/*如果兩墩麻將有相同的數字和花色, 回傳真值*/ 
private:
	int _suit;//1~4 
	int _rank;//1~9 
	int _flowernum;//1~4 
	int _tileId; //1~144. 總共有144張牌
	/*呈現出來的所有花色數字和id, 放在private裡面其他非MJtile.cpp的code就無法更動它*/
}; 
#endif
