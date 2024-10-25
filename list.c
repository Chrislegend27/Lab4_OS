// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
  list_t* mylist =  (list_t *) malloc(sizeof(list_t)); 
  mylist->head = NULL;
  return mylist;
}

void list_free(list_t *l) { //Free entire list
    node_t *current = l->head;
    node_t *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(l);
}

void list_print(list_t *l) {
  node_t *current = l->head;

  if (current == NULL) {
    printf("List is empty.\n");
    return;
  }
  while (current != NULL) {  // Traverse the list and print each value
    printf("%d -> ", current->value);
    current = current->next;
  }
  printf("NULL\n");  // Indicate the end of the list
}

char * listToString(list_t *l) {
  char* buf = (char *) malloc(sizeof(char) * 1024);
  char tbuf[20];

	node_t* curr = l->head;
  while (curr != NULL) {
    sprintf(tbuf, "%d->", curr->value);
    curr = curr->next;
    strcat(buf, tbuf);
  }
  strcat(buf, "NULL");
  return buf;
}

void list_add_to_front(list_t *l, elem value) {
    // Allocate memory for the new node and set its value
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        // Handle memory allocation failure
        return;
    }
    new_node->value = value;
    new_node->next = l->head;  // The new node points to the current head
    l->head = new_node;        // Update the head to be the new node
}


int list_length(list_t *l) {
    int length = 0;
    node_t *current = l->head;

    while (current != NULL) {
        length++;
        current = current->next;
    }
    return length;  // Ensure the length is returned
}


void list_add_to_back(list_t *l, elem value) {
    // Create a new node
    node_t* new_node = (node_t *) malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = NULL;

    // If the list is empty, set the new node as the head
    if (l->head == NULL) {
        l->head = new_node;
    } else {
        // Traverse to the end of the list
        node_t* curr_node = l->head;
        while (curr_node->next != NULL) {
            curr_node = curr_node->next;
        }
        // Add the new node at the end
        curr_node->next = new_node;
    }
}

void list_add_at_index(list_t *l, elem value, int index) {
    // Handle negative indices
    if (index < 0) {
        // Index is invalid
        return;
    }

    // Allocate memory for the new node and set its value
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        // Handle memory allocation failure
        return;
    }
    new_node->value = value;
    new_node->next = NULL;

    // If inserting at the front of the list
    if (index == 0) {
        new_node->next = l->head;
        l->head = new_node;
        return;
    }

    // Traverse to the node before the desired position
    node_t *current = l->head;
    int i;
    for (i = 0; i < index - 1; i++) {
        if (current == NULL) {
            // Index is out of bounds
            free(new_node);  // Free allocated memory before returning
            return;
        }
        current = current->next;
    }

    // If current is NULL after traversal, index is out of bounds
    if (current == NULL) {
        free(new_node);
        return;
    }

    // Insert the new node into the list
    new_node->next = current->next;
    current->next = new_node;
}



elem list_remove_from_back(list_t *l) {
    if (l->head == NULL) {  // If the list is empty
        return -1;  // Return an error value
    }

    if (l->head->next == NULL) {  // If the list has only one element
        elem value = l->head->value;
        free(l->head);
        l->head = NULL;  // The list is now empty
        return value;
    }

    // Traverse to the second-to-last element
    node_t *current = l->head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    // Remove the last node
    elem value = current->next->value;
    free(current->next);
    current->next = NULL;  // Update the second-to-last node to be the last node
    return value;  // Return the value of the removed node
}


//2 
elem list_remove_from_front(list_t *l) {
    if (l->head == NULL) {  // Check if the list is empty
        return -1;  // Return an error value
    }

    node_t *to_free = l->head;  // Store the current head in a temporary pointer
    elem value = to_free->value;  // Get the value of the current head
    l->head = l->head->next;  // Update the head to point to the next node
    free(to_free);  // Free the memory of the old head node

    return value;  // Return the value of the removed node
}


//3
elem list_remove_at_index(list_t *l, int index) {
    if (index < 0 || l->head == NULL) {  // Check for invalid index or empty list
        return -1;  // Return an error value
    }

    if (index == 0) {  // Special case: remove the first element
        return list_remove_from_front(l);  // Use the existing function to handle this
    }

    node_t *current = l->head;
    int i;
    for (i = 0; i < index - 1; i++) {  // Traverse to the node before the target index
        if (current->next == NULL) {  // If index is out of bounds
            return -1;  // Return an error value
        }
        current = current->next;
    }

    node_t *to_free = current->next;
    if (to_free == NULL) {  // If index is out of bounds (e.g., list is shorter than expected)
        return -1;  // Return an error value
    }

    elem value = to_free->value;
    current->next = to_free->next;  // Bypass the node to be removed
    free(to_free);  // Free the removed node
    return value;  // Return the value of the removed node
}


//1
bool list_is_in(list_t *l, elem value) { return false; }

//2
elem list_get_elem_at(list_t *l, int index) {
    if (index < 0) {  // Check for invalid negative index
        return -1;  // Return an error value
    }

    node_t *current = l->head;
    int current_index = 0;

    while (current != NULL) {
        if (current_index == index) {  // If we've reached the desired index
            return current->value;  // Return the value at that index
        }
        current = current->next;  // Move to the next node
        current_index++;  // Increment the current index
    }

    return -1;  // If the index is out of bounds, return an error value
}


//3
int list_get_index_of(list_t *l, elem value) {
    node_t *current = l->head;  // Start from the head of the list
    int index = 0;  // Initialize the index counter

    while (current != NULL) {  // Traverse the list
        if (current->value == value) {  // If the current node's value matches the target value
            return index;  // Return the current index
        }
        current = current->next;  // Move to the next node
        index++;  // Increment the index counter
    }

    return -1;  // If the value was not found, return -1
}

