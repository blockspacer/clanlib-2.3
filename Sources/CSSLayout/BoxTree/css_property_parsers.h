
#pragma once

class CL_CSSProperty2;
class CL_CSSPropertyParser;
class CL_CSSBoxProperties;

class CL_CSSPropertyParsers
{
public:
	CL_CSSPropertyParsers();
	~CL_CSSPropertyParsers();

	void parse(CL_CSSBoxProperties &properties, const CL_CSSProperty2 &property);

private:
	void add(CL_CSSPropertyParser *parser);

	std::vector<CL_CSSPropertyParser *> parsers;
	std::map<CL_String, CL_CSSPropertyParser *> name_to_parser;
};
