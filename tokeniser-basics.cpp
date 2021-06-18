// a skeleton implementation of a tokeniser

#include "tokeniser-extras.h"

// to shorten the code
using namespace std ;

/////////////////////////////////////////////////////////////////////////

namespace Assignment_Tokeniser
{

    // the current input character
    // the eof marker can be confused with a legal character but not one accepted by our tokeniser
    char ch ;

    // to create a new token we need the characters read since the last token was created
    // the tokeniser always reads one character past a token so ch is never part of a new token
    // when nextch() is called the old ch value must be part of the token currently being parsed
    // this minimal implementation just remembers the spelling of the current token
    static string spelling ;
    //these are the variables used in other functions in the basics.cpp file
    //remember variable is used to remember the variable when you press the tab
    //column and row number will tell how far the program is working on
    //in specific; which row and coloumn the code is working on
    //count variable is used to find the remaining number of the tab to be pressed.
    static char rem = EOF;
    static int curCol = 0;
    static int curRow = 0;
    static int startCol = 1;
    static int startRow = 1;
    int count = 0;

    // create a new token using characters remembered since the last token was created
    // in the final submission tests new_token() will require the correct line and column numbers
    // this will require keeping an entire history of the input, not just the latest spelling
    // NOTE: the first character in the input is in column 1 of line 1
    Token new_token(TokenKind kind)
    {
        // create a new token object and reset spelling
        Token token = new_token(kind,spelling,startRow,startCol) ;

        // reset the spelling
        spelling = "" ;

	    startCol = curCol;
	    startRow = curRow;

        return token ;
    }

    // generate a context string for the given token
    // all lines are shown after the preprocssing of special characters
    // it shows the line before the token, with a '$' marking the end of line,
    // the line containing the token up to the end of the token, and
    // a line of spaces ending with a ^ under the token's first character to mark its position
    // the lines are prefixed by line numbers, right justified in four spaces, followed by ": ", eg "  45: "
    // NOTE: if a token includes newline character(s), only display the token before the first newline followed by a '$'
    // NOTE: this function is not required for the milestone submission
    // NOTE: it is a good idea to build a data structure to remember where each line starts
    string token_context(Token token)
    {

        return "" ;
    }

    // read next character if not at the end of input and update the line and column numbers
    // additional code will be required here to handle preprocessing of '\t' and '\r'
    // in some cases you may wish to remember a character to use next time instead of calling read_char()
    void nextch() {

        if ( ch == EOF ) return ;           // stop reading once we have read EOF
        //spelling += ch ;
        // remember the old ch, it is part of the current token being parsed
        // this checks for the new line in the function
        //when the code has \n, it will acknowledge as new line
        //it will add one to the colomn and keep the input into the one colomn below
        if (ch == '\n') {
            curRow++;
            curCol = 0;
        }

        //store the ch into the spelling
        spelling += ch;

        //when the remember input is the end of the input
        //read the next character
        if (rem == EOF) {
            ch = read_char(); // read the next character
            //when the remember is not the end of the input,
            //change the variable to the ch.
        } else {
            //when, its not end of the input, change the character to remembered variable
            ch = rem;
            //we get space as mush as the count goes on
            if (count != 0) {
                rem = ' ';
                count--;
            } else {
                rem = EOF;
            }
        }


        //now we check the cases for the tab
        //the tab will have '\t'
        if (ch=='\t'){
            //this variable will show how many times you still need to put the space into your token
            int numTab = 4 - curCol % 4;
            //as long as the number is larger than 1
            //remebered variable is tab, and character will be space
            if (numTab > 1) {
                rem = '\t';
            }
            ch = ' ';

        //now we carraige return
        //the carriage return is represented as /r/n in this question.
        //therefore, we first check whether the first character has /r
        //then if it  has the /r, we check for the next character and see if it has /n
        } else if (ch == '\r') {
            ch = read_char();
            if (ch != '\n'){
                rem = ch;
                ch = '\n';
            }
        }

        //add 1 to the column that shows that it is shifting to the next column.
        curCol++;
    }

    // initialise the tokeniser
    void initialise_tokeniser()
    {
                                            // add any other initialisation code you need here
                                            // ...
        ch = '\n' ;                         // initialise ch to avoid accidents
        nextch() ;                          // make first call to nextch to initialise ch using the input
        spelling = "" ;                     // discard the initial '\n', it is not part of the input
    }
}
