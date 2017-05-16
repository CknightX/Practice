#ifndef _PARSER_H
#define _PARSER_H
#include "type.h"
/*�﷨������*/

#include "lexer.h"

class Parser
{
public:
	Parser() :root(nullptr){}
	Parser(Lexer& lexer);
	Type* get_ast();
private:
	Type* deal_expression();
	Type* deal_define();
	Type* deal_base_procedure();
	Type* deal_procedure();
	Type* deal_apply();
	Type* deal_variable();
	Type* deal_lambda();
	Type* deal_num();

	std::vector<std::string> deal_parms_name();
	std::vector<Type*> deal_parms_value();
	Type* deal_parm_value();

	Type* root;
	Lexer lexer;

};
#endif