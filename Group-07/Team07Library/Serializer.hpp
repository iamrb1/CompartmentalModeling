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
#include <filesystem>

/**
 * @brief Trait to mark custom user-defined types as serializable
 * Users should specialize this for their types.
 */
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
	 */
	class SerializationError : public std::exception
	{
	public:
		explicit SerializationError(const std::string &message)
			: message_("SerializationError: " + message) {}

		const char *what() const noexcept override { return message_.c_str(); }

	private:
		std::string message_;
	};

	/**
	 * @concept Serializable
	 * @brief Determines if a type T can be serialized by serializer S
	 */
	template <typename T, typename S>
	concept Serializable =
		requires(T &t, S &s) {
			s.Serialize(t);
		} ||
		(requires(T &t, S &s, bool b) {
			s.Serialize(t, b);
		} && is_custom_type<T>::value);

	/// Concept for containers supporting resize and iteration (excluding std::string)
	template <typename T>
	concept ResizableContainer = requires(T a, size_t n) {
		a.resize(n);
		a.begin();
		a.end();
	} && !std::is_same_v<T, std::string>;

	/// Concept for containers with key-value mapping
	template <typename T>
	concept MapContainer = requires(T a) {
		typename T::key_type;
		typename T::mapped_type;
		a.begin();
		a.end();
	};

	/// Concept for fixed-size containers (e.g. std::array)
	template <typename T>
	concept FixedSizeContainer = requires {
		std::tuple_size<T>::value;
	} && requires(T a) {
		a.begin();
		a.end();
	};

	/// Concept for set-like containers (excluding maps)
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
		std::ifstream inFile;					   ///< Input file stream.
		std::ofstream outFile;					   ///< Output file stream.
		std::string currType = "";				   ///< Type name read from file (for verification).
		bool setType = false;					   ///< Whether type info has been written yet.
		std::filesystem::path baseDirectory = "."; ///< Base directory for file path
		std::string filename = "";				   ///< Path to the file used for serialization.
		Mode mode_;								   ///< Current operating mode (SAVE or LOAD).

		/**
		 * @brief Opens the file stream based on the current mode
		 * @throws SerializationError if file cannot be opened
		 */
		void InitFile()
		{
			std::filesystem::path fullfile = baseDirectory / filename;
			if (filename.empty())
				throw SerializationError("Filename not set.");

			if (mode_ == Mode::SAVE)
			{
				std::filesystem::create_directories(fullfile.parent_path());
				outFile.open(fullfile, std::ios::binary | std::ios::trunc);
				if (!outFile)
					throw SerializationError("Cannot open file for writing.");
			}
			else
			{
				inFile.open(fullfile, std::ios::binary);
				if (!inFile)
					throw SerializationError("Cannot open file for reading.");
			}
		}

		/**
		 * @brief Writes type information to the file
		 * @tparam T The data type being serialized
		 */
		template <typename T>
		void PrintType(T &data)
		{
			std::string type = typeid(data).name();
			int length = static_cast<int>(type.size());
			outFile.write(reinterpret_cast<const char *>(&length), sizeof(int));
			outFile.write(type.c_str(), length);
			outFile.flush();
			setType = true;
		}

		/**
		 * @brief Reads type information from the file and sets currType
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
		 * @brief Constructs a serializer given a filename and I/O mode
		 * @param filename Path to the file
		 * @param mode SAVE or LOAD
		 */
		explicit Serializer(const std::string &filename, Mode mode = Mode::SAVE)
			: filename(filename), mode_(mode)
		{
			InitFile();
		}

		/**
		 * @brief Constructs a serializer given a base directory, filename and I/O mode
		 * @param baseDir Directory to prepend to the filename
		 * @param filename File name (relative to baseDir)
		 * @param mode SAVE or LOAD
		 */
		explicit Serializer(const std::string &baseDir, const std::string &filename, Mode mode)
			: baseDirectory(baseDir), filename(filename), mode_(mode)
		{
			InitFile();
		}

		~Serializer() = default;

		/// @return Current serializer mode
		constexpr Mode GetMode() const { return mode_; }

		/// @return True if in SAVE mode
		constexpr bool IsSave() const { return mode_ == Mode::SAVE; }

		/// @return True if in LOAD mode
		constexpr bool IsLoad() const { return mode_ == Mode::LOAD; }

		/**
		 * @brief Sets the mode of the serializer (does not re-open file)
		 * @param mode Desired I/O mode
		 */
		void SetMode(Mode mode) { mode_ = mode; }

		/**
		 * @brief Resets and reinitializes the file stream (e.g. after file name change)
		 */
		void ResetFileStream()
		{
			setType = false;
			currType = "";
			inFile.close();
			outFile.close();
			InitFile();
		}

		/**
		 * @brief Changes the output/input file and resets the stream
		 * @param newname New filename to use
		 */
		void ChangeName(const std::string &newname)
		{
			filename = newname;
			ResetFileStream();
		}

		/**
		 * @brief Changes the base directory (does not reopen file until ChangeName is called)
		 * @param newDir New base directory path
		 */
		void ChangeDirectory(const std::string &newDir)
		{
			baseDirectory = newDir;
			ResetFileStream();
		}

		/**
		 * @brief Checks if type T meets Serializable constraints
		 * @tparam T Type to check
		 * @return True if T is serializable
		 */
		template <typename T>
		static constexpr bool IsSerializable() { return Serializable<T, Serializer>; }

		/**
		 * @brief Serializes or deserializes an arithmetic type (e.g., int, float, double) to/from a binary file.
		 * @tparam T An arithmetic type (integral or floating-point).
		 * @param data Reference to the variable to write or read.
		 *
		 * In SAVE mode, writes the contents of `data` to the file.
		 * In LOAD mode, reads from the file into `data`.
		 */
		template <typename T>
			requires std::is_arithmetic_v<T>
		void Serialize(T &data)
		{
			if (mode_ == Mode::SAVE)
			{
				if (!setType)
					PrintType(data);
				outFile.write(reinterpret_cast<const char *>(&data), sizeof(T));
				outFile.flush();
			}
			else
			{
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
		 *
		 * In SAVE mode, writes the size of the string followed by the characters.
		 * In LOAD mode, reads the size and then resizes the string before reading its contents.
		 */
		void Serialize(std::string &str)
		{
			if (mode_ == Mode::SAVE)
			{
				if (!setType)
					PrintType(str);
				int length = str.size();
				outFile.write(reinterpret_cast<const char *>(&length), sizeof(int));
				outFile.write(str.c_str(), length);
				outFile.flush();
			}
			else
			{
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
		 *
		 * In SAVE mode, writes the size of the vector followed by each element.
		 * In LOAD mode, reads the size, resizes the vector, and then reads each element.
		 */
		template <ResizableContainer Vec>
		void Serialize(Vec &container)
		{
			using T = typename Vec::value_type;
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");

			if (mode_ == Mode::SAVE)
			{
				if (!setType)
					PrintType(container);
				int size = container.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				outFile.flush();
				for (auto &elem : container)
				{
					Serialize(elem);
				}
			}
			else
			{
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
					Serialize(elem);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::array` of fixed size to/from a binary file.
		 * @tparam T The type of elements stored in the array.
		 * @tparam N The size of the array.
		 * @param arr Reference to the array to write or read.
		 */
		template <typename T, std::size_t N>
		void Serialize(std::array<T, N> &arr)
		{
			// Check if the element type is serializable before processing.
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");
			if (mode_ == Mode::SAVE)
			{
				if (!setType)
					PrintType(arr);
				// Process each element in the fixed-size array.
				for (unsigned int i = 0; i < N; i++)
				{
					Serialize(arr[i]);
				}
			}
			else // LOAD mode
			{
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
					Serialize(arr[i]);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::set` and related of elements to/from a binary file.
		 * @param set Reference to the set to write or read.
		 */
		template <SetContainer Set>
		void Serialize(Set &set)
		{
			using T = typename Set::value_type;
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");

			if (mode_ == Mode::SAVE)
			{
				if (!setType)
					PrintType(set);
				int size = set.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				outFile.flush();
				for (auto item = set.begin(); item != set.end(); ++item)
				{
					T data = *item;
					Serialize(data);
				}
			}
			else
			{
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
					Serialize(item);
					set.insert(item);
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::map` and related (key-value pairs) to/from a binary file.
		 * @param map Reference to the map to write or read.
		 */
		template <MapContainer Map>
		void Serialize(Map &map)
		{
			using K = typename Map::key_type;
			using V = typename Map::mapped_type;
			static_assert(IsSerializable<K>(), "Trying to serialize a type that is not serializable.");
			static_assert(IsSerializable<V>(), "Trying to serialize a type that is not serializable.");

			if (mode_ == Mode::SAVE)
			{
				if (!setType)
					PrintType(map);
				int size = map.size();
				outFile.write(reinterpret_cast<const char *>(&size), sizeof(int));
				outFile.flush();
				for (auto item = map.begin(); item != map.end(); ++item)
				{
					K key = item->first;
					V val = item->second;
					Serialize(key);
					Serialize(val);
				}
			}
			else
			{
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
					Serialize(key);
					Serialize(val);
					map.insert({key, val});
				}
			}
		}

		/**
		 * @brief Serializes or deserializes a `std::stack` to/from a binary file.
		 * @tparam T The type of elements in the stack.
		 * @param stk Reference to the stack to write or read.
		 *
		 * In SAVE mode, this method extracts all elements into a temporary vector (top to bottom)
		 * and writes them. In LOAD mode, the elements are read into a vector and then pushed
		 * onto the stack in reverse order to restore the original stack order.
		 */
		template <typename T>
		void Serialize(std::stack<T> &stk)
		{
			// Check if the element type is serializable before processing.
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");
			if (mode_ == Mode::SAVE)
			{
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
				outFile.flush();
				// Serialize each element.
				for (int i = 0; i < size; i++)
				{
					Serialize(vec[i]);
				}
			}
			else
			{
				// Deserialize the vector and then reconstruct the stack.
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
					Serialize(vec[i]);
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
		 */
		template <typename T>
		void Serialize(std::queue<T> &q)
		{
			// Check if the element type is serializable before processing.
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");
			if (mode_ == Mode::SAVE)
			{
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
				outFile.flush();
				// Serialize each element.
				for (int i = 0; i < size; i++)
				{
					Serialize(vec[i]);
				}
			}
			else
			{
				// Read the size and deserialize elements into a vector.
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
					Serialize(vec[i]);
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
		 */
		template <typename T>
		void Serialize(std::priority_queue<T> &pq)
		{
			// Check if the element type is serializable before processing.
			static_assert(IsSerializable<T>(), "Trying to serialize a type that is not serializable.");
			if (mode_ == Mode::SAVE)
			{
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
				outFile.flush();
				// Serialize each element.
				for (int i = 0; i < size; i++)
				{
					Serialize(vec[i]);
				}
			}
			else
			{
				// Deserialize into a vector and rebuild the priority_queue.
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
					Serialize(vec[i]);
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
			requires is_custom_type<T>::value
		void Serialize(T &obj)
		{
			obj.Serialize(*this);
		}
	};
} // namespace cse

#endif // CSE_SERIALIZER_H
