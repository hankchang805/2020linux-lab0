#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *ptr = q->head;
        q->head = q->head->next;
        free(ptr->value);
        free(ptr);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    char *in = malloc(sizeof(char) * (strlen(s) + 1));
    if (!in) {
        free(newh);
        return false;
    }
    int len = strlen(s);
    strncpy(in, s, len);
    in[len] = '\0';
    newh->value = in;
    newh->next = q->head;
    q->head = newh;
    if (!q->tail) {
        q->tail = q->head;
    }
    q->size = q->size + 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newt;
    if (!q)
        return false;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    char *in = malloc((strlen(s) + 1) * sizeof(char));
    if (!in) {
        free(newt);
        return false;
    }
    int len = strlen(s);
    strncpy(in, s, len);
    in[len] = '\0';
    newt->value = in;
    newt->next = NULL;
    if (!q->tail) {
        q->head = q->tail = newt;
        q->size = q->size + 1;
        return true;
    }
    q->tail->next = newt;
    q->tail = newt;
    q->size = q->size + 1;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    if (sp) {
        strncpy(sp, q->head->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_ele_t *ptr = q->head;
    q->head = q->head->next;
    q->size = q->size - 1;
    free(ptr->value);
    free(ptr);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q)
        return q->size;
    return 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *temp = q->head;
    list_ele_t *middle = NULL;
    list_ele_t *last = NULL;
    while (temp) {
        last = middle;
        middle = temp;
        temp = temp->next;
        middle->next = last;
    }
    temp = q->head;
    q->head = q->tail;
    q->tail = temp;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
bool less_than(list_ele_t *l, list_ele_t *r)
{
    char *str_l = l->value;
    char *str_r = r->value;
    while (*str_l && *str_r) {
        if (*str_l > *str_r)
            return false;
        else if (*str_l++ < *str_r++)
            return true;
    }
    if (!*str_l && *str_r)
        return true;
    return false;
}
void q_merge(queue_t *left, queue_t *right, queue_t *q)
{
    list_ele_t *l = left->head;
    list_ele_t *r = right->head;
    if (less_than(l, r)) {
        q->head = l;
        l = l->next;
    } else {
        q->head = r;
        r = r->next;
    }
    q->tail = q->head;
    list_ele_t *cur = NULL;
    while (l && r) {
        if (less_than(l, r)) {
            cur = l;
            l = l->next;
        } else {
            cur = r;
            r = r->next;
        }
        q->tail->next = cur;
        q->tail = cur;
    }
    while (l) {
        q->tail->next = l;
        q->tail = l;
        l = l->next;
    }
    while (r) {
        q->tail->next = r;
        q->tail = r;
        r = r->next;
    }
}
void q_sort(queue_t *q)
{
    if (!q || q->size < 2)
        return;
    queue_t left;
    queue_t right;
    left.head = q->head;
    right.tail = q->tail;
    left.size = (q->size >> 1) + (q->size & 1);
    right.size = (q->size >> 1);
    list_ele_t *ptr = q->head;
    for (int i = 0; i < left.size - 1; i++) {
        ptr = ptr->next;
    }
    left.tail = ptr;
    right.head = ptr->next;
    left.tail->next = NULL;
    q->tail = q->head = NULL;
    q_sort(&left);
    q_sort(&right);
    q_merge(&left, &right, q);
}
