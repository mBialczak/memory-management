#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct Resource {
    Resource(unique_ptr<char> byte)
        : byte_(move(byte)) {}
    char byte() const { return *byte_; }
    virtual string name() const = 0;

protected:
    unique_ptr<char> byte_;
};

struct ResourceA : Resource {
    ResourceA(unique_ptr<char> byte)
        : Resource(move(byte)) {}
    // tries to append *char to string??
    string name() const override {
        return string("ResourceA ").append(byte_.get(), byte_.get() + 1);
    }
};

struct ResourceB : Resource {
    ResourceB(unique_ptr<char> byte)
        : Resource(move(byte)) {}
    string name() const override {
        return string("ResourceB ").append(byte_.get(), byte_.get() + 1);
    }
};

struct ResourceFactory {
    std::shared_ptr<Resource>
    makeResourceA(unique_ptr<char> byte) {
        return std::make_shared<ResourceA>(move(byte));
    }
    std::shared_ptr<Resource> makeResourceB(unique_ptr<char> byte) {
        return std::make_shared<ResourceB>(move(byte));
    }
};

struct ResourceCollection {
    void add(std::shared_ptr<Resource> r) { resources.push_back(r); }
    void clear() { resources.clear(); }  // clearing what about deleting?
    std::shared_ptr<Resource> operator[](int index) { return resources[index]; }
    void printAll() {
        for (const auto& res : resources) {
            cout << res->name() << endl;
        }
    }

private:
    vector<std::shared_ptr<Resource>> resources;
};

int main() {
    ResourceCollection collection;
    ResourceFactory rf;
    collection.add(rf.makeResourceA(make_unique<char>(0x01)));
    collection.add(rf.makeResourceB(make_unique<char>(0x02)));
    collection.printAll();

    auto firstByte = collection[0]->byte();
    collection.clear();
    cout << firstByte << endl;

    return 0;
}
