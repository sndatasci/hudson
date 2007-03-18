#include <string>
#include <set>

class Person
{
public:
  Person(int id, std::string name): _id(id), _name(name) { }

  bool operator<(const Person& p) const { return _id < p._id; }

private:
  int _id;
  std::string _name;
};


typedef std::set<Person> sPerson;

int main()
{
  sPerson sp;

  sp.insert(Person(5, "one"));
  sp.insert(Person(6, "two"));

  sPerson::iterator iter = sp.find(Person(5, "whatever"));

  return 0;
}
