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

template <typename T>
struct is_custom_type : std::false_type
{
};

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
	concept Serializable =
		requires(T &t, S &s, const std::string &filename) {
			s.Serialize(t, filename);
		} ||
		(requires(T &t, S &s, const std::string &filename, bool b) {
			s.Serialize(t, filename, b);
		} &&
		 is_custom_type<T>::value);

	template <typename T>
	concept ResizableContainer = requires(T a, size_t n) {
		a.resize(n);
		a.begin();
		a.end();
	} && !std::is_same_v<T, std::string>;

	template <typename T>
	concept MapContainer = requires(T a) {
		typename T::key_type;
		typename T::mapped_type;
		a.begin();
		a.end();
	};

	template <typename T>
	concept FixedSizeContainer = requires {
		std::tuple_size<T>::value;
	} && requires(T a) {
		a.begin();
		a.end();
	};

	template <typename T>
	concept SetContainer = requires(T a, typename T::value_type val) {
		a.insert(val);
		a.begin();
		a.end();
	} && !MapContainer<T>;

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
		std::string currType = "";	  ///< Current type for Loading
		bool setType = false;		  ///< Bool indicating if the type is save in the file or not

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
				setType = false;
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
				currType = "";
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

		/**
		 * @brief Print the type of data in the beginning of the output file.
		 * @tparam T The data
		 * @param filename The path to the output file.
		 *
		 * Print the type of the data in the beginning of an OPENED file, with outFile already been set
		 */
		template <typename T>
		void PrintType(T &data)
		{
			std::string type = typeid(data).name();
			int length = type.size();
			outFile.write(reinterpret_cast<const char *>(&length), sizeof(int));
			outFile.write(type.c_str(), length);
			outFile.flush();
			setType = true;
		}

		/**
		 * @brief Print the type of data in the beginning of the output file.
		 *
		 * Print the type of the data in the beginning of an OPENED file, with outFile already been set
		 */
		void GetType()
		{
			int length;
			inFile.read(reinterpret_cast<char *>(&length), sizeof(int));
			currType.resize(length);
			inFile.read(&currType[0], length);
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
			setType = false;
			currType = "";
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
		static constexpr bool IsSerializable() { return Serializable<T, Serializer>; }

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
				if (!setType)
					PrintType(data);
				outFile.write(reinterpret_cast<const char *>(&data), sizeof(T));
				outFile.flush();
			}
			else
			{
				// Set the input file stream and read the binary data.
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					std::string datatype = typeid(data).name();
					if (currType != datatype)
					{
						throw cse::SerializationError("Type mismatch or corrupted file.");
					}
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
				// Write string size and characters.
				SetOutFile(filename);
				if (!setType)
					PrintType(str);
				int length = str.size();
				outFile.write(reinterpret_cast<const char *>(&length), sizeof(int));
				outFile.write(str.c_str(), length);
				outFile.flush();
			}
			else
			{
				// Read string size and then its content.
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					std::string datatype = typeid(str).name();
					if (currType != datatype)
					{
						throw cse::SerializationError("Type mismatch or corrupted file.");
					}
				}
				int length;
				inFile.read(reinterpret_cast<char *>(&length), sizeof(int));
				str.resize(length);
				inFile.read(&str[0], length);
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::vector` or related of any type to/from a binary file.
		 * @tparam T The type of elements stored in the vector.
		 * @param container Reference to the resizable container to write or read.
		 * @param filename Path to the binary file.
		 *
		 * In SAVE mode, writes the size of the vector followed by each element.
		 * In LOAD mode, reads the size, resizes the vector, and then reads each element.
		 */
		template <ResizableContainer Vec>
		void Serialize(Vec &container, const std::string &filename)
		{
			using T = typename Vec::value_type;
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");

			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				if (!setType)
					PrintType(container);
				int size = container.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				for (auto &elem : container)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<T>>::value)
						Serialize(elem, filename, true);
					else
						Serialize(elem, filename);
				}
			}
			else
			{
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					if (currType != typeid(container).name())
						throw SerializationError("Type mismatch or corrupted file.");
				}
				int size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(int));
				container.resize(size);
				for (auto &elem : container)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<T>>::value)
						Serialize(elem, filename, true);
					else
						Serialize(elem, filename);
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
			// Check if the element type is serializable before processing.
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");
			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				if (!setType)
					PrintType(arr);
				// Process each element in the fixed-size array.
				for (unsigned int i = 0; i < N; i++)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(arr[i])>>::value)
						Serialize(arr[i], filename, true);
					else
						Serialize(arr[i], filename);
				}
			}
			else // LOAD mode
			{
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					std::string datatype = typeid(arr).name();
					if (currType != datatype)
					{
						throw cse::SerializationError("Type mismatch or corrupted file.");
					}
				}
				// Read each element from the file.
				for (unsigned int i = 0; i < N; i++)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(arr[i])>>::value)
						Serialize(arr[i], filename, true);
					else
						Serialize(arr[i], filename);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::set` and related of elements to/from a binary file.
		 * @param set Reference to the set to write or read.
		 * @param filename Path to the binary file.
		 */
		template <SetContainer Set>
		void Serialize(Set &set, const std::string &filename)
		{
			using T = typename Set::value_type;
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");

			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				if (!setType)
					PrintType(set);
				int size = set.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				for (auto item = set.begin(); item != set.end(); ++item)
				{
					T data = *item;
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(data)>>::value)
						Serialize(data, filename, true);
					else
						Serialize(data, filename);
				}
			}
			else
			{
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					if (currType != typeid(set).name())
						throw SerializationError("Type mismatch or corrupted file.");
				}
				int size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(int));
				set.clear();
				for (int i = 0; i < size; ++i)
				{
					T item;
					if constexpr (is_custom_type<std::remove_cvref_t<T>>::value)
						Serialize(item, filename, true);
					else
						Serialize(item, filename);
					set.insert(item);
				}
			}
		}

		// /**
		//  * @brief Serializes or deserializes a `std::map` and related (key-value pairs) to/from a binary file.
		//  * @param map Reference to the map to write or read.
		//  * @param filename Path to the binary file.
		//  */
		template <MapContainer Map>
		void Serialize(Map &map, const std::string &filename)
		{
			using K = typename Map::key_type;
			using V = typename Map::mapped_type;
			static_assert(IsSerializable<K>(), "Trying to serialize a type that is not serializable.");
			static_assert(IsSerializable<V>(), "Trying to serialize a type that is not serializable.");

			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				if (!setType)
					PrintType(map);
				int size = map.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				for (auto item = map.begin(); item != map.end(); ++item)
				{
					K key = item->first;
					V val = item->second;
					if constexpr (is_custom_type<std::remove_cvref_t<K>>::value)
						Serialize(key, filename, true);
					else
						Serialize(key, filename);

					if constexpr (is_custom_type<std::remove_cvref_t<V>>::value)
						Serialize(val, filename, true);
					else
						Serialize(val, filename);
				}
			}
			else
			{
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					if (currType != typeid(map).name())
						throw SerializationError("Type mismatch or corrupted file.");
				}
				int size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(int));
				map.clear();
				for (int i = 0; i < size; i++)
				{
					K key;
					V val;
					if constexpr (is_custom_type<std::remove_cvref_t<K>>::value)
						Serialize(key, filename, true);
					else
						Serialize(key, filename);

					if constexpr (is_custom_type<std::remove_cvref_t<V>>::value)
						Serialize(val, filename, true);
					else
						Serialize(val, filename);

					map.insert({key, val});
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
			// Check if the element type is serializable before processing.
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");
			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				if (!setType)
					PrintType(stk);
				// Extract elements from the stack into a vector.
				std::vector<T> vec;
				std::stack<T> temp = stk;
				while (!temp.empty())
				{
					vec.push_back(temp.top());
					temp.pop();
				}
				// Write the number of elements.
				int size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				// Serialize each element.
				for (int i = 0; i < size; i++)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(vec[i])>>::value)
						Serialize(vec[i], filename, true);
					else
						Serialize(vec[i], filename);
				}
			}
			else
			{
				// Deserialize the vector and then reconstruct the stack.
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					std::string datatype = typeid(stk).name();
					if (currType != datatype)
					{
						throw cse::SerializationError("Type mismatch or corrupted file.");
					}
				}
				int size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(int));
				std::vector<T> vec(size);
				for (int i = 0; i < size; i++)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(vec[i])>>::value)
						Serialize(vec[i], filename, true);
					else
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
			// Check if the element type is serializable before processing.
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");
			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				if (!setType)
					PrintType(q);
				// Extract elements into a vector.
				std::vector<T> vec;
				std::queue<T> temp = q;
				while (!temp.empty())
				{
					vec.push_back(temp.front());
					temp.pop();
				}
				// Write the size of the queue.
				int size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				// Serialize each element.
				for (int i = 0; i < size; i++)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(vec[i])>>::value)
						Serialize(vec[i], filename, true);
					else
						Serialize(vec[i], filename);
				}
			}
			else
			{
				// Read the size and deserialize elements into a vector.
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					std::string datatype = typeid(q).name();
					if (currType != datatype)
					{
						throw cse::SerializationError("Type mismatch or corrupted file.");
					}
				}
				int size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(int));
				std::vector<T> vec(size);
				for (int i = 0; i < size; i++)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(vec[i])>>::value)
						Serialize(vec[i], filename, true);
					else
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
			// Check if the element type is serializable before processing.
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");
			if (mode_ == Mode::SAVE)
			{
				SetOutFile(filename);
				if (!setType)
					PrintType(pq);
				// Extract elements from the priority_queue into a vector.
				std::vector<T> vec;
				std::priority_queue<T> temp = pq;
				while (!temp.empty())
				{
					vec.push_back(temp.top());
					temp.pop();
				}
				// Write the number of elements.
				int size = vec.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				// Serialize each element.
				for (int i = 0; i < size; i++)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(vec[i])>>::value)
						Serialize(vec[i], filename, true);
					else
						Serialize(vec[i], filename);
				}
			}
			else
			{
				// Deserialize into a vector and rebuild the priority_queue.
				SetInFile(filename);
				if (currType == "")
				{
					GetType();
					std::string datatype = typeid(pq).name();
					if (currType != datatype)
					{
						throw cse::SerializationError("Type mismatch or corrupted file.");
					}
				}
				int size;
				inFile.read(reinterpret_cast<char *>(&size), sizeof(int));
				std::vector<T> vec(size);
				for (int i = 0; i < size; i++)
				{
					if constexpr (is_custom_type<std::remove_cvref_t<decltype(vec[i])>>::value)
						Serialize(vec[i], filename, true);
					else
						Serialize(vec[i], filename);
				}
				pq = std::priority_queue<T>(vec.begin(), vec.end());
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
		template <typename T>
		void Serialize(T &obj, const std::string &filename, bool isCustom)
		{
			assert(isCustom == is_custom_type<T>::value);
			obj.Serialize(*this, filename);
		}
	};
} // namespace cse

#endif // CSE_SERIALIZER_H
