#include "Util.h"

std::string ToString(int Input)
{
	std::string Result;
	_itoa(Input, &Result[0], 10);
	Result=std::string(Result.c_str());

	return Result;
}

std::vector<std::string> Split(std::string Input, char Delimiter)
{
	std::vector<std::string> Segments;
	std::string Temp;
	for(unsigned int x=0; x<Input.size(); x++)
	{
		if(Input[x]==Delimiter)
		{
			if(Temp.size()>0)
			{
				Segments.push_back(Temp);
				Temp.clear();
			}
		}
		else
		{
			Temp+=Input[x];
		}
	}
	if(Temp.size()>0)
	{
		Segments.push_back(Temp);
	}
	return Segments;
}