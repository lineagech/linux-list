#include <assert.h>
#include <stdlib.h>
#include "list.h"
#include "common.h"

static uint16_t values[256];

void list_split(struct list_head *head,
                struct list_head *front,
                struct list_head *back)
{
    struct list_head *slow, *fast;

    if (head == NULL || list_empty(head)) {
        return;
    }

    for (slow = head->next, fast = head->next;
         fast != head && fast->next != head;) {
        slow = slow->next;
        fast = fast->next->next;
    }

    list_cut_position(front, head, slow->prev);
    list_cut_position(back, head, (fast == head ? head->prev : fast));
}

void list_merge(struct list_head *head,
                struct list_head *front,
                struct list_head *back)
{
    struct list_head *front_curr = front->next;
    struct list_head *back_curr = back->next;
    struct list_head *next;
    struct listitem *front_item = NULL;
    struct listitem *back_item = NULL;

    while (front_curr != front && back_curr != back) {
        front_item = list_entry(front_curr, struct listitem, list);
        back_item = list_entry(back_curr, struct listitem, list);
        if (front_item->i <= back_item->i) {
            next = front_curr->next;
            // delete from origin
            list_del(front_curr);
            // add to new
            list_add_tail(front_curr, head);
            front_curr = next;
        } else {
            next = back_curr->next;
            list_del(back_curr);
            list_add_tail(back_curr, head);
            back_curr = next;
        }
    }
    if (!list_empty(front)) {
        list_splice_tail_init(front, head);
    } else {
        list_splice_tail_init(back, head);
    }

    assert(list_empty(front));
    assert(list_empty(back));
}

void list_mergesort(struct list_head *head)
{
    struct list_head left;
    struct list_head right;

    /* Handle corner cases */
    if (list_is_singular(head))
        return;

    /* Split two parts */
    list_split(head, &left, &right);

    /* Sort each part */
    list_mergesort(&left);
    list_mergesort(&right);

    /* Merge to the final result */
    list_merge(head, &left, &right);
}

int main(void)
{
    struct list_head head;
    struct listitem *item = NULL;
    struct listitem *is = NULL;
    size_t i;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&head);

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&(item->list), &head);
    }

    /* Get correct sorted array */
    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);

    /* Merge sort */
    list_mergesort(&head);

    /* Verify the result */
    i = 0;
    list_for_each_entry_safe (item, is, &head, list) {
        assert(item->i == values[i++]);
        list_del(&item->list);
        free(item);
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&head));

    return 0;
}