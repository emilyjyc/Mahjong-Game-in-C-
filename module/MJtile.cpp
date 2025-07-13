
#include<iostream>
#include "MJtile.h"
/*此程式編排麻將的方式*/
/*每個麻將代表的編號都在投影片的image中*/
using std::endl;
using std::cout;
using std::string;

MJtile::MJtile(){
    _tileId = 0;
    _suit = 0;
    _rank = 0;
    _flowernum = 0;
}//全設為0, 代表手中無牌

MJtile::MJtile(int tileId){
    _tileId = tileId;

    int suit = tileId % 16;
    if(suit==0){
        _suit = 4; 
    } else if(suit <= 4){
        _suit = 1;
    } else if(suit <= 8){
        _suit = 2;
    } else if(suit <= 12){
        _suit = 3;
    } else{
        _suit = 4;
    }

    if(tileId % 16 == 0){
        _rank = tileId / 16;
    } else{
        _rank = tileId / 16 + 1;
    }
    _flowernum = 0;

    if(tileId == 125 || tileId == 141)
        _flowernum = 1;
    if(tileId == 126 || tileId == 142)
        _flowernum = 2;
    if(tileId == 127 || tileId == 143)
        _flowernum = 3;
    if(tileId == 128 || tileId == 144)
        _flowernum = 4;

} //對傳入id進行編碼的處理, 輸出值為麻將物件中所有private的東西

MJtile::~MJtile() {}

void MJtile::setfromId(int tileId){
    _tileId = tileId;

    int suit = tileId % 16;
    if(suit==0){
        _suit = 4; 
    } else if(suit <= 4){
        _suit = 1; //筒牌
    } else if(suit <= 8){
        _suit = 2; //條牌
    } else if(suit <= 12){
        _suit = 3; //萬牌
    } else{
        _suit = 4; //風牌花牌
    }

    if(tileId % 16 == 0){
        _rank = tileId / 16;
    } else{
        _rank = tileId / 16 + 1; 
    }

    _flowernum = 0; //筒條萬風的花色都是0

    if(tileId == 125 || tileId == 141)
        _flowernum = 1; //梅
    if(tileId == 126 || tileId == 142)
        _flowernum = 2; //蘭
    if(tileId == 127 || tileId == 143)
        _flowernum = 3; //竹
    if(tileId == 128 || tileId == 144)
        _flowernum = 4; //菊
} //函式, 做處理用

int MJtile::suit() const{
    return _suit;
}
/*get suit, 筒子=1, 條子=2, 萬子=3, 風牌花牌=4 */
int MJtile::rank() const{
    return _rank;
}
/*get rank, 1~9*/
int MJtile::flower() const{
    return _flowernum;
}
/*get flower number, if not flower return 0*/
int MJtile::getTileId() const{
    return _tileId;
}
/*get tile id*/
bool MJtile::operator == (const MJtile& mj2) const{
    return (_suit==mj2._suit && _rank==mj2._rank);
}
/*compare same kind of tile, don't need to worry of flower tile*/
bool MJtile::operator != (const MJtile& mj2) const{
    return !(_suit==mj2._suit && _rank==mj2._rank);
}
/*compare not the same kind of tile, don't need to worry of flower tile*/
bool MJtile::fromsuitrank(int suit, int rank) const{
    return (_suit == suit && _rank == rank);
}
/*若是完全一樣的牌則回傳真值*/
//大多用於比較牌池與牌堆中的牌

/*一敦麻將的UI*/
ostream& operator << (ostream&, const MJtile& mj){
    cout << " __ " << endl; //麻將的蓋子
    if(mj._suit==4){
        if(mj._rank==1){
            cout << "|  |" << endl;
            cout << "|東|";
        }
        else if(mj._rank==2){
            cout << "|  |" << endl;
            cout << "|南|";
        }
        else if(mj._rank==3){
            cout << "|  |" << endl;
            cout << "|西|";
        }
        else if(mj._rank==4){
            cout << "|  |" << endl;
            cout << "|北|";
        }
        else if(mj._rank==5){
            cout << "|  |" << endl;
            cout << "|中|"; 
        }
        else if(mj._rank==6){
            cout << "|  |" << endl;
            cout << "|發|";
        }
        else if(mj._rank==7){
            cout << "|  |" << endl;
            cout << "|  |";
        }
        else if(mj._tileId==125){
            cout << "| 1|" << endl;
            cout << "|梅|";
        }
        else if(mj._tileId==126){
            cout << "| 2|" << endl;
            cout << "|蘭|";
        }
        else if(mj._tileId==127){
            cout << "| 3|" << endl;
            cout << "|竹|";
        }
        else if(mj._tileId==128){
            cout << "| 4|" << endl;
            cout << "|菊|";
        }
        else if(mj._tileId==141){
            cout << "| 1|" << endl;
            cout << "|春|";
        }
        else if(mj._tileId==142){
            cout << "| 2|" << endl;
            cout << "|夏|";
        }
        else if(mj._tileId==143){
            cout << "| 3|" << endl;
            cout << "|秋|";
        }
        else if(mj._tileId==144){
            cout << "| 4|" << endl;
            cout << "|冬|";
        } else{
            assert(false);    
        }
        cout << endl;
    } else { // suit 1, 2, 3
        switch(mj._rank){
            case 1: cout << "|一|" << endl; break;
            case 2: cout << "|二|" << endl; break;
            case 3: cout << "|三|" << endl; break;
            case 4: cout << "|四|" << endl; break;
            case 5: cout << "|五|" << endl; break;
            case 6: cout << "|六|" << endl; break;
            case 7: cout << "|七|" << endl; break;
            case 8: cout << "|八|" << endl; break;
            case 9: cout << "|九|" << endl; break;
        }     
    }
    switch(mj._suit){
        case 1: cout << "|筒|" << endl;
                break;
        case 2: cout << "|條|" << endl;
                break;
        case 3: cout << "|萬|" << endl;
                break;
    }
    cout << " -- " << endl; //麻將的底
    return cout;
}


