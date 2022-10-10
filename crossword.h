#ifndef CROSSWORD_H
#define CROSSWORD_H
#include <vector>
#include <string>

struct aWordConnection
{
    char charConnector;
    std::vector<std::string *> parentPointer;
    std::vector<int> parentIndex; // same as right hang, left hang can always be calculated as: parents[i].size() - parentIndex[i] - 1;
};

struct wordPosition {
    std::string word;
    int startX;
    int startY;
    bool isVertical;
};

class crossword
{
public:
    crossword(std::vector<aWordConnection> connections);
    crossword(std::vector<std::string> wordList);
    ~crossword(){ return;}
    void print();
    bool addWord(std::string aWord);
private:
    std::vector<wordPosition> usedWords;
    std::vector<std::vector<char>> boardState;
    std::vector<std::string> unusedWord;
    bool checkWhiteSpace(std::string aWord);
    bool checkHorizontal(int x, int y, std::string aWord, int index);
    bool checkVertical(int x, int y, std::string aWord, int index);


};
/*

std::vector<aWordConnection> genCrossWordConnections(std::vector<std::string> listOfWords)
{
    std::vector<aWordConnection> retVal;
    for (int i = 0; i < listOfWords.size(); i++)
    {
        // replace below loops with find() for efficiency, n^3 kinda gross
        for (int j = 0; j < listOfWords[i].length(); j++)
        {
            int k = -1;
            while (true)
            {
                k++;
                if (k == retVal.size())
                {
                    aWordConnection newConnection;
                    newConnection.charConnector = listOfWords[i][j];
                    newConnection.parentPointer.push_back(&listOfWords[i]);
                    newConnection.parentIndex.emplace_back(j);
                    retVal.push_back(newConnection);
                    break;
                }
                if (retVal[k].charConnector == listOfWords[i][j])
                {
                    retVal[k].parentPointer.push_back(&listOfWords[i]);
                    retVal[k].parentIndex.push_back(j);
                    break;
                }
            }
        }
    }
    return retVal;
}
*/
#endif // CROSSWORD_H
