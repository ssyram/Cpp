//
//  LimitedSingleThread.h
//  Limited_Sudoku_Toke_Project
//
//  Created by 潇湘夜雨 on 2018/11/20.
//  Copyright © 2018 ssyram. All rights reserved.
//

#ifndef LimitedSingleThread_h
#define LimitedSingleThread_h

#include <functional>
using std::function;

#include <unordered_set>
using std::unordered_set;

#include <thread>
using std::thread;
using std::mutex;

#include <memory>
using std::shared_ptr;

#include <iostream>
using std::cout;
using std::endl;

struct point {
    uint8_t l;
    uint8_t c;
    point() = default;
    point(uint8_t l, uint8_t c): l(l), c(c) {}
    bool operator==(const point &r) const {
        return r.l == l && r.c == c;
    }
};
namespace std {
    template <>
    class hash<point> {
    public:
        size_t operator()(const point &p) const {
            return (((size_t)p.l) << 8) | ((size_t)p.c);
        }
    };
}
// 分析过程
class SingleThread_Sudoku {
    unordered_set<uint8_t> model_set; // do as the model of each set
    shared_ptr<unordered_set<uint8_t>> target_set;
    mutex ts_mutex;
    const size_t dms;
    const function<void (uint8_t **)> recall;
    unordered_set<point> waitSet;
    uint8_t **data;
    const shared_ptr<unordered_set<uint8_t>> null = shared_ptr<unordered_set<uint8_t>>(nullptr);
    
    void fillTarget_Set() {
        ts_mutex.lock();
        target_set = shared_ptr<unordered_set<uint8_t>>(new unordered_set<uint8_t>(model_set));
        ts_mutex.unlock();
    }
    
    shared_ptr<unordered_set<uint8_t>> getANewModel() {
        return shared_ptr<unordered_set<uint8_t>>(new unordered_set<uint8_t>(model_set));
//        ts_mutex.lock();
//        shared_ptr<unordered_set<uint8_t>> r = target_set;
//        target_set = null;
//        ts_mutex.unlock();
//        thread t(&SingleThread_Sudoku::fillTarget_Set, std::ref(*this));
//        t.detach();
//        return r;
    }
    
    // 将所有当前情况下的必然点都登记，同时将已修改的点放到save_set并且将下一轮循环应该进行的循环点next和nextLoopSet填充
    // 返回值影响：如果nextLoopSet为空说明这次trace已经结束，或者是成功recall或者是
    void calTrace(unordered_set<point> &save_set, point &next, shared_ptr<unordered_set<uint8_t>> &nextLoopSet) {
//        printData();
        int change = 1;
        // 返回值表示是否已经不能继续循环
        auto trace = [&save_set, this, &nextLoopSet, &next, &change] (unordered_set<point>::const_iterator &p) -> bool {
            const uint8_t &l = p->l;
            const uint8_t &c = p->c;
            shared_ptr<unordered_set<uint8_t>> tset = getANewModel();
//            printData();
            for (uint8_t i = 0; i < dms; ++i) {
                if (data[l][i])
                    tset->erase(data[l][i]);
                if (data[i][c])
                    tset->erase(data[i][c]);
            }
            size_t size = tset->size();
            if (!size) return true;
            if (size == 1) {
                data[l][c] = *(tset->begin());
                save_set.insert(*p);
                p = waitSet.erase(p); // here p is a dangle pointer
                ++change;
                return false;
            }
            if (!nextLoopSet || nextLoopSet->size() > size) {
                next = *p;
                nextLoopSet = tset;
            }
            return false;
        };
        
        static shared_ptr<unordered_set<uint8_t>> null(nullptr);
        
        // loop to fill all good
        while (change > 0) {
            change = 0;
            unordered_set<point>::const_iterator it = waitSet.begin();
            nextLoopSet = null;
            
            while (it != waitSet.end()) {
                int bc = change;
                if (trace(it)) {
                    change = 0;
                    break;
                }
                if (bc == change)
                    ++it;
            }
        }
//        printData();
    }
    
    void printData() {
        cout << "---------------------" << endl;
        for (uint8_t i = 0; i < dms; ++i) {
            for (uint8_t j = 0; j < dms; ++j)
                cout << (int) data[i][j];
            cout << endl;
        }
        cout << "---------------------" << endl;
    }
    
    void call(const point &tp, const shared_ptr<unordered_set<uint8_t>> tempLoopSet) {
        waitSet.erase(tp);
        unordered_set<point> save_set;
        point next;
        shared_ptr<unordered_set<uint8_t>> nextLoopSet;
        
        for (uint8_t loopValue: *tempLoopSet) {
            data[tp.l][tp.c] = loopValue;
            calTrace(save_set, next, nextLoopSet);
            if (!waitSet.size()) {
                recall(data);
            }
            else if (nextLoopSet) {
                call(next, nextLoopSet);
            }
            
            for (auto p: save_set)
                data[p.l][p.c] = 0;
            waitSet.insert(save_set.begin(), save_set.end());
        }
        
        waitSet.insert(tp);
    }
    SingleThread_Sudoku(const SingleThread_Sudoku&) = delete;
//    void inner_cal() {
//        point change[dms * dms];
//        size_t csize = 0;
//        auto checkCertainty = [this] () {
//        };
//    }
public:
    SingleThread_Sudoku(size_t demension, function<void (uint8_t **)> recall): dms(demension), recall(recall) {
        data = new uint8_t*[dms];
        data[0] = new uint8_t[dms * dms]();
        for (size_t i = 1; i < dms; ++i)
            data[i] = data[i - 1] + dms;
        for (uint8_t i = 1; i <= dms; ++i)
            model_set.insert(i);
    }
    void addLine(uint8_t *line, size_t lineNum) {
        if (lineNum >= dms) throw 2;
        auto &l = data[lineNum];
        for (size_t i = 0; i < dms; ++i)
            l[i] = line[i];
    }
    uint8_t *operator[](size_t line) {
        return data[line];
    }
    void calculate() {
        for (uint8_t i = 0; i < dms; ++i) {
            for (uint8_t j = 0; j < dms; ++j) {
//                cout << (int) data[i][j];
                if (!data[i][j])
                    waitSet.insert(point(i, j));
            }
//            cout << endl;
        }
        point next;
        shared_ptr<unordered_set<uint8_t>> nextLoopSet;
        unordered_set<point> saveSet;
        fillTarget_Set();
        calTrace(saveSet, next, nextLoopSet);
        if (!waitSet.size())
            recall(data);
        else if (nextLoopSet)
            call(next, nextLoopSet);
    }
};

#endif /* LimitedSingleThread_h */
