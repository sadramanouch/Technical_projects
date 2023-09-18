// a3.cpp

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
// Do not #include any other files!
//
#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// This is my Queue class that tkaes templaate typename T
template <typename T>
class Queue : public Queue_base<T>{

    // we first create the struct Node that takes in the values 
    struct Node{
        Announcement data;
        Node* next;
    };

    // This is the private part of our class where we define a pointer to head, last and have int size for our queues
    private:
        Node* head;
        Node* last;
        int sz;
    public:

        // This is the destructor which si called automatically before the class ends to clear everything
        ~Queue() {
            while (head != nullptr){
                Node* p = head;
                head = head->next;
                delete p;
            }
        }

        // This is our constructor that initializes everything 
        Queue() {
            head = nullptr;
            last = nullptr;
            sz = 0;
        }

        // This is our size function which return the size of the queue
        int size() const {
            return sz;
        }

        // This funtion inputs new data to our desired queue but chaning head and tail pointer and adding to tail
        void enqueue(const T &item) {
            Node* p = new Node{item, nullptr};
            if (head == nullptr){
                head = p;
                last = p;
            }
            else{
                last->next = p;
                last = p;
            }
            sz++;
        }

        // This function returns the value of the head which is a object in announcement
        const T &front() const {
            if (head == nullptr){
                throw runtime_error("front: queue is empty");
            }
            else{
                return head->data;
            }
        }

        // This function removes from the head
        void dequeue() {
            if (head == nullptr){
                throw runtime_error("dequeue: queue is empty");
            }
            else{
                Node* p = head;
                head = head->next;
                delete p;
                sz--;
            }
        }
       
};

// This is our class jinglenets and it has Announcement objects which we wil use to fill our 5 queues 
class JingleNet : public Queue <Announcement>{

    // Here we initialize our queues
    private:
        Queue <Announcement> santa;
        Queue <Announcement> reindeer;
        Queue <Announcement> elf2;
        Queue <Announcement> elf1;
        Queue <Announcement> snowman;

    public:

        // This is the destructor which is always needed
        ~JingleNet(){
        };

        // This function takes in a jinglenet objects 3 different parameters and queues them in the appropriate queue
        void send(const string &name, Rank R, const string &txt){
            // we create the object based on the given parameters
            Announcement obj(name, R, txt);
            // we see if the rank of the abject matches any of the queue ranks and put them when it matches
            if (R == Rank::SNOWMAN){
                snowman.enqueue(obj);
            }
            else if (R == Rank::ELF1){
                elf1.enqueue(obj);
            }
            else if (R == Rank::ELF2){
                elf2.enqueue(obj);
            }
            else if (R == Rank::REINDEER){
                reindeer.enqueue(obj);
            }

            // we do this for all the different queues
            else{
                santa.enqueue(obj);
            }
        };

        // This function looks for the sender's name and if it matches the sender's name in the queues, it gets promoted one higher (other than santa)
        void promote(const string &sender){

            // we run through each queue independently
            while (reindeer.size() != 0){

                // pull out their object and see if it matches the sender name provided
                Announcement g = reindeer.front();
                if (g.get_sender_name() == sender){

                    // if yes, we create a new object with the appropriate rank and promote them
                    Announcement gg = {g.get_sender_name(), Rank::SANTA, g.get_text()};
                    santa.enqueue(gg);
                }
            }
            while (elf2.size() != 0){
                Announcement g = elf2.front();
                if (g.get_sender_name() == sender){
                    Announcement gg = {g.get_sender_name(), Rank::SANTA, g.get_text()};
                    reindeer.enqueue(gg);
                }
            }
            while (elf1.size() != 0){
                Announcement g = elf1.front();
                if (g.get_sender_name() == sender){
                    Announcement gg = {g.get_sender_name(), Rank::SANTA, g.get_text()};
                    elf2.enqueue(gg);
                }
            }

            // we do this for all but santa as he cannot be promoted anymore
            while (snowman.size() != 0){
                Announcement g = snowman.front();
                if (g.get_sender_name() == sender){
                    Announcement gg = {g.get_sender_name(), Rank::SANTA, g.get_text()};
                    elf1.enqueue(gg);
                }
            }
        }

        // This function takes in the num of time announcement needs to be made     
        void call (const int num){

            // we run a loop to make sure to announce from highest priority to lowest
            for (int i = 0; i<num; i++){

                // go through each queue and see if they are not empty, we announce
                if (santa.size() != 0){

                    // in priority, if they have an object, we announce them and remove them
                    jnet.announce(santa.front());
                    santa.dequeue();
                }
                else if (reindeer.size() != 0){
                    jnet.announce(reindeer.front());
                    reindeer.dequeue();
                }
                else if (elf2.size() != 0){
                    jnet.announce(elf2.front());
                    elf2.dequeue();
                }
                else if (elf1.size() != 0){
                    jnet.announce(elf1.front());
                    elf1.dequeue();
                }
                // we do this for all 5 queues
                else if (snowman.size() != 0){
                    jnet.announce(snowman.front());
                    snowman.dequeue();
                }

                // we make sure to do this because if the announce number is wrong, we do not go out of bounce and mess stuff up
                else{
                    break;
                }
            }
        }

        // This function removes all objects with the sender name provided
        void remove(const string &sender){

            // we first see if they do have any objects in them
            while (santa.size() != 0){

                // we go through all objects in priority order to see if their sender name matches the string provided
                Announcement g = santa.front();
                if (g.get_sender_name() == sender){

                    // if it does, we remove them
                    santa.dequeue();
                }
            }
            while (reindeer.size() != 0){
                Announcement g = reindeer.front();
                if (g.get_sender_name() == sender){
                    reindeer.dequeue();
                }
            }
            while (elf2.size() != 0){
                Announcement g = elf2.front();
                if (g.get_sender_name() == sender){
                    elf2.dequeue();
                }
            }
            while (elf1.size() != 0){
                Announcement g = elf1.front();
                if (g.get_sender_name() == sender){
                    elf1.dequeue();
                }
            }

            // this is repeated for every queue
            while (snowman.size() != 0){
                Announcement g = snowman.front();
                if (g.get_sender_name() == sender){
                    snowman.dequeue();
                }
            }
        }
};

int main(int argc, char* argv[])
{
    // for our main function, we first check if the user has passed in a filename or not
    if (argc != 2){
        cout << "enter filename" << endl;
        return 1;
    }

    // we create our JingleNet object that we work with
    JingleNet obj; 

    // we read the file and take in each line
    string file = argv[1];
    ifstream infile(file);
    string line;

    // we run a loop that while there exists another line
    while (getline(infile, line)){

        // store the first work in first
        string first = line.substr(0, line.find(" "));

        // and store the second word in sender
        string sender = line.substr(0, line.find(" "));

        // and we update the line as we go 
        line = line.substr(line.find(" ") + 1);

        // if the first work is send, proote announcements, announce, or remove all, we run these functions that do the job as described previously
        if (first == "SEND"){

            // we create a new object and pass on the values with the help of anouncement.h
            Announcement use(sender);
            obj.send(use.get_sender_name(), use.get_rank(), use.get_text());
        }
        else if (first == "PROMOTE ANNOUNCEMENTS"){
            obj.promote(sender);
        }
        else if (first == "ANNOUNCE"){

            // if announce is called, we get the digit that tells us which anounceents need to be copied onto announcements.txt
            int digit = stoi(sender.substr(0,sender.find(" ")));
            obj.call(digit);
        }
        else{
            obj.remove(sender);
        }
    }
}
