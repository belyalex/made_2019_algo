#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <string>
#include "Huffman.h"

using byte=unsigned char;

class BitsReader {
public:
    bool ReadBit(bool& bit);
    bool ReadByte(byte& byte);

    BitsReader(std::vector<byte>&& buffer);

    bool isEncoded() {return encoded;}
private:
    std::vector<byte> buffer_;
    size_t position_ = 0;
    size_t bits_count_ = 4;
    size_t bits_count_last = 0;
    bool encoded = true;
};

BitsReader::BitsReader(std::vector<byte>&& buffer) :
    buffer_(std::move(buffer))
{
    bits_count_last = buffer_[0] & 7;
    encoded = (buffer_[0] & 8 != 0);
}

bool BitsReader::ReadBit(bool& bit) {
    if (((position_==buffer_.size()-1)&&(bits_count_==bits_count_last)&&(bits_count_last!=0)) ||
        (position_==buffer_.size())) {
        return false;
    }
    byte byte_ = buffer_[position_];
    bit = (byte_ & (1<<bits_count_++))!=0;
    if (bits_count_==8) {
        position_++;
        bits_count_=0;
    }
    return true;
}

bool BitsReader::ReadByte(byte& byte_) {
    if (bits_count_==0) {
        byte_=buffer_[position_++];
        return true;
    }
    byte b1=buffer_[position_++];
    byte b2=buffer_[position_];
    byte_ = (b1>>bits_count_)|(b2<<(8-bits_count_));

    return true;
}


class BitsWriter {
public:
    void WriteBit(bool bit);
    void WriteByte(byte byte);

    std::vector<byte> GetResult();

    void setEncoded(bool encoded) {encoded_=encoded;}
private:
    std::vector<byte> buffer_;
    byte accumulator_ = 0;
    size_t bits_count_ = 4;
    bool encoded_ = true;
};

void BitsWriter::WriteBit(bool bit) {
    accumulator_ |= static_cast<byte>(bit) << bits_count_++;
    if (bits_count_ == 8) {
        bits_count_ = 0;
        buffer_.push_back(accumulator_);
        accumulator_ = 0;
    }
}

void BitsWriter::WriteByte(byte byte) {
    if (bits_count_ == 0) {
        buffer_.push_back(byte);
    }
    else {
        accumulator_ |= byte << bits_count_;
        buffer_.push_back(accumulator_);
        accumulator_ = byte >> (8 - bits_count_);
    }
}

std::vector<byte> BitsWriter::GetResult() {
    if (bits_count_ != 0) {
        buffer_.push_back(accumulator_);
    }
    buffer_[0]|=bits_count_;
    if (encoded_) {
        buffer_[0]|=8;
    }

    return std::move(buffer_);
}

const int initial_capacity = 16;

struct HeapNode {
    HeapNode* left = nullptr;
    HeapNode* right = nullptr;
    size_t count = 0;
    byte   value = 0;
    std::string code = "";

    HeapNode(HeapNode&& n) :
            count(n.count),
            value(n.value),
            code(std::move(n.code))
    {
        if (n.left != nullptr) {
            left = n.left;
            n.left = nullptr;
        }
        if (n.right != nullptr) {
            right = n.right;
            n.right = nullptr;
        }
    }

    HeapNode& operator=(HeapNode&& n) {
        count = n.count;
        value = n.value;
        code = std::move(n.code);
        if (n.left != nullptr) {
            left = n.left;
            n.left = nullptr;
        }
        if (n.right != nullptr) {
            right = n.right;
            n.right = nullptr;
        }

        return *this;
    }

    explicit HeapNode(size_t count = 0, byte value = 0, HeapNode* left = nullptr, HeapNode* right = nullptr) :
            count(count),
            value(value),
            left(left),
            right(right)
    {
    }

    ~HeapNode() {
        if (left != nullptr) {
            left->BFS([](HeapNode* n) {n->left = nullptr; n->right = nullptr; delete n; });
        }
        if (right != nullptr) {
            right->BFS([](HeapNode* n) {n->left = nullptr; n->right = nullptr; delete n;  });
        }
    }

    void build_code(const std::string& prefix) {
        code = prefix;
        if (left != nullptr) left->build_code(prefix + "1");
        if (right != nullptr) right->build_code(prefix + "0");
    }
    void print() {
        std::cerr << std::endl;
        BFS([](HeapNode* n) {
            std::cerr << n << " " << n->left << " " << n->right << " " << n->count << " " << n->value <<" "<<n->code<< std::endl;
        });
        std::cerr << std::endl;
    }

    bool operator<=(const HeapNode& n) const {
        return count <= n.count;
    }

    template <class Callback>
    void BFS(Callback f) {
        std::queue<HeapNode*> q;
        q.push(this);
        while (!q.empty()) {
            HeapNode* n = q.front();
            q.pop();
            if (n != nullptr) {
                q.push(n->left);
                q.push(n->right);
                f(n);
            }
        }
    }

    void print2(HeapNode* n, BitsWriter& bw) {
        if (n==nullptr) return;
        if (n->left!=nullptr) {
            bw.WriteBit(false);
            print2(n->left, bw);
        }
        if (n->right!=nullptr) {
            print2(n->right, bw);
        }
        if ((n->left==nullptr)&&(n->right==nullptr)) {
            bw.WriteBit(true);
            bw.WriteByte(n->value);
        }
    }

    void buildTreeFromBitsReader(BitsReader& br, HeapNode* n) {
        bool bit;
        br.ReadBit(bit);
        if (bit) {
            byte byte_;
            br.ReadByte(byte_);
            n->value=byte_;
            return;
        }
        n->left=new HeapNode();
        buildTreeFromBitsReader(br,n->left);
        n->right=new HeapNode();
        buildTreeFromBitsReader(br,n->right);
    }
};


template <class C>
class Heap {
public:
    bool empty() const { return size == 0; };
    const C& top() const {
        assert(!empty());

        return buffer[0];
    }
    C pop() {
        assert(!empty());

        C result = std::move(buffer[0]);
        if (--size > 0) {
            //в корень записываем последний элемент и просеиваем его вниз
            buffer[0] = std::move(buffer[size]);
            sift_down(0);
        }

        return result;
    }
    void push(C& value) {
        if (size == capacity) {
            //надо перевыделить место
            //новый размер буфера увеличим вдвое
            capacity <<= 1;
            C* new_buffer = new C[capacity];
            for (size_t i=0;i<size;i++)
                new_buffer[i]=std::move(buffer[i]);
            delete[] buffer;
            buffer = new_buffer;
        }

        buffer[size] = std::move(value);
        sift_up(size++);
    }
private:
    C* buffer = new C[initial_capacity];
    size_t capacity = initial_capacity;
    size_t size = 0;

    //просеивание элемента вниз
    void sift_down(size_t index) {
        assert((index >= 0) && (index < size));

        size_t child1_index = (index << 1) + 1;
        if (child1_index >= size) {
            //нет потомков
            return;
        }

        if (child1_index + 1 == size) {
            //есть только 1 потомок
            if ((buffer[index]) <= (buffer[child1_index])) {
                return;
            }
            std::swap(buffer[index], buffer[child1_index]);
            //дальше просеиваться не имеет смысла
            return;
        }

        size_t child2_index = child1_index + 1;

        if ((buffer[index] <= buffer[child1_index]) && (buffer[index] <= buffer[child2_index])) {
            //текущий элемент больше или равен дочерним
            return;
        }

        //меняемся местами с бОльшим из дочерних элементов и пытаемся просеиться дальше
        if (buffer[child1_index] <= buffer[child2_index]) {
            std::swap(buffer[index], buffer[child1_index]);
            sift_down(child1_index);
        }
        else {
            std::swap(buffer[index], buffer[child2_index]);
            sift_down(child2_index);
        }
    }
    //просеивание элемента вверх
    void sift_up(size_t index) {
        assert((index >= 0) && (index < size));

        if (index == 0) {
            //корневой элемент
            return;
        }

        size_t parent_index = (index - 1) >> 1;
        if (buffer[index] <= buffer[parent_index]) {
            std::swap(buffer[parent_index], buffer[index]);
            sift_up(parent_index);
        }
    }
public:
    ~Heap() { if (buffer != nullptr) delete[] buffer; };
};

void Encode(IInputStream& original, IOutputStream& compressed)
{
    Heap<HeapNode> heap;
    unsigned int gist[256]{ 0 };
    std::vector<byte> bytes;
    byte value;

    while (original.Read(value)) {
        bytes.push_back(value);
        gist[value]++;
    }

    for (size_t i = 0; i<256; i++) {
        if (gist[i] != 0) {
            HeapNode n( gist[i], (byte)i );
            heap.push(n);
        }
    }

    HeapNode n;
    while (!heap.empty()) {
        n = std::move(heap.pop());
        if (!heap.empty()) {
            HeapNode n1 = std::move(heap.pop());
            HeapNode n2 (n.count + n1.count, 0, new HeapNode(std::move(n)), new HeapNode(std::move(n1)));
            heap.push(n2);
        }
    }

    std::unordered_map<byte, std::string> encode_map;
    n.build_code("");

    n.BFS([&encode_map](HeapNode* n) {
        if ((n->left == nullptr) && (n->right == nullptr)) {
            encode_map[n->value] = n->code;
        }
    });

    BitsWriter bw;
    n.print2(&n, bw);

    for (size_t i = 0; i < bytes.size(); i++) {
        for (auto b:encode_map[bytes[i]]) {
            bw.WriteBit(b=='1');
        }
    }

    auto result=bw.GetResult();

    double k=double(result.size())/double(bytes.size());

    if (k<1) {
        for (byte b : result) {
            compressed.Write(b);
        }
    }
    else {
        compressed.Write(0);
        for (byte b: bytes){
            compressed.Write(b);
        }
    }
}

void Decode(IInputStream& compressed, IOutputStream& original)
{
    std::vector<byte> bytes;
    byte value;

    if (compressed.Read(value)) {
        if (value!=0) {
            bytes.push_back(value);
        }
        else {
            while (compressed.Read(value)) {
                original.Write(value);
            }
            return;
        }
    }

    while (compressed.Read(value)) {
        bytes.push_back(value);
    }

    BitsReader br(std::move(bytes));

    HeapNode n;

    n.buildTreeFromBitsReader(br, &n);
    n.build_code("");

    std::unordered_map<std::string, byte> decode_map;
    n.BFS([&decode_map](HeapNode* n) {
        if ((n->left == nullptr) && (n->right == nullptr)) {
            decode_map[n->code] = n->value;
        }
    });

    bool bit;
    std::string code;
    while (br.ReadBit(bit)) {
        code+=bit?"1":"0";
        auto v=decode_map.find(code);
        if (v!=decode_map.end()) {
            original.Write(v->second);
            code="";
        }
    }
}


