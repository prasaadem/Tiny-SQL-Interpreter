#include"LineParser.h"
AppKeyWords LineParser::keyword;

LineParser::LineParser(){
}


LineParser::LineParser(const char* inputtext){
    
    strcpy(inputToParse,inputtext);
    current_char = inputToParse;
    previous_char = current_char;
}

char LineParser::GetNextCharacter(){
    return *current_char;
}

AppTokens LineParser::GetNextToken(){
    previous_char = current_char;
    AppTokens token;

    while(*current_char == '\n' || *current_char == '\t' || *current_char == ' ')
    {
        const char temp = *current_char++;
    }

    if(*current_char == ',')
    {
        token = AppTokens(COMMA, ",");
        current_char++;
    }
    else if(*current_char == '"')
    {

        int index = 0;
        current_char++;
        while(*current_char != '"')
        {
            token.tokenValue[index] = *current_char;
            index++;
            current_char++;
        }
        token.tokenValue[index] = '\0';
        current_char++;
        if(*current_char != ' ' && *current_char != '\n' && *current_char!=',' && *current_char!= ')' && *current_char!='\0') {
            throw string("Incorrect Literal");
        }
        token.tokenType = LITERAL;
    }
    else if(*current_char == '/')
    {
        token = AppTokens(DIVISION,"/");
        current_char++;

    }
    else if(*current_char == '.')
    {
        token = AppTokens(PERIOD,".");
        current_char++;

    }
    else if(*current_char == ')')
    {
        token = AppTokens(ENCLOSINGPAREN,")");
        current_char++;

    }
    else if(*current_char == ']')
    {
        token = AppTokens(ENCLOSINGSQPAREN,"]");
        current_char++;

    }
    else if(*current_char == '(')
    {
        token = AppTokens(OPENINGPAREN,"(");
        current_char++;

    }
    else if(*current_char == '[')
    {
        token = AppTokens(OPENINGSQPAREN,"[");
        current_char++;

    }
    else if(*current_char == '-')
    {
        token = AppTokens(MINUS,"-");
        current_char++;

    }
    else if(*current_char == '+')
    {
        token = AppTokens(ADDITION,"+");
        current_char++;

    }
    else if(*current_char == ';')
    {
        token = AppTokens(SEMICOLON,";");
        current_char++;

    }
    else if(*current_char == '*')
    {
        token = AppTokens(MULTIPLY,"*");
        current_char++;

    }
    else if(*current_char == '=' || *current_char == '>' || *current_char == '<')
    {
        token.tokenValue[0] = *current_char;
        token.tokenValue[1] = '\0';
        token.tokenType = COMPOPERATOR;
        current_char++;
    }
    else if(isalnum(*current_char))
    {
        int index =0;
        bool all_digit = true;
        bool not_an_identifier = false;
        if(isdigit(*current_char))
            not_an_identifier = true; 
            while(isalnum(*current_char))
            {
                if(isalpha(*current_char)) {
                    all_digit = false; 
                }
                token.tokenValue[index] = *current_char;
                current_char++;
                index++;
            }
            token.tokenValue[index] = '\0';
            int type;
            if(all_digit)
            {
                token.tokenType = INTEGER;
            }
            else if((type = keyword.KeywordToken(token.tokenValue))!=0) {
                token.tokenType = type;
            }
            else if(type == 0)
            {
                if(!not_an_identifier) {
                    token.tokenType = IDENTIFIER;
                }
                else {
                    token.tokenType = UNDEFINEDTOK;
                    strcat(token.tokenValue,"Error - Invalid Token");
                }
            }
        }
        else if(*current_char == '\0')
        {
            token = AppTokens(EOF,"End of File");
        }
        else if(*current_char!= '\n' && *current_char!=' ')
        {
            token.tokenType = UNDEFINEDTOK;
            token.tokenValue[0] = *current_char;
            token.tokenValue[1] = '\0';
            strcat(token.tokenValue, "ERROR - Undefined Token");
            current_char++;
        }
    return token;
}

void LineParser::RestorePreviousToken()
{
    current_char = previous_char;
}

const char* LineParser::getCurrentCharacter() {
    return current_char;
}

void LineParser::LineParserReset(const char* reset_character){
    current_char = reset_character;
    previous_char = current_char;
}
