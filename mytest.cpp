#include "hash.h"
#include <random>
using namespace std;

std::mt19937 generator(10); // 10 is the fixed seed value

std::uniform_int_distribution<> distribution(1000, 99999);

unsigned int hashCode(const string str);

unsigned int vaccineSerial() { return distribution(generator); }

class Tester
{ // Tester class to implement test functions
public:
   Tester() {}
   ~Tester() {}

private:
};

int main()
{
   int tempSerials[50] = {0};
   int secondIndex = 0;
   HashTable aTable(101, hashCode);
   int temp = 0;

   cout << "\n\tTESTING CONSTRUCTOR FUNCTIONALITY...\n"
        << endl;

   //Test that the capacity is between MINPRIME and MAXPRIME
   cout << "Testing that capacity is set to MINPRIME when 99 is passed in:"
        << endl;

   HashTable bTable(99, hashCode);
   if (bTable.tableSize() == MINPRIME)
   {
      cout << "\tTest Passed!" << endl;
   }
   else
   {
      cout << "\tTest Failed!" << endl;
   }

   cout << "\nTesting that capacity is set to MAXPRIME when 99994 is passed in:"
        << endl;
   HashTable cTable(99994, hashCode);
   if (cTable.tableSize() == MAXPRIME)
   {
      cout << "\tTest Passed!" << endl;
   }
   else
   {
      cout << "\tTest Failed!" << endl;
   }

   cout << "\nTesting that capacity is set to a prime num when a non-prime num is passed in:"
        << endl;
   HashTable dTable(860, hashCode);
   if (dTable.tableSize() != 860)
   {
      cout << "\tTest Passed!" << endl;
   }
   else
   {
      cout << "\tTest Failed!" << endl;
   }

   cout << "\n\tTESTING INSERTION FUNCTIONALITY...\n"
        << endl;

   cout << "Testing insertion of non-colliding keys:" << endl;
   bTable.insert(Person("Ann", 12));
   bTable.insert(Person("Bill", 22));
   bTable.insert(Person("Don", 55));
   bTable.insert(Person("Zoe", 2));
   bTable.insert(Person("Lilly", 72));
   bTable.insert(Person("Zack", 78));

   bTable.dump();

   //we insert 50 Person objects with the same key, person's name is the key
   cout << "\nTesting insertion of colliding keys:" << endl;
   for (int i = 0; i < 50; i++)
   {
      temp = vaccineSerial(); // generates a random number
      if (i % 3 == 0)
      { //this saves 17 numbers from the generated numbers for later use
         tempSerials[secondIndex] = temp;
         cout << temp << " was saved for later use." << endl;
         secondIndex++;
      }
      // print the random numbers which are inserted
      cout << "Insertion # " << i << " => " << temp << endl;
      aTable.insert(Person("John", temp));
   }

   cout << "\nDump after 50 insertions in a table with 101 buckets:" << endl;
   aTable.dump();

   //test that inserting same person will trigger error message;
   cout << "\nTesting insertion of a Person that already exists:" << endl;
   if (!aTable.insert(Person("John", 49964)))
   {
      cout << "\tTest Passed!" << endl;
   }
   else
   {
      cout << "\tTest Failed!" << endl;
   }

   // We insert a data point to change the load factor, this should initiate a rehash
   cout << "\nTesting that probing is changed and rehash is triggered:" << endl;
   temp = vaccineSerial();
   cout << "Insertion # 51"
        << " => " << temp << endl;
   aTable.insert(Person("John", temp));

   cout << "\nDump after 51st insertion, the rehash should have happended to a table with 211 buckets:"
        << endl;
   aTable.dump();

   cout << "\n\tTESTING REMOVE FUNCTIONALITY...\n"
        << endl;

   // Now we remove some data points which were saved in tempSerials array
   // Deleting 13 data points changes the deleteRatio, it should initiate a rehash
   for (int i = 0; i < 14; i++)
      aTable.remove(Person("John", tempSerials[i]));
   cout << "Dump after 14 removes, a rehash should have happened to a table with 157 buckets,"
        << endl;
   cout << "after 13 removals the rehash happens, then the 14th removal will be tagged as DELETED in the new table."
        << endl;
   aTable.dump();
   cout << endl;

   //Test removal of object that doesn't exist
   cout << "Testing removal of a Person that doesn't exist:" << endl;
   if (!aTable.remove(Person("Sam", 5)))
   {
      cout << "\tTest Passed!" << endl;
   }
   else
   {
      cout << "\tTest Failed!" << endl;
   }

   cout << endl;
   return 0;
}

unsigned int hashCode(const string str)
{
   unsigned int val = 0;
   const unsigned int thirtyThree = 33; // magic number from textbook
   for (int i = 0; i < str.length(); i++)
      val = val * thirtyThree + str[i];
   return val;
}
