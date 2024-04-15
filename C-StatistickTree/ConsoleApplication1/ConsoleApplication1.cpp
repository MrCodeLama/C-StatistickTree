#include <iostream>
using namespace std;

// Структура для представлення раціонального числа
struct Rational {
    int numerator; // Лічильник
    int denominator; // Знаменник
};

// Функція для обчислення найбільшого спільного дільника (НСД)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Функція для спрощення раціонального числа
void simplify(Rational& num) {
    int common_divisor = gcd(num.numerator, num.denominator);
    num.numerator /= common_divisor;
    num.denominator /= common_divisor;
}

// Структура для вузла червоно-чорного дерева
struct Node {
    Rational key;
    Node* parent;
    Node* left;
    Node* right;
    char color;
};

// Клас для дерева порядкової статистики
class RedBlackTree {
private:
    Node* root;

    // Приватні методи для роботи з деревом
    void rotateLeft(Node* x);
    void rotateRight(Node* x);
    void fixInsertion(Node* z);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* x);
    void deleteNode(Node* z);
    void fixDeletion(Node* x);

public:
    // Конструктор
    RedBlackTree() {
        root = nullptr;
    }

    // Метод для вставки нового вузла в дерево
    void insert(Rational key);

    // Метод для видалення вузла з дерева
    void remove(Rational key);

    // Метод для знаходження k-го порядкової статистики в дереві
    Rational findKth(int k);
};

// Метод для вставки нового вузла в дерево
void RedBlackTree::insert(Rational key) {
    Node* z = new Node;
    z->key = key;
    z->left = nullptr;
    z->right = nullptr;
    z->color = 'R';

    Node* y = nullptr;
    Node* x = root;

    while (x != nullptr) {
        y = x;
        if (z->key.numerator < x->key.numerator || (z->key.numerator == x->key.numerator && z->key.denominator < x->key.denominator))
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == nullptr)
        root = z;
    else if (z->key.numerator < y->key.numerator || (z->key.numerator == y->key.numerator && z->key.denominator < y->key.denominator))
        y->left = z;
    else
        y->right = z;

    fixInsertion(z);
}

// Приватний метод для виправлення властивостей червоно-чорного дерева після вставки вузла
void RedBlackTree::fixInsertion(Node* z) {
    while (z->parent != nullptr && z->parent->color == 'R') {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y != nullptr && y->color == 'R') {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                rotateRight(z->parent->parent);
            }
        }
        else {
            Node* y = z->parent->parent->left;
            if (y != nullptr && y->color == 'R') {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->color = 'B';
}

// Приватний метод для лівого обертання
void RedBlackTree::rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Приватний метод для правого обертання
void RedBlackTree::rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

// Метод для видалення вузла з дерева
void RedBlackTree::remove(Rational key) {
    Node* z = root;
    while (z != nullptr) {
        if (key.numerator < z->key.numerator || (key.numerator == z->key.numerator && key.denominator < z->key.denominator))
            z = z->left;
        else if (key.numerator > z->key.numerator || (key.numerator == z->key.numerator && key.denominator > z->key.denominator))
            z = z->right;
        else {
            deleteNode(z);
            return;
        }
    }
    cout << "Node with key (" << key.numerator << "/" << key.denominator << ") not found in the tree." << endl;
}

// Приватний метод для видалення вузла з дерева
void RedBlackTree::deleteNode(Node* z) {
    Node* y = z;
    Node* x;
    char y_original_color = y->color;
    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == 'B')
        fixDeletion(x);
    delete z;
}

// Приватний метод для виправлення властивостей червоно-чорного дерева після видалення вузла
void RedBlackTree::fixDeletion(Node* x) {
    while (x != root && (x == nullptr || x->color == 'B')) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == 'R') {
                w->color = 'B';
                x->parent->color = 'R';
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if ((w->left == nullptr || w->left->color == 'B') && (w->right == nullptr || w->right->color == 'B')) {
                w->color = 'R';
                x = x->parent;
            }
            else {
                if (w->right == nullptr || w->right->color == 'B') {
                    if (w->left != nullptr)
                        w->left->color = 'B';
                    w->color = 'R';
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = 'B';
                if (w->right != nullptr)
                    w->right->color = 'B';
                rotateLeft(x->parent);
                x = root;
            }
        }
        else {
            Node* w = x->parent->left;
            if (w->color == 'R') {
                w->color = 'B';
                x->parent->color = 'R';
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if ((w->right == nullptr || w->right->color == 'B') && (w->left == nullptr || w->left->color == 'B')) {
                w->color = 'R';
                x = x->parent;
            }
            else {
                if (w->left == nullptr || w->left->color == 'B') {
                    if (w->right != nullptr)
                        w->right->color = 'B';
                    w->color = 'R';
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = 'B';
                if (w->left != nullptr)
                    w->left->color = 'B';
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    if (x != nullptr)
        x->color = 'B';
}

// Приватний метод для заміни вузлів в дереві
void RedBlackTree::transplant(Node* u, Node* v) {
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v != nullptr)
        v->parent = u->parent;
}

// Приватний метод для пошуку мінімального вузла в дереві
Node* RedBlackTree::minimum(Node* x) {
    while (x->left != nullptr)
        x = x->left;
    return x;
}

// Метод для знаходження k-го порядкової статистики в дереві
Rational RedBlackTree::findKth(int k) {
    Node* x = root;
    while (x != nullptr) {
        if (k == 1)
            return x->key;
        int left_size = (x->left != nullptr) ? 1 : 0;
        if (k == left_size + 1)
            return x->key;
        else if (k <= left_size)
            x = x->left;
        else {
            k -= (left_size + 1);
            x = x->right;
        }
    }
    Rational not_found = { 0, 1 }; // Повертаємо 0/1 як помилку, якщо k виходить за межі дерева
    return not_found;
}

// Тестовий приклад
int main() {
    RedBlackTree tree;
    Rational numbers[] = { {3, 2}, {1, 2}, {5, 2}, {2, 1}, {7, 3}, {4, 1} };

    // Вставка чисел у дерево
    for (Rational num : numbers) {
        simplify(num); // Спрощення раціональних чисел перед вставкою
        tree.insert(num);
    }

    // Пошук 3-го елемента у дереві (3/2)
    Rational kth_element = tree.findKth(3);
    cout << "3rd order statistic: " << kth_element.numerator << "/" << kth_element.denominator << endl;

    // Видалення числа (3/2) з дерева
    tree.remove({ 3, 2 });

    // Пошук 3-го елемента у дереві після видалення
    kth_element = tree.findKth(3);
    cout << "3rd order statistic after removal: " << kth_element.numerator << "/" << kth_element.denominator << endl;

    return 0;
}
