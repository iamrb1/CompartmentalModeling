/**
 * @file Serializer.h
 * @brief A collection of serialization/deserialization functions for common C++ types and containers.
 *
 * This file defines the `cse::Serializer` class, which can operate in SAVE or
 * LOAD mode to serialize (write) or deserialize (read) data to/from binary files.
 * It supports primitive types, `std::string`, and many standard containers (e.g.,
 * `std::vector`, `std::map`, `std::queue`, etc.).
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
	 * @class SerializationError
	 * @brief Exception class for handling serialization errors.
	 *
	 * This class extends std::exception and is thrown when file operations or
	 * serialization/deserialization processes fail.
	 */
	class SerializationError : public std::exception
	{
	public:
		/**
		 * @brief Constructs a SerializationError with the specified error message.
		 * @param message A description of the error.
		 */
		explicit SerializationError(const std::string &message)
			: message_("SerializationError: " + message) {}

		/**
		 * @brief Retrieves the error message.
		 * @return A C-style string describing the error.
		 */
		const char *what() const noexcept override
		{
			return message_.c_str();
		}

	private:
		std::string message_; ///< Detailed error message.
	};

	/**
	 * @tparam T the type to check
	 * @brief Concept used to categorize the types allowed to be serialized.
	 *
	 * This concept requires that a type T, when paired with a Serializer S, must have
	 * a valid Serialize() call with a filename.
	 */
	template <typename T, typename S>
	concept Serializable = requires(T &t, S &s, const std::string &filename) {
		s.Serialize(t, filename);
	};

	/**
	 * @class Serializer
	 * @brief A utility class for serializing and deserializing various data types to/from binary files.
	 *
	 * The Serializer can be toggled between SAVE and LOAD modes. In SAVE mode, data
	 * will be written to the specified file in a binary format; in LOAD mode, data
	 * will be read from the file and stored in the provided variables or containers.
	 */
	class Serializer
	{
	private:
		Mode mode_;					  ///< Current operating mode (SAVE or LOAD).
		std::string currInFile = "";  ///< Cache for the current input file name.
		std::string currOutFile = ""; ///< Cache for the current output file name.
		std::ifstream inFile;		  ///< Input file stream.
		std::ofstream outFile;		  ///< Output file stream.

		/**
		 * @brief Opens the input file for reading.
		 * @param filename The path to the input file.
		 *
		 * If the provided filename differs from the current one, the current file is closed,
		 * and the new file is opened in binary mode. Throws a SerializationError if the file cannot be opened.
		 */
		void SetInFile(const std::string &filename)
		{
			// If switching to a new file, close the previous stream and open the new one.
			if (filename != currInFile)
			{
				inFile.close();
				currInFile = filename;
				inFile.open(filename, std::ios::binary);
				if (!inFile)
					throw cse::SerializationError("Error in opening file for reading.");
			}
			// If the current file stream is not valid, throw an error.
			else if (!inFile)
				throw cse::SerializationError("Error in opening file for reading.");
		}

		/**
		 * @brief Opens the output file for writing.
		 * @param filename The path to the output file.
		 *
		 * If the provided filename differs from the current one, the current stream is closed,
		 * and the new file is opened in binary mode with truncation. Throws a SerializationError
		 * if the file cannot be opened.
		 */
		void SetOutFile(const std::string &filename)
		{
			// If switching to a new file, close the previous stream and open the new one.
			if (filename != currOutFile)
			{
				outFile.close();
				currOutFile = filename;
				outFile.open(filename, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw cse::SerializationError("Error in opening file for writing.");
			}
			// If the current file stream is not valid, throw an error.
			else if (!outFile)
				throw cse::SerializationError("Error in opening file for writing.");
		}

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
		 * @brief Reset Filestream to default, usually use to reset cache after deleting files.
		 */
		void ResetFileStream()
		{
			currInFile = "";
			currOutFile = "";
			inFile.close();
			outFile.close();
		}

		/**
		 * @brief Determines if the given type T is serializable.
		 * @tparam T The type to check.
		 * @param data An instance of the type.
		 * @return True if the type meets the Serializable concept requirements.
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
				// Set the output file stream and write the binary data.
				SetOutFile(filename);
				outFile.write(reinterpret_cast<const char *>(&data), sizeof(T));
				outFile.flush();
			}
			else
			{
				// Set the input file stream and read the binary data.
				SetInFile(filename);
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
				// Write string size and characters.
				SetOutFile(filename);
				size_t length = str.size();
				outFile.write(reinterpret_cast<const char *>(&length), sizeof(size_t));
				outFile.write(str.c_str(), length);
				outFile.flush();
			}
			else
			{
				// Read string size and then its content.
				SetInFile(filename);
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
		 *
		 * In SAVE mode, writes the size of the vector followed by each element.
		 * In LOAD mode, reads the size, resizes the vector, and then reads each element.
		 */
		template <typename T>
		void Serialize(std::vector<T> &vec, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				// Write vector size.
				SetOutFile(filename);
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Write each element in the vector.
				for (size_t i = 0; i < size; i++)
				{
					Serialize(vec[i], filename);
				}
			}
			else // LOAD mode
			{
				// Read vector size and elements.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				vec.resize(size);
				for (size_t i = 0; i < size; i++)
				{
					// Check if the element type is serializable before processing.
					if (!IsSerializable(vec[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(vec[i], filename);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::array` of fixed size to/from a binary file.
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
				SetOutFile(filename);
				// Process each element in the fixed-size array.
				for (size_t i = 0; i < N; i++)
				{
					if (!IsSerializable(arr[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(arr[i], filename);
				}
			}
			else // LOAD mode
			{
				SetInFile(filename);
				// Read each element from the file.
				for (size_t i = 0; i < N; i++)
				{
					if (!IsSerializable(arr[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(arr[i], filename);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::set` of elements to/from a binary file.
		 * @tparam T The type of elements in the set.
		 * @param set Reference to the set to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::set<T> &set, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				// Write set size.
				size_t size = set.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Iterate through the set and serialize each element.
				for (auto item = set.begin(); item != set.end(); ++item)
				{
					T data = *item;
					if (!IsSerializable(data))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(data, filename);
				}
			}
			else
			{
				// Read set size and then each element.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				set.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (!IsSerializable(item))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(item, filename);
					set.insert(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::unordered_set` of elements to/from a binary file.
		 * @tparam T The type of elements in the unordered_set.
		 * @param uset Reference to the unordered_set to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::unordered_set<T> &uset, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				// Write unordered_set size.
				size_t size = uset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each element.
				for (auto item = uset.begin(); item != uset.end(); ++item)
				{
					T data = *item;
					if (!IsSerializable(data))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(data, filename);
				}
			}
			else
			{
				// Read size and then deserialize each element.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				uset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (!IsSerializable(item))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(item, filename);
					uset.insert(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::multiset` of elements to/from a binary file.
		 * @tparam T The type of elements in the multiset.
		 * @param mset Reference to the multiset to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::multiset<T> &mset, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				size_t size = mset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Iterate through multiset elements.
				for (auto item = mset.begin(); item != mset.end(); ++item)
				{
					T data = *item;
					if (!IsSerializable(data))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(data, filename);
				}
			}
			else
			{
				// Read size and then each element.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				mset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (!IsSerializable(item))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(item, filename);
					mset.insert(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::unordered_multiset` of elements to/from a binary file.
		 * @tparam T The type of elements in the unordered_multiset.
		 * @param umset Reference to the unordered_multiset to write or read.
		 * @param filename Path to the binary file.
		 */
		template <typename T>
		void Serialize(std::unordered_multiset<T> &umset, const std::string &filename)
		{
			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				size_t size = umset.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each element in the unordered_multiset.
				for (auto item = umset.begin(); item != umset.end(); ++item)
				{
					T data = *item;
					if (!IsSerializable(data))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(data, filename);
				}
			}
			else
			{
				// Read size and then deserialize each element.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				umset.clear();
				for (size_t i = 0; i < size; i++)
				{
					T item;
					if (!IsSerializable(item))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(item, filename);
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
				SetOutFile(filename);
				// Write map size.
				size_t size = map.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each key-value pair.
				for (auto item = map.begin(); item != map.end(); ++item)
				{
					K key = item->first;
					V val = item->second;
					if (!IsSerializable(key))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(key, filename);
					if (!IsSerializable(val))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(val, filename);
				}
			}
			else
			{
				// Read map size and deserialize each key-value pair.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				map.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					if (!IsSerializable(key))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(key, filename);
					if (!IsSerializable(val))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(val, filename);
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
				SetOutFile(filename);
				size_t size = umap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each key-value pair.
				for (auto item = umap.begin(); item != umap.end(); ++item)
				{
					K key = item->first;
					V val = item->second;
					if (!IsSerializable(key))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(key, filename);
					if (!IsSerializable(val))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(val, filename);
				}
			}
			else
			{
				// Deserialize map size and each key-value pair.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				umap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					if (!IsSerializable(key))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(key, filename);
					if (!IsSerializable(val))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(val, filename);
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
				SetOutFile(filename);
				size_t size = mmap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each key-value pair.
				for (auto item = mmap.begin(); item != mmap.end(); ++item)
				{
					K key = item->first;
					V val = item->second;
					if (!IsSerializable(key))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(key, filename);
					if (!IsSerializable(val))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(val, filename);
				}
			}
			else
			{
				// Deserialize multimap size and key-value pairs.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				mmap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					if (!IsSerializable(key))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(key, filename);
					if (!IsSerializable(val))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(val, filename);
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
				SetOutFile(filename);
				size_t size = ummap.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each key-value pair.
				for (auto item = ummap.begin(); item != ummap.end(); ++item)
				{
					K key = item->first;
					V val = item->second;
					if (!IsSerializable(key))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(key, filename);
					if (!IsSerializable(val))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(val, filename);
				}
			}
			else
			{
				// Deserialize size and each key-value pair.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				ummap.clear();
				for (size_t i = 0; i < size; i++)
				{
					K key;
					V val;
					if (!IsSerializable(key))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(key, filename);
					if (!IsSerializable(val))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(val, filename);
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
				SetOutFile(filename);
				// Extract elements from the stack into a vector.
				std::vector<T> vec;
				std::stack<T> temp = stk;
				while (!temp.empty())
				{
					vec.push_back(temp.top());
					temp.pop();
				}
				// Write the number of elements.
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each element.
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(vec[i], filename);
				}
			}
			else
			{
				// Deserialize the vector and then reconstruct the stack.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(vec[i], filename);
				}
				// Rebuild the stack by pushing elements in reverse order.
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
				SetOutFile(filename);
				// Extract elements into a vector.
				std::vector<T> vec;
				std::queue<T> temp = q;
				while (!temp.empty())
				{
					vec.push_back(temp.front());
					temp.pop();
				}
				// Write the size of the queue.
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each element.
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(vec[i], filename);
				}
			}
			else
			{
				// Read the size and deserialize elements into a vector.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(vec[i], filename);
				}
				// Rebuild the queue by pushing each element.
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
				SetOutFile(filename);
				// Extract elements from the priority_queue into a vector.
				std::vector<T> vec;
				std::priority_queue<T> temp = pq;
				while (!temp.empty())
				{
					vec.push_back(temp.top());
					temp.pop();
				}
				// Write the number of elements.
				size_t size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each element.
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(vec[i], filename);
				}
			}
			else
			{
				// Deserialize into a vector and rebuild the priority_queue.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(vec[i], filename);
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
				SetOutFile(filename);
				// Write the size of the deque.
				size_t size = deq.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(size_t));
				// Serialize each element using its index.
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(deq[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(deq[i], filename);
				}
			}
			else
			{
				// Read size and then deserialize each element into a temporary vector.
				SetInFile(filename);
				size_t size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(size_t));
				std::vector<T> vec(size);
				for (size_t i = 0; i < size; i++)
				{
					if (!IsSerializable(vec[i]))
						throw cse::SerializationError("There is unserializable type.");
					Serialize(vec[i], filename);
				}
				// Clear and rebuild the deque from the vector.
				deq.clear();
				for (const T &item : vec)
				{
					deq.push_back(item);
				}
			}
		}

		/**
		 * @brief Specialized function for serializing custom user-defined classes/structs.
		 * @tparam T A user-defined class or struct that implements a `Serialize` member function.
		 * @param obj Reference to the object to write or read.
		 *
		 * This function calls `obj.Serialize(*this);`. Your custom class must define its own
		 * Serialize(Serializer&) method. Ensure that your Serialize method handles file paths or directory management.
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
