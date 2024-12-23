#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <sstream>

#define MAX_TRIES 6 // Maximum number of incorrect guesses allowed
#define MAX_WORD_LENGTH 50 // Maximum length of the word

// Function to display the current state of the word (with guessed letters)
void showCurrentState(const std::string& targetWord, const std::string& currentGuess);

// Function to display the hangman figure based on the number of tries
void renderHangman(int tries);

// Function to check if the word has been completely guessed
bool isCompleteGuess(const std::string& targetWord, const std::string& currentGuess);

// Function to load words and their associated hints from a file
void loadWordsAndHints(const std::string& filename, std::vector<std::pair<std::string, std::string>>& wordList);

int main() {
    // Seed the random number generator with the current time
    srand(time(0));

    // Vector to hold pairs of words and their hints
    std::vector<std::pair<std::string, std::string>> wordList;

    // Load words and hints from the file
    loadWordsAndHints("words.txt", wordList);

    // If no words were loaded, exit with an error
    if (wordList.empty()) {
        std::cerr << "Error: No words found in the file." << std::endl;
        return 1;
    }

    // Select a random word from the list of loaded words
    int randomIndex = rand() % wordList.size();
    std::string targetWord = wordList[randomIndex].first;  // The word to guess
    std::string hint = wordList[randomIndex].second;  // The hint for the word
    int wordLength = targetWord.length();

    // Initialize the current guess with underscores, representing unguessed letters
    std::string currentGuess(wordLength, '_');
    // String to track guessed letters
    std::string guessedLetters;

    // Number of incorrect tries
    int tries = 0;

    // Welcome message and display the hint
    std::cout << "Welcome to Hangman!\n";
    std::cout << "Hint: " << hint << "\n";

    // Game loop, running until the player either guesses the word or runs out of tries
    while (tries < MAX_TRIES) {
        std::cout << "\n";
        showCurrentState(targetWord, currentGuess); // Show the current state of the word
        renderHangman(tries); // Show the current hangman figure based on tries

        char guess;
        std::cout << "Enter a letter: ";
        std::cin >> guess;
        guess = tolower(guess); // Convert the guess to lowercase to avoid case sensitivity

        // Check if the letter has already been guessed
        if (guessedLetters.find(guess) != std::string::npos) {
            std::cout << "You've already guessed that letter. Try again.\n";
            continue;
        }

        // Add the guessed letter to the guessedLetters string
        guessedLetters += guess;

        bool found = false;
        // Check if the guessed letter is in the target word
        for (int i = 0; i < wordLength; i++) {
            if (targetWord[i] == guess) {
                currentGuess[i] = guess;  // Update the current guess with the correct letter
                found = true;
            }
        }

        // If the letter was found, notify the player
        if (found) {
            std::cout << "Good guess!\n";
        } else {
            std::cout << "Sorry, the letter '" << guess << "' is not in the word.\n";
            tries++; // Increment the tries if the guess was incorrect
        }

        // Check if the word has been completely guessed
        if (isCompleteGuess(targetWord, currentGuess)) {
            std::cout << "\nCongratulations! You've guessed the word: " << targetWord << "\n";
            break; // Exit the loop if the word is fully guessed
        }
    }

    // If the player runs out of tries, display the game over message
    if (tries >= MAX_TRIES) {
        std::cout << "\nSorry, you've run out of tries. The word was: " << targetWord << "\n";
    }

    return 0;
}

// Function to display the current state of the word (with underscores for unguessed letters)
void showCurrentState(const std::string& targetWord, const std::string& currentGuess) {
    std::cout << "Word: ";
    // Loop through the current guess and display each character
    for (char ch : currentGuess) {
        std::cout << ch << " ";
    }
    std::cout << "\n";
}

// Function to render the hangman figure based on the number of incorrect guesses
void renderHangman(int tries) {
    // Array of strings representing each part of the hangman figure
    const std::string hangmanParts[] = {
        "     _________", 
        "    |         |", 
        "    |         O", 
        "    |        /|\\", 
        "    |        / \\", 
        "    |"
    };

    // Loop through the hangmanParts array and print the corresponding part based on the tries
    std::cout << "\n";
    for (int i = 0; i <= tries; i++) {
        std::cout << hangmanParts[i] << "\n";
    }
}

// Function to check if the word has been completely guessed
bool isCompleteGuess(const std::string& targetWord, const std::string& currentGuess) {
    // Return true if the target word matches the current guess (no underscores left)
    return targetWord == currentGuess;
}

// Function to load words and their hints from a file
void loadWordsAndHints(const std::string& filename, std::vector<std::pair<std::string, std::string>>& wordList) {
    std::ifstream file(filename);  // Open the file for reading
    std::string line;

    // Check if the file was successfully opened
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // Read each line from the file
    while (std::getline(file, line)) {
        std::stringstream ss(line); // Create a stringstream to parse the line
        std::string word, hint;

        // Split the line into the word and hint (separated by a comma)
        if (std::getline(ss, word, ',') && std::getline(ss, hint)) {
            wordList.push_back({word, hint}); // Add the word and hint pair to the wordList
        }
    }

    file.close();  // Close the file after reading
}
