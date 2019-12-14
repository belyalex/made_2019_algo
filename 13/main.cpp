#include <iostream>
#include <unordered_map>
#include <vector>
#include <initializer_list>
#include <cassert>
#include <algorithm>
#include <queue>

const uint64_t ul15=15;

const uint64_t mask1111[]={
        ul15<<60, ul15<<56, ul15<<52, ul15<<48,
        ul15<<44, ul15<<40, ul15<<36, ul15<<32,
        ul15<<28, ul15<<24, ul15<<20, ul15<<16,
        ul15<<12, ul15<< 8, ul15<< 4, ul15
};
const uint64_t mask0000[]={
        ~mask1111[ 0],~mask1111[ 1],~mask1111[ 2],~mask1111[ 3],
        ~mask1111[ 4],~mask1111[ 5],~mask1111[ 6],~mask1111[ 7],
        ~mask1111[ 8],~mask1111[ 9],~mask1111[10],~mask1111[11],
        ~mask1111[12],~mask1111[13],~mask1111[14],~mask1111[15]
};

class Position {
public:
    explicit Position(std::initializer_list<uint8_t> &&pos);
    explicit Position(const std::vector<uint8_t> &pos);


    std::vector<Position> GetSiblings() const;

    //void Move(uint8_t n);
    void Move(uint8_t n, uint8_t zero);

    void Trace() const;
    friend std::ostream &operator<<(std::ostream &out,const  Position& position);

    size_t GetPosition() const {return position;}
    char GetDirection() const {return direction;}
    bool operator==(const Position& p) const { return position==p.position; }
    bool operator!=(const Position& p) const { return position!=p.position; }

    bool operator<(const Position& p) const{ return distance>p.distance;}
private:
    uint64_t position;
    uint32_t distance;
    char     direction;
    uint8_t zero;
    uint8_t Zero() const;

    void CalcDistance();
};

bool BFS(const Position &position, std::unordered_map<Position, Position> &map);

std::string GetPath(const Position &position, const std::unordered_map<Position, Position> &map);

namespace std {
        template <>
        struct hash<Position> {
            size_t operator()(const Position& key) const {
                return key.GetPosition();
            }
        };
}

const Position FINISH_POSITION({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0});

Position::Position(std::initializer_list<uint8_t> &&pos) {
    assert(pos.size()==16);
    uint8_t pz=0;
    position = 0;
    for (auto p: pos) {
        if (p==0) {
            zero=pz;
        }
        pz++;
        position = (position << 4) | (p & 15);
    }
    CalcDistance();
}

Position::Position(const std::vector<uint8_t> &pos) {
    assert(pos.size()==16);
    uint8_t pz=0;
    position = 0;
    for (auto p: pos) {
        if (p==0) {
            zero=pz;
        }
        pz++;
        position = (position << 4) | (p & 15);
    }
    CalcDistance();
}

std::ostream &operator<<(std::ostream &out,const  Position& position) {
    uint64_t pos=position.position;
    for (int i=0;i<16;i++) {
        uint8_t p=(pos>>((15-i)*4))&15;
        out<<(uint)p<<" ";
    }
    out<<std::endl;
    return out;
}

void Position::Trace() const {
    uint64_t pos=position;
    std::cerr<<distance<<" "<<direction<<std::endl;
    for (int i=0;i<16;i++) {
        if (i%4==0) std::cerr<<"[";
        uint8_t p=(pos>>((15-i)*4))&15;
        std::cerr.width(2);
        std::cerr<<(uint)p;
        if (i%4==3) std::cerr<<"]\n";
        else std::cerr<<" ";
    }
    std::cerr<<std::endl;
}

std::vector<Position> Position::GetSiblings() const {
    std::vector<Position> result;

    auto zero=Zero();

    if (zero>3) {
        Position p=*this;
        p.Move(zero-4, zero);
        p.direction='D';
        result.push_back(p);
    }
    if (zero<12) {
        Position p=*this;
        p.Move(zero+4, zero);
        p.direction='U';
        result.push_back(p);
    }
    if (zero%4>0) {
        Position p=*this;
        p.Move(zero-1, zero);
        p.direction='R';
        result.push_back(p);
    }
    if (zero%4<3) {
        Position p=*this;
        p.Move(zero+1, zero);
        p.direction='L';
        result.push_back(p);
    }

    return result;
}
/*
void Position::Move(const uint8_t n) {
    assert(n<16);
    //uint64_t pos=position;
    uint64_t x=(position>>(15-n)*4) & 15;
    position = (position & mask0000[n]) | (x<<((15-Zero())*4));
    //zero=n;
}
*/
void Position::Move(uint8_t n, uint8_t zero) {
    assert(n<16);
    assert(zero<16);
    //uint64_t pos=position;
    uint64_t x=(position>>(15-n)*4) & 15;
    position = (position & mask0000[n]) | (x<<((15-zero)*4));
    this->zero=n;
    CalcDistance();
}

uint8_t Position::Zero() const {
    return zero;
    /*
    for (auto i=0;i<16;i++) {
        if ((position & mask0000[i])==position) {
            return i;
        }
    }
    return 255;
     */
}

void Position::CalcDistance() {
    distance=0;
    for (auto i=0;i<16;i++) {
        uint8_t n=(position>>((15-i)*4))&15;
        if (n!=0) {
            n--;
            distance+=abs(n/4-i/4)+abs(n%4-i%4);
        }
    }
}

bool isSolvable(const std::vector<uint8_t>& pos) {
    int p=0;
    int z=0;
    //подсчитаем кол-во перестановок
    for (auto i=0;i<pos.size();i++) {
        if (pos[i]!=0) {
            for (auto j = i + 1; j < pos.size(); j++) {
                if (pos[j]!=0) {
                    if (pos[i] > pos[j]) {
                        p++;
                    }
                }
            }
        }  else {
            z=i/4+1;
        }
    }
    p+=z;
    //чтобы позиция была разрешимой, кол-во перестановок должно быть чётным
    return (p%2==0);
}

bool BFS(const Position &start, std::unordered_map<Position, Position> &parents) {
    std::priority_queue<Position> positions_queue;
    positions_queue.push(start);
    while (!positions_queue.empty()) {
        Position current = positions_queue.top();
        //current.Trace();
        if (current==FINISH_POSITION) {
            return true;
        }

        positions_queue.pop();
        for (auto sibling : current.GetSiblings()) {
            if (parents.count(sibling)) {
                continue;
            }
            parents.insert(std::make_pair(sibling, current));
            /*
            if (sibling==FINISH_POSITION) {
                return true;
            }
             */
            positions_queue.push(sibling);
        }
    }
    return false;
}

std::string GetPath(const Position &start, const std::unordered_map<Position, Position> &parents) {
    std::string result;

    size_t c=0;
    auto it = parents.find(FINISH_POSITION);
    Position current=it->first;
    while (current != start) {
        Position parent = parents.at(current);

        //std::cerr<<c++<<std::endl;
        /*parent.Trace();*/
        result.push_back(current.GetDirection());
        ////    result.push_back(GetMoveSymbol(parent, current));!!!rr
        current = parent;
    }
    // Развернем result, так как собирали его с конца.
    std::reverse(result.begin(), result.end());
    return result;
}

std::string SolvePuzzle15(const Position& start) {
    // Запустим BFS, запоминая предков всех пройденных позиций.
    std::unordered_map<Position, Position> parents;
    if (!BFS(start, parents)) {
        return std::string();
    }
    return GetPath(start, parents);
}



int main() {
    //Position p({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0});
    assert(sizeof(Position)==16);

    std::vector<uint8_t> pos(16);
    for (int i=0;i<16;i++) {
        uint x=0;
        std::cin>>x;
        pos[i]=(uint8_t)x;
    }
    if (!isSolvable(pos)) {
        std::cout<<-1;
        return 0;
    }

    Position position(pos);

    //std::cerr<<"sizeof(Position)="<<sizeof(Position)<<std::endl;
    /*
    std::cerr<<"Position:"<<isSolvable(pos)<<std::endl;
    position.Trace();
    std::cerr<<"Siblings:"<<std::endl;
    auto siblings=position.GetSiblings();
    for (const auto &sibling : siblings) {
        sibling.Trace();
    }
    */
    auto path=SolvePuzzle15(position);
    std::cout<<path.length()<<std::endl;
    std::cout<<path<<std::endl;

    return 0;
}
