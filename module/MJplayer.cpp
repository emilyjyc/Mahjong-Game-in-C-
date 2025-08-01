
 //在314行附近我改了一下
 //要用原本的也行因為我找不到tiles_num在哪裡@@
#include <iostream>
#include <map>
#include <vector>
#include "MJplayer.h"
#include "MJhand.h"
#include "MJcollection.h"

using namespace std;

MJplayer::MJplayer() {
    // cout << "Call MJplayer constructor." << endl;
    _position = 0;
    _money = 0;
    _hand = MJhand();
    count_angone = 0;
    out = vector<vector<int> >(4, vector<int>(9, 0));

    // 1 的上家是 2，2 的上家是 3，3 的上家是 4，4 的上家是 1
    previousPlayer[1] = 2;
    previousPlayer[2] = 3;
    previousPlayer[3] = 4;
    previousPlayer[4] = 1;
}

//玩家的贏錢數量
MJplayer::MJplayer(int money) {
    _position = 0;
    _money = money;

    _hand = MJhand();
    count_angone = 0;
    out = vector<vector<int> >(4, vector<int>(9, 0));

    // 1 的上家是 2，2 的上家是 3，3 的上家是 4，4 的上家是 1
    previousPlayer[1] = 2;
    previousPlayer[2] = 3;
    previousPlayer[3] = 4;
    previousPlayer[4] = 1;
}

//設置玩家
MJplayer::MJplayer(int pos, int money, MJtile* t, int n) {
    // cout << "Call MJplayer constructor with input value." << endl;
    _position = pos;
    _money = money;
    _hand = MJhand(t, n);
    count_angone = 0;
    out = vector<vector<int> >(4, vector<int>(9, 0));

    // 1 的上家是 2，2 的上家是 3，3 的上家是 4，4 的上家是 1
    previousPlayer[1] = 2;
    previousPlayer[2] = 3;
    previousPlayer[3] = 4;
    previousPlayer[4] = 1;
}


MJplayer::~MJplayer() {
}


void MJplayer::Set_Pos(int position) {
    _position = position;
    return;
}


int MJplayer::Get_Pos() const {
    return _position;
}


void MJplayer::Set_Mon(int money) {
    _money = money;
    return;
}


int MJplayer::Get_Mon() const {
    return _money;
}


int MJplayer::faceup_len() const {
    return _hand.faceup_len();
}


void MJplayer::Set_Hand(MJhand& input) {
    _hand = input;
    return;
}

void MJplayer::Set_Hand(MJtile* mjtiles, int number) {
    _hand = MJhand(mjtiles, number);
    return;
}


void MJplayer::Print_Hand() const {
    // public showing
    cout << _hand;
    return;
}

// 只有faceup能給其他玩家看
void MJplayer::showhandtoothers(void) {
    
    _hand.showhandtoothers();
    return;
}

//初始化
void MJplayer::initiate(MJcollection& mjcol) {
    _hand.initial(mjcol);
    return;
}


void MJplayer::draw(MJcollection& mjcol) {
    _hand.draw(mjcol);
    return;
}


MJtile MJplayer::play(int index) {
    return _hand.play(index);
}



void MJplayer::clear_info(void) {
    out = vector<vector<int> >(4, vector<int>(9, 0));
}



// actiontype: nothing=0 eat=1 pong=2 minggone=3 angone=4 bugone=5 hu=7
void MJplayer::act(int type, int param, MJtile& t, MJcollection& mjcol) {
    int index = _hand.total_len() - _hand.faceup_len(); //還沒吃碰過, 可以出的牌
    switch (type) {
    case 7:
        if (param == 2)
            _hand.huother(t);
        if (param == 1)
            _hand.huown();
        break;
    case 1:
        _hand.eat(t, param);
        break;
    case 2:
        _hand.pong(t);
        break;
    case 3:
        _hand.minggone(t, mjcol);
        break;
    case 4:
        _hand.angone(index, mjcol);
        break;
    case 5:
        _hand.bugone(index, mjcol);
        break;
    default:
        return;
    }
    return;
}


void MJplayer::strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
    // 每輪都叫出一次，給玩家看的
    int method = 0;
    map<int, string> action_map;
    action_map[0] = "nothing";
    action_map[1] = "吃(eat)";
    action_map[2] = "碰(pong)";
    action_map[3] = "明槓(minggone)";
    action_map[5] = "補槓(bugone)";
    action_map[7] = "胡(hu)";
    action_map[8] = "出牌(play)";
    vector<bool> avail(9, false); // 指示可做的動作
    // if 現在出牌的人是上家, check if caneat
    avail[0] = true;
    if (previousPlayer[_position] == position) {
        //cout << "check if caneat: ";
        if (_hand.caneat(t)) {
            avail[1] = true;
            method = _hand.caneat(t);
            switch (method) { //吃的方法
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
        }
    }
    // check if canpong
    if (_hand.canpong(t)) {
        avail[2] = true;
    }

    // check if canminggone
    if (_hand.canminggone(t)) {
        avail[3] = true;
    }

    // check if canbugone
    if (_hand.canbugone(t)) {
        avail[5] = true;
    }

    // check if canhu
    if (_hand.canhu(t)) {
        avail[7] = true;
    }

    // your turn, you can play
    if (position == _position) {
        avail[8] = true;
        // draw stage, you must play
        if (_hand.stage() == 1) {
            avail[0] = false;
        }
    }

    // 向玩家提示可以有哪些策略
    fprintf(stdout, "You can do the following actions:\n");
    for (int i = 8; i >= 0; i--) {
        if (avail[i]) {
            fprintf(stdout, "%d: %s\n", i, action_map[i].c_str());
        }
    }
    int a = 0;
    int action_count = -1; // 0 if only_one_action else count of available actions

    for (int i = 0; i < avail.size(); i++) {
        if (avail[i]) action_count++;
    }

    cout << "Your Hands:" << endl;
    for (int i = 0; i < _hand.total_len() + _hand.faceup_len() + _hand.stage(); i++) {
        if (i <= _hand.faceup_len() || i > _hand.total_len()) {
            cout << "    ";
        } else if (i == _hand.total_len()) {
            cout << " " << i - _hand.faceup_len() << "    "; 
        } else if (i - _hand.faceup_len() < 10) {
            cout << "  " << i - _hand.faceup_len() << " ";    
        } else {
            cout << " " << i - _hand.faceup_len() << " ";    
        }
    }
    cout << endl;
    cout << _hand;

    if (action_count) {
        fprintf(stdout, "Please choose one of the actions above:\n");
        fscanf(stdin, "%d", &a);
    }

    // Default actions in case of invalid input
    if (a > 8 || a < 0 || !avail[a]) {
        if (position == _position) {
            fprintf(stdout, "Default: play a tile\n");
            a = 8;
        } else {
            fprintf(stdout, "Default: do nothing\n");
            a = 0;
        }
    }
	cout << "Press \033[1;93mEnter\033[0m ...\n";
    cin.sync();
    cin.get();
    if (a == 8) {
        actiontype = 8;
        int number = this->decidePlay();
        actionparameter = number;
    } else if (a == 1) {
        actiontype = a;
        actionparameter = method;
    } else {
        actiontype = a;
    }
    return;
}

/*輸入1-index的數字決定要丟出哪張牌*/
int MJplayer::decidePlay(void) {
    int pos = 0;
    cout << "Which tile do you want to play?" << endl;
    cout << "First: 1, Last: " << _hand.total_len() + _hand.stage() - _hand.faceup_len() << endl;
    cin >> pos;
    if (pos < 1 || pos > _hand.total_len() + _hand.stage()) {
        pos = _hand.total_len() + _hand.stage();
    }
    return pos;
}


void MJplayer::getinfo(int position, int type, MJtile* ts, int tiles_num) {
    // type: eat=1 pong=2 minggone=3 angone=4 bugone=5 applique=6
    // call after any type above
    int suit = ts->suit();
    int rank = ts->rank();
    if (type == 8) {
        // 顯示打出去的牌
        out[suit - 1][rank - 1] += 1;
    } else if (type == 1) {
        // someone eat
        if (tiles_num == 1) { // (001)
            out[suit - 1][rank - 2] += 1;
            out[suit - 1][rank - 3] += 1;
        }else if(tiles_num == 2){ // (010)
            out[suit - 1][rank] += 1;
            out[suit - 1][rank - 2] += 1;
        }else { // (100)新加的@@, 解決有些情況無法吃的條件不足問題
            out[suit - 1][rank + 2] += 1;
            out[suit - 1][rank + 3] += 1;
        }
        /*原本的：
           if (tiles_num == 1) { // (001)
            out[suit - 1][rank - 2] += 1;
            out[suit - 1][rank - 3] += 1;
        }else { // (010)
            out[suit - 1][rank] += 1;
            out[suit - 1][rank - 2] += 1;
        }*/
    } else if (type == 2) {
        // someone pong
        out[suit - 1][rank - 1] += 2;
    } else if (type == 3 ) {
        // someone minggone 
        out[suit - 1][rank - 1] += 3;
    } else if (type == 4) {
        // someone angone
        count_angone += 1;
    } else if (type == 5 ) {
        // someone bugone
        out[suit - 1][rank - 1] += 1;
    }
    previousTile = *ts;
    return;
}

bool MJplayer::is_human(void) {
    return true;
}

string MJplayer::className(void) {
    return "MJplayer";
}

string MJplayer::getFunctionOrder(void) {
    return "";
}

