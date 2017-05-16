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
	std::string c = lexer.get_next_token();
	std::string str = lexer.get_next_token();
	if (c[0] == '(')
	{
		str = lexer.get_next_token();
		if (str == "define")
		{
			result = deal_define();
		}
		if (str == "lambda")
		{
			result = deal_lambda();
		}
		else //apply
		{
			result = new Type_Apply(str, deal_parms_value());
		}
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

		}
	}
	lexer.get_next_token(); //)
	return result;
}


Type* Parser::deal_lambda()
{
	return new Type_Procedure(deal_parms_name(), deal_expression());
}

Type* Parser::deal_define()
{
	std::string name = lexer.get_next_token();
	return new Type_Variable(name, deal_expression());
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

}
