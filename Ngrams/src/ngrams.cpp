/*
John Kolden, SCPD
Standford University CS106B
Filename: ngrams.cpp
Assignment 1
April 16, 2015
Section Leader: Sarah Spikes

Purpose of Program:
This program allows users to generate random text based on a given book or collection of text that should sound as though it was written by the author.
The user will select a text file, enter the number of words (N) that will comprise the word map and will also enter the number of words to be generated.
The result will be a string of text resembling the original text.
*/


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "hashmap.h"
#include "simpio.h"
#include "filelib.h"

using namespace std;

//function prototypes:
void displayWelcomeMessages();
string getFileName();
void readInputFile(Vector<string>&, ifstream&, int);
int getValueOfN();
void generateWordMap(HashMap<Vector<string>, Vector<string> >& , Vector<string>&, int);
int getWordCount();
void generateRandomText(HashMap<Vector<string>, Vector<string> >&, int);

int main() {

    displayWelcomeMessages();
    ifstream input;
    input.open(getFileName());
    int n = getValueOfN();

    //read text file into a vector. The vector will be used to create our word map
    Vector<string> allWords;
    readInputFile(allWords, input, n);

    HashMap<Vector<string>, Vector<string> > map;
    generateWordMap(map, allWords, n);

    while (true) {
        int wordCount = getWordCount();
        if (wordCount == 0)
            break;
        generateRandomText(map, wordCount);
    }

    cout << "Exiting." << endl;
    return 0;
}

void displayWelcomeMessages() {

    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
}

int getValueOfN() {

    int n = getInteger("Value of N?");

    while (n < 2) {
        cout << "N must be 2 or greater" << endl;
        n = getInteger("Value of N?");
    }

    return n;
}

int getWordCount() {
    cout <<endl;
    int wordCount = getInteger("# of random words to generate (0 to quit)? ");

    while (wordCount != 0 and wordCount < 4) {
        cout << "Must be at least 4 words." << endl;
        cout << endl;
        wordCount = getInteger("# of random words to generate (0 to quit)? ");
    }
    return wordCount;
}

string getFileName() {

    string fileName = getLine("Input file name? ");

    while (!fileExists(fileName)) {
        cout << "Unable to open that file.  Try again." << endl;
        fileName   = getLine("Input file name? ");
    }
    return fileName;
}

/* This function accepts as parameters a reference to the map that was created in main(), a vector of all words
 * in the text file and the value of N which represents the size of the window that will be used to add values to
 * the map. The result will be a word map of N-1 words as the map key and the map value of all words in the text
 * that follow the key.
*/
void generateWordMap (HashMap<Vector<string>, Vector<string> >& map, Vector<string>& allWords, int n) {

    Vector<string> window;
    Vector<string> keyValues;
    Vector<string> values;

    for (int i = 0; i < allWords.size(); i++) {

        //create window of N-1 words
        window.add(allWords[i]);

        if (window.size() == n) {
            values.add(allWords[i]);
            keyValues = window;
            keyValues.remove(n-1); //e.g. if n = 5, remove the vector position 4, leaving positions 0-3.

            //check if key value already exists
            if (map.containsKey(keyValues) ){
                Vector<string> existingValues = map[keyValues];
                existingValues.add(allWords[i]);
                map.put(keyValues, existingValues);

            } else {
                map.put(keyValues, values);
            }

            window.remove(0);//clear window and continue reading...
            values.clear();
        }

    }
}

/* This function accepts as parameters a reference to the map created in main(), populated in
 * generateWordMap() and the desired word count entered by the user. The result is a random selection of
 * map keys and values that should look similar to the original text.
*/
void generateRandomText(HashMap<Vector<string>, Vector<string> >& map, int wordCount) {

    Vector<Vector<string>> allKeys = map.keys();

    cout << "...";

    while (wordCount > 0) {

        int randomNumber = randomInteger(0, (map.size()-1));
        Vector<string> randomString = allKeys[randomNumber];
        Vector<string> values = map[randomString];
        int randomValuePosition = randomInteger(values.size()-1, (values.size()-1));

        for (int i = 0; i <  randomString.size(); i ++) {
            cout << randomString[i] << " " ;
            wordCount--;

            if (wordCount == 0) {
                cout << "..." << endl;
                return;
            }
        }

        cout << values[randomValuePosition] << " ";
        wordCount--;

    }
    cout << "..." << endl;

}

//This function reads the input file into a Vector for processing by the word map.
void readInputFile(Vector<string>& allWords, ifstream& input, int n) {

    Vector<string> firstNMinusOneWords;
    string word;
    int i = 0;

    while (input >> word) {

        allWords.add(word);

        if (i < n-1) {//wrap the first N-1 words around to to the back of the vector
            firstNMinusOneWords.add(word);
        }
        i++;
    }
    allWords += firstNMinusOneWords;
}



