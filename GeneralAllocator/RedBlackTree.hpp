
#include <iostream>

enum class Color { RED, BLACK };

template <typename T, typename V>
class Node {
public:
    //the key identify the node
    T key;
    //the val is the value used to create the tree
    V val;
    Color color;
    Node* parent;
    Node* left;
    Node* right;

    Node(T key, V value) : key(key), val(value), color(Color::RED), parent(nullptr), left(nullptr), right(nullptr) {}
    ~Node()
    {
        if (left)
            delete left;
        if (right)
            delete right;
    }
};

template <typename T, typename V>
class RedBlackTree {
public:
    RedBlackTree();
    ~RedBlackTree();
    /*
        @brief
        Insert the node on the tree and balance the tree keeping the balance
        @param key the key of the node
        @param val the value of the node
    */
    void Insert(T key, V val);
    /*
        @brief
        Remove the node on the tree and balance the tree keeping the balance
        @param key the key of the node
        @param val the value of the node
    */
    void Remove(T key, V val);
    /*
        @brief
        Search the node on the tree based on the value
        @param val the value of the node
    */
    Node<T, V>* Search(V val);
    /*
        @brief
        Clear the tree
    */
    void Clear();
private:
    int size;
    Node<T, V>* root;
    /*
        @brief
        Rotate the tree on the left based on a node as center
        @param node the center node
    */
    void LeftRotate(Node<T, V>* node);
    /*
        @brief
        Rotate the tree on the right based on a node as center
        @param node the center node
    */
    void RightRotate(Node<T, V>* node);
    void RemoveNode(Node<T, V>* node);
};

template <typename T, typename V>
RedBlackTree<T, V>::RedBlackTree() :root(nullptr), size(0) {}

template <typename T, typename V>
RedBlackTree<T, V>::~RedBlackTree() {
    if (root) {
        delete root;
    }
}

template<typename T, typename V>
void RedBlackTree<T, V>::Insert(T key, V val)
{

    Node<T, V>* node = new Node<T, V>(key, val);

    if (root == nullptr) {
        root = node;
        node->color = Color::BLACK;
        return;
    }

    {
        Node<T, V>* curr = root;
        while (curr->left != nullptr || curr->right != nullptr)
        {
            if (val < curr->val)
                curr = curr->left;
            else
                curr = curr->right;
        }

        node->parent = curr;
        if (val < curr->val)
            curr->left = node;
        else
            curr->right = node;

        while (curr->color == Color::RED && curr->parent != nullptr)
        {
            Node<T, V>* bro;
            if (curr == curr->parent->right)
                bro = curr->parent->left;
            else
                bro = curr->parent->right;

            if (bro == nullptr) {
                curr->color = Color::BLACK;
                curr->parent->color = Color::RED;
                if (bro == curr->parent->right) {
                    RightRotate(curr->parent);
                }
                else {
                    LeftRotate(curr->parent);
                }
                break;
            }
            else if (bro->color == Color::RED) {
                curr->color = Color::BLACK;
                bro->color = Color::BLACK;
                curr->parent->color = Color::RED;
                curr = curr->parent;
            }
            else {
                curr->color = Color::BLACK;
                curr->parent->color = Color::RED;

                if (curr == curr->parent->right) {
                    if (node == curr->left) {
                        RightRotate(curr);
                        curr = node;
                    }
                    LeftRotate(curr->parent);
                }
                else {
                    if (node == curr->right) {
                        LeftRotate(curr);
                        curr = node;
                    }
                    RightRotate(curr->parent);
                }
            }
            root->color = Color::BLACK;
        }

        ++size;
    }


}

template<typename T, typename V>
void RedBlackTree<T, V>::Remove(T key, V val)
{
    Node<T, V>* curr = root;
    while (curr->left != nullptr || curr->right != nullptr)
    {
        if (curr->key == key)
            break;

        if (val >= curr->val)
            curr = curr->right;
        else
            curr = curr->left;
    }
    if (curr->key != key)
        return;

    RemoveNode(curr);
    --size;
}

template<typename T, typename V>
void RedBlackTree<T, V>::RemoveNode(Node<T, V>* node)
{
    if (node->color == Color::RED) {
        if (node->left != nullptr && node->right != nullptr) {
            Node<T, V>* successor = node->right;
            while (successor->left != nullptr)
                successor = successor->left;
            node->key = successor->key;
            node->val = successor->val;
            RemoveNode(successor);
        }
        else if (node->left != nullptr) {
            node->key = node->left->key;
            node->val = node->left->val;
            node->color = node->left->color;
            RemoveNode(node->left);
        }
        else if (node->right != nullptr) {
            node->key = node->right->key;
            node->val = node->right->val;
            node->color = node->right->color;
            RemoveNode(node->right);
        }
        else {
            // no child
            if (node->parent == nullptr) {
                delete node;
                root = nullptr;
                return;
            }

            if (node->parent->left == node)
                node->parent->left = nullptr;
            else
                node->parent->right = nullptr;

            delete node;
        }
    }
    else {
        if (node->left != nullptr && node->right != nullptr) {
            Node<T, V>* successor = node->right;
            while (successor->left != nullptr)
                successor = successor->left;
            node->key = successor->key;
            node->val = successor->val;
            RemoveNode(successor);
        }
        else if (node->left != nullptr) {
            node->key = node->left->key;
            node->val = node->left->val;
            RemoveNode(node->left);
        }
        else if (node->right != nullptr) {
            node->key = node->right->key;
            node->val = node->right->val;
            RemoveNode(node->right);
        }
        else {
            if (node->parent == nullptr) {
                delete node;
                root = nullptr;
                return;
            }

            if (node->parent->left == node) {
                node->parent->left = nullptr;
                if (node->parent->right != nullptr
                    && node->parent->right->color == Color::RED) {
                    node->parent->right->color = Color::BLACK;
                    LeftRotate(node->parent);
                }
            }
            else {
                node->parent->right = nullptr;
                if (node->parent->left != nullptr
                    && node->parent->left->color == Color::RED) {
                    node->parent->left->color = Color::BLACK;
                    LeftRotate(node->parent);
                }
            }

            if (node->parent->left == nullptr
                && node->parent->right == nullptr
                && node->parent->parent != nullptr) {
                LeftRotate(node->parent->parent);
            }

            delete node;
        }
    }
}

template<typename T, typename V>
Node<T, V>* RedBlackTree<T, V>::Search(V val)
{
    Node<T, V>* curr = root;
    while (curr->left != nullptr || curr->right != nullptr)
    {
        if (curr->val >= val) {
            break;
        }

        if (val < curr->val)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if (curr->val < val)
        return nullptr;
    return curr;
}

template<typename T, typename V>
void RedBlackTree<T, V>::LeftRotate(Node<T, V>* node)
{
    if (node->right != nullptr) return;
    Node<T, V>* temp = node->right;

    // update the two nodes
    node->right = temp->left;
    if (temp->left != nullptr)
        temp->left->parent = node;
    temp->left = node;
    temp->parent = node->parent;
    node->parent = temp;

    // update the parent
    if (root == node) {
        root = temp;
        return;
    }
    if (temp->parent->left == node)
        temp->parent->left = temp;
    else
        temp->parent->right = temp;
}
template<typename T, typename V>
void RedBlackTree<T, V>::RightRotate(Node<T, V>* node)
{
    if (node->left != nullptr) return;
    Node<T, V>* temp = node->left;

    node->left = temp->right;
    if (temp->right != nullptr)
        temp->right->parent = node;
    temp->right = node;
    temp->parent = node->parent;
    node->parent = temp;

    if (root == node) {
        root = temp;
        return;
    }
    if (temp->parent->left == node)
        temp->parent->left = temp;
    else
        temp->parent->right = temp;
}

template<typename T, typename V>
void RedBlackTree<T, V>::Clear()
{
    delete root;
    root = nullptr;
}