// a skeleton implementation of a tokeniser

#include "tokeniser-extras.h"
#include "tokeniser.h"

// to shorten the code
using namespace std ;

/////////////////////////////////////////////////////////////////////////

namespace Assignment_Tokeniser {
    //this will be the token variable that we will be dealing with throughout the program
    static TokenKind new_token_kind;

    // return the next Token object by reading more of the input
    // you must read input using the nextch() function
    // the last character read is in the static variable ch
    // always read one character past the end of the token being returned

    //when the chaacter is either space or new line this fucntion is called
    //assing the tokenind to the new token kind
    static void parse_wspace(TokenKind kind) {
        new_token_kind = kind;
        nextch();
    }

    //when the character is alphabet, we call the parse_identifier function
    //we check the character when it has identifier
    static void parse_identifier() {
        new_token_kind = tk_identifier;
        do nextch(); while (c_have(cg_extends_identifier));
    }

    //parsing the fraction
    //as fraction starts from ., when chekc whether the first character is dot or not
    //after the dot. digits will follow.
    //therefore, we have while loop to check if numbers are followed
    static void parse_fraction() {
        c_mustbe('.');
        while (c_have(cg_digit))c_mustbe(cg_digit);
    }

    //scientific starts from cg_start_of_exponenet which is either e or E
    //then, we check the next character that can either be + or -
    //after the sign, we can have digits either upto three
    static void parse_scientific() {
        c_mustbe(cg_start_of_exponent);
        c_have_next(cg_sign);

        if (!c_have_next('0')) {
            c_mustbe(cg_digit19);
            c_have_next(cg_digit);
            c_have_next(cg_digit);
        }
    }

    //when the character is inter, we call this function
    //like how we did in the identiier function
    //it also check the next character as well
    static void parse_integer() {
        new_token_kind = tk_integer;
        do nextch(); while (c_have(cg_digit));

        if (ch == '.') {
            parse_fraction();
            parse_scientific();
        } else if (c_have(cg_start_of_exponent)) {
            parse_scientific();
        }
    }

    //this is the unique function only for zero as we allocated different case for zero
    //it has the same functionality, but its in the version of the zero.
    static void parse_zero() {
        new_token_kind = tk_integer;
        nextch();

        if (ch == '.') {
            parse_fraction();
            parse_scientific();
        } else if (c_have(cg_start_of_exponent)) {
            parse_scientific();
        }
    }

    //this si the unique function only for add operator
    //according to the grammar, the next character of the "+" has to be "="
    //therefore, we will be checking that using the nexth() and mustbe()
    static void parse_add() {
        nextch();
        c_mustbe('=');
        new_token_kind = tk_add_assign;
    }

    //this is the operation for the sub operator
    //as we did in the parse_add, call nextch and must be to check equal
    //then assign the token
    static void parse_sub() {
        nextch();
        c_mustbe('=');
        new_token_kind = tk_sub_assign;
    }

    //unique operation for the multiplication
    //as we did in the parse_add, call nextch and must be to check equal
    //then assign the token
    static void parse_multi() {
        nextch();
        c_mustbe('=');
        new_token_kind = tk_mult_assign;
    }

    //static function for eol comments
    //get the input as long as /n contitnues
    //while the token is eol comment, we assign the variable to the eol comment
    static void parse_eol() {
        new_token_kind = tk_eol_comment;
        //when the content is cg_eol_comment, call the next ch
        do nextch();while(c_have(cg_eol_comment_char));
        //then, the last character of the comment should be \n
        c_mustbe('\n');
    }

    //static function for the ahdhoc omments
    static void parse_adhoc() {
        new_token_kind = tk_adhoc_comment;
        //as the adhoc can go forever, we accept the input as long as it reaches the end of the file
        //or when it reaches to the */
        //we use the if statment to chekc either they have reach the end or they reached the */

        while (c_have(cg_adhoc_comment_char)) {

            if (c_have_next(EOF)) {
                break;
            } else if (c_have_next('*')) {
                if (c_have_next('/')) {
                    break;
                }
            }

            nextch();
        }
    }

    //unique operation for division
    //when the first word is = the token channge to the /=, which is for division
    //when the first word is /, it should change the parse function to //, which is for eol comment
    //when the first word is *, it should change the parse function to /*, which is for adhoc comment
    static void parse_divi() {
        nextch();
        if (c_have_next('=')) {
            new_token_kind = tk_div_assign;
        } else if (c_have_next('/')) {
            parse_eol();
        } else if (c_have_next('*')) {
            parse_adhoc();
        }
    }

    //unique opperation for equal
    //as we did in the parse_add, call nextch and must be to check equal
    //then assign the token
    static void parse_equal() {
        nextch();
        c_mustbe('=');
        new_token_kind = tk_eq;
        nextch();
    }

    //unique operation for not equal
    //as we did in the parse_add, call nextch and must be to check equal
    //then assign the token
    static void parse_notEqual() {
        nextch();
        c_mustbe('=');
        new_token_kind = tk_not_eq;
        nextch();
    }

    //unique operation for less than
    static void parse_lessThan() {
        //first we check either the next word has '<' or not
        //then, there is a possiblity of same character will follow
        //therefore, we have an if statment once again
        //if that is false the character is '<<'
        nextch();
        if (c_have_next('<')) {
            if (c_have_next('<')) {
                new_token_kind = tk_lshift_l;
            } else {
                new_token_kind = tk_lshift;
            }
        }
    }

    //unique opeartion for more than
    static void parse_moreThan() {
        //As we did int the parse_lessthan function
        //we use follow the same step as we did
        nextch();
        if (c_have_next('>')) {
            if (c_have_next('>')) new_token_kind = tk_rshift_l;
            else new_token_kind = tk_rshift;
        }
    }

    //unique operation for symbol
    //as it is single character, assignt the variable to the token
    static void parse_symbol(TokenKind kind) {
        new_token_kind = kind;
        nextch();
    }


    static void parse_instring() {
	    new_token_kind = tk_string;
	    do nextch(); while (c_have(cg_instring));
    }

    //unique operation for the string
    //first we check that 
    static void parse_string() {
	    nextch();
	    c_have_next('"');
	    c_have_next('\'');
	    parse_instring();
	    c_have_next('\'');
	    c_have_next('"');
	    c_have_next('\'');
    }

    //clearning the adhoc comments for the output
    //we erase the first two and last two as we did
    static string clean_adhoc_comment(string input) {
        string answer = input.substr(2,input.length()-4);
        return answer;
    }

    //cleaning the eol comment for the output
    //we erase the first two characters and the last two characters as we dont need it.
    static string clean_eol_comment(string input) {
        string answer = input.substr(2,input.length()-3);
        return answer;
    }

    //In this function, we will go through switch statement to check different types of variables
    Token next_token() {
        switch (ch)      // ch is always the next char to read
        {
            // this should follow the style used in the workshops ...
            // but remember that the token grammar is different in this assignment
            //
            // add additional case labels here for characters that can start tokens
            // call a parse_*() function to parse the token

            // switch statment to chekc whether next character is  space or newline
            case ' ':
                parse_wspace(tk_space);
                break;
            case '\n':
                parse_wspace(tk_newline);
                break;

                //case for identifier
                //when the character is alphabet, the identifier function is called and the function called
            case 'a'...'z':
            case 'A'...'Z':
            case '$':
                parse_identifier();
                break;

                //case for integer token
                //when the character is intiger, the integer function is claeed and the function called
            case '1'...'9':
                parse_integer();
                break;
            case '0':
                parse_zero();
                break;

                //This section below is the switch statment for symbols
                //we pass the symbols with token parameter
                //as we did in the workshop 5, the single character symbol will be pasted to parse_symbol function
                //However, the symbols with more than two characters will use different function as they have different characteristic
            case '@':
                parse_symbol(tk_at);
                break;
            case '.':
                parse_symbol(tk_stop);
                break;
            case '{':
                parse_symbol(tk_lcb);
                break;
            case '}':
                parse_symbol(tk_rcb);
                break;
            case '(':
                parse_symbol(tk_lrb);
                break;
            case ')':
                parse_symbol(tk_rrb);
                break;
            case '[':
                parse_symbol(tk_lsb);
                break;
            case ']':
                parse_symbol(tk_rsb);
                break;
                //the functions below from here has unique opeartion to be done in the function
                //because they have more than two characters and it is not single character
                //when it only has single character, we cna implement it just by using the parse_symbol function
            case '+':
                parse_add();
                break;
            case '-':
                parse_sub();
                break;
            case '*':
                parse_multi();
                break;
            case '/':
                parse_divi();
                break;
            case '<':
                parse_lessThan();
                break;
            case '>':
                parse_moreThan();
                break;
            case '=':
                parse_equal();
                break;
            case '!':
                parse_notEqual();
                break;

            case EOF:
                new_token_kind = tk_eoi;
                break;
            default:
                c_did_not_find(cg_start_of_token);
        }

        Token token = new_token(new_token_kind);

        if (new_token_kind == tk_identifier) {
            set_token_kind(token, keyword_or_identifier(token_spelling(token)));
        } //have else if for eol adhoc, scientific and string as well
        else if (new_token_kind == tk_eol_comment) {
            //delete the first two character
            //and delete the last two if its eol
            string parameter = token_original(token);
            set_token_spelling(token, clean_eol_comment(parameter));
        } else if (new_token_kind == tk_adhoc_comment) {
            string parameter = token_original(token);
            set_token_spelling(token, clean_adhoc_comment(parameter));
        }

        return token;
    }
}
