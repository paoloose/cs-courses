#include <stdlib.h>
#include <stdio.h>

typedef struct node node;
typedef struct linked_list linked_list;
// "Node" and "linked list" data structures
struct node {
    int value;
    node* next;
};
struct linked_list {
    int size;
    node* head;
    node* tail;
};

linked_list new_list();
node* new_node(int value);
void list_push(linked_list* list, int element);
void list_push_front(linked_list* list, int element);
int list_push_at(linked_list* list, int value, int at);
node* list_at(const linked_list* list, int i);
void list_print(const linked_list* list);
void list_free(linked_list* list);
int list_remove_at(linked_list* list, int at);

int main() {
    linked_list list = new_list();
    list_push_front(&list, 69);

    for (int i = 0; i < 10; i++) {
        list_push(&list, i);
    }
    list_print(&list);

    return 0;
}

linked_list new_list() {
    linked_list list;
    list.size = 0;
    list.head = NULL;
    list.tail = NULL;
    return list;
}

node* new_node(int value) {
    node* to_return = malloc(sizeof(node));
    if (!to_return) {
        exit(1);
    }
    to_return->next = NULL;
    to_return->value = value;
    return to_return;
}

void list_push(linked_list* list, int element) {
    if (list->size == 0) {
        list->head = new_node(element);
        list->tail = list->head;
        list->size++;
        return;
    }
    node* new_tail = new_node(element);
    list->tail->next = new_tail;
    list->tail = new_tail;
    list->size++;
}

void list_push_front(linked_list* list, int element) {
    if (list->size == 0) {
        list->head = new_node(element);
        list->tail = list->head;
        list->size++;
        return;
    }
    node* new_head = new_node(element);
    new_head->next = list->head;
    list->head = new_head;
    list->size++;
}

// returns 0 if push was ok
int list_push_at(linked_list* list, int value, int at) {
    if (list->size <= at) {
        return -1;
    }
    if (at == 0) {
        node* to_insert = new_node(value);
        to_insert->next = list->head;
        list->head = to_insert;
        return 0;
    }
    node* curr = list->head;
    int count = 0;
    do {
        if (count == at - 1) {
            node* to_insert = new_node(value);
            to_insert->next = curr->next;
            curr->next = to_insert;
            list->size++;
            return 0;
        }
        count++;
    } while ((curr = curr->next) != NULL);
    return -1;
}

// Returns a pointer to the node at the i position
// If doesn't exist, returns NULL
node* list_at(const linked_list* list, int i) {
    if (list->size <= i) {
        return NULL;
    }
    int count = 0;
    for (node* curr = list->head; curr != NULL; curr = curr->next) {
        if (count == i) {
            return curr;
        }
        count++;
    }
    return NULL;
}

// Prints the linked list
void list_print(const linked_list* list) {
    printf("[ ");
    for (node* curr = list->head; curr != NULL; curr = curr->next) {
        printf("%d", curr->value);
        if (curr != list->tail) {
            printf(" ➜ ");
        }
    }
    printf(" ]\n");
}

void list_free(linked_list* list) {
    node* next_node = NULL;
    for (node* curr = list->head; curr != NULL; curr = next_node) {
        next_node = curr->next;
        free(curr);
    }
    list->head = 0;
    list->tail = 0;
    list->size = 0;
}

// returns 0 if good, otherwise < 0
int list_remove_at(linked_list* list, int at) {
    if (list->size <= at) {
        return -1;
    }
    // if wants to remove head
    if (at == 0) {
        // this is a special case, since the head have no "previous node"
        node* tmp = list->head->next;
        free(list->head);
        list->head = tmp;
        list->size--;
        return 0;
    }
    // the only node that has information about the node that we want to remove
    // is the node before it
    node* curr = list->head;
    int count = 0;
    do {
        // the previous node
        if (count == at - 1) {
            node* to_remove = curr->next;
            node* tmp = to_remove->next;
            free(to_remove);
            curr->next = tmp;
            list->size--;
            return 0;
        }
        count++;
    } while ((curr = curr->next) != NULL);
    return -1;
}
