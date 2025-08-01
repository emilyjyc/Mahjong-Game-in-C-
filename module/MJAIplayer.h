
#ifndef MJAIPLAYER
#define MJAIPLAYER
#include <iostream>
#include "MJplayer.h"
using namespace std;

class MJGreedyAIplayer: public MJplayer {
public:
	MJGreedyAIplayer() : MJplayer() {
		
	}
	MJGreedyAIplayer(int money) : MJplayer(money) {
		
	}

	// 動作種類: nothing=0 eat（吃）=1 pong（碰）=2 minggone（明槓）=3 angone（暗槓）=4 bugone（補槓）=5 hu（胡）=7 play（出牌）=8
	// 動作參數: huown=1 huother=2 play=number
	virtual void strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
		// 上家出的牌才能吃
		if (previousPlayer[_position] == position) {
			if (_hand.caneat(t)) {
				// eat 為可以吃的方式
				int method = _hand.caneat(t);
				actiontype = 1;
				switch (method) {
				case 3:
					method = 1;
					break;
				case 5:
					method = 1;
					break;
				case 6:
					method = 2;
					break;
				case 7:
					method = 1;
					break;
				}
				actionparameter = method;

				return;
			}
		}

		if (position != _position) { // 別的玩家正在進行
			// 確認能不能胡
			if (_hand.canhu(t)) {
				actiontype = 7;
				actionparameter = 2;
				return;
			}
			// 確認能不能明槓
			if (_hand.canminggone(t)) {
				actiontype = 3;
				return;
			}
			// 確認能不能碰
			if (_hand.canpong(t)) {
				actiontype = 2;
				return;
			}
		} else {
			// 剛從牌底抽牌，決定要不要補槓、暗槓
			if (_hand.canhu(t)) {
				actiontype = 7;
				actionparameter = 1;
				return;
			}
			if (_hand.canbugone(t)) {
				actiontype = 5;
				return;
			}
			if (_hand.canangone(t)) {
				actiontype = 4;
				return;
			}
			// 以上動作皆沒有，出牌囉
			actiontype = 8;
			int index = this->decidePlay();
			int number = index - _hand.faceup_len() + 1;
			actionparameter = number;
		}
		return;
	};

	int singleSuit4(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理落單的東南西北中發白 *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: suit is 4." << endl;
			// 看第一張，如果下一張跟自己不一樣就回傳這張
			int i = _hand.faceup_len();
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			if (suit == 4 && !(_hand[i + 1].fromsuitrank(suit, rank))) return i;
			// 看最後一張，如果前一張跟自己不一樣就回傳這張
			i = _hand.total_len();
			suit = _hand[i].suit();
			rank = _hand[i].rank();
			if (suit == 4 && !(_hand[i - 1].fromsuitrank(suit, rank))) return i;
			// 看中間的牌，如果前一張跟自己不一樣且後一張跟自己不一樣才會回傳
			for (int i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				suit = _hand[i].suit();
				rank = _hand[i].rank();
				if (suit == 4) {
					bool previousTile = _hand[i - 1].fromsuitrank(suit, rank);
					bool nextTile = _hand[i + 1].fromsuitrank(suit, rank);
					if (!previousTile && !nextTile) return i;
				}
			}
		}
		return -1;
	}

	int singleRank19(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理落單的 1 和 9 *****
			// 檢查第一張牌，如果是 rank 1，則下一張 rank 要是 1 or 2 or 3
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: rank is 1 or 9." << endl;
			int i = _hand.faceup_len();
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			if ((suit == 1 || suit == 2 || suit == 3) && (rank == 1)) {
				if (!(_hand[i + 1].fromsuitrank(suit, 1)) && !(_hand[i + 1].fromsuitrank(suit, 2))
				        && !(_hand[i + 1].fromsuitrank(suit, 3))) return i;
			}
			// 檢查最後一張，如果是 rank 9，則前一張 rank 要是 7 or 8 or 9
			i = _hand.total_len();
			suit = _hand[i].suit();
			rank = _hand[i].rank();
			if ((suit == 1 || suit == 2 || suit == 3) && (rank == 9)) {
				if (!(_hand[i - 1].fromsuitrank(suit, 8)) && !(_hand[i - 1].fromsuitrank(suit, 9))
				        && !(_hand[i - 1].fromsuitrank(suit, 7))) return i;
			}
			// 檢查中間其他張
			for (i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				suit = _hand[i].suit();
				rank = _hand[i].rank();
				// 處理 rank 是 1，至少要前面是 1，或後面是 1 or 2 or 3
				if ((suit == 1 || suit == 2 || suit == 3) && (rank == 1)) {
					bool previousTile = _hand[i - 1].fromsuitrank(suit, 1);
					bool nextTile = _hand[i + 1].fromsuitrank(suit, 1) ||
					                _hand[i + 1].fromsuitrank(suit, 2) ||
					                _hand[i + 1].fromsuitrank(suit, 3);
					if (!previousTile && !nextTile) return i;
				}

				// 處理 rank 是 9，至少要前面是 7 or 8 or 9，或後面是 9
				if ((suit == 1 || suit == 2 || suit == 3) && (rank == 9)) {
					bool previousTile = _hand[i - 1].fromsuitrank(suit, 7) ||
					                    _hand[i - 1].fromsuitrank(suit, 8) ||
					                    _hand[i - 1].fromsuitrank(suit, 9);
					bool nextTile = _hand[i + 1].fromsuitrank(suit, 9);
					if (!previousTile && !nextTile) return i;
				}
			}
		}
		return -1;
	}

	int singleTile(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理其他落單的牌 *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: lonely tile." << endl;
			// 從 faceup_len + 1 到 total_len - 1 找落單的，即左右兩邊都沒有跟自己一樣或 rank 加減 1, 2 的
			// cout << "Next find the lonely tile." << endl;
			// 檢查第一張牌
			int i = _hand.faceup_len();
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			if ((suit == 1 || suit == 2 || suit == 3)) {
				if (!(_hand[i + 1].fromsuitrank(suit, rank)) &&
				        !(_hand[i + 1].fromsuitrank(suit, rank + 1)) &&
				        !(_hand[i + 1].fromsuitrank(suit, rank + 2))) return i;
			}
			// 檢查最後一張
			i = _hand.total_len();
			suit = _hand[i].suit();
			rank = _hand[i].rank();
			if ((suit == 1 || suit == 2 || suit == 3)) {
				if (!(_hand[i - 1].fromsuitrank(suit, rank)) &&
				        !(_hand[i - 1].fromsuitrank(suit, rank - 1)) &&
				        !(_hand[i - 1].fromsuitrank(suit, rank - 2))) return i;
			}
			// 檢查中間其他張
			for (int i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				int suit = _hand[i].suit();
				int rank = _hand[i].rank();
				if ((suit == 1 || suit == 2 || suit == 3)) {
					bool previousTile = _hand[i - 1].fromsuitrank(suit, rank - 2) ||
					                    _hand[i - 1].fromsuitrank(suit, rank - 1) ||
					                    _hand[i - 1].fromsuitrank(suit, rank);
					                    
					bool nextTile = _hand[i + 1].fromsuitrank(suit, rank) ||
					                _hand[i + 1].fromsuitrank(suit, rank + 1) ||
					                _hand[i + 1].fromsuitrank(suit, rank + 2);

					if (!previousTile && !nextTile) return i;
				}
			}
		}
		return -1;
	}

	int pairSuit4(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理只有兩個 suit 4 (Fa Fa) 這種狀況 *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: Fa Fa condition." << endl;
			// 先直接看最後兩張是不是這種狀況
			int i = _hand.total_len() - 1;
			if (_hand[i].suit() == 4 && _hand[i + 1] == 4) {
				if (_hand[i].rank() == _hand[i + 1].rank())
					return i;
			}
			// 從 faceup 開始一次看兩張，且第三張要不一樣
			for (i = _hand.faceup_len(); i < _hand.total_len() - 1; i++) {
				if (_hand[i].suit() == 4 && _hand[i + 1] == 4) {
					if (_hand[i].rank() == _hand[i + 1].rank())
						if (!_hand[i + 2].fromsuitrank(_hand[i].suit(), _hand[i].rank()))
							return i;
				}
			}
		}
		return -1;
	}

	int gapSuit123Rank19(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理只有 7W 9W 或 1W 3W ，應先丟 9W 或 1W *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 7W 9W or 1W 3W condition." << endl;
			// 先 1W 3W 這種
			// 看第一張
			int i = _hand.faceup_len();
			if (_hand[i].suit() != 4 && _hand[i].rank() == 1) {
				if (_hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 3) {
					return i;
				}
			}
			for (i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				if (_hand[i].suit() != 4 && _hand[i].rank() == 1) {
					// 前一張不能是同一張，後一張是 3W
					if (!_hand[i].fromsuitrank(_hand[i - 1].suit(), _hand[i - 1].rank())) {
						if (_hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 3) {
							return i;
						}
					}
				}
			}
			// 再 7W 9W 這種
			// 看最後一張
			i = _hand.total_len();
			if (_hand[i].suit() != 4 && _hand[i].rank() == 9) {
				if (_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == 7) {
					return i;
				}
			}
			// 從前面
			for (i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				if (_hand[i].suit() != 4 && _hand[i].rank() == 9) {
					// 後一張不能是同一張，前一張必須是 7W
					if (!_hand[i].fromsuitrank(_hand[i + 1].suit(), _hand[i + 1].rank())) {
						if (_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == 7) {
							return i;
						}
					}
				}
			}
		}
		return -1;
	}

	int gapSuit123(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理如果只有 6W 8W 應先丟，因為只能等 7W *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 6W 8W condition." << endl;
			// 先直接看最後兩張是不是這種狀況
			int i = _hand.total_len();
			if (_hand[i].suit() != 4 && _hand[i].suit() == _hand[i - 1].suit()) {
				// 不用寫最後一張是 1，有的話在前面就應該回傳了
				// 看最後一張 8W，前一張是 6W 就回傳，但 6W 前面有 6W 或 5W 則
				if (_hand[i - 1].rank() == _hand[i].rank() - 2) return i;
			}
			// 再看第一張
			i = _hand.faceup_len();
			if (_hand[i].suit() != 4 && _hand[i].suit() == _hand[i + 1].suit()) {
				if (_hand[i + 1].rank() == _hand[i].rank() + 2) return i;
			}

			// 再看中間其他張
			for (i = _hand.faceup_len(); i < _hand.total_len(); i++) {
				if (_hand[i].suit() != 4 && _hand[i].suit() == _hand[i + 1].suit()) {
					// 下一張如果是自己 +2
					// 再檢查自己的前面是否是自己或自己 -1，不是的話才回傳
					if (_hand[i + 1].rank() == _hand[i].rank() + 2) {
						if (_hand[i - 1].rank() != _hand[i].rank() && _hand[i - 1].rank() != _hand[i].rank() - 1)
							return i;
					}
				}
			}
		}
		return -1;
	}

	int pairSuit123(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理 6W 6W 這種，但要先確定是不是 6W 6W 6W *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 6W 6W condition." << endl;
			// 先看最後兩個
			int i = _hand.total_len();
			if (_hand[i].suit() != 4) {
				// 如果跟前一個一樣
				if (_hand[i].fromsuitrank(_hand[i - 1].suit(), _hand[i - 1].rank())) {
					// 還要跟前二個不一樣
					if (!_hand[i].fromsuitrank(_hand[i - 2].suit(), _hand[i - 2].rank())) {
						return i;
					}
				}
			}
			// 從第一個開始看
			for (i = _hand.faceup_len(); i < _hand.total_len() - 1; i++) {
				if (_hand[i].suit() != 4) {
					// 跟後一個一樣
					if (_hand[i].fromsuitrank(_hand[i + 1].suit(), _hand[i + 1].rank())) {
						// 跟後二個不一樣
						if (!_hand[i].fromsuitrank(_hand[i + 2].suit(), _hand[i + 2].rank())) {
							return i;
						}
					}
				}
			}
		}
		return -1;
	}

	int contiSuit123Rank19(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			bool condition[10];
			// ***** 然後才丟 8W 9W 或 1W 2W 這種，但要先確定不是 789 或 123 *****
			// 先 1W 2W
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 1W 2W or 8W 9W condition." << endl;
			// 看第一張
			int i = _hand.faceup_len();
			// 條件一：suit 不為 4 ，rank 是 1
			condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 1;
			// 條件二：第二張是 2W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 2;
			// 條件三：第三張不是 3W
			condition[3] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == 3);
			if (condition[1] && condition[2] && condition[3]) return i;

			// 看倒數第二張
			i = _hand.total_len() - 1;
			// 條件一：suit 不為 4 ，rank 是 1
			condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 1;
			// 條件二：下一張是 2W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 2;
			if (condition[1] && condition[2]) return i;

			// 看其他張
			for (i = _hand.faceup_len() + 1; i < _hand.total_len() - 1; i++) {
				// 條件一：suit 不為 4 ，rank 是 1
				condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 1;
				// 條件二：前一張跟自己不一樣
				condition[2] = !_hand[i].fromsuitrank(_hand[i - 1].suit(), _hand[i - 1].rank());
				// 條件三：第二張是 2W
				condition[3] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 2;
				// 條件四：第三張不是 3W
				condition[4] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == 3);
				if (condition[1] && condition[2] && condition[3] && condition[4]) return i;
			}
			// cout << "finish 1W 2W" << endl;

			// 再 8W 9W 這種
			// 看最後一張
			i = _hand.total_len();
			// 條件一：suit 不為 4 ，rank 是 9
			condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 9;
			// 條件二：倒數第二張是 8W
			condition[2] = _hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == 8;
			// 條件三：倒數第三張不是 7W
			condition[3] = !(_hand[i].suit() == _hand[i - 2].suit() && _hand[i - 2].rank() == 7);
			if (condition[1] && condition[2] && condition[3]) return i;

			// 看第二張
			i = _hand.faceup_len() + 1;
			// 條件一：suit 不為 4 ，rank 是 9
			condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 9;
			// 條件二：前一張是 8W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 8;
			if (condition[1] && condition[2]) return i;

			// 看其他張
			for (i = _hand.total_len() - 1; i > _hand.faceup_len() + 1; i--) {
				// 條件一：suit 不為 4 ，rank 是 1
				condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 9;
				// 條件二：後一張跟自己不一樣
				condition[2] = !_hand[i].fromsuitrank(_hand[i + 1].suit(), _hand[i + 1].rank());
				// 條件三：前一張是 8W
				condition[3] = _hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == 8;
				// 條件四：前二張不是 7W
				condition[4] = !(_hand[i].suit() == _hand[i - 2].suit() && _hand[i - 2].rank() == 7);
				if (condition[1] && condition[2] && condition[3] && condition[4]) return i;
			}
		}
		return -1;
	}

	int contiSuit123(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 4) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			bool condition[10];
			// ***** 處理 3W 4W 這種，但要先確定不是 234 或 345 *****

			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 3W 4W condition." << endl;
			// 先最後兩張
			int i = _hand.total_len() - 1;
			// 條件一：suit 不為 4
			condition[1] = !(_hand[i].suit() == 4);
			// 條件二：下一張是 4W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == _hand[i].rank() + 1;
			// 條件三：前一張不是 2W
			condition[3] = !(_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == _hand[i].rank() - 1);
			// 條件四：前一張不是 3W
			condition[4] = !(_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == _hand[i].rank());
			if (condition[1] && condition[2] && condition[3] && condition[4]) return i;

			// 處理第一張
			i = _hand.faceup_len();
			// 條件一：suit 不為 4
			condition[1] = !(_hand[i].suit() == 4);
			// 條件二：下一張是 4W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == _hand[i].rank() + 1;
			// 條件三：下二張不是 4W
			condition[3] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == _hand[i].rank() + 1);
			// 條件四：下二張不是 5W
			condition[4] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == _hand[i].rank() + 2);
			if (condition[1] && condition[2] && condition[3] && condition[4]) return i;

			// 處理其他張
			for (i = _hand.faceup_len() + 1; i < _hand.total_len() - 1; i++) {
				// 條件一：不是 suit 4
				condition[1] = !(_hand[i].suit() == 4);
				// 條件二：下一張是自己 4W
				condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == _hand[i].rank() + 1;
				// 條件三：前一張不是 2W
				condition[3] = !(_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == _hand[i].rank() - 1);
				// 條件四：前一張不是 3W
				condition[4] = !(_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == _hand[i].rank());
				// 條件五：下二張不是 4W
				condition[5] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == _hand[i].rank() + 1);
				// 條件六：下二張不是 5W
				condition[6] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == _hand[i].rank() + 2);
				if (condition[1] && condition[2] && condition[3] && condition[4] && condition[5] && condition[6]) return i;
			}
		}
		return -1;
	}

	virtual int decidePlay(void) {
		extern bool greedyAIPlayer_decidePlay_checkPoint;
		// 目前策略：
		// 以下策略請在手牌數量 >= 5 時做，目前 <5 仍無策略
		// 先找落單的東西北中發白 (singleSuit4, id = 1)
		// 再找 rank 是 1 或 9，如果落單先打，定義是非 1, 1... 或 1, 2... 或 1, 3... (singleRank19, id = 2)
		// 再從第一張開始找任何 rank 是落單的 (singleTile, id = 3)
		// 只有 Fa Fa 應先丟，因為只能等 Fa，但應先確定手牌是不是 Fa Fa Fa 以免丟掉 (pairSuit4, id = 4)
		// 如果只有 7W 9W 或 1W 3W 先丟 9W 或 1W (gapSuit123Rank19, id = 5)
		// 如果只有 6W 8W 應先丟，因為只能等 7W (gapSuit123, id = 6)
		// 然後丟 6W 6W 這種，但要先確定是不是 6W 6W 6W (pairSuit123, id = 7)
		// 然後才丟 8W 9W 或 1W 2W 這種，但要先確定不是 789 或 123 (contiSuit123Rank19, id = 8)
		// 然後 3W 4W 這種，但要先確定不是 234 或 345 (contiSuit123, id = 9)
		// 都不是前述狀況的話打第一張牌

		// 這時手中應該會比 total len 多一張牌，所以 arrange 時不會排到最後這張
		_hand.set_total_len(_hand.total_len() + 1);
		_hand.arrange();
		_hand.set_total_len(_hand.total_len() - 1);
		int i = -1;
		int suit, rank;
		if (_hand.total_len() - _hand.faceup_len() == 1) {
			int t1suit = _hand[_hand.faceup_len()].suit();
			int t1rank = _hand[_hand.faceup_len()].rank();
			int t2suit = _hand[_hand.faceup_len() + 1].suit();
			int t2rank = _hand[_hand.faceup_len() + 1].rank();
			int t1out = out[t1suit - 1][t1rank - 1];
			int t2out = out[t2suit - 1][t2rank - 1];
			if (t1out == 4) return _hand.faceup_len();
			if (t2out == 4) return _hand.faceup_len() + 1;
			if (t1out > t2out) return _hand.faceup_len();
			if (t1out < t2out) return _hand.faceup_len() + 1;
		}

		for (int j = 0; j < functionOrder.length(); j++) {
			char functionToCall = functionOrder.at(j);
			
			if (functionToCall == '1') i = singleSuit4();
			if (functionToCall == '2') i = singleRank19();
			if (functionToCall == '3') i = singleTile();
			if (functionToCall == '4') i = pairSuit4();
			if (functionToCall == '5') i = gapSuit123Rank19();
			if (functionToCall == '6') i = gapSuit123();
			if (functionToCall == '7') i = pairSuit123();
			if (functionToCall == '8') i = contiSuit123Rank19();
			if (functionToCall == '9') i = contiSuit123();
			// cout << i << endl;
			if (i != -1) return i;
		}

		if (greedyAIPlayer_decidePlay_checkPoint) cout << "Not the above method. Return first tile." << endl;
		// 沒其他判斷方式了，就 return 第一張牌吧
		return _hand.faceup_len();
	}

	virtual bool is_human(void) {
		return false;
	}

	virtual string className(void) {
		return "MJGreedyAIplayer";
	}

	virtual string getFunctionOrder(void) {
		// cout << functionOrder << endl;
		return functionOrder;
	}


private:
	string functionOrder = "123456789";
};



class MJCustomAIplayer: public MJGreedyAIplayer {


public:
	MJCustomAIplayer() : MJGreedyAIplayer() {
		
	}
	MJCustomAIplayer(int money) : MJGreedyAIplayer(money) {
		
	}

	bool defensive = false; // 如果開局牌太爛就優先跟打牌
	void initiate(MJcollection& mjcol) {
		_hand.initial(mjcol);
		int count_lonely = 0;

		int i = _hand.faceup_len();
		int suit = _hand[i].suit();
		int rank = _hand[i].rank();
		if (suit != 4) {
			if (!(_hand[i + 1].fromsuitrank(suit, rank)) &&
			        !(_hand[i + 1].fromsuitrank(suit, rank + 1)) &&
			        !(_hand[i + 1].fromsuitrank(suit, rank + 2))) count_lonely++;
		} else {
			if (!_hand[i + 1].fromsuitrank(suit, rank)) { count_lonely++; }
		}
		for (int i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
			suit = _hand[i].suit();
			rank = _hand[i].rank();
			if (suit != 4) {
				bool previousTile = _hand[i - 1].fromsuitrank(suit, rank - 2) ||
				                    _hand[i - 1].fromsuitrank(suit, rank - 1) ||
				                    _hand[i - 1].fromsuitrank(suit, rank);
				bool nextTile = _hand[i + 1].fromsuitrank(suit, rank) ||
				                _hand[i + 1].fromsuitrank(suit, rank + 1) ||
				                _hand[i + 1].fromsuitrank(suit, rank + 2);
				if (!previousTile && !nextTile) count_lonely++;
			} else { // suit == 4
				bool previousTile = _hand[i - 1].fromsuitrank(suit, rank);
				bool nextTile = _hand[i + 1].fromsuitrank(suit, rank);
				if (!previousTile && !nextTile) count_lonely++;
			}
		}
		if (count_lonely > 7) defensive = true;;
		return;
	}

	void strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
		vector<bool> condition(4, false);
		vector<bool> avail(9, false);
		avail = this->get_avail_actions(position, t);

		// type: eat（吃）=1 pong（碰）=2 minggone（明槓）=3 angone（暗槓）=4 bugone（補槓）=5 applique（補花）=6
		int s = t.suit();
		int r = t.rank();
		// hu
		if (avail[7]) { actiontype = 7; return; }
		// 明槓＆碰
		if (avail[3]) {
			// 可以明槓就可以碰
			for (int i = _hand.faceup_len(); i < _hand.total_len() - 1; i++) {
				condition[0] = (_hand[i].fromsuitrank(s, r - 2) && _hand[i + 1].fromsuitrank(s, r - 2));
				condition[1] = (_hand[i].fromsuitrank(s, r - 1) && _hand[i + 1].fromsuitrank(s, r - 1));
				condition[2] = (_hand[i].fromsuitrank(s, r + 1) && _hand[i + 1].fromsuitrank(s, r + 1));
				condition[3] = (_hand[i].fromsuitrank(s, r + 2) && _hand[i + 1].fromsuitrank(s, r + 2));
				if (condition[0] || condition[1] || condition[2] || condition[3]) {
					actiontype = 3;
					return;
				}
			}
			actiontype = 2;
			return;

		} else if (avail[2]) {
			// 只能碰
			actiontype = 2;
			return;
		}
		// 吃
		int method = 0;
		if (avail[1]) {
			actiontype = 1;
			method = _hand.caneat(t);
			switch (method) {
			case 3:
				actionparameter = 1;
				break;
			case 5:
				actionparameter = 1;
				break;
			case 6:
				actionparameter = 2;
				break;
			case 7:
				actionparameter = 1;
				break;
			}
		}
		// 出牌
		if (avail[8]) {
			actiontype = 8;
			int index = this->decidePlay();
			actionparameter = index - _hand.faceup_len() + 1;
			return;
		}

		return;
	};


	int decidePlay(void) {

		_hand.set_total_len(_hand.total_len() + 1);
		_hand.arrange();
		_hand.set_total_len(_hand.total_len() - 1);

		// 記錄每張手牌在場上已經出現的次數
		vector<int> hand_occur(30, 0); 
		for (int i = _hand.faceup_len(); i < _hand.total_len() + 1; i++) {
			hand_occur[i] = out[_hand[i].suit() - 1][_hand[i].rank() - 1];
		}
		// 記錄手牌中哪張出現最多次
		int max = 0;
		int max_tile = _hand.faceup_len();
		for (int i = _hand.faceup_len(); i < _hand.total_len() + 1; i++) {
			if (hand_occur[i] > max) {
				max = hand_occur[i];
				max_tile = i;
			}
		}

		if (defensive) {
			// 打前一張一樣的牌>打出現過最多次的牌
			for (int i = _hand.faceup_len(); i < _hand.total_len() + 1; i++) {
				if (_hand[i].fromsuitrank(previousTile.suit(), previousTile.rank())) {
					return i;
				}
			}
			return max_tile;
		}

		//大部分應該都跟Greedy策略類似
		functionOrder = "123456789";
		int i = -1;
		for (int j = 0; j < functionOrder.length(); j++) {
			char functionToCall = functionOrder.at(j);
			
			if (functionToCall == '1') i = singleSuit4();
			if (functionToCall == '2') i = singleRank19();
			if (functionToCall == '3') i = singleTile();
			if (functionToCall == '4') i = pairSuit4();
			if (functionToCall == '5') i = gapSuit123Rank19();
			if (functionToCall == '6') i = gapSuit123();
			if (functionToCall == '7') i = pairSuit123();
			if (functionToCall == '8') i = contiSuit123Rank19();
			if (functionToCall == '9') i = contiSuit123();
			
			if (i != -1) return i;
		}

		return max_tile;
	}


	vector<bool> get_avail_actions(int position, MJtile t) {
		vector<bool> avail(9, false);
		// 如果現在出牌的人是上家, 確認能不能吃牌
		avail[0] = true;
		if (previousPlayer[_position] == position) {
		// 確認能不能吃
			if (_hand.caneat(t)) {
				avail[1] = true;
			}
		}
		// 確認能不能碰
		if (_hand.canpong(t)) {
			avail[2] = true;
		}

		// 確認能不能明槓
		if (_hand.canminggone(t)) {
			avail[3] = true;
		}

		// 確認能不能補槓
		if (_hand.canbugone(t)) {
			avail[5] = true;
		}

		// 確認能不能胡
		if (_hand.canhu(t)) {
			avail[7] = true;
		}

		// 換到自己，能出牌
		if (position == _position) {
			avail[8] = true;

			if (_hand.stage() == 1) {
				avail[0] = false;
			}
		}
		return avail;
	};


	bool is_human(void) {
		return false;
	}

	virtual string className(void) {
		return "MJCustomAIplayer";
	}

private:
	string functionOrder;

};

#endif

