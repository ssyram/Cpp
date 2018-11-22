//
//  main.cpp
//  Limited_Sudoku_Toke_Project
//
//  Created by 潇湘夜雨 on 2018/11/20.
//  Copyright © 2018 ssyram. All rights reserved.
//

#include <iostream>
#include "LimitedSingleThread.h"
using std::bind;
using std::cout;
using std::cin;
using std::endl;

class optional {
public:
    
};

class SpecialSudokuToke {
    const static shared_ptr<SingleThread_Sudoku> null;
    shared_ptr<SingleThread_Sudoku> sudoku = null;
    point queryPosition;
    size_t dimension;
    void resultCoper(uint8_t **data) {
        throw data[queryPosition.l][queryPosition.c];
    }
    SingleThread_Sudoku &getSudoku() {
        if (!sudoku)
            sudoku = shared_ptr<SingleThread_Sudoku>(new SingleThread_Sudoku(dimension, std::bind(&SpecialSudokuToke::resultCoper, std::ref(*this), std::placeholders::_1)));
        return *sudoku;
    }
public:
    SpecialSudokuToke(size_t dimension): dimension(dimension) {
        
    }
    uint8_t *operator[](size_t line) {
        return getSudoku()[line];
    }
    void setQuery(uint8_t l, uint8_t c) {
        queryPosition.l = l;
        queryPosition.c = c;
    }
    std::pair<uint8_t, bool> getResult() {
        try {
            getSudoku().calculate();
        }
        catch (uint8_t &r) {
            sudoku = null;
            return std::make_pair(r, true);
        }
        sudoku = null;
        return std::make_pair(0, false);
    }
};

const shared_ptr<SingleThread_Sudoku> SpecialSudokuToke::null = shared_ptr<SingleThread_Sudoku>(nullptr);

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Please input dimension (no more than 9 and no less than 3)" << endl;
    uint8_t buff;
    cin >> buff;
    buff -= '0';
    if (buff > 9 || buff < 3)
        cout << "Error input" << endl;
    size_t dimension = buff;
    SpecialSudokuToke sudoku(dimension);
    while (true) {
        for (uint8_t i = 0; i < dimension; ++i) {
            cout << "line " << (int)i << ": ";
            for (uint8_t j = 0; j < dimension; ++j) {
                cin >> buff;
                if (buff == '0')
                    continue;
                if (buff == '?')
                    sudoku.setQuery(i, j);
                else if (buff >= '1' && buff <= '9')
                    sudoku[i][j] = buff - '0';
                else
                    return 1;
            }
        }
        auto r = sudoku.getResult();
        if (r.second)
            cout << "?: " << (int)r.first << endl;
        else
            cout << "No Result!" << endl;
    }
    
    return 0;
}
