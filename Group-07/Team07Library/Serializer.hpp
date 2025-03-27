/**
 * @file Serializer.h
 * @brief A collection of serialization/deserialization functions for common C++
 * types and containers.
 *
 * This file defines the `cse::Serializer` class, which can operate in SAVE or
 * LOAD mode to serialize (write) or deserialize (read) data to/from binary
 * files. It supports primitive types, `std::string`, and many standard
 * containers (e.g., `std::vector`, `std::map`, `std::queue`, etc.).
 *
 * @author tdkduong
 * ChatGPT Assistance: Documentation
 */

#ifndef CSE_SERIALIZER_H
#define CSE_SERIALIZER_H

#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <concepts>
#include <cassert>

namespace cse
{
	// Forward declaration of Serializer class. This lead to a warning: invalid use of incomplete type ‘class cse::Serializer’, but there is no other choice currently for simple implementation.
	class Serializer;
	/**
	 * @enum Mode
	 * @brief Specifies whether the Serializer is in SAVE mode or LOAD mode.
	 */
	enum class Mode
	{
		SAVE, ///< Serializer writes data to file.
		LOAD  ///< Serializer reads data from file.
	};

	/**
	 * @tparam T the type need to check
	 * @brief Concept use to categorized the types allowed to Serialize
	 */
	template <typename T, typename S>
	concept Serializable = requires(T &t, S &s, const std::string &filename) {
		s.Serialize(t, filename);
	};

	/**
	 * @class Serializer
	 * @brief A utility class for serializing and deserializing various data types
	 * to/from binary files.
	 *
	 * The Serializer can be toggled between SAVE and LOAD modes. In SAVE mode, data
	 * will be written to the specified file in a binary format; in LOAD mode, data
	 * will be read from the file and stored in the provided variables or
	 * containers.
	 */
	class Serializer
	{
	private:
		Mode mode_; ///< Current operating mode (SAVE or LOAD).

	public:
		/**
		 * @brief Default constructor that initializes the serializer in SAVE mode.
		 */
		explicit Serializer() { mode_ = cse::Mode::SAVE; }

		/**
		 * @brief Constructor that initializes the serializer with the specified mode.
		 * @param mode The desired operating mode (SAVE or LOAD).
		 */
		explicit Serializer(Mode mode) : mode_(mode) {}

		/**
		 * @brief Default destructor.
		 */
		~Serializer() = default;

		/**
		 * @brief Retrieves the current operating mode of the serializer.
		 * @return The current mode (SAVE or LOAD).
		 */
		Mode GetMode() const { return mode_; }

		/**
		 * @brief Checks if the serializer is in SAVE mode.
		 * @return True if SAVE mode, otherwise false.
		 */
		bool IsSave() const { return (mode_ == cse::Mode::SAVE); }

		/**
		 * @brief Checks if the serializer is in LOAD mode.
		 * @return True if LOAD mode, otherwise false.
		 */
		bool IsLoad() const { return (mode_ == cse::Mode::LOAD); }

		/**
		 * @brief Sets the mode of the serializer.
		 * @param mode The desired mode (SAVE or LOAD).
		 */
		void SetMode(Mode mode) { mode_ = mode; }

		/**
		 * @brief Decide if the type T is allowed to Serialize
		 * @tparam T the type
		 */
		template <typename T>
		bool IsSerializable(T &) { return Serializable<T, Serializer>; }

		/**
		 * @brief Serializes or deserializes an arithmetic type (e.g., int, float, double) to/from a binary file.
		 * @tparam T An arithmetic type (integral or floating-point).
		 * @param data Reference to the variable to write or read.
		 * @param filename Path to the binary file.
		 *
		 * In SAVE mode, writes the contents of `data` to the file.
		 * In LOAD mode, reads from the file into `data`.
		 */
		template <typename T>
			requires std::is_arithmetic_v<T>
		void Serialize(T &data, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				outFile.write(reinterpret_cast<const char *>(&data), sizeof(T));
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				inFile.read(reinterpret_cast<char *>(&data), sizeof(T));
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::string` to/from a binary file.
		 * @param str Reference to the string to write or read.
		 * @param filename Path to the binary file.
		 *
		 * In SAVE mode, writes the size of the string followed by the characters.
		 * In LOAD mode, reads the size and then resizes the string before reading its contents.
		 */
		void Serialize(std::string &str, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t length = str.size();
				outFile.write(reinterpret_cast<const char *>(&length), sizeof(size_t));
				outFile.write(str.c_str(), length);
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t length;
				inFile.read(reinterpret_cast<char *>(&length), sizeof(size_t));
				str.resize(length);
				inFile.read(&str[0], length);
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::vector` of any type to/from a binary file.
		 * @tparam T The type of elements stored in the vector.
		 * @param vec Reference to the vector to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::vector<T> &vec, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
			}
			else // LOAD mode
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				vec.resize(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::array` of fixed size to/from a
		 * binary file.
		 * @tparam T The type of elements stored in the array.
		 * @tparam N The size of the array.
		 * @param arr Reference to the array to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T, std::size_t N>
		void Serialize(std::array<T, N> &arr, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				for (size_t i = 0; i < N; i++)
				{
					if (!IsSerializable(arr[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(arr[i], newFile);
				}
			}
			else // LOAD mode
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				for (size_t i = 0; i < N; i++)
				{
					if (!IsSerializable(arr[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(arr[i], newFile);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::set` of elements to/from a binary
		 * file.
		 * @tparam T The type of elements in the set.
		 * @param set Reference to the set to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::set<T> &set, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = set.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				int i = 0;
				for (auto item = set.begin(); item != set.end(); item++, i++)
				{
					T data = *item;
					if (!IsSerializable(data))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(data, newFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				set.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (!IsSerializable(item))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(item, newFile);
					set.insert(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::unordered_set` of elements
		 * to/from a binary file.
		 * @tparam T The type of elements in the unordered_set.
		 * @param uset Reference to the unordered_set to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::unordered_set<T> &uset, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = uset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				int i = 0;
				for (auto item = uset.begin(); item != uset.end(); item++, i++)
				{
					T data = *item;
					if (!IsSerializable(data))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(data, newFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				uset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (!IsSerializable(item))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(item, newFile);
					uset.insert(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::multiset` of elements to/from a
		 * binary file.
		 * @tparam T The type of elements in the multiset.
		 * @param mset Reference to the multiset to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::multiset<T> &mset, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = mset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				int i = 0;
				for (auto item = mset.begin(); item != mset.end(); item++, i++)
				{
					T data = *item;
					if (!IsSerializable(data))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(data, newFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				mset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (!IsSerializable(item))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(item, newFile);
					mset.insert(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::unordered_multiset` of elements
		 * to/from a binary file.
		 * @tparam T The type of elements in the unordered_multiset.
		 * @param umset Reference to the unordered_multiset to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::unordered_multiset<T> &umset, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = umset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				int i = 0;
				for (auto item = umset.begin(); item != umset.end(); item++, i++)
				{
					T data = *item;
					if (!IsSerializable(data))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(data, newFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				umset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (!IsSerializable(item))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(item, newFile);
					umset.insert(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::map` (key-value pairs) to/from a binary file.
		 * @tparam K The key type.
		 * @tparam V The value type.
		 * @param map Reference to the map to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename K, typename V>
		void Serialize(std::map<K, V> &map, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = map.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				int i = 0;
				for (auto item = map.begin(); item != map.end(); item++, i++)
				{
					K key = (*item).first;
					V val = (*item).second;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (!IsSerializable(key))
						throw std::runtime_error("There is unserializable type.");
					Serialize(key, KeyFile);
					if (!IsSerializable(val))
						throw std::runtime_error("There is unserializable type.");
					Serialize(val, ValFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				map.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (!IsSerializable(key))
						throw std::runtime_error("There is unserializable type.");
					Serialize(key, KeyFile);
					if (!IsSerializable(val))
						throw std::runtime_error("There is unserializable type.");
					Serialize(val, ValFile);
					map[key] = val;
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::unordered_map` (key-value pairs) to/from a binary file.
		 * @tparam K The key type.
		 * @tparam V The value type.
		 * @param umap Reference to the unordered_map to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename K, typename V>
		void Serialize(std::unordered_map<K, V> &umap, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = umap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				int i = 0;
				for (auto item = umap.begin(); item != umap.end(); item++, i++)
				{
					K key = (*item).first;
					V val = (*item).second;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (!IsSerializable(key))
						throw std::runtime_error("There is unserializable type.");
					Serialize(key, KeyFile);
					if (!IsSerializable(val))
						throw std::runtime_error("There is unserializable type.");
					Serialize(val, ValFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				umap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (!IsSerializable(key))
						throw std::runtime_error("There is unserializable type.");
					Serialize(key, KeyFile);
					if (!IsSerializable(val))
						throw std::runtime_error("There is unserializable type.");
					Serialize(val, ValFile);
					umap[key] = val;
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::multimap` (key-value pairs) to/from a binary file.
		 * @tparam K The key type.
		 * @tparam V The value type.
		 * @param mmap Reference to the multimap to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename K, typename V>
		void Serialize(std::multimap<K, V> &mmap, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = mmap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				int i = 0;
				for (auto item = mmap.begin(); item != mmap.end(); item++, i++)
				{
					K key = (*item).first;
					V val = (*item).second;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (!IsSerializable(key))
						throw std::runtime_error("There is unserializable type.");
					Serialize(key, KeyFile);
					if (!IsSerializable(val))
						throw std::runtime_error("There is unserializable type.");
					Serialize(val, ValFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				mmap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (!IsSerializable(key))
						throw std::runtime_error("There is unserializable type.");
					Serialize(key, KeyFile);
					if (!IsSerializable(val))
						throw std::runtime_error("There is unserializable type.");
					Serialize(val, ValFile);
					mmap.insert({key, val});
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::unordered_multimap` (key-value pairs) to/from a binary file.
		 * @tparam K The key type.
		 * @tparam V The value type.
		 * @param ummap Reference to the unordered_multimap to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename K, typename V>
		void Serialize(std::unordered_multimap<K, V> &ummap, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = ummap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				int i = 0;
				for (auto item = ummap.begin(); item != ummap.end(); item++, i++)
				{
					K key = (*item).first;
					V val = (*item).second;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (!IsSerializable(key))
						throw std::runtime_error("There is unserializable type.");
					Serialize(key, KeyFile);
					if (!IsSerializable(val))
						throw std::runtime_error("There is unserializable type.");
					Serialize(val, ValFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				ummap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (!IsSerializable(key))
						throw std::runtime_error("There is unserializable type.");
					Serialize(key, KeyFile);
					if (!IsSerializable(val))
						throw std::runtime_error("There is unserializable type.");
					Serialize(val, ValFile);
					ummap.insert({key, val});
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::stack` to/from a binary file.
		 * @tparam T The type of elements in the stack.
		 * @param stk Reference to the stack to write or read.
		 * @param filename Path to the binary file.
		 *
		 * In SAVE mode, this method extracts all elements into a temporary vector (top to bottom)
		 * and writes them. In LOAD mode, the elements are read into a vector and then pushed
		 * onto the stack in reverse order to restore the original stack order.
		 */
		template <typename T>
		void Serialize(std::stack<T> &stk, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				std::vector<T> vec;
				std::stack<T> temp = stk;
				while (!temp.empty())
				{
					vec.push_back(temp.top());
					temp.pop();
				}
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
				stk = std::stack<T>();
				for (auto it = vec.rbegin(); it != vec.rend(); ++it)
				{
					stk.push(*it);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::queue` to/from a binary file.
		 * @tparam T The type of elements in the queue.
		 * @param q Reference to the queue to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::queue<T> &q, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				std::vector<T> vec;
				std::queue<T> temp = q;
				while (!temp.empty())
				{
					vec.push_back(temp.front());
					temp.pop();
				}
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
				q = std::queue<T>();
				for (const T &item : vec)
				{
					q.push(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::priority_queue` to/from a binary file.
		 * @tparam T The type of elements in the priority_queue.
		 * @param pq Reference to the priority_queue to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::priority_queue<T> &pq, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				std::vector<T> vec;
				std::priority_queue<T> temp = pq;
				while (!temp.empty())
				{
					vec.push_back(temp.top());
					temp.pop();
				}
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
				pq = std::priority_queue<T>(vec.begin(), vec.end());
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::deque` to/from a binary file.
		 * @tparam T The type of elements in the deque.
		 * @param deq Reference to the deque to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::deque<T> &deq, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw std::runtime_error("Error in opening file for writing.");
				size_t size = deq.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(deq[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(deq[i], newFile);
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
					throw std::runtime_error("Error in opening file for reading.");
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw std::runtime_error("There is unserializable type.");
					std::string newFile = filename + "_" + std::to_string(i);
					Serialize(vec[i], newFile);
				}
				deq.clear();
				for (const T &item : vec)
				{
					deq.push_back(item);
				}
			}
		}

		/**
		 * @brief Specialized function for serializing custom user-defined
		 * classes/structs.
		 * @tparam T A user-defined class or struct that implements a `Serialize`
		 * member function.
		 * @param obj Reference to the object to write or read.
		 *
		 * This function calls `obj.Serialize(*this, filename);`.
		 * Your custom class must define its own `Serialize(Serializer&)` method.
		 * For easy management, the filename in your own Serialize method should include directory
		 * This does not contain any filename/foldername, so that nothing will accidentally return true for IsSerializable for unimplemented type.
		 */
		template <typename T,
				  typename std::enable_if_t<
					  std::is_class_v<T> && !std::is_same_v<T, std::string>, int> = 0>
		void Serialize(T &obj)
		{
			obj.Serialize(*this);
		}
	};
} // namespace cse

#endif // CSE_SERIALIZER_H
