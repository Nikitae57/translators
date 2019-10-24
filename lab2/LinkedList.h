//
// Created by nikita on 17.10.2019.
//

#ifndef LAB2_LINKEDLIST_H
#define LAB2_LINKEDLIST_H

template <class T>
class LinkedList {
    struct ITEM {
        T* data;
        ITEM* next = nullptr;
    } item;

    ITEM* root = nullptr;
    long length = 0;

public:
    unsigned long size() {
        return length;
    }

    void addItem(T* newItem) {
        if (root == nullptr) {
            root = new ITEM();
            root->data = newItem;
            length++;

            return;
        }

        ITEM* currentItem = root;
        while (currentItem->next != nullptr) {
            currentItem = currentItem->next;
        }

        currentItem->next = new ITEM();
        currentItem = currentItem->next;

        currentItem->data = newItem;
        length++;
    }

    T* getItem(unsigned long index) {
        if (index + 1 > length || index < 0) {
            return nullptr;
        }

        ITEM* currentItem = root;
        for (int i = 0; i < index; i++) {
            currentItem = currentItem->next;
        }

        return currentItem->data;
    }

    void deleteItem(int index) {
        if (index + 1 > length || index < 0) {
            return;
        }

        ITEM* currentItem = root;
        for (int i = 0; i < index - 1; i++) {
            currentItem = currentItem->next;
        }

        ITEM* temp = currentItem->next;

        currentItem->next = currentItem->next->next;

        delete temp->data;
        delete temp;

        length--;
    }
};


#endif //LAB2_LINKEDLIST_H
