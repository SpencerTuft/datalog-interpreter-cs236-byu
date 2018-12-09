//
// Created by Spencer Tuft on 10/16/18.
//

#ifndef DATALOG_PARSER_CS236_BYU_DATALOGLEXER_H
#define DATALOG_PARSER_CS236_BYU_DATALOGLEXER_H

#include "./Lexer.h"
#include "./Processes.h"
#include "./Routes.h"

class DatalogLexer : public Lexer {
 private:
  std::vector<State> states {
      State("WHITESPACE", rwhitespace, pwhitespace),
      State("COLON", rcolon, pcolon),
      State("COMMA", rcomma, pcomma),
      State("PERIOD", rperiod, pperiod),
      State("Q_MARK", rqmark, pqmark),
      State("MULTIPLY", rmultiply, pmultiply),
      State("ADD", radd, padd),
      State("STRING", rstring, pstring),
      State("POSSIBLE_STRING_END", nullptr, ppstring),
      State("IDENTIFIER", ridentifier, pidentifier),
      State("COMMENT", rcomment, pcomment),
      State("PAREN", rparen, pparen),
      State("UNDEFINED", nullptr, pund)
  };

 public:
  DatalogLexer() {
    initializeStates(std::move(states));
  };
};

#endif //DATALOG_PARSER_CS236_BYU_DATALOGLEXER_H
