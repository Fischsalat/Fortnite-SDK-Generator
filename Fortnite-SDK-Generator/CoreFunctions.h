#pragma once
#include "Global.h"


static std::string MakeValidCppName(std::string name)
{
	for (int i = 0; i < name.length(); i++)
	{
		for (int i = 0; i < name.length(); i++)
		{
			if (   name[i] == ' '
				|| name[i] == '.'
				|| name[i] == ','
				|| name[i] == ':'
				|| name[i] == ';'
				|| name[i] == '-'
				|| name[i] == '+'
				|| name[i] == '<'
				|| name[i] == '>'
				|| name[i] == '&'
				|| name[i] == '|'
				|| name[i] == '^'
				|| name[i] == '#'
				|| name[i] == '%'
				|| name[i] == '@'
				|| name[i] == '/'
				|| name[i] == '\\'
				|| name[i] == '\''
				|| name[i] == '"'
				|| name[i] == ')'
				|| name[i] == '('
				|| name[i] == '['
				|| name[i] == ']'
				|| name[i] == '{'
				|| name[i] == '}')
			{
				name[i] == '_';
			}
		}
		return name;
	}
}

