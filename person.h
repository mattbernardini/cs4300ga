#ifndef PERSON_H
#define PERSON_H

namespace AI
{
class GaDriver;
class Person
{
    friend class GaDriver;
private:
    int * bitString;
    int id;
    int stringSize;
    void generateBitString();
public:
  Person(int stringSize);
  Person(Person * p);
  ~Person();
  Person& operator= (Person * lhs);
  bool operator== (Person * lhs);
  int getFitness();
  void mutate();
};
}

#endif /* end of include guard: PERSON_H */
