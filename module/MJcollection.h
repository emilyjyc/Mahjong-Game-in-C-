#ifndef MJCOLLECTION
#define MJCOLLECTION

#include "MJtile.h"
#include <algorithm>
using namespace std;

class MJcollection {
public:
	MJcollection() {
		_frontind = 0;
		_backind = 143;
		for (int i = 0; i < _backind + 1; i++) {
			_t[i] = MJtile();
		}
	};

	MJcollection(MJtile* mjs) {
		_frontind = 0;
		_backind = 143;
		for (int i = 0; i < _backind + 1; i++) {
			_t[i].setfromId(mjs[i].getTileId());
		}
	};

	~MJcollection() { };

	int frontind() const { return _frontind; };
	int backind() const { return _backind; };
	int size() const {
		// 回傳剩下的張數
		return _backind - _frontind + 1;
	};
	bool checkduplicate() const {

		int temp[144];
		for (int i = 0; i < 144; i++) {
			temp[i] = 0;
		}
		for (int j = 0; j < 144; j++) {
			temp[_t[j].getTileId()]++;
		}
		for (int k = 0; k < 144; k++) {
			if (temp[k] > 1) return false;
		}
		return true;

	};
	MJtile drawfronttile() const {

		MJtile out = _t[_frontind];
		_frontind++;
		return out;
	};
	MJtile drawbacktile() const {

		MJtile out = _t[_backind];
		_backind--;
		return out;
	};

private:
	MJtile _t[144];
	mutable int _frontind;
	mutable int _backind;
};
#endif

