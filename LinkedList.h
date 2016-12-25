#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
namespace aisdi
{

template <typename Type>
class LinkedList
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
  class Item
  {
    public:
      value_type item;
      Item *next;
      Item *prev;
  };
 Item *first;
 Item *last;
 int   n;
public:
  LinkedList();

  LinkedList(std::initializer_list<Type> l);

  LinkedList(const LinkedList& other);

  LinkedList(LinkedList&& other);

  ~LinkedList();

  LinkedList& operator=(const LinkedList& other);

  LinkedList& operator=(LinkedList&& other);

  bool isEmpty() const;

  size_type getSize() const;

  void append(const Type& item);

  void prepend(const Type& item);

  void insert(const const_iterator& insertPosition, const Type& item);

  Type popFirst();

  Type popLast();

  void erase(const const_iterator& possition);

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded);


  iterator begin()
  {
    Iterator start(ConstIterator(this,first));
    return start;
  }

  iterator end()
  {
   Iterator endit(ConstIterator(this,nullptr));
   return endit;
  }

  const_iterator cbegin() const
  {
    return ConstIterator(this,first);
  }

  const_iterator cend() const
  {
    ConstIterator endit(this,nullptr);

    return endit;
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
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;
private:
  const LinkedList* mylist;
  Item* node;

public:
  explicit ConstIterator()  {}

  reference operator*() const
  {
    if (node == nullptr) throw std::out_of_range("there is no such item");
    return node->item;
  }

  Item* GetNode() const
  {
    return node;
  }

   ConstIterator(const ConstIterator &other)
  {
    mylist = other.mylist;
    node = other.node;
  }

ConstIterator(const LinkedList* l, Item* item)
  {
    mylist = l;
    node = item;
  }
  ConstIterator& operator++()
  {
     if (node == nullptr) throw std::out_of_range("you cannot increase iterator");
     node = node->next;
     return *this;
  }

  ConstIterator operator++(int)
  {
    if (node == mylist->last) node = nullptr;
    if ( node == nullptr ) throw std::out_of_range("you cannot increase iterator");
    ConstIterator tmp(*this);
    node = node->next;
    return tmp;
  }

  ConstIterator& operator--()
  {
     if (mylist->n == 0) throw std::out_of_range("you cannot decrease iterator");
     if (node == nullptr) node = mylist->last;
     else node = node->prev;
     return *this;
  }

  ConstIterator operator--(int)
  {
    if (mylist->n == 0) throw std::out_of_range("you cannot decrease iterator");
    ConstIterator tmp(*this);
    if (node == nullptr) node = mylist->last;
    else node = node->prev;
    return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
    ConstIterator tmp(*this);
    for (int i=0; i<d;i++)
    {
        if (node == nullptr) throw std::out_of_range("you cannot increase iterator");
        tmp.node = tmp.node->next;
    }
    return tmp;
  }

  ConstIterator operator-(difference_type d) const
  {
    ConstIterator tmp(*this);
    for (int i=0; i<d;i++)
    {
        if (tmp.node == mylist->first) throw std::out_of_range("you cannot decrease iterator");
        if (tmp.node == nullptr) tmp.node = mylist->last;
        else tmp.node = tmp.node->prev;
    }
    return tmp;
  }

  bool operator==(const ConstIterator& other) const
  {
     if (mylist == other.mylist && node == other.node) return true;
     else return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
     if (mylist != other.mylist || node != other.node) return true;
     else return false;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator()  {}

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
LinkedList <value_type> :: LinkedList()
{
    n = 0;
    first = nullptr;
    last = nullptr;
}

template <class value_type>
LinkedList <value_type> :: LinkedList(std::initializer_list<value_type> l)
{
    n=0;
    first =nullptr;
    last = nullptr;
    for (auto it=l.begin(); it != l.end(); it++)
      append(*it);
}
template <class value_type>
LinkedList <value_type> :: LinkedList(const LinkedList& other)
{
    Item *current, *current_other, *tmp;
    n = other.n;
    if (other.n == 0)
    {
        first = nullptr;
        last = nullptr;
        return;
    }
    current_other = other.first;
    first = new Item;
    first->prev = nullptr;
    current = first;
    while (current_other != other.last)
    {
       current->item = current_other->item;
       current->next = new Item;
       tmp = current;
       current = current->next;
       current->prev = tmp;
       current_other = current_other->next;
    }
    current->item = current_other->item;
    current->next = nullptr;
    last = current;

}

template <class value_type>
LinkedList <value_type> :: LinkedList(LinkedList&& other)
{

    first = other.first;
    last = other.last;
    n = other.n;
    other.first = nullptr;
    other.last = nullptr;
    other.n = 0;
}
template <class value_type>
LinkedList <value_type> :: ~LinkedList()
{
    if (n==0) return;
    Item *tmp, *current;
    current = first;
    while (current != last)
    {
        tmp = current->next;
        delete current;
        current = tmp;
    }
    delete current;
    n = 0;
}

template <class value_type>
bool LinkedList <value_type> :: isEmpty() const
{
    if (n == 0) return true;
    else return false;
}

template <class value_type>
size_t LinkedList <value_type> :: getSize() const
{
    return n;
}

template <class value_type>
void  LinkedList <value_type> :: append(const value_type& item)
{
    if (n == 0)
    {
        first = new Item;
        first->item = item;
        last = first;
        first->prev = nullptr;
        last->next = nullptr;
    }
    else
    {

        Item *tmp;
        //tmp = last;
        tmp = new Item;
        tmp->item = item;
        tmp->next = nullptr;
        tmp->prev = last;
        last->next = tmp;
        last =tmp;
    }
    n++;
}

template <class value_type>
void LinkedList <value_type> ::prepend(const value_type& item)
{
    Item* tmp;
    tmp = first;
    first = new Item;
    first->item = item;
    first->prev = nullptr;
    first->next = tmp;
    if (n != 0) tmp->prev = first;
    n++;
}


template <class value_type>
void LinkedList <value_type> :: insert(const const_iterator& insertPosition, const value_type& item)
{
    Item* tmp, *inserted, *prev;
    tmp = insertPosition.GetNode();

    inserted = new Item;
    inserted->item = item;

    if (tmp == first)
    {
        prepend(item);
        return;
    }

    if (tmp == nullptr ) //insertPosition = end or empty list
    {
        append(item);
        return;
    }

    prev = tmp->prev;
    tmp->prev = inserted;
    inserted->next = tmp;

    prev->next = inserted;
    inserted->prev = prev;
     n++;

}


template <class value_type>
value_type LinkedList <value_type> :: popFirst()
{
    Item *tmp;
    value_type item;
    if (n==0) throw std::logic_error("list is empty");
    n--;
    tmp =first;
    item = first->item;
    first = first->next;
    delete tmp;
    return item;
}
template <class value_type>
value_type LinkedList <value_type> :: popLast()
{
    Item *tmp;
    value_type item;
    if (n==0) throw std::logic_error("list is empty");
    n--;

    item = last->item;
    tmp = last;


    last = tmp->prev;
    if (last != nullptr) last->next = nullptr;
    delete tmp;
    return item;
}

template <class value_type>
 void LinkedList<value_type>:: erase(const const_iterator& possition)
 {

    Item* node, *tmp;
    node = possition.GetNode();
    if (node == nullptr) throw std:: out_of_range("");
    n--;
    if (n==0)
    {
        first = nullptr;
        last = nullptr;
        return;
    }

    if (node ==first)
    {
        tmp = first;
        first = first->next;
        delete tmp;
        first->prev =nullptr;
        return;
    }
    if (node == last)
    {
        tmp = last;
        last = last->prev;
        delete tmp;
        last->next = nullptr;
        return;
    }

    tmp = node->prev;
    tmp->next = node->next;
    node->next->prev = tmp;
    delete node;

 }


template <class value_type>
void LinkedList<value_type>:: erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
{
    Item* node1, *node2, *tmp, *current;
    node1 = firstIncluded.GetNode();
    node2 = lastExcluded.GetNode();
    if (node1 == node2) return;
    if (node1 == first)
    {
    for (auto it = firstIncluded; it != lastExcluded; it++)
        erase(begin());
    return;
    }

    tmp = node1->prev;

    if (node2 == nullptr && n!= 0)
    {
        last = tmp;
        last->next =nullptr;
    }
    else
    {
        tmp->next = node2;
        node2->prev = tmp;
    }

    current = node1;
    for (auto it = firstIncluded; it != lastExcluded; it++)
    {
        current = current->next;
        delete node1;
        node1 = current;
        n--;
    }
}


template <class value_type>
LinkedList<value_type>& LinkedList<value_type> :: operator=(LinkedList&& other)
{
    first = other.first;
    last = other.last;
    n = other.n;
    other.first = nullptr;
    other.last = nullptr;
    other.n = 0;
    return *this;
}

template <class value_type>
LinkedList<value_type>& LinkedList<value_type> :: operator=(const LinkedList& other)
{
    Item *current, *current_other, *tmp;
    n = other.n;
    if (other.n == 0)
    {
        first = nullptr;
        last = nullptr;
        return *this;
    }
    current_other = other.first;
    first = new Item;
    first->prev = nullptr;
    current = first;
    while (current_other != other.last)
    {
       current->item = current_other->item;
       current->next = new Item;
       tmp = current;
       current = current->next;
       current->prev = tmp;
       current_other = current_other->next;
    }
    current->item = current_other->item;
    current->next = nullptr;
    last = current;

    return *this;
}




}
#endif // AISDI_LINEAR_LINKEDLIST_H
