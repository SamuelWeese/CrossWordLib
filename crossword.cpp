#include "crossword.h"
#include <iostream>
#include <algorithm>

int globalSpaceInt = 126;

using namespace std;

crossword::crossword(vector<aWordConnection> connections){}
crossword::crossword(vector<string> wordList, int algorithm = 0)
{
    bool changed = true;
    while (changed)
    {
        changed = false; // change below to a while true loop
        // TODO
        for (int iterI = 0; iterI < wordList.size(); iterI++)
        {
            if (this->addWord(wordList[iterI]))
            {
                wordList.erase(wordList.begin()+iterI);
                changed = true;
                iterI--;
            }
            if (iterI < 0 || iterI > wordList.size())
            {
                break;
            }
        }
    }
    for (auto iterStr : wordList)
    {
        unusedWord.emplace_back(iterStr);
    }
}

void crossword::print()
{
    cout << "Words In Que:";
    for (auto iterI : this->unusedWord)
    {
        cout << ' ';
        cout << iterI;
    }
    cout << "\n";
    cout << "Board State: \n";
    for (auto iterI : this->boardState)
    {
        for (auto iterJ : iterI)
        {
            cout << '\'' << iterJ << '\'';
        }
        cout << '\n';
    }

}

bool crossword::addWord(string aWord)
{
    if (usedWords.size() == 0)
    {
        vector<char> aVect;
        for (auto iterI : aWord)
        {
            aVect.push_back(iterI);
        }
        this->boardState.push_back(aVect);
        wordPosition newWord;
        newWord.word = aWord;
        newWord.startX = 0;
        newWord.startY = 0;
        newWord.isVertical = false;
        usedWords.push_back(newWord);
        return true;
    }
    // looping through used words
    for (int iterWord = 0; iterWord < this->usedWords.size(); iterWord++)
    {
        for (int iterI = 0; iterI < usedWords[iterWord].word.length(); iterI++)
        {
            for (int iterJ = 0; iterJ < aWord.length(); iterJ++)
            {
                // below is the debug string. I am leaving it in because it will speak more to a programmer than me explaining it verbatim
                /*cout << "Checking " << aWord[iterJ] << " in added word "
                     << aWord << " against current word " << iterStr.word
                     << " with " << iterStr.word[iterI] << " at "
                     << (iterStr.startX) << ", " << iterStr.startY << endl;*/
                if (usedWords[iterWord].word[iterI] == aWord[iterJ])
                {
                   // cout << "valid match!" << endl;
                    if (usedWords[iterWord].isVertical)
                    {
                        //cout << "is vertical" << endl;
                        // try to add horizontal word
                        if (checkHorizontal(usedWords[iterWord].startX + iterI, usedWords[iterWord].startY, aWord, iterJ))
                        {
                                return true;
                        }
                    }
                    else
                    {
                        //cout << "is usable" << endl;
                        // try to add vertical word
                        if (checkVertical(usedWords[iterWord].startX, usedWords[iterWord].startY + iterI, aWord, iterJ))
                        {
                            //cout << "Added word." << endl;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// x is vertical index
bool crossword::checkVertical(int xCoord, int yCoord, string addedWord, int addedWordIndex)
{
    if (boardState[xCoord][yCoord] != addedWord[addedWordIndex])
    {
        return false;
    }
    // check other word collision
    for (int iterCheckWord = 0; iterCheckWord < addedWord.length(); iterCheckWord++)
    {
        int boardXIndex = iterCheckWord + xCoord - addedWordIndex;
        // boardYIndex remains the same for this entire thing
        if (boardXIndex < 0 || boardXIndex >= boardState.size())
        {
            // ensuring no segfaults
            continue;
        }
        if (boardState[boardXIndex][yCoord] != globalSpaceInt && boardState[boardXIndex][yCoord] != addedWord[iterCheckWord])
        {
            return false;
        }
    }
    // check resize if blank rows need to be added to the top of the crossword
    if (xCoord < addedWordIndex)
    {
        int downShiftSize = addedWordIndex - xCoord;
        for (int downShiftCounter = 0; downShiftCounter < downShiftSize; downShiftCounter++)
        {
            // the length of the first vector should be the same throughout the entire board
            boardState.push_back(vector<char>(boardState[0].size(), globalSpaceInt));
        }
        // adding to the array and writing across the entire array once backwards to avoid spacing issues
        // This project would be more efficient using deque for large amounts of data, but gains more efficiency for small amounts
        for (int downShiftReverseIter = boardState.size() - 1; downShiftReverseIter >= 0; downShiftReverseIter--)
        {
            for (int iterY = 0; iterY < boardState[0].size(); iterY++)
            {
                int dataToDownShiftIndex = downShiftReverseIter - downShiftSize;
                if (dataToDownShiftIndex < 0)
                {
                    boardState[downShiftReverseIter][iterY] = globalSpaceInt;
                }
                else
                {
                    boardState[downShiftReverseIter][iterY] = boardState[dataToDownShiftIndex][iterY];
                }
            }
        }
        for (unsigned long iterWord = 0; iterWord < this->usedWords.size(); iterWord++)
        {
            cout << "Old index:" << this->usedWords[iterWord].startX;
            this->usedWords[iterWord].startX += (downShiftSize);
            cout << " New Index: " << this->usedWords[iterWord].startX << endl;
        }
        xCoord += downShiftSize;
    }
    int additionalRowsRequired = xCoord + addedWord.length() - addedWordIndex - boardState.size();
    if (additionalRowsRequired > 0)
    {
        for (;additionalRowsRequired > 0; additionalRowsRequired--)
        {
            boardState.push_back(vector<char>(boardState[0].size(), globalSpaceInt));
        }
    }
    // finally placing the word
    for (int iterWord = 0; iterWord < addedWord.length(); iterWord++)
    {
        boardState[xCoord - addedWordIndex + iterWord][yCoord] = addedWord[iterWord];
    }
    wordPosition newWord;
    newWord.word = addedWord;
    newWord.startX = xCoord - addedWordIndex;
    newWord.startY = yCoord;
    newWord.isVertical = true;
    usedWords.push_back(newWord);
    return true;
}

bool crossword::checkHorizontal(int xCoord, int yCoord, string addedWord, int addedWordIndex)
{
    // identical to above function (checkVertical), maybe template this
    if (boardState[xCoord][yCoord] != addedWord[addedWordIndex])
    {
        return false;
    }
    // check other word collision
    for (int iterCheckWord = 0; iterCheckWord < addedWord.size(); iterCheckWord++)
    {
        int boardYIndex =iterCheckWord + yCoord -addedWordIndex;
        // boardXIndex remains the same for this entire thing
        if (boardYIndex < 0 || boardYIndex >= boardState[xCoord].size())
        {
            // ensuring no segfaults
            continue;
        }
        if (boardState[xCoord][boardYIndex] != globalSpaceInt && boardState[xCoord][boardYIndex] != addedWord[iterCheckWord])
        {
            return false;
        }
    }
    // check resize if blank rows need to be added to the top of the crossword
    if (yCoord < addedWordIndex)
    {
        int sideShiftSize = addedWordIndex - yCoord;
        // the length of the first vector should be the same throughout the entire board
        // one line since the entire board needs to be pushed back
        vector<char> sideShiftVector(sideShiftSize,globalSpaceInt);
        for (auto iterBoard : boardState)
        {
            iterBoard.insert(iterBoard.end(), sideShiftVector.begin(), sideShiftVector.end());
            for (int iterY = iterBoard.size() - 1; iterY >= 0; iterY--)
            {
                int dataToSideShiftIndex = iterY - sideShiftSize;
                if (dataToSideShiftIndex < 0)
                {
                    iterBoard[iterY] = globalSpaceInt;
                }
                else
                {
                    iterBoard[iterY] = iterBoard[dataToSideShiftIndex];
                }
            }
        }
        for (unsigned long iterWord = 0; iterWord < this->usedWords.size(); iterWord++)
        {
            this->usedWords[iterWord].startY += sideShiftSize;
        }
        yCoord += sideShiftSize;
    }
    int additionalColumnsRequired = yCoord + addedWord.length() - addedWordIndex - boardState[xCoord].size();
    if (additionalColumnsRequired > 0)
    {
        vector<char> additionalSpaceVector(additionalColumnsRequired, globalSpaceInt);
        for (auto iterBoard : boardState)
        {
            iterBoard.insert(iterBoard.end(), additionalSpaceVector.begin(), additionalSpaceVector.end());
        }
    }
    // finally placing the word
    for (int iterWord = 0; iterWord < addedWord.length(); iterWord++)
    {
        boardState[xCoord][yCoord - addedWordIndex + iterWord] = addedWord[iterWord];
    }
    wordPosition newWord;
    newWord.word = addedWord;
    newWord.startX = xCoord;
    newWord.startY = yCoord - addedWordIndex;
    newWord.isVertical = false;
    usedWords.push_back(newWord);
    return true;
}
