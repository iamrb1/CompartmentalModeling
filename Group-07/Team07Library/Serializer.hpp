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
 * ChatGPT Assistance: Documentation (17 Feb 2025)
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

namespace cse
{
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
		Mode GetMode() { return mode_; }

		/**
		 * @brief Checks if the serializer is in SAVE mode.
		 * @return True if SAVE mode, otherwise false.
		 */
		bool IsSave() { return (mode_ == cse::Mode::SAVE); }

		/**
		 * @brief Checks if the serializer is in LOAD mode.
		 * @return True if LOAD mode, otherwise false.
		 */
		bool IsLoad() { return (mode_ == cse::Mode::LOAD); }

		/**
		 * @brief Sets the mode of the serializer.
		 * @param mode The desired mode (SAVE or LOAD).
		 */
		void SetMode(Mode mode) { mode_ = mode; }

		// Specializations for implemented serializable types
		template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		bool IsSerializable(T &) { return true; }

		bool IsSerializable(std::string &) { return true; }

		template <typename T>
		bool IsSerializable(std::vector<T> &) { return true; }

		template <typename T, std::size_t N>
		bool IsSerializable(std::array<T, N> &) { return true; }

		template <typename T>
		bool IsSerializable(std::set<T> &) { return true; }

		template <typename T>
		bool IsSerializable(std::unordered_set<T> &) { return true; }

		template <typename T>
		bool IsSerializable(std::multiset<T> &) { return true; }

		template <typename T>
		bool IsSerializable(std::unordered_multiset<T> &) { return true; }

		template <typename K, typename V>
		bool IsSerializable(std::map<K, V> &) { return true; }

		template <typename K, typename V>
		bool IsSerializable(std::unordered_map<K, V> &) { return true; }

		template <typename K, typename V>
		bool IsSerializable(std::multimap<K, V> &) { return true; }

		template <typename K, typename V>
		bool IsSerializable(std::unordered_multimap<K, V> &) { return true; }

		template <typename T>
		bool IsSerializable(std::stack<T> &) { return true; }

		template <typename T>
		bool IsSerializable(std::queue<T> &) { return true; }

		template <typename T>
		bool IsSerializable(std::priority_queue<T> &) { return true; }

		template <typename T>
		bool IsSerializable(std::deque<T> &) { return true; }

		template <typename T, std::enable_if_t<!std::is_arithmetic_v<T>, int> = 0>
		bool IsSerializable(T &) { return false; }

		/**
		 * @brief Serializes or deserializes an arithmetic type (e.g., int, float, double) to/from a binary file.
		 * @tparam T An arithmetic type (integral or floating-point).
		 * @param data Reference to the variable to write or read.
		 * @param filename Path to the binary file.
		 *
		 * In SAVE mode, writes the contents of `data` to the file.
		 * In LOAD mode, reads from the file into `data`.
		 */
		template <typename T, typename std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
		void Serialize(T &data, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				outFile.write(reinterpret_cast<const char *>(&data), sizeof(T));
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t length = str.size();
				outFile.write(reinterpret_cast<const char *>(&length), sizeof(size_t));
				outFile.write(str.c_str(), length);
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (size_t i = 0; i < size; i++)
				{
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else // LOAD mode
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				vec.resize(size);
				for (size_t i = 0; i < size; i++)
				{
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				for (size_t i = 0; i < N; i++)
				{
					if (IsSerializable(arr[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(arr[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else // LOAD mode
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				for (size_t i = 0; i < N; i++)
				{
					if (IsSerializable(arr[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(arr[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = set.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (auto item = set.begin(), i = 0; item != set.end(); item++, i++)
				{
					T data = *item;
					if (IsSerializable(data))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(data, newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error: Unable to read set size from file: " << filename << std::endl;
					return;
				}
				set.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (IsSerializable(item))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(item, newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = uset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (auto item = uset.begin(), i = 0; item != uset.end(); item++, i++)
				{
					T data = *item;
					if (IsSerializable(data))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(data, newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error: Unable to read set size from file: " << filename << std::endl;
					return;
				}
				uset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (IsSerializable(item))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(item, newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = mset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (auto item = mset.begin(), i = 0; item != mset.end(); item++, i++)
				{
					T data = *item;
					if (IsSerializable(data))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(data, newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error: Unable to read set size from file: " << filename << std::endl;
					return;
				}
				mset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (IsSerializable(item))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(item, newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = umset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (auto item = umset.begin(), i = 0; item != umset.end(); item++, i++)
				{
					T data = *item;
					if (IsSerializable(data))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(data, newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error: Unable to read set size from file: " << filename << std::endl;
					return;
				}
				umset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (IsSerializable(item))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(item, newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}

				size_t size = map.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (auto item = map.begin(), i = 0; item != map.end(); item++, i++)
				{
					K key = (*item).first;
					V val = (*item).second;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (IsSerializable(key))
						Serialize(key, KeyFile);
					else
					{
						std::cerr << "Key type is not implemented yet." << std::endl;
						return;
					}
					if (IsSerializable(val))
						Serialize(val, ValFile);
					else
					{
						std::cerr << "Value type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error reading map size from file: " << filename << std::endl;
					return;
				}
				map.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (IsSerializable(key))
						Serialize(key, KeyFile);
					else
					{
						std::cerr << "Key type is not implemented yet." << std::endl;
						return;
					}
					if (IsSerializable(val))
						Serialize(val, ValFile);
					else
					{
						std::cerr << "Value type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = umap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (auto item = umap.begin(), i = 0; item != umap.end(); item++, i++)
				{
					K key = (*item).first;
					V val = (*item).second;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (IsSerializable(key))
						Serialize(key, KeyFile);
					else
					{
						std::cerr << "Key type is not implemented yet." << std::endl;
						return;
					}
					if (IsSerializable(val))
						Serialize(val, ValFile);
					else
					{
						std::cerr << "Value type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error reading map size from file: " << filename << std::endl;
					return;
				}
				umap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (IsSerializable(key))
						Serialize(key, KeyFile);
					else
					{
						std::cerr << "Key type is not implemented yet." << std::endl;
						return;
					}
					if (IsSerializable(val))
						Serialize(val, ValFile);
					else
					{
						std::cerr << "Value type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = mmap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (auto item = mmap.begin(), i = 0; item != mmap.end(); item++, i++)
				{
					K key = (*item).first;
					V val = (*item).second;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (IsSerializable(key))
						Serialize(key, KeyFile);
					else
					{
						std::cerr << "Key type is not implemented yet." << std::endl;
						return;
					}
					if (IsSerializable(val))
						Serialize(val, ValFile);
					else
					{
						std::cerr << "Value type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error reading map size from file: " << filename << std::endl;
					return;
				}
				mmap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (IsSerializable(key))
						Serialize(key, KeyFile);
					else
					{
						std::cerr << "Key type is not implemented yet." << std::endl;
						return;
					}
					if (IsSerializable(val))
						Serialize(val, ValFile);
					else
					{
						std::cerr << "Value type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = ummap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (auto item = ummap.begin(), i = 0; item != ummap.end(); item++, i++)
				{
					K key = (*item).first;
					V val = (*item).second;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (IsSerializable(key))
						Serialize(key, KeyFile);
					else
					{
						std::cerr << "Key type is not implemented yet." << std::endl;
						return;
					}
					if (IsSerializable(val))
						Serialize(val, ValFile);
					else
					{
						std::cerr << "Value type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error reading map size from file: " << filename << std::endl;
					return;
				}
				ummap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					std::string KeyFile = filename + "_K" + std::to_string(i);
					std::string ValFile = filename + "_V" + std::to_string(i);
					if (IsSerializable(key))
						Serialize(key, KeyFile);
					else
					{
						std::cerr << "Key type is not implemented yet." << std::endl;
						return;
					}
					if (IsSerializable(val))
						Serialize(val, ValFile);
					else
					{
						std::cerr << "Value type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
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
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				if (!inFile)
				{
					std::cerr << "Error reading stack size from file: " << filename << std::endl;
				}
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
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
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}

				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));

				if (!inFile)
				{
					std::cerr << "Error reading queue size from file: " << filename << std::endl;
					return;
				}

				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
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
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}

				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));

				if (!inFile)
				{
					std::cerr << "Error reading priority queue size from file: " << filename << std::endl;
					return;
				}

				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
				{
					std::cerr << "Error opening file for writing: " << filename << std::endl;
					return;
				}
				size_t size = deq.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				for (size_t i = 0; i < size; i++)
				{
					if (IsSerializable(deq[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(deq[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
				}
			}
			else
			{
				std::ifstream inFile(filename, std::ios::binary);
				if (!inFile)
				{
					std::cerr << "Error opening file for reading: " << filename << std::endl;
					return;
				}

				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));

				if (!inFile || inFile.eof())
				{
					std::cerr << "Error reading deque size from file: " << filename << std::endl;
					std::abort();
				}

				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (IsSerializable(vec[i]))
					{
						std::string newFile = filename + "_" + std::to_string(i);
						Serialize(vec[i], newFile);
					}
					else
					{
						std::cerr << "This type is not implemented yet." << std::endl;
						return;
					}
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
		 * Your custom class must define its own `Serialize(Serializer&, const
		 * std::string&)` method.
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
