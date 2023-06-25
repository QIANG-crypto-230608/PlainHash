#include<iostream>
#include<string>
#include<vector>
#include<functional>
#include<cmath>
#include<algorithm>
#include<stdexcept>
#include<array>
#include<random>
#include<sstream>
using namespace std;

template<typename Key, typename Value, size_t k, size_t b>
class PlainHash
{
private:
	vector<vector<pair<Key, Value>>> table; //Hash table
	array<hash<Key>, k> hashFunctions;//Array of hash functions

public:
	PlainHash()
	{
		table.resize(b);//Resize the table to have b buckets
	}

	void insert(const Key& key, const Value& value)
	{
		size_t bucketIndex = hash(key); //Compute the bucket index for the given key

		//Search for the key in the bucket
		auto it = find_if(table[bucketIndex].begin(), table[bucketIndex].end(), [&key](const pair<Key, Value>& entry) {
			return entry.first == key; });
		if (it != table[bucketIndex].end())
		{
			//Key already exists, update the value
			it->second = value;
			cout << "Key updated: " << key << endl;
		}
		else {
			//Key does not exist, insert the key-value pair into the bucket
			table[bucketIndex].emplace_back(key, value);
			cout << "key inserted: " << key << endl;
		}
	}

	bool find(const Key& key, Value& value)
	{
		size_t bucketIndex = hash(key); //Compute the bucket index for the given key

		//Search for the key in the bucket
		auto it = find_if(table[bucketIndex].begin(), table[bucketIndex].end(), [&key](const pair<Key, Value>& entry) {
			return entry.first == key; });

		if (it != table[bucketIndex].end())
		{
			//Key found, retrieve the corresponding value
			value = it->second;
			return true;
		}
		//Key not found
		return false;
	}

	void erase(const Key& key)
	{
		size_t bucketIndex = hash(key);//Compute the bucket index for the given key

		////Search for the key in the bucket
		auto it = find_if(table[bucketIndex].begin(), table[bucketIndex].end(), [&key](const pair<Key, Value>& entry) {
			return entry.first == key; });

		if (it != table[bucketIndex].end())
		{
			//Key found, erase the corresponding value
			table[bucketIndex].erase(it);
			cout << "Key erased: " << key << endl;
		}
		else {
			//Key not found, throw an expection
			throw runtime_error("Key not found");
		}
	}

private:
	size_t hash(const Key& key)
	{
		size_t index = 0;

		//Apply k hash functions to compute the index
		for (size_t i = 0; i < k; i++)
		{
			index = (index + hashFunctions[i](key)) % b;
		}

		return index;
	}
};

int generateRandomNumber(int min, int max)
{
	static default_random_engine engine(static_cast<unsigned int>(time(nullptr)));
	uniform_int_distribution<int> distribution(min, max);
	return distribution(engine);
}

template<typename T>
string to_string(T value)
{
	ostringstream oss;
	oss << value;
	return oss.str();
}

int main()
{
	PlainHash<int, string, 3, 10> hashTable;

	//Generate and insert random key-value pairs
	for (int i = 0; i < 100; ++i)
	{
		int key = generateRandomNumber(1, 1000);
		string value = "value " + to_string(key);
		hashTable.insert(key, value);
	}

	//Find values for random keys
	for (int i = 0; i < 50; i++)
	{
		int key = generateRandomNumber(1, 1000);
		string value;
		if (hashTable.find(key, value))
			cout << "Value found: " << value << endl;
		else
			cout << "Value not found for key: " << key << endl;
	}

	//Erase random keys
	for (int i = 0; i < 20; ++i)
	{
		int key = generateRandomNumber(1, 1000);
		try {
			hashTable.erase(key);
		}
		catch (const runtime_error& e) {
			cout << "Error: " << e.what() << endl;
		}
	}
	return 0;
}