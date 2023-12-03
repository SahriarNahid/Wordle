#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_NUM_OF_WORDS 100



bool processGuess(const char* theAnswer, const char* theGuess) {
  //the clue
  char clue[6] = {'-', '-', '-', '-', '-', '\0'};
  //a set of flags indicating if that letter in the answer is used as clue
  bool answerFlags[5] = {false, false, false, false, false};

  //first check, exact matches
  for (int i=0; i<5; i++) {
    if (theGuess[i] == theAnswer[i]) {
      clue[i] = 'G';
      answerFlags[i] = true;
    }
  }

  //second check, wrong positioning
  for (int i=0; i<5; i++) {
    if (clue[i] == '-') {//no exact match from first check
      for (int j=0; j<5; j++) {
        if (theGuess[i] == theAnswer[j] && !answerFlags[j]) {
          //a match at another position and has not been used as clue
          clue[i] = 'Y';
          answerFlags[j] = true;
          break; //end this j-loop because we don't want multiple clues from the same letter
        }
      }
    }
  }

  printf("%s\n", clue);
  return strcmp(clue, "GGGGG") == 0; 
}







int main() {

//describe rules
  FILE *file;
  char line[100];
  file = fopen("rules.txt", "r");
    while (!feof(file)) {
        if (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
    }
  fclose(file);


  //load word list
  char** wordsList = calloc(MAX_NUM_OF_WORDS, sizeof(char*));
  int wordCount = 0;
  char* fiveLetterWord = malloc(6*sizeof(char));

  FILE* wordsFile = fopen("words.txt", "r");
  while (fscanf(wordsFile, "%s", fiveLetterWord) != EOF && wordCount < MAX_NUM_OF_WORDS) {
    wordsList[wordCount] = fiveLetterWord;
    wordCount++;
    fiveLetterWord = malloc(6*sizeof(char));
  }
  fclose(wordsFile);


  //picks a word randomly
  srand(time(NULL));
  char* answer = wordsList[rand()%wordCount];

  //the game loop
  int num_of_guesses = 0;
  bool guessed_correctly = false;
  char* guess = malloc(6*sizeof(char));

  while (num_of_guesses < 6 && !guessed_correctly) {
    //Gets guesses from player
    printf("\n \nInput a 5-letter word and press enter: ");
    scanf("%s", guess);
    printf("You have guessed %s\n", guess);
    num_of_guesses += 1;

    //Process guess
    guessed_correctly = processGuess(answer, guess);
  }
  

  //End game message
  if (guessed_correctly) {
    printf("Congratulations!:) You guessed the correct word in %d times!\n", num_of_guesses);
  } else {
    printf("You have used up yours guesses 0-0 ... the correct word is %s\n", answer);
  }

  for (int i=0; i<wordCount; i++) {
    free(wordsList[i]);
  }
  free(wordsList);
  free(fiveLetterWord);
  free(guess);
  
  return 0;
}