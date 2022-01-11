// hash.cpp
// The implementation of the class HashTable
#include "hash.h"
#include "math.h"

HashTable::HashTable(unsigned size, hash_fn hash)
{
  if (size < MINPRIME)
  {
    m_capacity = MINPRIME;
  }
  else if (size > MAXPRIME)
  {
    m_capacity = MAXPRIME;
  }
  else if (!isPrime(size))
  {
    int newPrime = findNextPrime(size);
    m_capacity = newPrime;
  }
  else
  {
    m_capacity = size;
  }
  m_hash = hash;
  m_table = new Person[m_capacity];
  m_size = 0;
  m_numDeleted = 0;
  m_currentProb = LINEAR;
}

HashTable::~HashTable()
{
  delete[] m_table;
}

bool HashTable::insert(Person person)
{
  unsigned index = getIndex(person);
  if (m_table[index].key() == "" && m_table[index].serial() == 0)
  {
    m_table[index] = person;
    m_size++;
    if ((float)(m_size) / float(m_capacity) > 0.5)
    {
      if (m_currentProb == LINEAR)
      {
        setProbingPolicy(QUADRATIC);
      }
      else
      {
        setProbingPolicy(LINEAR);
      }
    }
    return true;
  }
  else
  {
    cout << "ERROR: Person already exists!" << endl;
    return false;
  }
}

unsigned HashTable::getIndex(Person person)
{
  unsigned index = m_hash(person.key());
  int i = 0;
  index = index % m_capacity;
  while (m_table[index].key() != "" && m_table[index].serial() != person.serial())
  {
    if (m_currentProb == LINEAR)
    {
      index = (m_hash(person.key()) + i) % m_capacity;
    }
    else
    {
      index = ((m_hash(person.key()) % m_capacity) + (i * i)) % m_capacity;
    }
    i++;
  }
  return index;
}

bool HashTable::remove(Person person)
{
  bool removed = false;
  unsigned index = getIndex(person);
  if (m_table[index].key() == person.key() && m_table[index].serial() == person.serial())
  {
    m_table[index] = DELETED;
    m_numDeleted++;
    removed = true;
  }
  if (m_numDeleted > (float)(m_size * .25))
  {
    if (m_currentProb == LINEAR)
    {
      setProbingPolicy(QUADRATIC);
    }
    else
    {
      setProbingPolicy(LINEAR);
    }
  }

  return removed;
}

void HashTable::setProbingPolicy(probing prob)
{
  m_currentProb = prob;
  rehash();
}

void HashTable::rehash()
{
  int newSize = findNextPrime((m_size - m_numDeleted) * 4);
  Person *oldTable = new Person[m_capacity];
  int oldSize = m_capacity;
  for (int i = 0; i < oldSize; i++)
  {
    oldTable[i] = m_table[i];
  }

  delete[] m_table;
  m_table = new Person[newSize];
  m_capacity = newSize;

  for (int i = 0; i < oldSize; i++)
  {
    if (oldTable[i].key() != DELETEDKEY && oldTable[i].key() != "" &&
        oldTable[i].serial() != 0)
    {
      unsigned index = getIndex(oldTable[i]);
      m_table[index] = oldTable[i];
    }
  }

  delete[] oldTable;
  oldTable = nullptr;

  m_size = m_size - m_numDeleted;
  m_numDeleted = 0;
}

float HashTable::lambda() const
{
  return m_size / m_capacity;
}

float HashTable::deletedRatio() const
{
  return m_numDeleted / m_size;
}

void HashTable::dump() const
{
  for (int i = 0; i < m_capacity; i++)
  {
    cout << "[" << i << "] : " << m_table[i] << endl;
  }
}

bool HashTable::isPrime(int number)
{
  // If number is prime this function returns true
  // otherwise it returns false
  bool result = true;
  for (int i = 2; i <= number / 2; ++i)
  {
    if (number % i == 0)
    {
      result = false;
      break;
    }
  }
  return result;
}

int HashTable::findNextPrime(int current)
{
  //we won't go beyond MAXPRIME
  //the smallest prime would be next after MINPRIME
  if (current < MINPRIME)
    current = MINPRIME;
  for (int i = current; i < MAXPRIME; i++)
  {
    for (int j = 2; j * j <= i; j++)
    {
      if (i % j == 0)
        break;
      else if (j + 1 > sqrt(i) && i != current)
      {
        return i;
      }
    }
  }
  //if a user tries to go over MAXPRIME
  return MAXPRIME;
}
