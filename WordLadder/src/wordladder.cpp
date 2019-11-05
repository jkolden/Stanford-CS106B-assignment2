/*
John Kolden, SCPD
Standford University CS106B
Filename: wordladder.cpp
Assignment 2
April 13, 2015
Section Leader: Sarah Spikes

Purpose of Program:
-------------------
This program allows users' to create a word ladder between two words. A word ladder is a 'map' between two words that is created by changing one letter
in a word to get to the next word, and so on, until a map is build between two words. While there can be many word ladders between two words, this program
return the shortest one.
*/


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "simpio.h"
#include <algorithm>
#include "lexicon.h"

using namespace std;

void displayWelcomeMessages();
bool usedWord(string, Vector<string>&);
bool englishWord(Lexicon&, string);
Vector<string> findNeighborWords(Lexicon&, string);
void deriveWordLadder(Lexicon&, string, string);


int main() {

    //read in dictionary file
    Lexicon dictionary;
    ifstream input;



    //begin user interaction:
    displayWelcomeMessages();
    string filename = promptUserForFile(input, "Dictionary file name? ", "Unable to open that file.  Try again. ");
    dictionary.addWordsFromFile(filename);


    while (true) {

        cout << endl;
        string w1 =  toLowerCase(getLine("Word #1 (or Enter to quit): "));

        if (w1 == "") {
            break;
        }

        string w2 =  toLowerCase(getLine("Word #2 (or Enter to quit): "));

        if (w2 == "") {
            break;
        }

        if (!englishWord(dictionary, w1)||!(englishWord(dictionary, w2))) {
            cout << "The two words must be found in the dictionary." << endl;

        } else if (w1.length() != w2.length()) {
            cout << "The two words must be the same length." << endl;

        } else if (w1 == w2) {
            cout << "The two words must be different." << endl;

        } else {

            deriveWordLadder(dictionary, w1, w2);

        }
    }


    cout << "Have a nice day." << endl;
    return 0;

}


void displayWelcomeMessages() {

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}


bool usedWord(string word, Vector<string>& usedWords) {
    if (!usedWords.isEmpty() && find(usedWords.begin(), usedWords.end(), word) != usedWords.end()) {

        return true;

    } else {

        return false;
    }

}

bool englishWord(Lexicon& dictionary,string word) {

    if (dictionary.contains(word)) {
        return true;
    } else {
        return false;
    }

}

Vector<string> findNeighborWords(Lexicon& dictionary, string wordBeingTested) {

    const string ALPHABET = toLowerCase("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    Vector<string> neighborWords;
    string newWord = wordBeingTested;

    for (int i = 0; i < wordBeingTested.length(); i++) {
        for (int j = 0; j < ALPHABET.length(); j++) {
                newWord.replace(i, 1, charToString(ALPHABET[j]));

                if (englishWord(dictionary, newWord) && (newWord != wordBeingTested)) {
                neighborWords.add(newWord);
            }
        }//end of alphabet loop

        //we've tried all the letters in the alphabet at this position so let's set the word back to where it was
        //and try all the letters in the next position:
        newWord = wordBeingTested;
    }//end of word length loop

    return neighborWords;
}


void deriveWordLadder(Lexicon& dictionary, string w1, string w2) {

    Stack<string> firstStack;
    Queue<Stack<string>> queueOfStacks;
    Vector<string> usedWords;

    firstStack.push(w1);
    queueOfStacks.enqueue(firstStack);

    while (!queueOfStacks.isEmpty()) {

        Stack<string> testStack =  queueOfStacks.dequeue();
        Vector<string> neighbors = findNeighborWords(dictionary, testStack.peek());

        for (int i = 0; i< neighbors.size(); i++) {

            if (!usedWord(neighbors[i], usedWords)) {
                if (neighbors[i] == w2) {
                    cout << "A ladder from " << w2 << " back to " << w1 << ":" << endl;
                    cout << w2 << " ";


                    while (!testStack.isEmpty()) {
                        cout <<  testStack.pop() << " " ;
                    }
                    cout << endl;
                    return;

                } else {

                    Stack<string> copyOfStack = testStack; //create a copy of the current partial-ladder stack.
                    copyOfStack.push(neighbors[i]); //put the neighbor word on top of the copy stack.
                    queueOfStacks.enqueue(copyOfStack); //add the copy stack to the end of the queue.
                    usedWords.add(neighbors[i]); //do not reuse words that have been included in a previous ladder

                }
            }
        }

    }
//if stack has been emptied:
cout << "no word ladder could be derived." << endl;
}

