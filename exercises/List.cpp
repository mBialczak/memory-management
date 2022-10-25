#include <iostream>
#include <memory>

using namespace std;

class Node {
public:
    Node(const int v)
        : next(nullptr),
          value(v) {}

    unique_ptr<Node> next;
    int value;
};

class List {
public:
    List();
    void add(unique_ptr<Node> node);
    Node* get(const int value);

private:
    unique_ptr<Node> first;
};

List::List()
    : first(nullptr) {}

void List::add(unique_ptr<Node> node) {
    if (!first) {
        first = move(node);
    } else {
        Node* current = first.get();
        while (current->next) {
            current = current->next.get();
        }
        current->next = move(node);
    }
}

Node* List::get(const int value) {
    if (!first) {
        cout << "List is empty!" << endl;
        return nullptr;
    } else {
        Node* current = first.get();
        do {
            if (current->value == value) {
                cout << "Found value " << current->value << endl;
                return current;
            } else {
                cout << "Going through " << current->value << endl;
                current = current->next.get();
            }
        } while (current);
        cout << "Not found: value " << value << endl;
        return nullptr;
    }
}

int main() {
    List lista;
    unique_ptr<Node> node4 = make_unique<Node>(4);
    unique_ptr<Node> node7 = make_unique<Node>(7);

    lista.add(move(node4));
    lista.add(make_unique<Node>(2));
    lista.add(move(node7));
    lista.add(make_unique<Node>(9));
    auto node = lista.get(1);

    if (node)
        cout << node->value << '\n';

    return 0;
}
