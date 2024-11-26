#include "stdafx.h"
#include "StringTable.h"
#include "rapidcsv.h"
#include <io.h>

std::wstring StringTable::Undefined = L"Undefined Id";

bool StringTable::Load()
{
	//20241126 황규영 파일 유무 확인 후 없을 시 false로 날려버림
	if (_access(filePath.c_str(), 0) == -1)
	{
		return false;
	}

	Release();

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		std::vector<std::string> strings = doc.GetRow<std::string>(i);
		auto it = table.find(strings[0]);
		if (it != table.end())
		{
			std::cout << "스트링 테이블 키 중복!" << std::endl;
			return false;
		}

		for (int j = 0; j < (int)Languages::Count; ++j)
		{
			auto& vec = table[strings[0]];
			vec.resize((int)Languages::Count);
			vec[j] = converter.from_bytes(strings[j + 1]);
			// vec.push_back(converter.from_bytes(strings[j + 1]));
		}
	}
	return true;
}

void StringTable::Release()
{
	table.clear();
}

const std::wstring& StringTable::Get(const std::string& id)
{
	return Get(id, Variables::currentLang);
}

const std::wstring& StringTable::Get(const std::string& id, Languages lang)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}
	return (find->second)[(int)lang];
}
