#pragma once

#include <string>
#include <vector>
#include <functional>

enum class ParserToken
{
	WORD,
	INST,
	THING,
	OTHER,
};

enum class ParserCode
{
	OK,

	// erros
	INVALID_FIRST_TOKEN,
	INVALID_SECOND_TOKEN,
	OTHER_RANDOM_THING,
};

class ParserResult
{
	private:
		std::string msg;
		ParserCode stored;

	public:
		ParserResult();
		ParserResult(ParserCode code);
		ParserResult(ParserCode code, std::string message);

		bool is_ok();
		std::string reason() const;
		void match(std::function<void()> success, std::function<void()> failure);
};

class Parser
{
	private:
		std::vector<ParserToken> content;

	public:
		ParserResult evaluate(std::string expression);
		void show();
};

