#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <cstring>
#include <stdexcept>
#define CAPACITY 50

namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  pointer first;
  size_type capacity; //max vector's size
  size_type n; //how many elements vector inludes

public:

  Vector(); //creates empty vector, with defined capacity
  Vector(std::initializer_list<Type> l);
  Vector(const Vector& other);
  Vector(Vector&& other);
  ~Vector();

  Vector& operator=(const Vector other);
  value_type& operator[](int i);
  bool isEmpty() const;
  size_type getSize() const;
  void swapVectors (Vector &x, Vector &y);
  void addMemory(); //function doubles vector's capacity
  void append(const Type& item);
  void prepend(const Type& item);
  Type popFirst();
  Type popLast();
  void insert(const const_iterator& insertPosition, const Type& item);
  void erase(const const_iterator& position);
  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded);

  iterator begin()
  {
    Iterator start(ConstIterator(this,0));
    return start;
  }

  iterator end()
  {
    Iterator last(ConstIterator(this,n));
    return last;
  }

  const_iterator cbegin() const
  {
    return ConstIterator(this,0);
  }

  const_iterator cend() const
  {
    return ConstIterator(this,n);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
private:
  const Vector<value_type> *p;
  int index;
public:
  explicit ConstIterator() {}

  ConstIterator(const Vector<value_type> *vec, int i)
  {
    p = vec;
    if (i > (vec->n +1) || i<0) throw std::out_of_range ("");
    index = i;
  }

  ConstIterator(const ConstIterator &other)
  {
    p = other.p;
    index = other.index;
  }

  reference operator*() const
  {
    if (index >= p->n || index<0) throw std::out_of_range("there is no element with this index");
    return p->first[index];
  }

  int getIndex() const
  {
    return index;
  }

  ConstIterator& operator++()
  {
     if (index >= p->n) throw std::out_of_range("you cannot increase iterator");
     index++;
     return *this;
  }

  ConstIterator operator++(int)
  {
    if (index >= p->n) throw std::out_of_range("you cannot increase iterator");
    ConstIterator tmp(*this);
    index++;
    return tmp;
  }

  ConstIterator& operator--()
  {
     if (index == 0) throw std::out_of_range("iterator cannot be smaller than 0");
     index--;
     return *this;
  }

  ConstIterator operator--(int)
  {
    if (index == 0) throw std::out_of_range("iterator cannot be smaller than 0");
    ConstIterator tmp(*this);
    index--;
    return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
    if (index > (p->n-d)) throw std::out_of_range("you cannot incerease iterator");
    ConstIterator tmp(*this);
    tmp.index += d;
    return tmp;
  }

  ConstIterator operator-(difference_type d) const
  {
    if (index < d) throw std::out_of_range("iterator cannot be smaller than 0");
    ConstIterator tmp(*this);
    tmp.index = tmp.index -d;
    return tmp;
  }

  bool operator==(const ConstIterator& other) const
  {
    if (p == other.p && index == other.index) return true;
    return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if (p == other.p && index == other.index) return false;
    return true;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
  public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator() {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}


  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

template <class value_type>
Vector <value_type> :: Vector()
{
    first = new value_type [CAPACITY]; // CAPACITY is defined as 50
    n = 0;
    capacity = CAPACITY;
}

template <class value_type>
Vector <value_type> :: Vector(std::initializer_list<value_type> l)
{
    n = l.size();
    int i=0;
    first = new value_type [CAPACITY + n];
    for (auto it=l.begin(); it <= l.end(); it++)
        {
            first[i] = *it;
            i++;
        }
}

template <class value_type>
Vector <value_type> :: Vector(const Vector &other)
{
    first = new value_type [other.n];
    memcpy(first,other.first,sizeof(value_type)*other.n);
    n = other.n;
    capacity = other.capacity;
}

template <class value_type>
Vector <value_type> :: Vector( Vector &&other): Vector()
{
   swapVectors(*this, other);
}

template <class value_type>
Vector <value_type> :: ~Vector()
{
    delete [] first;
}


template <class value_type>
Vector<value_type>& Vector<value_type>:: operator=(Vector other)
{
    swapVectors (*this, other);
    return *this;
}

template <class value_type>
void Vector<value_type>:: swapVectors(Vector &x, Vector &y)
{
   std::swap (x.capacity, y.capacity);
   std::swap (x.n, y.n);
   std::swap (x.first, y.first);
}

template <class value_type>
bool Vector<value_type>:: isEmpty() const
{
    if (n == 0) return true;
    return false;
}

template <class value_type>
size_t Vector<value_type>:: getSize() const
{
   return n;
}

template <class value_type>
value_type& Vector<value_type>:: operator[](int i)
{
    return first[i];
}

template <class value_type>
void Vector<value_type>:: addMemory()
{
    pointer p;
    p = new value_type [2*n];
    capacity = 2*n;
    memcpy(p,first,sizeof(value_type)*n);
    delete[] first;
    first = p;
}

template <class value_type>
void Vector<value_type>:: append(const value_type& item)
{
    if (n == capacity) addMemory();
    first[n] = item;
    n++;
}

template <class value_type>
void Vector<value_type>::prepend(const value_type& item)
{
    if (n == capacity) addMemory();
    memmove(&first[1], first, sizeof(value_type)*n);
    first[0] = item;
    n++;
}

template <class value_type>
void Vector<value_type>:: insert(const const_iterator& insertPosition, const value_type& item)
{
    int i=insertPosition.getIndex();
    if (n == capacity) addMemory();
    memmove(&first[i+1],&first[i], sizeof(value_type)*(n-i));
    first[i]=item;
    n++;
}

template <class value_type>
value_type Vector<value_type>::popLast()
{
    if (n==0) throw std::logic_error("vector is empty");
    value_type item = first[n-1];
    n--;
    return item;
}

template <class value_type>
value_type Vector<value_type>::popFirst()
{
    if (n==0) throw std::logic_error("vector is empty");
    value_type item = first[0];
    memmove(first, &first[1], sizeof(value_type)*n);
    n--;
    return item;
}

template <class value_type>
void Vector<value_type>::erase(const const_iterator& position)
{
    int i = position.getIndex();
    if (i>=n || i<0 ) throw std::out_of_range("");
    memmove(&first[i], &first[i+1], sizeof(value_type)*(n-i));
    n--;
}

 template <class value_type>
 void Vector<value_type>:: erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
 {
    int i = firstIncluded.getIndex();
    int j = lastExcluded.getIndex();
    if (n < (j-i)) throw std::out_of_range ("there is too little elements");
    memmove(&first[i], &first[j], sizeof(value_type)*( n-j));
    n = n-(j-i);
 }

}
#endif // AISDI_LINEAR_VECTOR_H
