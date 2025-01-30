
#include "MyMap.h"

// Should accept a text file via std in
// Should build a map of the counts of the characters in that file
// Do not add newlines (\n) to the dictionary (they'll mess up print)
// ./a.out < sample_input.txt should be the form of input
void get_char_frequency(MyMap<char, int> &in_tree);

int main() {

  MyMap<char, int> map_obj;
  get_char_frequency(map_obj);
  map_obj.print();

  return 0;
}

void get_char_frequency(MyMap<char, int> &in_tree) {
  std::string str;
  while (cin >> str) {
    for (long character = 0; character < static_cast<int>(str.size());
         character++) {
      if (in_tree.find(str.at(static_cast<int>(character))) == nullptr) {
        in_tree.insert(str.at(static_cast<int>(character)), 1);
      } else {
        in_tree.find(str.at(static_cast<int>(character)))->value += 1;
      }
    }
  }
}
