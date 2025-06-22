#include <bits/stdc++.h>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>

using namespace std;

struct Node {
  Node() : val({-1, -1}), next(nullptr), prev(nullptr) {}
  Node(const std::pair<int, int>& val)
      : val(val), next(nullptr), prev(nullptr) {}

  std::pair<int, int> val;
  Node*               next;
  Node*               prev;
};

class LinkedList {
public:
  LinkedList() {
    header_  = new Node();
    trailer_ = new Node();

    header_->next  = trailer_;
    trailer_->prev = header_;
  }

  void insert_to_front(Node* node) {
    // moving to the front
    node->next       = header_->next;
    node->prev       = header_;
    node->next->prev = node;
    header_->next    = node;
  }

  void move_to_front(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    insert_to_front(node);
  }

  void delete_node(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
  }

  Node* header() { return header_; }

  Node* traier() { return trailer_; }

private:
  Node* header_;
  Node* trailer_;
};

class LRUCache {

public:
  LRUCache(int cap) : capacity(cap) { cache = make_unique<LinkedList>(); }

  void put(int key, int value) {
    if (key_node_address.find(key) != key_node_address.end()) {
      Node* node       = key_node_address[key];
      node->val.second = value;
      cache->move_to_front(node);
    } else {
      if ((int)key_node_address.size() == capacity) {
        // clear the map and remove the last Node from the cache
        Node* node_to_delete = cache->traier()->prev;
        key_node_address.erase(
            key_node_address.find(node_to_delete->val.first));
        cache->delete_node(node_to_delete);
      }

      Node* node            = new Node({key, value});
      key_node_address[key] = node;
      cache->insert_to_front(node);
    }
  }

  int get(int key) {
    if (key_node_address.find(key) == key_node_address.end()) {
      return -1;
    }

    Node* node = key_node_address[key];
    cache->move_to_front(node);

    return node->val.second;
  }

private:
  std::map<int, Node*>        key_node_address;
  std::unique_ptr<LinkedList> cache;
  int                         capacity;
};

int main() {
  LRUCache cache(2);
  cache.put(1, 1);
  cache.put(2, 2);
  std::cout << cache.get(1) << std::endl;
  cache.put(3, 3);
  std::cout << cache.get(2) << std::endl;
  cache.put(4, 4);
  std::cout << cache.get(1) << std::endl;
  std::cout << cache.get(3) << std::endl;
  std::cout << cache.get(4) << std::endl;
  return 0;
}