/*-------------------------------------------------------------------*/
/* decomment.c                                                       */
/* Author: Isaac Wolfe                                               */
/* netid: iwolfe                                                     */
/*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

enum State {START_STATE, POSSIBLE_COMMENT_STATE, COMMENT_STATE, 
  POSSIBLE_END_COMMENT_STATE, STRING_LITERAL_STATE, 
  CHAR_LITERAL_STATE, STRING_ESCAPED_CHAR_STATE, 
  CHAR_ESCAPED_CHAR_STATE};

/*-------------------------------------------------------------------*/
/* Function Declarations                                             */
/*-------------------------------------------------------------------*/

/* Analyze and prints current character and returns the next state of
 the dfa.*/
  enum State handleStartState(char c);

/* Analyze the current character and returns the next state in the 
   case that the previous character was beginning a comment: "/" */
  enum State handlePossibleCommentState(char c);

/* Analyze the current charcter and return the next state once the 
   program is within a comment. Prints new lines/spacing */
  enum State handleCommentState(char c);

/* Analyze the current character and return the next state once the 
   program encounters the possible end to a commment */
  enum State handlePossibleEndCommentState(char c);

/* Analyze the current character, print the proper output, and 
   return the next state in the case of encountering a string */
  enum State handleStringLiteralState(char c);

/* Analyze the current character, print the proper output, and 
   return the next state in the case of encountering a char */
  enum State handleCharLiteralState(char c);

/* Analyze the current character in a string and allow it to 
   "escape" (have no function) and return back to the string state */
  enum State handleStringEscapedCharState(char c);

  /* Analyze the current character in a char literal and allow it 
   to "escape" (have no function) and return back to the char state */
  enum State handleCharEscapedCharState(char c);

/*-------------------------------------------------------------------*/
/* Function Definitions                                              */
/*-------------------------------------------------------------------*/

/* Read through input text from stdin. Remove all comments from the 
   input while inserting the proper number of spaces and newline 
   characters based on what was removed. Also handle literals, 
   escaped characters, and returning the line of an unterminated
   comment. Returns error if text ends with an unterminated comment. */

  int main(void)
  { 

    /* current character, current state, and local variables to
       keep track of newlines and unterminated comment line */
    int c;
    enum State eState = START_STATE;
    int iTotalNewLines = 1;
    int iUnterminatedCommentLine;

    while ((c = getchar()) != EOF)
    {

      /* Peek at each character and see it its a newline.
         Tally it if so. */
      if (c == '\n')
      {
        iTotalNewLines++;
      }

      switch (eState)
      {
        case START_STATE:
           eState = handleStartState(c);
           break;
        case POSSIBLE_COMMENT_STATE:
           eState = handlePossibleCommentState(c);
           /* store line comment begins on */
           if (eState == COMMENT_STATE) 
           {
            iUnterminatedCommentLine = iTotalNewLines;
           }
           break;
        case COMMENT_STATE:
            eState = handleCommentState(c);
            break;
        case POSSIBLE_END_COMMENT_STATE:
            eState = handlePossibleEndCommentState(c);
            /* make comment start line -1 if comment is exited */
            if (eState == START_STATE) iUnterminatedCommentLine = -1;
            break;
        case STRING_LITERAL_STATE:
            eState = handleStringLiteralState(c);
            break;
        case CHAR_LITERAL_STATE:
            eState = handleCharLiteralState(c);
            break;
        case STRING_ESCAPED_CHAR_STATE:
            eState = handleStringEscapedCharState(c);
            break;
        case CHAR_ESCAPED_CHAR_STATE:
            eState = handleCharEscapedCharState(c);
            break;
      }
    }

    /* Print '/' if dfa ends in POSSIBLE_COMMENT_STATE */
    if (eState == POSSIBLE_COMMENT_STATE) putchar('/');

    /* Print error and return failure if dfa ends with an
       unterminated comment,*/
    if ((eState == COMMENT_STATE) || 
      (eState == POSSIBLE_END_COMMENT_STATE))
    {
      fprintf(stderr, "Error: line %d: unterminated comment\n", 
        iUnterminatedCommentLine);

      return 1;
    }

    return 0;
  }

/*-------------------------------------------------------------------*/

  enum State handleStartState(char c)
  { enum State state;

    /* detect start of comment */
    if (c == '/') {
      state = POSSIBLE_COMMENT_STATE;
    }

    /* detect start if string */
    else if (c == '\"')
    {
     putchar(c);
     state = STRING_LITERAL_STATE;
   }

   /* detect start of char */
   else if (c == '\'')
   {
     putchar(c);
     state = CHAR_LITERAL_STATE;
   }

   /* all other cases, recall Start State */
   else {
     putchar(c);
     state = START_STATE;
   }

   return state;
 }

/*-------------------------------------------------------------------*/

 enum State handlePossibleCommentState(char c)
 { enum State state;

  /* detect start of comment */
  if (c == '*') {
    putchar(' ');
    state = COMMENT_STATE;
  }

  /* detect possible comment */
  else if (c == '/')
  {
   putchar('/');
   state = POSSIBLE_COMMENT_STATE;
 }

 /* detect start of string */
 else if (c == '\"')
 {
   putchar('/');
   putchar(c);
   state = STRING_LITERAL_STATE;
 }

 /* detect start of char */
 else if (c == '\'') {
   putchar('/');
   putchar('\'');
   state = CHAR_LITERAL_STATE;
 }

 /* all other cases, return to start state */
 else {
   putchar('/');
   putchar(c);
   state = START_STATE;
 }

 return state;
}

/*-------------------------------------------------------------------*/

enum State handleCommentState(char c)
{ enum State state;

  /* Print newline characters */
  if (c == '\n') putchar('\n');

  /* detect possible end comments */
  else if (c == '*') 
  {
    state = POSSIBLE_END_COMMENT_STATE;
  }

  /* otherwise, continue in comment state */
  else {
   state = COMMENT_STATE;
 }

 return state;
}

/*-------------------------------------------------------------------*/

enum State handlePossibleEndCommentState(char c)
{ enum State state;

  /* print newline characters */
  if (c == '\n') putchar('\n');

  /* return to start state if comment ends */
  if (c == '/') state = START_STATE; 
  
  /* stay in end comment state for another star */
  else if (c == '*') state = POSSIBLE_END_COMMENT_STATE;

  /* otherwise, return to comment state */
  else {
   state = COMMENT_STATE;
 }

 return state;
}

/*-------------------------------------------------------------------*/

enum State handleStringLiteralState(char c)
{ enum State state;

  /* return to start state if stirng ends */
  if (c == '\"') {
    putchar(c);
    state = START_STATE;
  }

  /* detect escaped character marker */
  else if (c == '\\') {
    putchar(c);
    state = STRING_ESCAPED_CHAR_STATE;
  }

  /* otherwise, stay in string literal state */
  else {
    putchar(c);
    state = STRING_LITERAL_STATE;
  }

  return state;
}

/*-------------------------------------------------------------------*/

enum State handleCharLiteralState(char c)
{ enum State state;

  /* return to start state is char literal ends */
  if (c == '\'') {
    putchar(c);
    state = START_STATE;
  }

  /* detect escae char marker */
  else if (c == '\\') {
    putchar(c);
    state = CHAR_ESCAPED_CHAR_STATE;
  }

  /* otherwise, stay in char literal state */
  else {
    putchar(c);
    state = CHAR_LITERAL_STATE;
  }

  return state;
}

/*-------------------------------------------------------------------*/

enum State handleStringEscapedCharState(char c)
{ enum State state;

  /* print charater */
  putchar(c);

  /* return to previous state (essentially skip current char) */
  state = STRING_LITERAL_STATE;

  return state;
}

enum State handleCharEscapedCharState(char c)
{ enum State state;

  /* print charater */
  putchar(c);

  /* return to previous state (essentially skip current char) */
  state = CHAR_LITERAL_STATE;

  return state;
}







