#pragma once
//
// #include
//
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <exception>
#include <algorithm>
#include <string>
#include <vector>
#include <codecvt>
#include <random>
#include <functional>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//
// namespace
//
namespace ItSoftware
{
namespace Linux
{
//
// using
//
using std::string;
using std::stringstream;
using std::vector;
using std::wstring;
using std::wstringstream;

//
// Macros
//
#define DECIMALTOSTRING2(d) #d
#define DECIMALTOSTRING1(d) DECIMALTOSTRING2(d)
#define ERR_AT_LOCATION(e) __FILE__ "(" DECIMALTOSTRING1(__LINE__) "): " #e
#define ERROR_HERE __FILE__ "(" DECIMALTOSTRING2(__LINE__) "): "
#define CHAR_BIT 8
#define BitsCount(val) (sizeof(val) * CHAR_BIT)
#define Shift(val, steps) (steps % BitsCount(val))
#define ROL(val, steps) ((val << Shift(val, steps)) | (val >> (BitsCount(val) - Shift(val, steps))))
#define ROR(val, steps) ((val >> Shift(val, steps)) | (val << (BitsCount(val) - Shift(val, steps))))
#define AddFlag(val, flag) (val | flag)
#define RemoveFlag(val, flag) (val & ~flag)

//
// Function: ItsAddFlag
//
template <typename T>
T ItsAddFlag(T value, T flag)
{
	return value | flag;
}

//
// Function:: ItsRemoveFlag
//
template <typename T>
T ItsRemoveFlag(T value, T flag)
{
	return value & ~flag;
}

//
// struct: ItsTime
//
struct ItsTime
{
	//
	// Split ms to its parts
	//
	static void SplitMsToHourMinuteSeconds(size_t time_ms, size_t *part_hours, size_t *part_minutes, size_t *part_seconds, size_t *part_ms)
	{
		size_t seconds = time_ms / 1000;

		*part_hours = 0;
		*part_minutes = 0;
		;
		*part_seconds = 0;
		*part_ms = 0;

		*part_hours = seconds / 3600;
		*part_minutes = (seconds - (*part_hours * 3600)) / 60;
		*part_seconds = seconds - (*part_hours * 3600) - (*part_minutes * 60);
		*part_ms = time_ms - (*part_seconds * 1000) - (*part_minutes * 60 * 1000) - (*part_hours * 3600 * 1000);
	}

	static void SplitHourToYearWeekDayHour(size_t houIn, size_t *houRest, size_t *day, size_t *week, size_t *year)
	{
		*houRest = houIn;

		*day = houIn / 24;
		*week = *day / 7;
		*year = *week / 52;

		*day -= (*week * 7);

		*week -= (*year * 52);

		*houRest -= *week * 7 * 24;
		*houRest -= *day * 24;
		*houRest -= *year * 52 * 7 * 24;
	}

	static string RenderMsToFullString(size_t milliseconds, bool bWithMilliseconds)
	{
		size_t hou = 0;
		size_t min = 0;
		size_t sec = 0;
		size_t ms = 0;
		ItsTime::SplitMsToHourMinuteSeconds(milliseconds, &hou, &min, &sec, &ms);

		size_t houRest = 0;
		size_t day = 0;
		size_t week = 0;
		size_t year = 0;
		ItsTime::SplitHourToYearWeekDayHour(hou, &houRest, &day, &week, &year);

		stringstream ss;
		if (year > 0)
		{
			if (year == 1)
			{
				ss << year << " year ";
			}
			else
			{
				ss << year << " years ";
			}
		}
		if (week > 0 || year > 0)
		{
			if (week == 1 || week == 0)
			{
				ss << week << " week ";
			}
			else
			{
				ss << week << " weeks ";
			}
		}
		if (day > 0 || week > 0 || year > 0)
		{
			if (day == 1 || day == 0)
			{
				ss << day << " day ";
			}
			else
			{
				ss << day << " days ";
			}
		}
		if (houRest > 0 || day > 0 || week > 0 || year > 0)
		{
			if (houRest == 1 || houRest == 0)
			{
				ss << houRest << " hour ";
			}
			else
			{
				ss << houRest << " hours ";
			}
		}

		if (min < 10)
		{
			ss << "0" << min << ":";
		}
		else
		{
			ss << min << ":";
		}
		if (sec < 10)
		{
			ss << "0" << sec;
		}
		else
		{
			ss << sec;
		}

		if ( bWithMilliseconds ) {
			if (ms < 10 ) {
				ss << ".00" << ms;
			}
			else if ( ms < 100 ) {
				ss << ".0" << ms;
			}
			else {
				ss << ms;
			}
		}

		string str = ss.str();

		return str;
	}
};

//
// struct: ItsString
//
struct ItsString
{
	static vector<string> Split(string data, string token)
	{
		vector<string> output;
		size_t pos = string::npos; // size_t to avoid improbable overflow
		do
		{
			pos = data.find(token);
			output.push_back(data.substr(0, pos));
			if (string::npos != pos)
				data = data.substr(pos + token.size());
		} while (string::npos != pos);

		return output;
	}
	// to lower case
	static string ToLowerCase(string &s)
	{
		std::transform(s.begin(), s.end(), s.begin(), tolower);
		return s;
	}

	// to upper case
	static string ToUpperCase(string &s)
	{
		std::transform(s.begin(), s.end(), s.begin(), toupper);
		return s;
	}

	// trim from left
	static string TrimLeft(string &s, const char *t = " \t\n\r\f\v")
	{
		s.erase(0, s.find_first_not_of(t));
		return s;
	}

	// trim from right
	static string TrimRight(string &s, const char *t = " \t\n\r\f\v")
	{
		s.erase(s.find_last_not_of(t) + 1);
		return s;
	}

	// trim from left & right
	static string Trim(string &s, const char *t = " \t\n\r\f\v")
	{
		auto val = TrimRight(s, t);
		return TrimLeft(val, t);
	}

	// left count chars
	static string Left(string &s, unsigned int count)
	{
		if (s.size() == 0 || count <= 0)
		{
			return string("");
		}

		if (count >= s.size())
		{
			return s;
		}

		stringstream ss;
		for (unsigned int i = 0; i < count; i++)
		{
			ss << s[i];
		}

		string str = ss.str();
		return str;
	}

	// mid index, count chars
	static string Mid(string &s, unsigned int index, unsigned int count)
	{
		if (s.size() == 0 || count <= 0 || index < 0 || index >= s.size())
		{
			return string("");
		}

		if (index + count >= s.size())
		{
			stringstream ss;
			for (size_t i = index; i < s.size(); i++)
			{
				ss << s[i];
			}
			string str = ss.str();
			return str;
		}

		stringstream ss;
		for (size_t i = index; i < index + count; i++)
		{
			ss << s[i];
		}

		string str = ss.str();
		return str;
	}

	// right count chars
	static string Right(string &s, unsigned int count)
	{
		if (s.size() == 0 || count <= 0)
		{
			return string("");
		}

		if (count >= s.size())
		{
			return s;
		}

		stringstream ss;
		for (size_t i = s.size() - count; i < s.size(); i++)
		{
			ss << s[i];
		}

		string str = ss.str();
		return str;
	}

	//
	//
	//
	static string Replace(string &s, string &replace, string &replace_with)
	{
		if (s.size() == 0 || replace.size() == 0 || replace.size() > s.size())
		{
			return string("");
		}

		stringstream ss;
		size_t index = s.find(replace);
		if (index == wstring::npos)
		{
			return s;
		}
		while (index != wstring::npos)
		{
			ss << ItsString::Left(s, (int)index);
			ss << replace_with;
			ss << ItsString::Right(s, (int)(s.size() - index - replace.size()));

			index = s.find(replace, index + replace.size());
		}

		string str = ss.str();
		return str;
	}
};

//
// struct: ItsConvert
//
struct ItsConvert
{
	static int ToInt(string str)
	{
		return atoi(str.c_str());
	}

	static int ToIntFromHex(string str)
	{
		char * p;
    	int n = strtol( str.c_str(), &p, 16 );
    	if ( *p != 0 ) { //my bad edit was here
        	return 0;
    	}
		return n;
	}

	static long ToLong(string str)
	{
		return atol(str.c_str());
	}

	static long long ToLongLong(string str)
	{
		return atoll(str.c_str());
	}

	static double ToFloat(string str)
	{
		return atof(str.c_str());
	}

	static string ToString(bool flag)
	{
		if (flag)
		{
			return string("true");
		}
		return string("false");
	}

	static string ToString(int number)
	{
		stringstream ss;
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToHexString(int number, bool uppercase, int width, char fillChar, bool showBase)
	{
		stringstream ss;
		ss << std::hex;
		if (showBase)
		{
			ss << "0x";
		}
		if (uppercase)
		{
			ss << uppercase;
		}
		if (width > 0)
		{
			ss << std::setfill(fillChar) << std::setw(width);
		}
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToString(long number)
	{
		stringstream ss;
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToHexString(long number, bool uppercase, int width, char fillChar, bool showBase)
	{
		stringstream ss;
		ss << std::hex;
		if (showBase)
		{
			ss << "0x";
		}
		if (uppercase)
		{
			ss << uppercase;
		}
		if (width > 0)
		{
			ss << std::setfill(fillChar) << std::setw(width);
		}
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToString(long long number)
	{
		stringstream ss;
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToStringFormatted(long long number)
	{
		string txt = ItsConvert::ToString(number);

		stringstream ss;

		int groupSize = 3;
		int i = groupSize;
		size_t iCount = 0;
		size_t iSize = txt.size();
		for (auto itr = txt.rbegin(); itr < txt.rend(); itr++)
		{
			if (i-- > 0)
			{
				ss << *itr;
				iCount++;
			}
			else
			{
				if (iCount < iSize)
				{
					ss << " ";
					ss << *itr;
					iCount++;
					i = groupSize - 1;
				}
				else
				{
					i = groupSize;
				}
			}
		}

		txt = ss.str();
		stringstream ss2;
		for (auto itr = txt.rbegin(); itr < txt.rend(); itr++)
		{
			ss2 << *itr;
		}

		string retVal = ss2.str();

		return retVal;
	}

	static string ToHexString(long long number, bool uppercase, int width, char fillChar, bool showBase)
	{
		stringstream ss;
		ss << std::hex;
		if (showBase)
		{
			ss << "0x";
		}
		if (uppercase)
		{
			ss << uppercase;
		}
		if (width > 0)
		{
			ss << std::setfill(fillChar) << std::setw(width);
		}
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToString(float number)
	{
		stringstream ss;
		ss << std::setprecision(std::numeric_limits<float>::digits10) << number;

		string tos = ss.str();
		return tos;
	}

	static string ToHexString(float number, bool uppercase, int width, char fillChar, bool showBase)
	{
		stringstream ss;
		ss << std::hex;
		if (showBase)
		{
			ss << "0x";
		}
		if (uppercase)
		{
			ss << uppercase;
		}
		if (width > 0)
		{
			ss << std::setfill(fillChar) << std::setw(width);
		}
		ss << std::setprecision(std::numeric_limits<float>::digits10) << number;

		string tos = ss.str();
		return tos;
	}

	static string ToString(double number)
	{
		stringstream ss;
		ss << std::setprecision(std::numeric_limits<double>::digits10) << number;

		string tos = ss.str();
		return tos;
	}

	static string ToHexString(double number, bool uppercase, int width, char fillChar, bool showBase)
	{
		stringstream ss;
		ss << std::hex;
		if (showBase)
		{
			ss << "0x";
		}
		if (uppercase)
		{
			ss << uppercase;
		}
		if (width > 0)
		{
			ss << std::setfill(fillChar) << std::setw(width);
		}
		ss << std::setprecision(std::numeric_limits<double>::digits10) << number;

		string tos = ss.str();
		return tos;
	}

	static string ToString(unsigned int number)
	{
		stringstream ss;
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToHexString(unsigned int number, bool uppercase, int width, char fillChar, bool showBase)
	{
		stringstream ss;
		ss << std::hex;
		if (showBase)
		{
			ss << "0x";
		}
		if (uppercase)
		{
			ss << uppercase;
		}
		if (width > 0)
		{
			ss << std::setfill(fillChar) << std::setw(width);
		}
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToString(unsigned long number)
	{
		stringstream ss;
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToHexString(unsigned long number, bool uppercase, int width, char fillChar, bool showBase)
	{
		stringstream ss;
		ss << std::hex;
		if (showBase)
		{
			ss << "0x";
		}
		if (uppercase)
		{
			ss << uppercase;
		}
		if (width > 0)
		{
			ss << std::setfill(fillChar) << std::setw(width);
		}
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToString(unsigned long long number)
	{
		stringstream ss;
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToHexString(unsigned long long number, bool uppercase, int width, char fillChar, bool showBase)
	{
		stringstream ss;
		ss << std::hex;
		if (showBase)
		{
			ss << "0x";
		}
		if (uppercase)
		{
			ss << uppercase;
		}
		if (width > 0)
		{
			ss << std::setfill(fillChar) << std::setw(width);
		}
		ss << number;

		string tos = ss.str();
		return tos;
	}

	static string ToString(tm dateTime)
	{
		stringstream ss;
		ss << std::setfill('0') << std::setw(4) << (dateTime.tm_year + 1900);
		ss << "-" << std::setfill('0') << std::setw(2) << (dateTime.tm_mon + 1);
		ss << "-" << std::setfill('0') << std::setw(2) << dateTime.tm_mday;
		ss << "T";
		ss << std::setfill('0') << std::setw(2) << dateTime.tm_hour;
		ss << ":" << std::setfill('0') << std::setw(2) << dateTime.tm_min;
		ss << ":" << std::setfill('0') << std::setw(2) << dateTime.tm_sec;

		string tos = ss.str();
		return tos;
	}

	static tm ToTM(string dateTime)
	{
		tm t = {0};

		t.tm_year = ItsConvert::ToInt(dateTime.substr(0, 4)) - 1900;
		t.tm_mon = ItsConvert::ToInt(dateTime.substr(5, 2)) - 1;
		t.tm_mday = ItsConvert::ToInt(dateTime.substr(8, 2));
		t.tm_hour = ItsConvert::ToInt(dateTime.substr(11, 2));
		t.tm_min = ItsConvert::ToInt(dateTime.substr(14, 2));
		t.tm_sec = ItsConvert::ToInt(dateTime.substr(17, 2));

		return t;
	}

	static string ToLowerCase(string text)
	{
		std::transform(text.begin(), text.end(), text.begin(), tolower);
		return text;
	}

	static string ToUpperCase(string text)
	{
		std::transform(text.begin(), text.end(), text.begin(), toupper);
		return text;
	}

	static bool ToBool(string flag)
	{
		flag = ItsConvert::ToLowerCase(flag);

		if (!strcmp(flag.c_str(), "true"))
		{
			return true;
		}
		else if (!strcmp(flag.c_str(), "1"))
		{
			return true;
		}
		else if (!strcmp(flag.c_str(), "yes"))
		{
			return true;
		}
		else if (!strcmp(flag.c_str(), "on"))
		{
			return true;
		}
		return false;
	}

	static string ToString(vector<long long> pks)
	{
		stringstream ss;
		bool bAddSep = false;
		for (auto &pk : pks)
		{
			if (bAddSep)
			{
				ss << L";";
			}
			ss << pk;
			bAddSep = true;
		}
		string tos = ss.str();
		return tos;
	}

	static vector<long long> ToPK(string pks)
	{
		stringstream ss;
		ss.str(string(pks.begin(), pks.end()));

		string item;
		vector<long long> topk;
		while (getline(ss, item, ';'))
		{
			if (!item.empty())
			{
				topk.push_back(ItsConvert::ToLongLong(string(item.begin(), item.end())));
			}
		}

		return topk;
	}
};

//
// struct: ItsRandom
//
struct ItsRandom
{
	static int RandomInt(int min, int max)
	{
		static bool isInit = false;
		if (!isInit)
		{
			srand((unsigned int)time(nullptr));
			isInit = true;
		}

		if (max < min)
		{
			unsigned int tempForSwap = max;
			max = min;
			min = tempForSwap;
		}

		return (rand() % max) + min;

		/*
			static default_random_engine re { };		
			using Dist = uniform_int_distribution<int>;
			static Dist uid { };
			
			return uid( re, Dist::param_type { min, max } );
			*/
	}

	static double RandomDouble(double min, double max)
	{
		static bool isInit = false;
		if (!isInit)
		{
			srand((unsigned int)time(nullptr));
			isInit = true;
		}

		static std::default_random_engine re{};
		using Dist = std::uniform_real_distribution<double>;
		static Dist uid{};
		return uid(re, Dist::param_type{min, max});
	}
};
} // namespace Linux
} // namespace ItSoftware
