#include "parser.h"
#include<cmath>
double string2double(std::string num)
{
	int status = 0;
	double sum = 0;
	int floor = 1;
	for (auto c : num)
	{
		if (c == '.')
		{
			status++;
		}
		else if (status == 0)
		{
			sum = 10 * sum + (c - '0');
		}
		else if (status==1)
		{
			sum += (c - '0')*pow(10, -(floor++));
		}
		else
		{
			throw "unexpected \".\"";
		}
	}
}


Type* Parser::deal_expression()
{
	Type* result = nullptr;
	std::string str = lexer.get_next_token();
	if (str[0] == '(')
	{
		str = lexer.get_next_token();
		if (str == "define")
		{
			result = deal_define();
		}
		else if (str == "lambda")
		{
			result = deal_lambda();
		}
		else //apply
		{
			result = new Type_Apply(str, deal_parms_value());
		}
		lexer.get_next_token(); //)
	}
	else
	{
		if (str[0] >= '0'&&str[0] <= '9')  //num
		{
			Type_BaseType* type_num = new Type_BaseType(BASE_TYPE_DOUBLE);
			type_num->u.value_double = string2double(str);
			result = type_num;
		}
		else  //variable
		{
			return new Type_Variable(str);
		}
	}
	return result;
}


Type* Parser::deal_lambda()
{
	return new Type_Procedure(deal_parms_name(), deal_expression());
}

Type* Parser::deal_define()
{
	std::string name = lexer.get_next_token();
	return new Type_Define(name, deal_expression());
}

std::vector<std::string> Parser::deal_parms_name()
{
	std::vector<std::string> result;
	lexer.get_next_token( ); //(
	std::string str;
	while ((str = lexer.get_next_token()) != ")")
		result.push_back(str);
	return result;
}
Type* Parser::deal_parm_value()
{
	return (deal_expression());
}
std::vector<Type*> Parser::deal_parms_value()
{
	std::vector<Type*> parms;
	while ((lexer.get_next_token()) != ")")
	{
		lexer.put_formal_token();
		parms.push_back(deal_expression());
	}
	lexer.put_formal_token();
	return parms;
}
