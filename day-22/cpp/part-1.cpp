#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct Node
{
  Node(int value, Node* parent=nullptr) 
  {
    v = value;
    p = parent;
    if (parent)
      parent->n = this;
  }

  int v = 0;
  Node* p = nullptr;
  Node* n = nullptr;
};

int main(int argc, char* argv[])
{
  if (argc != 2)
    return EXIT_FAILURE;
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open())
    return EXIT_FAILURE;

  constexpr int LEN = 10'007;

  Node* head = new Node(0);
  Node* tail = head;
  for (auto i = 1; i < LEN; i++)
    tail = new Node(i, tail);

  auto deal_reverse = [&]() {
    head = tail;
    Node* c = tail;
    for (auto i = 1; i < LEN; i++) {
      c = tail;
      tail = c->p;
      c->p = c->n;
      c->n = tail;
    }
    head->p = nullptr;
    tail->p = c;
    tail->n = nullptr;
  };

  auto cut =  [&](int n) {
    if (n == 0) {
      return;
    } else if (n > 0) {
      n %= LEN;
      if (n == 0)
        return;

      Node* cutoff = head;
      for (auto i = 1; i < n; i++)
        cutoff = cutoff->n;

      head->p = tail;
      tail->n = head;
      head = cutoff->n;
      tail = cutoff;
      head->p = nullptr;
      tail->n = nullptr;
    } else {
      n = -((-n) % LEN);
      if (n == 0)
        return;

      Node* cutoff = tail;
      for (auto i = -1; i > n; i--)
        cutoff = cutoff->p;

      head->p = tail;
      tail->n = head;
      head = cutoff;
      tail = cutoff->p;
      head->p = nullptr;
      tail->n = nullptr;
    }
  };

  auto deal_inc = [&](int n) {
    if (n <= 1)
      return;

    static Node* reindex[LEN];
    head->p = tail;
    tail->n = head;

    Node* c = head;
    for (auto i = 0, j = 0; i < LEN; i++, j = (j+n)%LEN) {
      reindex[j] = c;
      c = c->n;
    }

    c = head;
    for (auto i = 0; i < LEN; i++) {
      c->n = reindex[i];
      reindex[i]->p = c;
      c = c->n;
      tail = c;
    }

    head->p = nullptr;
    tail->n = nullptr;
  };

  //auto print = [&](Node* l) {
  //  Node* c = l;
  //  while (c) {
  //    std::cout << c->v << " ";
  //    //std::cout << c->p << " <- " 
  //    //          << c << " (" << c->v << ") -> "
  //    //          << c->n << std::endl;;
  //    c = c->n;
  //  }
  //  std::cout << std::endl;
  //};

  //auto copy = [&](Node* l) {
  //  auto* n = new Node(l->v);
  //  auto* t = n;
  //  while (l->n) {
  //    l = l->n;
  //    t = new Node(l->v, t);
  //  }
  //  return n;
  //};

  auto del = [&](Node* l) {
    while (l) {
      Node* t = l;
      l = l->n;
      delete(t);
    }
  };

  //auto same = [&](Node* a, Node* b) {
  //  while (a and b) {
  //    if (a->v != b->v)
  //      return false;
  //    a = a->n;
  //    b = b->n;
  //  }
  //  return (not a and not b);
  //};

  while (true) {
    std::string line;
    std::getline(infile, line);
    if (infile.eof())
      break;

    if (line == "deal into new stack") {
      deal_reverse();
    } else {
      std::istringstream iss{line};
      std::string s;
      int i;
      iss >> s;
      if (s == "cut") {
        iss >> i;
        cut(i);
      } else {
        iss >> s >> s;
        iss >> i;
        deal_inc(i);
      }
    }
  }

  Node* c = head;
  auto i = 0;
  while (c->v != 2019) {
    c = c->n;
    i++;
  }
  std::cout << i << std::endl;
  del(head);

  return EXIT_SUCCESS;
}

