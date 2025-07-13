/*處理手中的牌*/
#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include "MJhand.h"
#include "MJtile.h"
#include "MJcollection.h"

using namespace std;

//===================== Utility Functions ===============================
/*MJhand.h裡面沒有的內容*/


bool MJcompare(const MJtile& i, const MJtile& j) {
	// Utility function used in arrange()
	// 把牌照花色和大小牌（同時也是依照id)
	if (i.suit() < j.suit()) {
		return true;
	} else if (i.suit() == j.suit()) {
		// if flower not zero, compare id
		if (i.flower() != 0 && j.flower() != 0) {
			if (i.getTileId() < j.getTileId()) { return true; }
			else { return false; }
		}
		// compare rank
		if (i.rank() < j.rank()) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

/*檢查自己手裡的每一張牌*/
/*很重要！！也是判斷can系列函式的依據*/
bool tile_in_hand(const MJtile* tiles, int face_up, int total_len, int suit, int rank) {
	for (int i = face_up; i < total_len; i++) {
		if (tiles[i].fromsuitrank(suit, rank))
			return true;
	}
	return false;
}

/*交換牌的位置*/
void swap(MJtile& tile1, MJtile& tile2) {
	MJtile temp = tile1;
	tile1 = tile2;
	tile2 = temp;
}


//=================== Class Methods =====================================================
MJhand::MJhand() {
	// 歸零功用
	_total_len = 0;
	_faceup_len = 0;
	_stage = 0;
	for (int i = 0; i < 30; i++) {
		_tiles[i] = MJtile();
        noreveal[i] = false;
	}
}

/*目前手裡所擁有的所有牌*/
MJhand::MJhand(MJtile* t, int n) {
	_total_len = n;
	_faceup_len = 0;
	_stage = 0;
	for (int i = 0; i < 30; i++) {
		if (i < n) {
			_tiles[i].setfromId(t[i].getTileId());
		} else {
			_tiles[i] = MJtile();
		}
        noreveal[i] = false;
	}
}


MJhand::~MJhand() {}


int MJhand::faceup_len() const {
	return _faceup_len;
}


void MJhand::set_faceup_len(int f) {
	_faceup_len = f;
}


int MJhand::total_len() const {
	return _total_len;
}


void MJhand::set_total_len(int t) {
	_total_len = t;
}


int MJhand::stage() const {
	return _stage;
}


void MJhand::set_stage(int s) {
	_stage = s;
}


int MJhand::getTotalLen(void) {
	return _total_len;
}

int MJhand::getFaceupLen(void) {
	return _faceup_len;
}

MJtile MJhand::getLastTile(void) {
	// stage == 0 return total_len-1
	// stage == 1 return total_len
	return _tiles[_total_len + _stage - 1];
}
//======= can xxx =================================
//開始用tile_in_hand檢查牌堆
//const MJtile&裡的t為出到牌桌上的牌
int MJhand::caneat(const MJtile& t) {
	int suit = t.suit();
	int rank = t.rank();
	int return_value = 0;
	if (suit == 1 || suit == 2 || suit == 3) {
		// (001) 1為要吃的牌;0為手中的牌
		if (tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank - 1) &&
		        tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank - 2)) {
			return_value += 1;
		}
		// (010)
		if (tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank - 1) &&
		        tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank + 1)) {
			return_value += 2;
		}
		// (100)
		if (tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank + 1) &&
		        tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank + 2)) {
			return_value += 4;
		}
	}
	return return_value;
}

bool MJhand::canpong(const MJtile& t) {
	int count = 0;
	// 計算有幾個一樣的 tiles
	for (int i = _faceup_len; i < _total_len; i++) {
		if (_tiles[i].fromsuitrank(t.suit(), t.rank())) {
			count++;
		}
	}
	if (count >= 2) {
		return true;
	} else {
		return false;
	}
}

bool MJhand::canminggone(const MJtile& t) {
	int count = 0;
	// 計算有幾個一樣的 tiles
	for (int i = _faceup_len; i < _total_len; i++) {
		if (_tiles[i].fromsuitrank(t.suit(), t.rank())) {
			count++;
		}
	}
	if (count == 3) {
		return true;
	} else {
		return false;
	}
}

bool MJhand::canangone(const MJtile& t) {
	for (int i = _faceup_len; i < _total_len - 2; i++) {
		if (_tiles[i].fromsuitrank(t.suit(), t.rank()) && _tiles[i + 1].fromsuitrank(t.suit(), t.rank()) &&
		        _tiles[i + 2].fromsuitrank(t.suit(), t.rank())) {
			return true;
		}
	}
	return false;
}

bool MJhand::canbugone(const MJtile& t) {
	// i, i+1, i+2 should be the same as t
	if (_faceup_len < 3) return false;
	for (int i = 0; i < _faceup_len - 2; i++) {
		if (_tiles[i].rank() == t.rank() && _tiles[i + 1].rank() == t.rank() && _tiles[i + 2].rank() == t.rank() && _tiles[i].suit() == t.suit() && _tiles[i + 1].suit() == t.suit() && _tiles[i + 2].suit() == t.suit()) {
			return true;
		}
	}
	return false;
}





typedef vector<vector<int> > table;

//各個不同的組合
//用vector<MJtile>召喚出suit和rank, 以賦予tv這樣的物件型態
table getCombination(vector<MJtile> tv) {
	table combination_table;
	// 碰
	for (int i = 0; i < tv.size() - 2; i++) {
		int suit = tv[i].suit();
		int rank = tv[i].rank();
		if (tv[i + 1].fromsuitrank(suit, rank) && tv[i + 2].fromsuitrank(suit, rank)) {
			vector<int> combination = {i + 2, i + 1, i};
			combination_table.push_back(combination);
		}
	}
	// 槓
	for (int i = 0; i < tv.size() - 3; i++) {
		int suit = tv[i].suit();
		int rank = tv[i].rank();
		if (tv[i + 1].fromsuitrank(suit, rank) && tv[i + 2].fromsuitrank(suit, rank) && tv[i + 3].fromsuitrank(suit, rank)) {
			vector<int> combination = {i + 3, i + 2, i + 1, i};
			combination_table.push_back(combination);
		}
	}
	// 吃
	for (int suit = 1; suit <= 3; suit++) {
		for (int first_rank = 1; first_rank <= 7; first_rank++) {
			// first_rank 在 tv 裡面 && first_rank+1 在 tv 裡面 && first_rank+2
			vector<int> combination;
			for (int i = 0; i < tv.size(); i++) {
				if (tv[i].fromsuitrank(suit, first_rank + 2)) {
					combination.push_back(i);
					break;
				}
			}
			for (int i = 0; i < tv.size(); i++) {
				if (tv[i].fromsuitrank(suit, first_rank + 1)) {
					combination.push_back(i);
					break;
				}
			}
			for (int i = 0; i < tv.size(); i++) {
				if (tv[i].fromsuitrank(suit, first_rank)) {
					combination.push_back(i);
					break;
				}
			}
			if (combination.size() == 3) {
				combination_table.push_back(combination);
			}
		}
	}

	return combination_table;
}


bool hu_recursion(vector<MJtile>& tv) {
	// 剩一張就直接回傳
	if (tv.size() == 1){
		return false;
	}


	// 剩兩張且兩張一樣的話
	if (tv.size() == 2) {
		return (tv[0].suit() == tv[1].suit() && tv[0].rank() == tv[1].rank());
	}
	table combination_table = getCombination(tv);

	for (table::iterator it = combination_table.begin(); it != combination_table.end(); it++) {
		// *it 為 index 的 vector，例如 [3, 2, 1] 或 [7, 6, 5, 4]
		// 用一個 for 按 *it 去掉 tv，存到 tv_minus_combination
		vector<MJtile> tv_minus_combination = tv;
		for (vector<int>::iterator it2 = (*it).begin(); (it2) != (*it).end(); it2++) {
			tv_minus_combination.erase(tv_minus_combination.begin() + (*it2));
		}

		if (hu_recursion(tv_minus_combination)) return true;
	}

	return false;
}


bool MJhand::canhu(const MJtile& t) {
	// Eliminate flowers
	vector<MJtile> tv(_tiles + _faceup_len, _tiles + _total_len);
	tv.push_back(t);
	sort(tv.begin(), tv.end(), MJcompare);

	int flower_ind = -1;
	for (int i = 0; i < tv.size(); i++) {
		if (tv[i].flower() != 0) {
			flower_ind = i;
			break;
		}
	}
	if (flower_ind != -1) {

		int tmp = tv.size();
		for (int i = flower_ind; i < tmp; i++) {
			tv.pop_back();
			}
		}
	return hu_recursion(tv);
	
}


// ====== Actions ============
/*對抽到的t進行調整*/
void MJhand::arrange() {
	sort(begin(_tiles) + _faceup_len, end(_tiles) - (30 - _total_len), MJcompare);
}

void MJhand::draw(MJcollection& mjcol) {

	_stage = 1;
	_tiles[_total_len] = mjcol.drawfronttile();
	if (_tiles[_total_len].flower() != 0)
		applique(_total_len, mjcol);
}


void MJhand::draw(MJtile& t) {
	// do not input a flower!
	_stage = 1;
	_tiles[_total_len] = t;
}

MJtile MJhand::play(int index) {

	if (index >= _total_len + _stage - _faceup_len + 1 || index < 1) {
		return MJtile(0);
	}
	// play the selected index tile(count from _faceup_id)
	MJtile playout = _tiles[_faceup_len + index - 1];

	// and forward all the tiles in the array one place front
	for (int i = _faceup_len + index; i < _total_len + _stage; i++) {
		_tiles[i - 1] = _tiles[i];
	}
	_tiles[_total_len + _stage - 1] = MJtile();

	// change state to 0 (normal stage)
	_stage = 0;
	arrange();
	return playout;
}
void MJhand::faceup(int index) {
	swap(_tiles[_faceup_len], _tiles[_faceup_len + index - 1]);

	_faceup_len++;
	arrange();
}
void MJhand::applique(int index, MJcollection& mjcol) {
	
	if (_tiles[index].flower() == 0)
		return;
	else {
		swap(_tiles[_faceup_len], _tiles[index]);
		_faceup_len++;
		// draw tile from mjcollection
		_tiles[_total_len + _stage] = mjcol.drawbacktile();
		_total_len++;
		if (_tiles[_total_len + _stage - 1].flower() != 0)
			applique(_total_len + _stage - 1, mjcol);
	}
	arrange();
	return;
}

void MJhand::initial(MJcollection& mjcol) {

	arrange();
	for (int i = 0; i < _total_len + _stage; i++) {
		// if this tile is flower
		if (_tiles[i].flower() != 0)
			applique(i, mjcol);
	}
	arrange();
}

//別人能看到的牌
//如果能被看到noreveal就會是true
//faceup_len的牌都是能被看到的所以是true
void MJhand::showhandtoothers() {
	for (int i = 0; i < _total_len; i++) {
        if (i >= _faceup_len) {
            noreveal[i] = true;
        } else {
            noreveal[i] = false;    
        }
    }
    MJtile t[30];
    for (int i = 0; i < _total_len; i++) {
        if (!noreveal[i]) {
            t[i] = _tiles[i];
        } else {
            t[i] = MJtile(0);    
        }
    }
    MJhand print(t, _total_len);
    cout << print;
    return;
}


void MJhand::eat(const MJtile& t, int method) {
	// caneat 的
	// case 0: cout << "false";
	// case 1: cout << "(001)";
	// case 2: cout << "(010)";
	// case 3: cout << "(001) (010)";
	// case 4: cout << "(100)";
	// case 5: cout << "(001) (100)";
	// case 6: cout << "(010) (100)";
	// case 7: cout << "(001) (010) (100)";
	if (caneat(t) == 0) return;
	if (caneat(t) == 1) goto method1;
	if (caneat(t) == 2) goto method2;
	if (caneat(t) == 4) goto method3;
	if (caneat(t) == 3) {
		if (method == 1) goto method1;
		if (method == 2) goto method2;
		if (method == 3) return;
	}
	if (caneat(t) == 5) {
		if (method == 1) goto method1;
		if (method == 2) return;
		if (method == 3) goto method3;
	}
	if (caneat(t) == 6) {
		if (method == 1) return;
		if (method == 2) goto method2;
		if (method == 3) goto method3;
	}
	if (caneat(t) == 7) {
		if (method == 1) goto method1;
		if (method == 2) goto method2;
		if (method == 3) goto method3;
	}
	//傳進來的牌是t.suit()和t.rank()
method1: // (001)
	// 找 suit==t.suit() and rank==t.rank()-2, swap to _faceup_len
	for (int i = _faceup_len; i < _total_len; i++) {
		if (_tiles[i].suit() == t.suit() && _tiles[i].rank() == t.rank() - 2) {
			swap(_tiles[i], _tiles[_faceup_len]);
			break;
		}
	}
	// 找 suit==t.suit() and rank==t.rank()-1, swap to _faceup_len+2
	for (int i = _faceup_len; i < _total_len; i++) {
		if (_tiles[i].suit() == t.suit() && _tiles[i].rank() == t.rank() - 1) {
			swap(_tiles[i], _tiles[_faceup_len + 2]);
			break;
		}
	}
	// _faceup_len+1 to total_len, t to _faceup_len+1
	_tiles[_total_len] = _tiles[_faceup_len + 1];
	_tiles[_faceup_len + 1] = t;
	_faceup_len += 3;

	_total_len++;
	arrange();
	_total_len--;
	_stage = 1;
	return;

method2: //(010) 
	// 找 suit==t.suit() and rank==t.rank()-1, swap to _faceup_len
	for (int i = _faceup_len; i < _total_len; i++) {
		if (_tiles[i].suit() == t.suit() && _tiles[i].rank() == t.rank() - 1) {
			swap(_tiles[i], _tiles[_faceup_len]);
			break;
		}
	}
	// 找 suit==t.suit() and rank==t.rank()+1, swap to _faceup_len+2
	for (int i = _faceup_len; i < _total_len; i++) {
		if (_tiles[i].suit() == t.suit() && _tiles[i].rank() == t.rank() + 1) {
			swap(_tiles[i], _tiles[_faceup_len + 2]);
			break;
		}
	}
	// _faceup_len+1 to total_len, t to _faceup_len+1
	_tiles[_total_len] = _tiles[_faceup_len + 1];
	_tiles[_faceup_len + 1] = t;
	_faceup_len += 3;

	_total_len++;
	arrange();
	_total_len--;
	_stage = 1;
	return;

method3: // (100)
	// 找 suit==t.suit() and rank==t.rank()-1, swap to _faceup_len
	for (int i = _faceup_len; i < _total_len; i++) {
		if (_tiles[i].suit() == t.suit() && _tiles[i].rank() == t.rank() + 1) {
			swap(_tiles[i], _tiles[_faceup_len]);
			break;
		}
	}
	// 找 suit==t.suit() and rank==t.rank()+1, swap to _faceup_len+2
	for (int i = _faceup_len; i < _total_len; i++) {
		if (_tiles[i].suit() == t.suit() && _tiles[i].rank() == t.rank() + 2) {
			swap(_tiles[i], _tiles[_faceup_len + 2]);
			break;
		}
	}
	// _faceup_len+1 to total_len, t to _faceup_len+1
	_tiles[_total_len] = _tiles[_faceup_len + 1];
	_tiles[_faceup_len + 1] = t;
	_faceup_len += 3;


	_total_len++;
	arrange();
	_total_len--;
	_stage = 1;
	return;
}


void MJhand::pong(const MJtile& t) {
	// 驗證是可以碰的，以免不能碰又要使用這個函式
	if (canpong(t)) {
		for (int i = _faceup_len; i < _total_len; i++) {
			// 如果這張手中的牌 == 傳進來的 t
			if (_tiles[i].fromsuitrank(t.suit(), t.rank())) {
				// 這張跟 _tiles[_faceup_len] 換
				swap(_tiles[i], _tiles[_faceup_len]);
				// 下一張跟 _tiles[_faceup_len + 1] 換
				swap(_tiles[i + 1], _tiles[_faceup_len + 1]);
				// _tiles[_faceup_len + 2] 移到最後
				_tiles[_total_len] = _tiles[_faceup_len + 2];
				// 原來的位置放新進來的 t
				_tiles[_faceup_len + 2] = t;
				swap(_tiles[_faceup_len + 1], _tiles[_faceup_len + 2]);
				_faceup_len += 3;

				_total_len++;
				arrange();
				_total_len--;
				_stage = 1;
				return;
			}
		}
	} else {
		return;
	}
}


void MJhand::minggone(const MJtile& t, MJcollection& mjcol) {
	if (canminggone(t)) {
		// find the tile with the same suit and rank
		for (int i = _faceup_len; i < _total_len - 2; i++) {
			if (_tiles[i].fromsuitrank(t.suit(), t.rank()) && _tiles[i + 1].fromsuitrank(t.suit(), t.rank()) &&
			        _tiles[i + 2].fromsuitrank(t.suit(), t.rank())) {
				// 先把 i 到 i+2 hold 住
				MJtile hold[4];
				hold[0] = _tiles[i]; hold[1] = t;
				hold[2] = _tiles[i + 1]; hold[3] = _tiles[i + 2];

				// i+3 以後到 total_len -1 都要往後推一格
				// faceup_len 到 i-1 都要往後推 4 格
				// hold 填入 faceup_len 到 faceup_len+3
				for (int j = _total_len; j > i + 3; j--) {
					_tiles[j] = _tiles[j - 1];
				}
				for (int j = i + 3; j > _faceup_len; j--) {
					_tiles[j] = _tiles[j - 4];
				}
				_tiles[_faceup_len] = hold[0];
				_tiles[_faceup_len + 1] = hold[1];
				_tiles[_faceup_len + 2] = hold[2];
				_tiles[_faceup_len + 3] = hold[3];

				_faceup_len += 4;
				_total_len++;
				arrange();

				
				_stage = 1;
				_tiles[_total_len] = mjcol.drawbacktile();
				if (_tiles[_total_len].flower() != 0) {
					applique(_total_len, mjcol);
				}

				break;
			}
		}
	}
}


void MJhand::angone(int index, MJcollection& mjcol) {
	MJtile t = _tiles[_faceup_len + index - 1];
	if (canangone(t)) {
		for (int i = _faceup_len; i < _total_len - 2; i++) {
			if (_tiles[i].fromsuitrank(t.suit(), t.rank()) && _tiles[i + 1].fromsuitrank(t.suit(), t.rank()) &&
			        _tiles[i + 2].fromsuitrank(t.suit(), t.rank())) {
				// turn the four same tiles in your hand face up
				// i swap to faceup_len
				swap(_tiles[i], _tiles[_faceup_len]);
				// i+1 swap to faceup_len+2
				swap(_tiles[i + 1], _tiles[_faceup_len + 2]);
				// i+2 swap to faceup_len+3
				swap(_tiles[i + 2], _tiles[_faceup_len + 3]);
				// faceup_len+1 swap to _faceup_len+index-1
				swap(_tiles[_faceup_len + index - 1], _tiles[_faceup_len + 1]);
				_faceup_len += 4;
				_total_len++;
				arrange();

				// draw
				_tiles[_total_len] = mjcol.drawbacktile();
				if (_tiles[_total_len].flower() != 0) {
					applique(_total_len, mjcol);
				}
				_stage = 1;
				break;
			}
		}
        for (int i = 1; i < 5; i++) {
            noreveal[_faceup_len - i] = true; //蓋牌
        }
	}
	return;
}

void MJhand::bugone(int index, MJcollection& mjcol) {
	// _stage = 0;
	MJtile t = _tiles[_faceup_len + index - 1];
	if (canbugone(t)) {

		for (int i = 0; i < _faceup_len - 1; i++) {

			if (_tiles[i].rank() == t.rank() && _tiles[i + 1].rank() == t.rank() && _tiles[i + 2].rank() == t.rank() && _tiles[i].suit() == t.suit() && _tiles[i + 1].suit() == t.suit() && _tiles[i + 2].suit() == t.suit()) {
				_tiles[ i + 3] = t;
				swap(_tiles[_faceup_len + 1], _tiles[_faceup_len + 2]);
				_faceup_len += 1;
				_total_len += 1;
				

				// draw
				_tiles[_total_len] = mjcol.drawbacktile();
				if (_tiles[_total_len].flower() != 0) {
					applique(_total_len, mjcol);
				}
				_stage = 1;
				break;
			}
		}
	}
}

void MJhand::huother(const MJtile& t) {
	if (canhu(t)) {
		_tiles[_total_len] = t;
		_stage = 2;
		_total_len++;
		arrange();
	}
}

void MJhand::huown() {
	MJtile t = _tiles[_total_len];
	if (canhu(t)) {
		_stage = 2;
		_total_len++;
		arrange();
	}
}

MJhand* MJhand::gethuhand() {
	return this;
}
// ======== Print out Methods ===================
MJtile& MJhand::operator [] (int index) {
	// 用 <= 是因為少數函數如 decidePlay 會用到 index == _total_len
	if (index <= _total_len && index >= 0) {
		return _tiles[index];
	} else {
		cout << "Total len is " << _total_len << ". Try to get index " << index << endl;
		assert(false);
	}
}

//印出整個牌
//大概傳的是這樣:
// __
//| N|
//|XX|
// ￣
ostream& operator << (ostream& os, const MJhand& h){

	//part 1 " __ "
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";
		os<<" __ ";
	}
	os<<endl;

	//part 2 "| N|"
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<"|";
		if(h._tiles[i].suit()==4){
			if(h._tiles[i].flower())
				os<<" "<<h._tiles[i].flower();
			else
				os<<"  ";
		}
		else{
			switch(h._tiles[i].rank()){
				case 1:
					os<<"一";
					break;
				case 2:
					os<<"二";
					break;
				case 3:
					os<<"三";
					break;
				case 4:
					os<<"四";
					break;
				case 5:
					os<<"五";
					break;
				case 6:
					os<<"六";
					break;
				case 7:
					os<<"七";
					break;
				case 8:
					os<<"八";
					break;
				case 9:
					os<<"九";
					break;
				default:
					os<<"Ｘ";
                    //assert(false);
			}
		}
		os<<"|";
	}
	os<<endl;

	//part 3 "|XX|"
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<"|";
		if(h._tiles[i].suit()==4)
			switch(h._tiles[i].rank()){
				case 1:
					os<<"東";
					break;
				case 2:
					os<<"南";
					break;
				case 3:
					os<<"西";
					break;
				case 4:
					os<<"北";
					break;
				case 5:
					os<<"中";
					break;
				case 6:
					os<<"發";
					break;
				case 7:
					os<<"  ";
					break;
				case 8:
					switch(h._tiles[i].flower()){
						case 1:
							os<<"梅";
							break;
						case 2:
							os<<"蘭";
							break;
						case 3:
							os<<"竹";
							break;
						case 4:
							os<<"菊";
							break;
						default:
							os<<"Ｘ";
                            //assert(false);
					}
					break;
				case 9:
					switch(h._tiles[i].flower()){
						case 1:
							os<<"春";
							break;
						case 2:
							os<<"夏";
							break;
						case 3:
							os<<"秋";
							break;
						case 4:
							os<<"冬";
							break;
						default:
							os<<"Ｘ";
                            //assert(false);
					}
					break;
				default:
					os<<"Ｘ";
                    //assert(false);
			}
		else{
			switch(h._tiles[i].suit()){
				case 1:
					os<<"筒";
					break;
				case 2:
					os<<"條";
					break;
				case 3:
					os<<"萬";
					break;
				default:
					os<<"Ｘ";
                    //assert(false);
			}
		}
		os<<"|";
	}
	os<<endl;

	//part 4 " -- "
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<" ￣ ";
	}
	os<<endl;
	return os;
}

