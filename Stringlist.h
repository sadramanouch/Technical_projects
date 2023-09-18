// Stringlist.h

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <Sadra Manouchehrifar>
// St.# : <301395975>
// Email: <sma295@sfu.ca>
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not use any other #includes or #pragmas in this file.
//

#pragma once

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

class Stringlist 
{
    int cap;     // capacity
    string *arr; // array of strings
    int sz;      // size

    // this enum function helps us to distinguish between different functions that we have to call in our undo function
    enum Function{
        undo_copy, undo_remove, undo_set, undo_insert, counter
    };


    struct Node{
        string word;     // string word
        string *array;   // pointer to the array for the copy function
        int index;       // index of positions for remove and insert
        int cap;         // capacity for our copy function
        int count;       // count for our remove all function
        Node *next;      // pointer to the next position in stack
        Function x;      // enum keeper to keep things organized and clean
    };
    Node* head = nullptr; // set the head of stack to nullptr from the beginning

    


    //
    // Helper function for throwing out_of_range exceptions.
    //
    void bounds_error(const string &s) const
    {
        throw out_of_range("Stringlist::" + s + " index out of bounds");
    }

    //
    // Helper function for checking index bounds.
    //
    void check_bounds(const string &s, int i) const
    {
        if (i < 0 || i >= sz)
            bounds_error(s);
    }

    //
    // Helper function for copying another array of strings.
    //
    void copy(const string *other)
    {
        for (int i = 0; i < sz; i++)
        {
            arr[i] = other[i];
        }
    }

    //
    // Helper function for checking capacity; doubles size of the underlying
    // array if necessary.
    //
    void check_capacity()
    {
        if (sz == cap)
        {
            cap *= 2;
            string *temp = new string[cap];
            for (int i = 0; i < sz; i++)
            {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
        }
    }

    // every node created is deleted in the undo function for not leakage 
    // push function which is used to undo insert before, keep everything organized in the head
    void push_undo_insert(int index){
        head = new Node{"",nullptr, index, 0, 0,head, undo_insert};
    }

        // push function which is used to undo remove at, keep everything organized in the head
    void push_undo_remove(const string &w, int index){
        head = new Node{w,nullptr, index, 0, 0,head, undo_remove};
    }

        // push function which is used to undo set, keep everything organized in the head
    void push_undo_set(const string &w, int index){
        head = new Node{w,nullptr, index, 0, 0,head, undo_set};
    }

        // push function which is used to undo operator=, keep everything organized in the head
    void push_undo_copy(int size, int cap, string *w){
        head = new Node{"", w, size, cap, 0, head, undo_copy};
    }

        // push function to keep the count for the # of elements for remove all function, keep everything organized in the head
    void push_counter(int count){
        head = new Node{"", nullptr, 0, 0, count, head, counter};
    }

    // universal pop that return the top node of the stack
    Node* pop(){
        Node* p = head;
        string s = p->word;
        head = head->next;
        return p;
    }

    // returns a copy of the string at the top of the stack
    const string &top() const{
        assert(!empty());
        return head->word;
    }

    // checks if the stack is empty or not
    bool emptyy() const{
        return size() == 0;
    }

public:

    //
    // Default constructor: makes an empty StringList.
    //
    Stringlist()
        : cap(10), arr(new string[cap]), sz(0)
    {
    }

    //
    // Copy constructor: makes a copy of the given StringList.
    //
    // Does *not* copy the undo stack, or any undo information from other.
    //
    Stringlist(const Stringlist &other)
        : cap(other.cap), arr(new string[cap]), sz(other.sz)
    {
        copy(other.arr);
    }

    //
    // destructor
    //
    ~Stringlist()
    {
        delete[] arr;
        // here we delete every node that was used and created in the process of this function
        while (head!=nullptr){
            Node*p = head;
            delete[] p->array;
            head = head->next;
            delete p;

        }
    }

    //
    // Assignment operator: makes a copy of the given StringList.
    //
    // undoable
    //
    // For undoing, when assignment different lists, the undo stack is not
    // copied:
    //
    //    lst1 = lst2; // lst1 undo stack is updated to be able to undo the //
    //    assignment; lst1 does not copy lst2's stack
    //    //
    //    // lst2 is not change in any way
    //
    // Self-assignment is when you assign a list to itself:
    //
    //    lst1 = lst1;
    //
    // In this case, nothing happens to lst1. Nothing is changed. Both its
    // stirng data and undo stack are left as-is.
    //
    Stringlist &operator=(const Stringlist &other)
    {

        // we save the size, capacity, and the array itself by pushing them to the stack
        push_undo_copy(sz, cap, arr);
        if (this != &other)
        {
            cap = other.capacity();
            arr = new string[cap];
            sz = other.size();
            copy(other.arr);
        }
        return *this;
    }

    //
    // Returns the number of strings in the list.
    //
    int size() const { return sz; }

    //
    // Returns true if the list is empty, false otherwise.
    //
    bool empty() const { return size() == 0; }

    //
    // Returns the capacity of the list, i.e. the size of the underlying array.
    //
    int capacity() const { return cap; }

    //
    // Returns the string at the given index.
    //
    string get(int index) const
    {
        check_bounds("get", index);
        return arr[index];
    }

    //
    // Returns the index of the first occurrence of s in the list, or -1 if s is
    // not in the lst.
    //
    int index_of(const string &s) const
    {
        for (int i = 0; i < sz; i++)
        {
            if (arr[i] == s)
            {
                return i;
            }
        }
        return -1;
    }

    //
    // Returns true if s is in the list, false otherwise.
    //
    bool contains(const string &s) const
    {
        return index_of(s) != -1;
    }

    //
    // Returns a string representation of the list.
    //
    string to_string() const
    {
        string result = "{";
        for (int i = 0; i < size(); i++)
        {
            if (i > 0)
                result += ", ";
            result += "\"" + get(i) + "\"";
        }
        return result + "}";
    }

    //
    // Sets the string at the given index.
    //
    // undoable
    //
    void set(int index, string value)
    {
        check_bounds("set", index);

        // after checking bounds, we push the string value and index of set onto the stack to be saves for undo
        push_undo_set(value, index);
        arr[index] = value;
    }

    //
    // Insert s before index; if necessary, the capacity of the underlying array
    // is doubled.
    //
    // undoable
    //
    void insert_before(int index, const string &s)
    {
        if (index < 0 || index > sz) // allows insert at end, i == sz
            bounds_error("insert_before");
        check_capacity();

        // we push the index onto the stack to be saved
        push_undo_insert(index);
        for (int i = sz; i > index; i--)
        {
            arr[i] = arr[i - 1];
        }
        arr[index] = s;
        sz++;
    }



    //
    // Appends s to the end of the list; if necessary, the capacity of the
    // underlying array is doubled.
    //
    // undoable
    //
    void insert_back(const string &s)
    {
        insert_before(size(), s);
    }

    //
    // Inserts s at the front of the list; if necessary, the capacity of the
    // underlying array is doubled.
    //
    // undoable
    //
    void insert_front(const string &s)
    {
        insert_before(0, s);
    }

    //
    // Removes the string at the given index; doesn't change the capacity.
    //
    // undoable
    //
    void remove_at(int index)
    {
        // we save the content of the index in w 
        string w = arr[index];
        check_bounds("remove_at", index);

        // we push the content and index onto the stack
        push_undo_remove(w,index);
        for (int i = index; i < sz - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        sz--;
    }

    //
    // Removes all strings from the list; doesn't change the capacity.
    //
    // undoable
    //
    void remove_all()
    {
        string g = "";
        for (int i = cap-1; i>=0;i--){
            g += arr[i];
        }

        // we store size in size
        int size = sz;
        while (sz > 0)
        {
            remove_at(sz - 1);
        }

        // we push the size onto the stack to know how many times do we have to call pop when reduing
        push_counter(size);
    }

    //
    // Removes the first occurrence of s in the list, and returns true. If s is
    // nowhere in the list, nothing is removed and false is returned.
    //
    // undoable
    //
    bool remove_first(const string &s)
    {
        int index = index_of(s);

        // we push the string and index onto the stack to save
        push_undo_remove(s,index);
        if (index == -1)
            return false;
        remove_at(index);
        return true;
    }

    //
    // Removes all occurrences of s in the list, and returns the number of items
    // removed.
    //
    // undoable
    //
    int remove_all(const string &s)
    {
        int count = 0;
        for (int i = 0; i < sz; i++)
        {
            if (arr[i] == s)
            {
                remove_at(i);
                count++;
                i--;
            }
        }

        //we push the counter which is a node that store the number of items in the list
        push_counter(count);
        return count;
    }

    
    // Undoes the last operation that modified the list. Returns true if a
    // change was undone.
    
    // If there is nothing to undo, does nothing and returns false.
    
    bool undo()
    {
        // this is the default if the stack is empty, undoing is not possible
        if (head == nullptr){
            return false;
        }
        // we pop the first node and store in p
        Node* p = pop();
        // this functions sets the size, capacity, and array back to they original content when undo is called and matches the enum tag
        if (p->x == undo_copy){
            cap = p->cap;
            sz = p->index;
            delete[] arr;
            arr = p->array;
        }
        // this function undos remove by inserting the node at irs original index
        if (p->x == undo_remove){
            insert_before(p->index, p->word);
            // we pop after every call to ensure we are not double pushing an element onto the stack
            // we delete all the pop's after calling because they are not needed
            delete pop();
            delete p;
            return true;
        }
        // this function undos insert by removing that index when undo is called. pop is present for the same reason
        if (p->x == undo_insert){
            remove_at(p->index);
            delete pop();
            delete p;
            return true;
        }
        // this function undos set by calling set again with the original value and the same index. pop is called for the same reason
        if (p->x == undo_set){
            set(p->index, p->word);
            delete pop();
            delete p;
            return true;
        }
        // this function undos remove all
        // if we get to this part, p is the counter node that holds the number of elements removed by remove all
        while (p->count != 0){
            // we pop the nodes in a while loop and insert them back and reducing the count each time
            Node* q = pop();
            insert_before(q->index, q->word);
            delete pop(); 
            delete q;
            p->count--;
        }
        delete p;
        return true;
    }

}; // class Stringlist

//
// Prints list to in the format {"a", "b", "c"}.
//
ostream &operator<<(ostream &os, const Stringlist &lst)
{
    return os << lst.to_string();
}

//
// Returns true if the two lists are equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists. All
// that matters is that they have the same strings in the same order.
//
bool operator==(const Stringlist &a, const Stringlist &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (int i = 0; i < a.size(); i++)
    {
        if (a.get(i) != b.get(i))
        {
            return false;
        }
    }
    return true;
}

//
// Returns true if the two lists are not equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists.
//
bool operator!=(const Stringlist &a, const Stringlist &b)
{
    return !(a == b);
}
