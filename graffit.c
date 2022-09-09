
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
    char name[MAX_STR_LEN];
    struct friend_node_struct *friends;
    struct brand_node_struct *brands;
    bool visited;
    int degree;
} User;

typedef struct friend_node_struct
{
    User *user;
    struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
    char brand_name[MAX_STR_LEN];
    struct brand_node_struct *next;
} BrandNode;

FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
    for (FriendNode *cur = head; cur != NULL; cur = cur->next)
    {
        if (strcmp(cur->user->name, node->name) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
    for (BrandNode *cur = head; cur != NULL; cur = cur->next)
    {
        if (strcmp(cur->brand_name, name) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
    if (node == NULL)
        return head;

    if (in_friend_list(head, node))
    {
        printf("User already in list\n");
        return head;
    }

    FriendNode *fn = calloc(1, sizeof(FriendNode));
    fn->user = node;
    fn->next = NULL;

    if (head == NULL)
        return fn;

    if (strcmp(head->user->name, node->name) > 0)
    {
        fn->next = head;
        return fn;
    }

    FriendNode *cur;
    for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
         cur = cur->next)
        ;

    fn->next = cur->next;
    cur->next = fn;

    return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
    if (node == NULL)
        return head;

    if (in_brand_list(head, node))
    {
        printf("Brand already in list\n");
        return head;
    }

    BrandNode *fn = calloc(1, sizeof(BrandNode));
    strcpy(fn->brand_name, node);
    fn->next = NULL;

    if (head == NULL)
        return fn;

    if (strcmp(head->brand_name, node) > 0)
    {
        fn->next = head;
        return fn;
    }

    BrandNode *cur;
    for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
         cur = cur->next)
        ;

    fn->next = cur->next;
    cur->next = fn;

    return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
    if (node == NULL)
        return head;

    if (!in_friend_list(head, node))
    {
        printf("User not in list\n");
        return head;
    }

    if (strcmp(head->user->name, node->name) == 0)
    {
        FriendNode *temp = head->next;
        free(head);
        return temp;
    }

    FriendNode *cur;
    for (cur = head; cur->next->user != node; cur = cur->next)
        ;

    FriendNode *temp = cur->next;
    cur->next = temp->next;
    free(temp);

    return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
    if (node == NULL)
        return head;

    if (!in_brand_list(head, node))
    {
        printf("Brand not in list\n");
        return head;
    }

    if (strcmp(head->brand_name, node) == 0)
    {
        BrandNode *temp = head->next;
        free(head);
        return temp;
    }

    BrandNode *cur;
    for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
        ;

    BrandNode *temp = cur->next;
    cur->next = temp->next;
    free(temp);

    return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
    printf("User name: %s\n", user->name);

    printf("Friends:\n");
    for (FriendNode *f = user->friends; f != NULL; f = f->next)
    {
        printf("   %s\n", f->user->name);
    }

    printf("Brands:\n");
    for (BrandNode *b = user->brands; b != NULL; b = b->next)
    {
        printf("   %s\n", b->brand_name);
    }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
    for (int i = 0; i < MAT_SIZE; i++)
    {
        if (strcmp(brand_names[i], name) == 0)
        {
            return i;
        }
    }

    printf("Brand '%s' not found\n", name);
    return -1; // Not found
}


/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
    int idx = get_brand_index(brand_name);
    if (idx < 0)
    {
        printf("Brand '%s' not in the list.\n", brand_name);
        return;
    }

    printf("Brand name: %s\n", brand_name);
    printf("Brand idx: %d\n", idx);

    printf("Similar brands:\n");
    for (int i = 0; i < MAT_SIZE; i++)
    {
        if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
        {
            printf("   %s\n", brand_names[i]);
        }
    }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
    // Read the file
    char buff[MAX_STR_LEN];
    FILE *f = fopen(file_name, "r");
    fscanf(f, "%s", buff);
    char *line = buff;
    // Load up the brand_names matrix
    for (int i = 0; i < MAT_SIZE; i++)
    {
        if (i == MAT_SIZE - 1)
        {
            strcpy(brand_names[i], line);
            break;
        }
        int index = strchr(line, ',') - line;
        strncpy(brand_names[i], line, index);
        line = strchr(line, ',') + sizeof(char);
    }
    // Load up the brand_adjacency_matrix
    for (int x = 0; x < MAT_SIZE; x++)
    {
        fscanf(f, "%s", buff);
        for (int y = 0; y < MAT_SIZE; y++)
        {
            int value = (int)buff[y * 2];
            if (value == 48)
            {
                value = 0;
            }
            else
            {
                value = 1;
            }
            brand_adjacency_matrix[x][y] = value;
        }
    }
}

/**
 * Creates and returns a user.
 */
User *create_user(char *name)
{
    User *new_user = (User *)calloc(1, sizeof(User));
    strcpy(new_user->name, name);
    new_user->brands = NULL;
    new_user->friends = NULL;
    new_user->visited = false;
    new_user->degree = -1;
    if (in_friend_list(allUsers, new_user))
    {
        free(new_user);
        return NULL;
    }
    allUsers = insert_into_friend_list(allUsers, new_user);
    return new_user;
}

/**
 * Deletes a given user.
 */
int delete_user(User *user)
{
    if(user == NULL)
        return -1;
    if (in_friend_list(allUsers, user))
    {
        allUsers = delete_from_friend_list(allUsers, user);
        FriendNode *node = allUsers;
        while(node != NULL)
        {
            if(in_friend_list(node->user->friends, user))
            {
                node->user->friends = delete_from_friend_list(node->user->friends, user);
            }
            node = node->next;
        }
        if(!in_friend_list(allUsers, user))
        {
            free(user);
            return 0;
        }
    }
    return -1;
}

/**
 * Create a friendship between user and friend.
 */
int add_friend(User *user, User *friend)
{
    if (user != NULL && friend != NULL)
    {
        if (strcmp(user->name, friend->name) != 0)
        {
            if (!in_friend_list(user->friends, friend) && !in_friend_list(friend->friends, user))
            {
                user->friends = insert_into_friend_list(user->friends, friend);
                friend->friends = insert_into_friend_list(friend->friends, user);
                if (in_friend_list(user->friends, friend))
                    return 0;
            }
        }
    }
    return -1;
}

/**
 * Removes a friendship between user and friend.
 */
int remove_friend(User *user, User *friend)
{
    if (user != NULL && friend != NULL)
    {
        if (strcmp(user->name, friend->name) != 0)
        {
            if (in_friend_list(user->friends, friend) && in_friend_list(friend->friends, user))
            {
                user->friends = delete_from_friend_list(user->friends, friend);
                friend->friends = delete_from_friend_list(friend->friends, user);
                if (!in_friend_list(user->friends, friend))
                    return 0;
            }
        }
    }
    return -1;
}
int brand_in_brand_names(char *brand_name)
{
    int i = 0;
    while(i < MAT_SIZE)
    {
        if(strcmp(brand_names[i], brand_name) == 0)
            return 0;
        i++;
    }
    return 1;
}
/**
 * Creates a follow relationship, the user follows the brand.
 */
int follow_brand(User *user, char *brand_name)
{
    if (user != NULL)
    {
        if(brand_name != NULL)
        {
            if (brand_in_brand_names(brand_name) == 0)
            {
                if (!in_brand_list(user->brands, brand_name))
                {
                    user->brands = insert_into_brand_list(user->brands, brand_name);
                    if (in_brand_list(user->brands, brand_name))
                        return 0;
                }
            }
        }
    }
    return -1;
}

/**
 * Removes a follow relationship, the user unfollows the brand.
 */
int unfollow_brand(User *user, char *brand_name)
{
    if (user != NULL)
    {
        if(brand_name != NULL)
        {
            if(brand_in_brand_names(brand_name) == 0)
            {
                if (in_brand_list(user->brands, brand_name))
                {
                    user->brands = delete_from_brand_list(user->brands, brand_name);
                    if (!in_brand_list(user->brands, brand_name))
                        return 0;
                }
            }
        }
    }
    return -1;
}

/**
 * Return the number of mutual friends between two users.
 */
int get_mutual_friends(User *a, User *b)
{
    if(a == NULL || b == NULL)
        return 0;
    if (a->friends == NULL || b->friends == NULL)
        return 0;
    if(a == b)
        return 0;
    FriendNode *a_user = a->friends;
    int count = 0;
    while (a_user != NULL)
    {
        if (in_friend_list(b->friends, a_user->user))
        {
            count++;
        }
        a_user = a_user->next;
    }
    return count;
}

// adds a node to the end of the Queue data structure
FriendNode *enqueue(FriendNode *head, User *a)
{
    if (a == NULL)
        return head;

    FriendNode *fn = calloc(1, sizeof(FriendNode));
    fn->user = a;
    fn->next = NULL;

    if (head == NULL)
    {
        return fn;
    }

    FriendNode *cur = head;
    while(cur != NULL)
    {
        if(cur->next == NULL)
        {
            cur->next = fn;
            fn->next = NULL;
        }
        cur = cur->next;
    }
    return head;
}
//removes the first node of the Queue data structure
FriendNode *dequeue(FriendNode *head)
{
    if (head == NULL)
        return head;

    FriendNode *temp = NULL;
    temp = head->next;
    free(head);
    return temp;
}
//checks if the queue is empty
int empty(FriendNode *head)
{
    if(head == NULL)
        return 1;
    return 0;
}
//gets the amount of nodes needed to traverse to find a given friend
int get_degrees_of_connection(User *a, User *b)
{

    FriendNode *new_node = allUsers;
    while (new_node != NULL)
    {
        new_node->user->visited = false;
        new_node->user->degree = -1;
        new_node = new_node->next;
    }
    int degree = -1;
    if (a != NULL && b != NULL)
    {
        if (strcmp(a->name, b->name) == 0)
        {
            return 0;
        }
        if (in_friend_list(allUsers, b) && in_friend_list(allUsers, a))
        {
            FriendNode *queue_head = NULL;
            queue_head = enqueue(queue_head, a);
            queue_head->user->degree = 0;
            queue_head->user->visited = true;
            while (!empty(queue_head))
            {

                if (in_friend_list(queue_head, b))
                {
                    FriendNode *q = queue_head;
                    while(q != NULL)
                    {
                        if(strcmp(q->user->name, b->name) == 0)
                            break;
                        q = q->next;
                    }
                    degree = q->user->degree;
                    while (queue_head != NULL)
                    {
                        queue_head = dequeue(queue_head);
                    }
                    break;
                }

                FriendNode *node = queue_head->user->friends;
                while (node != NULL)
                {
                    if (!in_friend_list(queue_head, node->user) && node->user->visited == false)
                    {   
                        node->user->degree = queue_head->user->degree + 1;
                        node->user->visited = true;
                        queue_head = enqueue(queue_head, node->user);
                    }
                    node = node->next;
                }
                queue_head = dequeue(queue_head);
            }
        }
       
    }
    return degree;
}

/**
 * Marks two brands as similar.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
    int brandA = -1, brandB = -1;
    if (brand_in_brand_names(brandNameA) == 0 && brand_in_brand_names(brandNameB) == 0)
    {
        brandA = get_brand_index(brandNameA);
        brandB = get_brand_index(brandNameB);
        if (brandA >= 0 && brandA < MAT_SIZE && brandB >= 0 && brandB < MAT_SIZE)
        {
            brand_adjacency_matrix[brandA][brandB] = 1;
            brand_adjacency_matrix[brandB][brandA] = 1;
        }
    }
    return;
}

/**
 * Marks two brands as not similar.
 */
void remove_similar_brands(char *brandNameA, char *brandNameB)
{
    int brandA = -1, brandB = -1;
    if (brand_in_brand_names(brandNameA) == 0 && brand_in_brand_names(brandNameB) == 0)
    {
        brandA = get_brand_index(brandNameA);
        brandB = get_brand_index(brandNameB);
        if (brandA >= 0 && brandA < MAT_SIZE && brandB >= 0 && brandB < MAT_SIZE)
        {
            brand_adjacency_matrix[brandA][brandB] = 0;
            brand_adjacency_matrix[brandB][brandA] = 0;
        }
    }
    return;
}

/**
 * Returns a suggested friend for the given user.
 */
int search_brand(BrandNode *brand, char* name)
{
    BrandNode *new_node = brand;
    if (new_node == NULL)
        return 0;
    if (name == NULL)
        return 0;
    while (new_node != NULL)
    {
        if (strcmp(new_node->brand_name, name) == 0)
        {
            return 1; // found
        }
        new_node = new_node->next;
    }
    return 0; // not found
}

//suggests a friend given user based on the similar brands between two given nodes in the graph
User *get_suggested_friend(User *user)
{
    if(user == NULL)
        return NULL;
    FriendNode *new_friends = allUsers;
    int count = 0, max = 0;
    User *friend = NULL;
    while (new_friends != NULL)
    {
        if(strcmp(new_friends->user->name,user->name) != 0 && !in_friend_list(user->friends, new_friends->user))
        {

            count = 0;
            BrandNode *brand_traverseal = user->brands;
            while (brand_traverseal != NULL)
            {
                count += search_brand(new_friends->user->brands, brand_traverseal->brand_name);
                brand_traverseal = brand_traverseal->next;
            }
            if(count > max)
            {
                friend = new_friends->user;
                max = count;
            }
            else if(count == max)
            {
                if(friend != NULL)
                {
                    int greater = strcmp(friend->name, new_friends->user->name);
                    if (greater < 0)
                        friend = new_friends->user;
                }
                else
                {
                    friend = new_friends->user;
                }
            }
        }
        new_friends = new_friends->next;
    }
    return friend;
}

/**
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 */

int add_suggested_friends(User *user, int n)
{
    int count = 0;
    for (int i = 0; i < n && i< MAT_SIZE; i++)
    {
        User *new_friend = NULL;
        new_friend = get_suggested_friend(user);
        if(new_friend != NULL)
        {
            add_friend(user, new_friend);
            count++;
        }
    }
    return count;
}

/**
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 */
int follow_suggested_brands(User *user, int n)
{
    int counter = 0;
    if (user != NULL)
    {
        BrandNode *head = NULL;
        int count = 0, max = 0;
        char target[MAX_STR_LEN] = "";
        for (int i = 0; i < n && i < MAT_SIZE; i++)
        {
            count = 0, max = 0;
            strcpy(target, "");
            for (int j = 0; j < MAT_SIZE; j++)
            {
                count = 0;
                if (!in_brand_list(head, brand_names[j]) && !in_brand_list(user->brands, brand_names[j]))
                {
                    int index = get_brand_index(brand_names[j]);
                    BrandNode *node = user->brands;
                    while (node != NULL)
                    {
                        int user_brand_index = get_brand_index(node->brand_name);
                        if (brand_adjacency_matrix[index][user_brand_index] == 1 && index != user_brand_index)
                        {
                            count++;
                        }
                        node = node->next;
                    }
                    if(count > max)
                    {
                        max = count;
                        strcpy(target, brand_names[j]);
                    }
                    if(count == max)
                    {
                        max = count;
                        if(strcmp(brand_names[j], target) > 0)
                        {
                            strcpy(target, brand_names[j]);
                        }
                    }
                }
            }
            if(strcmp(target, "") != 0)
            {
                head = insert_into_brand_list(head, target);
            }
        }
        BrandNode *new_node = head;
        while (new_node != NULL)
        {
            user->brands = insert_into_brand_list(user->brands, new_node->brand_name);
            counter++;
            new_node = new_node->next;
        }
    }
    return counter;
}
