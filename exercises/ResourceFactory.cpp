#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct Resource {
    Resource(char byte)
        : byte_(byte) {}
    char byte() const { return byte_; }
    virtual string name() const = 0;

protected:
    char byte_;
};

struct ResourceA : Resource {
    ResourceA(char byte)
        : Resource(byte) {}
    string name() const override { return string("ResourceA ") + byte_; }
};

struct ResourceB : Resource {
    ResourceB(char byte)
        : Resource(byte) {}
    string name() const override { return string("ResourceB ") + byte_; }
};

struct ResourceFactory {
    shared_ptr<Resource> makeResourceA(char byte) { return make_shared<ResourceA>(byte); }
    shared_ptr<Resource> makeResourceB(char byte) { return make_shared<ResourceB>(byte); }
};

struct ResourceCollection {
    void add(shared_ptr<Resource> r) { resources.push_back(r); }
    void clear() { resources.clear(); }
    shared_ptr<Resource> operator[](int index) { return resources[index]; }
    void printAll() {
        for (const auto& res : resources) {
            cout << res->name() << endl;
        }
    }

private:
    vector<shared_ptr<Resource>> resources;
};

int main() {
    ResourceCollection collection;
    ResourceFactory rf;
    collection.add(rf.makeResourceA(char{0x01}));
    collection.add(rf.makeResourceB(char{0x02}));
    collection.printAll();

    auto firstByte = collection[0]->byte();
    collection.clear();
    cout << firstByte << endl;

    return 0;
}
