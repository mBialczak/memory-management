#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>

using namespace std;

class Node {
public:
    Node(const int v)
        : next(nullptr),
          value(v) {}

    unique_ptr<Node> next;
    int value;
};

class EmptyListError : public runtime_error {
public:
    EmptyListError(const std::string& message)
        : runtime_error(message) {}
};

class List {
public:
    List();
    void add(unique_ptr<Node> node);
    Node* getFirst(const int value);
    std::list<Node*> getAll(const int value);

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

Node* List::getFirst(const int value) {
    if (!first) {
        throw EmptyListError("List is empty!\n");
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

std::list<Node*> List::getAll(const int value) {
    std::list<Node*> result_list;
    if (!first) {
        throw EmptyListError("List is empty!\n");
    }
    Node* current = first.get();
    do {
        if (current->value == value) {
            cout << "Found value " << current->value << endl;
            result_list.push_front(current);
        } else {
            cout << "Going through " << current->value << endl;
        }
        current = current->next.get();
    } while (current);
    if (result_list.empty()) {
        cout << "Not found: value " << value << endl;
    }

    return result_list;
}

int main(int argc, [[maybe_unused]] const char* argv[]) try {
    List lista;
    unique_ptr<Node> node4 = make_unique<Node>(4);
    unique_ptr<Node> node7 = make_unique<Node>(7);

    lista.add(move(node4));
    lista.add(make_unique<Node>(2));
    lista.add(make_unique<Node>(5));
    lista.add(move(node7));
    lista.add(make_unique<Node>(9));
    lista.add(make_unique<Node>(7));
    lista.add(make_unique<Node>(5));
    lista.add(make_unique<Node>(12));
    lista.add(make_unique<Node>(1));
    auto node = lista.getFirst(7);
    lista.add(make_unique<Node>(5));

    if (node)
        cout << node->value << '\n';

    auto foundNodes = lista.getAll(5);

    for (const auto& node : foundNodes) {
        cout << node->value << '\n';
    }
    if (argc > 1) {
        List emptyList{};
        emptyList.getFirst(12);
    }

    return 0;
} catch (const exception& ex) {
    cout << "Exception! " << ex.what();
    return 1;
}
