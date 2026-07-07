#include "parser.h"
#include <iostream>
#include <format>

ParserResult::ParserResult()
{
	stored = ParserCode::OK;
}

ParserResult::ParserResult(ParserCode code)
{
	stored = code;
}

ParserResult::ParserResult(ParserCode code, std::string message)
{
	stored = code;
	msg = message;
}

bool ParserResult::is_ok()
{
	if (stored != ParserCode::OK) {
		return false;
	}

	return true;
}

std::string ParserResult::reason() const
{
	return msg;
}

void ParserResult::match(std::function<void()> success, std::function<void()> failure)
{
	if (is_ok()) {
		success();
	}

	failure();
}

ParserResult Parser::evaluate(std::string expression)
{
	if (expression == "WORD") {
		content.push_back(ParserToken::WORD);
		return ParserResult();
	}

	else if (expression == "INST") {
		content.push_back(ParserToken::INST);
		return ParserResult();
	}

	else if (expression == "THING") {
		content.push_back(ParserToken::THING);
		return ParserResult();
	}

	else if (expression == "OTHER") {
		content.push_back(ParserToken::OTHER);
		return ParserResult();
	}

	else {
		return ParserResult(ParserCode::OTHER_RANDOM_THING, 
				std::format("Invalid token received: {}", expression));
	}
}

void Parser::show()
{
	std::cout << "done, " <<  content.size() << " tokens parsed." << std::endl;
}

