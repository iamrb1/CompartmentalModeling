/**
 * @file ReferenceVector.cpp
 *
 * @author Calen Green
 */

#include "ReferenceVector.h"

namespace cse {
/**
* Assigns a new Datum value to the Proxy.
*
* @param value The Datum instance to assign.
* @return Proxy& A reference to the updated Proxy object.
*/
ReferenceVector::Proxy& ReferenceVector::Proxy::operator=(const Datum& value) {
  *mDatum = value;
  return *this;
}

/**
* Removes the last element in the reference vector.
*/
void ReferenceVector::pop_back() {
  if (!mData.empty())
    mData.pop_back();
}

/**
* Accesses a Datum element at the given index using Proxy.
*
* @param index The position of the element.
* @return Proxy A Proxy object referencing the element.
*/
ReferenceVector::Proxy ReferenceVector::operator[](size_t index) {
  if (index >= mData.size())
    throw std::out_of_range("Index out of range");
  return Proxy(mData[index]);
}

/**
* Returns a reference to the datum at a position.
*
* @param index The desired index.
* @return Datum& The reference to a Datum.
*/
Datum& ReferenceVector::at(size_t index) {
  if (index >= mData.size())
    throw std::out_of_range("Index out of range");
  return *mData[index];
}

/**
* Returns a reference to the Datum at the front of the vector.
*
* @return Datum& The reference to the Datum at the front.
*/
Datum& ReferenceVector::front() {
  if (mData.empty())
    throw std::out_of_range("ReferenceVector is empty");
  return *mData.front();
}

/**
* Returns a reference to the Datum at the back of the vector.
*
* @return Datum& The reference to the Datum at the back.
*/
Datum& ReferenceVector::back() {
  if (mData.empty())
    throw std::out_of_range("ReferenceVector is empty");
  return *mData.back();
}

}  // namespace cse