#include <climits>
#include <fstream>
#include <iostream>
#include <random>
#include <filesystem>
#include <string>

#include "../../Group-07/Team07Library/Serializer.hpp"
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

const int MAX_CASE = 25;
const int MAX_SIZE = 100;
const std::string filename = "result.bin";

std::random_device rd;
std::mt19937_64 gen(rd());

// This test is testing constructor
TEST_CASE("Serializer Construction", "[Serializer]")
{
	cse::Serializer Neutral(filename);
	REQUIRE(Neutral.GetMode() == cse::Mode::SAVE);
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	REQUIRE(Saver.IsSave());
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	REQUIRE(Loader.IsLoad());
	std::filesystem::remove(filename);
}

// This test is testing destructor
TEST_CASE("Serializer Destruction", "[Serialzer]")
{
	static bool called = false;
	class TestSerializer : public cse::Serializer
	{
	public:
		TestSerializer() : cse::Serializer("result.bin") {} // Need to use literal here since filename is private
		~TestSerializer()
		{
			called = true;
		}
	};
	called = false;
	{
		TestSerializer test;
	}
	REQUIRE(called);
	std::filesystem::remove(filename);
}

// This test is testing swapping mode
TEST_CASE("Serializer Swap Mode", "[Serializer]")
{
	cse::Serializer Neutral(filename);
	REQUIRE((Neutral.IsSave() && !Neutral.IsLoad()));
	Neutral.SetMode(cse::Mode::LOAD);
	REQUIRE((Neutral.IsLoad() && !Neutral.IsSave()));
	Neutral.SetMode(cse::Mode::SAVE);
	REQUIRE((Neutral.IsSave() && !Neutral.IsLoad()));
	std::filesystem::remove(filename);
}

// This test is testing basic save load
TEST_CASE("Serializer Save Load Basic", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	int number = 22;
	int result = -1;
	Saver.Serialize(number);
	REQUIRE(result == -1);
	Loader.Serialize(result);
	REQUIRE(result == 22);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
}

TEST_CASE("Serializer Change Name", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::string newname = "result2.bin";
	int one = 1, two = 2, num = -1;
	Saver.Serialize(one);
	Saver.ChangeName(newname);
	Saver.Serialize(two);
	Loader.Serialize(num);
	REQUIRE(num == one);
	Loader.ChangeName(newname);
	Loader.Serialize(num);
	REQUIRE(num == two);
	std::filesystem::remove(filename);
	std::filesystem::remove(newname);
}

// This test is testing the case Saver/Loader works seperately
TEST_CASE("Basic Save Load In Different Processes", "[Serializer]")
{
	int number = 4000;
	int other = 1000;
	// Anything with Saver need to run first, then comment ALL of it, then uncomment Loader for Load, since Saver will delete the file once open.
	// cse::Serializer Saver("4000.dat", cse::Mode::SAVE);
	// Saver.Serialize(number);
	cse::Serializer Loader("4000.dat", cse::Mode::LOAD);
	Loader.Serialize(other);
	REQUIRE(other == number);
}

// This test is testing all fundamental types
TEST_CASE("Serializer Save Load Fundamental Types", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	// Testing Int
	std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
	int Integer;
	for (int i = 0; i < MAX_CASE; i++)
	{
		Integer = ranInt(gen);
		int Result;
		Saver.Serialize(Integer);
		Loader.Serialize(Result);
		REQUIRE(Integer == Result);
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	// Testing Char
	std::uniform_int_distribution<int> ranChar(static_cast<int>('A'),
											   static_cast<int>('Z'));
	char Character;
	for (int i = 0; i < MAX_CASE; i++)
	{
		Character = static_cast<char>(ranChar(gen));
		char Result;
		Saver.Serialize(Character);
		Loader.Serialize(Result);
		REQUIRE(Character == Result);
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	// Testing Long Long
	std::uniform_int_distribution<long long> ranLL(LLONG_MIN, LLONG_MAX);
	long long LL;
	for (int i = 0; i < MAX_CASE; i++)
	{
		LL = ranLL(gen);
		long long Result;
		Saver.Serialize(LL);
		Loader.Serialize(Result);
		REQUIRE(LL == Result);
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	// Testing Double
	std::uniform_real_distribution<double> ranDBL(-MAX_SIZE, MAX_SIZE);
	double Double;
	for (int i = 0; i < MAX_CASE; i++)
	{
		Double = ranDBL(gen);
		double Result;
		Saver.Serialize(Double);
		Loader.Serialize(Result);
		REQUIRE(Double == Result);
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

// This test is testing string
TEST_CASE("Serializer Save Load String", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranInt(1, MAX_SIZE);
		std::uniform_int_distribution<int> ranChar(static_cast<int>(' '),
												   static_cast<int>('~'));
		int Length = ranInt(gen);
		std::string String = "";
		for (int l = 0; l < Length; l++)
		{
			char Character = static_cast<char>(ranChar(gen));
			String = String + Character;
		}
		std::string Result;
		Saver.Serialize(String);
		Loader.Serialize(Result);
		REQUIRE(String == Result);
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

// This test is testing vector
TEST_CASE("Serializer Save Load Vector", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		int Length = ranSize(gen);
		std::vector<int> vec;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			vec.push_back(value);
		}
		std::vector<int> Result;
		Saver.Serialize(vec);
		Loader.Serialize(Result);
		for (int l = 0; l < Length; l++)
		{
			REQUIRE(vec[l] == Result[l]);
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

// This test is testing array
TEST_CASE("Serializer Save Load Array", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	const int size = MAX_SIZE;
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		std::array<int, size> arr;
		for (int l = 0; l < size; l++)
		{
			int value = ranInt(gen);
			arr[l] = value;
		}
		std::array<int, size> Result;
		Saver.Serialize(arr);
		Loader.Serialize(Result);
		REQUIRE(Result.size() == size);
		for (int l = 0; l < size; l++)
		{
			REQUIRE(arr[l] == Result[l]);
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Set", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		int Length = ranSize(gen);
		std::set<int> s, res;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			s.insert(value);
		}
		Saver.Serialize(s);
		Loader.Serialize(res);
		REQUIRE(res.size() == s.size());
		auto is = s.begin();
		auto ir = res.begin();
		for (int l = 0; l < Length; l++)
		{
			REQUIRE(*is == *ir);
			is++, ir++;
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Unordered Set", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		int Length = ranSize(gen);
		std::unordered_set<int> s, res;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			s.insert(value);
		}
		Saver.Serialize(s);
		Loader.Serialize(res);
		REQUIRE(res.size() == s.size());
		int len = static_cast<int>(s.size());
		auto is = s.begin();
		auto ir = res.begin();
		for (int l = 0; l < len; l++)
		{
			REQUIRE((res.find(*is) != res.end() && s.find(*ir) != s.end()));
			is++, ir++;
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Multiset", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		int Length = ranSize(gen);
		std::multiset<int> s, res;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			s.insert(value);
		}
		Saver.Serialize(s);
		Loader.Serialize(res);
		REQUIRE(res.size() == s.size());
		auto is = s.begin();
		auto ir = res.begin();
		for (int l = 0; l < Length; l++)
		{
			REQUIRE(*is == *ir);
			is++, ir++;
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Unordered Multiset", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		int Length = ranSize(gen);
		std::unordered_multiset<int> s, res;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			s.insert(value);
		}
		Saver.Serialize(s);
		Loader.Serialize(res);
		REQUIRE(res.size() == s.size());
		std::unordered_map<int, int> cs, cr;
		for (const auto &v : s)
			cs[v]++;
		for (const auto &v : res)
			cr[v]++;
		REQUIRE(cs == cr);
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Map", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::map<int, int> m, res;
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		for (int l = 0; l < MAX_SIZE; l++)
		{
			int value = ranInt(gen);
			m[l] = value;
		}
		Saver.Serialize(m);
		Loader.Serialize(res);
		for (int l = 0; l < MAX_SIZE; l++)
		{
			REQUIRE(res.find(l) != res.end());
			REQUIRE(res[l] == m[l]);
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Unordered Map", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::unordered_map<int, int> m, res;
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		for (int l = 0; l < MAX_SIZE; l++)
		{
			int value = ranInt(gen);
			m[l] = value;
		}
		Saver.Serialize(m);
		Loader.Serialize(res);
		for (int l = 0; l < MAX_SIZE; l++)
		{
			REQUIRE(res.find(l) != res.end());
			REQUIRE(res[l] == m[l]);
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Multi Map", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::multimap<int, int> m, res;
		std::uniform_int_distribution<int> ranKey(INT_MIN, INT_MAX);
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		for (int l = 0; l < MAX_SIZE; l++)
		{
			int key = ranKey(gen);
			int value = ranInt(gen);
			m.insert({key, value});
		}
		Saver.Serialize(m);
		Loader.Serialize(res);
		REQUIRE(m == res);
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Unordered Multi Map", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::unordered_multimap<int, int> m, res;
		std::uniform_int_distribution<int> ranKey(INT_MIN, INT_MAX);
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		for (int l = 0; l < MAX_SIZE; l++)
		{
			int key = ranKey(gen);
			int value = ranInt(gen);
			m.insert({key, value});
		}
		Saver.Serialize(m);
		Loader.Serialize(res);
		REQUIRE(m == res);
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Stack", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		int Length = ranSize(gen);
		std::stack<int> s, res;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			s.push(value);
		}
		Saver.Serialize(s);
		Loader.Serialize(res);
		REQUIRE(s.size() == res.size());
		while (!s.empty())
		{
			REQUIRE(s.top() == res.top());
			s.pop();
			res.pop();
		}
		for (int l = 0; l < Length; l++)
		{
			std::string dataname = (std::string)filename + "_" + std::to_string(l);
			std::filesystem::remove(dataname);
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Queue", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		int Length = ranSize(gen);
		std::queue<int> q, res;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			q.push(value);
		}
		Saver.Serialize(q);
		Loader.Serialize(res);
		REQUIRE(q.size() == res.size());
		while (!q.empty())
		{
			REQUIRE(q.front() == res.front());
			q.pop();
			res.pop();
		}
		for (int l = 0; l < Length; l++)
		{
			std::string dataname = (std::string)filename + "_" + std::to_string(l);
			std::filesystem::remove(dataname);
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Priority Queue", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		int Length = ranSize(gen);
		std::priority_queue<int> pq, res;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			pq.push(value);
		}
		Saver.Serialize(pq);
		Loader.Serialize(res);
		REQUIRE(pq.size() == res.size());
		while (!pq.empty())
		{
			REQUIRE(pq.top() == res.top());
			pq.pop();
			res.pop();
		}
		for (int l = 0; l < Length; l++)
		{
			std::string dataname = (std::string)filename + "_" + std::to_string(l);
			std::filesystem::remove(dataname);
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

TEST_CASE("Serializer Save Load Deque", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
		int Length = ranSize(gen);
		std::deque<int> d, res;
		for (int l = 0; l < Length; l++)
		{
			int value = ranInt(gen);
			d.push_back(value);
		}
		Saver.Serialize(d);
		Loader.Serialize(res);
		REQUIRE(d.size() == res.size());
		for (size_t i = 0; i < d.size(); i++)
		{
			REQUIRE(d[i] == res[i]);
			std::string dataname = (std::string)filename + "_" + std::to_string(i);
			std::filesystem::remove(dataname);
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

// This test is testing empty file and empty containers
TEST_CASE("Serializer Empty File or Empty Containers", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	std::vector<int> vec{}, res{}; // Keep vec empty
	Saver.Serialize(vec);
	Loader.Serialize(res);
	REQUIRE(res.size() == 0);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	REQUIRE_THROWS_AS(Loader.Serialize(res), cse::SerializationError);
	std::filesystem::remove(filename);
}

// This test is verifying which is serializable
TEST_CASE("Implemented Verifier", "[Serializer]")
{
	cse::Serializer Saver(filename);

	// Verifying implemented IsSerializable cases
	REQUIRE(Saver.IsSerializable<int>() == true);
	REQUIRE(Saver.IsSerializable<double>() == true);
	REQUIRE(Saver.IsSerializable<char>() == true);
	REQUIRE(Saver.IsSerializable<std::vector<int>>() == true);
	REQUIRE(Saver.IsSerializable<std::vector<std::vector<int>>>() == true);
	REQUIRE(Saver.IsSerializable<std::string>() == true);
	REQUIRE(Saver.IsSerializable<std::vector<std::string>>() == true);
	REQUIRE(Saver.IsSerializable<std::array<int, MAX_SIZE>>() == true);
	REQUIRE(Saver.IsSerializable<std::set<std::string>>() == true);
	REQUIRE(Saver.IsSerializable<std::unordered_set<int>>() == true);
	REQUIRE(Saver.IsSerializable<std::multiset<double>>() == true);
	REQUIRE(Saver.IsSerializable<std::unordered_multiset<int>>() == true);
	REQUIRE(Saver.IsSerializable<std::map<int, double>>() == true);
	REQUIRE(Saver.IsSerializable<std::map<std::vector<int>, std::string>>() == true);
	REQUIRE(Saver.IsSerializable<std::unordered_map<std::string, int>>() == true);
	REQUIRE(Saver.IsSerializable<std::multimap<std::string, double>>() == true);
	REQUIRE(Saver.IsSerializable<std::unordered_map<int, char>>() == true);
	REQUIRE(Saver.IsSerializable<std::stack<int>>() == true);
	REQUIRE(Saver.IsSerializable<std::queue<double>>() == true);
	REQUIRE(Saver.IsSerializable<std::priority_queue<int>>() == true);
	REQUIRE(Saver.IsSerializable<std::deque<std::string>>() == true);
	REQUIRE(Saver.IsSerializable<std::pair<int, int>>() == false);
	std::filesystem::remove(filename);
}

// This test is testing nested containers
TEST_CASE("Serializer Nested Containers and Similars", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	for (int i = 0; i < MAX_CASE; i++)
	{
		std::uniform_int_distribution<int> ranSize(1, MAX_SIZE);
		int Length = ranSize(gen);
		std::vector<std::vector<int>> vvec;
		for (int j = 0; j < Length; j++)
		{
			std::uniform_int_distribution<int> ranSizeInner(1, MAX_SIZE);
			std::uniform_int_distribution<int> ranInt(INT_MIN, INT_MAX);
			int LengthInner = ranSizeInner(gen);
			std::vector<int> vec;
			for (int l = 0; l < LengthInner; l++)
			{
				int value = ranInt(gen);
				vec.push_back(value);
			}
			vvec.push_back(vec);
		}
		std::vector<std::vector<int>> Result;
		Saver.Serialize(vvec);
		Loader.Serialize(Result);
		REQUIRE(vvec.size() == Result.size());
		int length = vvec.size();
		for (int l = 0; l < length; l++)
		{
			REQUIRE(vvec[l].size() == Result[l].size());
			int llength = vvec[l].size();
			for (int ll = 0; ll < llength; ll++)
			{
				REQUIRE(vvec[l][ll] == Result[l][ll]);
			}
		}
		std::filesystem::remove(filename);
		Saver.ResetFileStream();
		Loader.ResetFileStream();
	}
	std::filesystem::remove(filename);
}

// Struct Person
struct Person
{
	std::string name;
	int age;
	double height;
	std::vector<std::string> hobbies;
	void Serialize(cse::Serializer &Ser)
	{
		Ser.Serialize(name);
		Ser.Serialize(age);
		Ser.Serialize(height);
		Ser.Serialize(hobbies);
	}
};

// Assign that Person is a custom type
template <>
struct is_custom_type<Person> : std::true_type
{
};

// This test is testing External Class/Struct
TEST_CASE("Serialize External Class/Struct (must be global)", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	Person P1, P2;
	P1.name = "John Doe";
	P1.age = 40;
	P1.height = 177.5;
	P1.hobbies.push_back("Playing Games");
	P1.hobbies.push_back("Listening to Music");
	Saver.Serialize(P1);
	Loader.Serialize(P2);
	REQUIRE(P1.name == P2.name);
	REQUIRE(P1.age == P2.age);
	REQUIRE(P1.height == P2.height);
	REQUIRE(P1.hobbies.size() == P2.hobbies.size());
	for (size_t i = 0; i < P1.hobbies.size(); i++)
	{
		REQUIRE(P1.hobbies[i] == P2.hobbies[i]);
	}
	std::filesystem::remove(filename);
}

// This test is testing if the types are incompatible
TEST_CASE("Incompatible Types Serialization", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	int num = 22;
	double dbl = 3.14;
	Saver.Serialize(num);
	REQUIRE_THROWS_AS(Loader.Serialize(dbl), cse::SerializationError);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	Saver.Serialize(dbl);
	REQUIRE_THROWS_AS(Loader.Serialize(num), cse::SerializationError);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	std::string str = "Hello World!";
	Saver.Serialize(str);
	REQUIRE_THROWS_AS(Loader.Serialize(num), cse::SerializationError);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	Saver.Serialize(num);
	REQUIRE_THROWS_AS(Loader.Serialize(str), cse::SerializationError);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	std::vector<int> v{1, 2, 3, 4};
	std::vector<std::string> vs;
	Saver.Serialize(v);
	REQUIRE_THROWS_AS(Loader.Serialize(vs), cse::SerializationError);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	vs = {"1", "2", "3", "4"};
	Saver.Serialize(vs);
	REQUIRE_THROWS_AS(Loader.Serialize(v), cse::SerializationError);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	std::vector<std::vector<int>> vv{{1}, {2, 22}, {3, 33, 333}, {4, 44, 444, 4444}};
	Saver.Serialize(v);
	REQUIRE_THROWS_AS(Loader.Serialize(vv), cse::SerializationError);
	std::filesystem::remove(filename);
	Saver.ResetFileStream();
	Loader.ResetFileStream();
	Saver.Serialize(vv);
	REQUIRE_THROWS_AS(Loader.Serialize(v), cse::SerializationError);
	std::filesystem::remove(filename);
}

// Class Student
class Student
{
public:
	std::string name;
	int id;
	void Serialize(cse::Serializer &Ser)
	{
		Ser.Serialize(name);
		Ser.Serialize(id);
	}
};

// Assigning Student as custom type
template <>
struct is_custom_type<Student> : std::true_type
{
};

// Test Vector of Struct/Class
TEST_CASE("Serialize Vector of Struct/Class", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::vector<Student> vecs, vect;
	Student S1, S2;
	S1.name = "Student 1";
	S1.id = 1;
	S2.name = "Student 2";
	S2.id = 2;
	vecs.push_back(S1);
	vecs.push_back(S2);
	Saver.Serialize(vecs);
	Loader.Serialize(vect);
	REQUIRE(vect.size() == vecs.size());
	int length = vecs.size();
	REQUIRE(is_custom_type<std::remove_cvref_t<decltype(vecs[0])>>::value);
	for (auto i = 0; i < length; i++)
	{
		REQUIRE(vecs[i].id == vect[i].id);
		REQUIRE(vecs[i].name == vect[i].name);
	}
	std::filesystem::remove(filename);
}

// Inner class and Outer struct, also set them as custom type
class Inner
{
public:
	int intIn;
	std::string strIn;
	void Serialize(cse::Serializer &Ser)
	{
		Ser.Serialize(intIn);
		Ser.Serialize(strIn);
	}
};

struct Outer
{
	char chrOut;
	std::vector<Inner> vecOut;
	void Serialize(cse::Serializer &Ser)
	{
		Ser.Serialize(chrOut);
		Ser.Serialize(vecOut);
	}
};

template <>
struct is_custom_type<Inner> : std::true_type
{
};

template <>
struct is_custom_type<Outer> : std::true_type
{
};

// Test Nested Struct/Class with Another implemented types
TEST_CASE("Ultimate Test: Nested Struct/Class and STLs", "[Serializer]")
{
	cse::Serializer Saver(filename, cse::Mode::SAVE);
	cse::Serializer Loader(filename, cse::Mode::LOAD);
	std::vector<Outer> vec, res;
	Outer O1, O2;
	O1.chrOut = '1';
	O2.chrOut = '2';
	Inner I11, I12, I21, I22;
	I11.intIn = 11;
	I11.strIn = "11";
	I12.intIn = 12;
	I12.strIn = "12";
	I21.intIn = 21;
	I21.strIn = "21";
	I22.intIn = 22;
	I22.strIn = "22";
	O1.vecOut.push_back(I11);
	O1.vecOut.push_back(I12);
	O2.vecOut.push_back(I21);
	O2.vecOut.push_back(I22);
	vec.push_back(O1);
	vec.push_back(O2);
	Saver.Serialize(vec);
	Loader.Serialize(res);
	REQUIRE(vec.size() == res.size());
	int s = vec.size();
	for (int i = 0; i < s; i++)
	{
		REQUIRE(vec[i].chrOut == res[i].chrOut);
		REQUIRE(vec[i].vecOut.size() == res[i].vecOut.size());
		int t = vec[i].vecOut.size();
		for (int j = 0; j < t; j++)
		{
			REQUIRE(vec[i].vecOut[j].intIn == res[i].vecOut[j].intIn);
			REQUIRE(vec[i].vecOut[j].strIn == res[i].vecOut[j].strIn);
		}
	}
	std::filesystem::remove(filename);
}