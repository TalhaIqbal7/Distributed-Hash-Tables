#include<iostream>
#include<string>
#include<sstream>
#include<cmath>
#include<fstream>
#include<cstdlib>
#include <algorithm>
using namespace std;
unsigned int SHF(string input, int bitspace)
{
    unsigned int Init = 124564352;
    unsigned int Magic = 5674356;
    unsigned int Hash;
    int modulus = 2;
    for (int i = 0; i < input.length(); ++i)
    {
        Hash = Hash ^ (input[i]);
        Hash = Hash ^ Magic;
    }
    modulus = pow(2, bitspace);
    Hash = Hash % modulus;
    return Hash;
}
template<typename T>
class circular;
template<typename T>
struct Node
{
	T ID;
    string filepath;
    int linenumber;
	Node* left, * right;
	Node(const T& k,string file,int line)
	{
		ID = k;
        filepath = file;
        linenumber = line;
		left = NULL;
		right = NULL;
	}
};
template<typename T>
class AVLTree
{
public:
    AVLTree<T>() { root = nullptr; }
public:
    Node<T>* root;
    int getHeight(Node<T>* node);
    int getBalance(Node<T>* node);
    Node<T>* rotationLeft(Node<T>* node);
    Node<T>* rotationRight(Node<T>* node);
    Node<T>* successor(Node<T>* node);
    Node<T>* put(Node<T>*& node, const T& key,string filepath,int line);
    Node<T>* del(Node<T>* node, const T& key);
    bool existornot(Node<T>* node, int key);
    void distributer_inserter(Node<T>* node,circular<T> *obj);
    void value_display(Node<T>* node,int hash);
public:
    void put(const T& key,string filepath,int line);
    void del(const T& key);
    void inOrder(Node<T>* node);
    void preOrder(Node<T>* node);
};
template<typename T>
int AVLTree<T>::getHeight(Node<T>* node)
{
    if (node == nullptr)
        return -1;
    return max(getHeight(node->left), getHeight(node->right)) + 1;
}
template<typename T>
int AVLTree<T>::getBalance(Node<T>* node)
{
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}
template<typename T>
Node<T>* AVLTree<T>::rotationLeft(Node<T>* x)
{
    Node<T>* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}
template<typename T>
Node<T>* AVLTree<T>::rotationRight(Node<T>* x)
{
    Node<T>* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}
template<typename T>
Node<T>* AVLTree<T>::successor(Node<T>* node)
{
    while (node->left != nullptr)
        node = node->left;
    return node;
}

template<typename T>
Node<T>* AVLTree<T>::put(Node<T>*& node, const T& key,string filepath,int line)
{
    if (node == nullptr)
        return new Node<T>(key,filepath,line);
    else if (key < node->ID)
        node->left = put(node->left, key,filepath,line);
    else if (key > node->ID)
        node->right = put(node->right, key,filepath,line);
    else
        return node;
    int balance = getBalance(node);
    if (balance > 1)
    {
        if (key < node->left->ID)
            return rotationRight(node);
        if (key > node->left->ID)
        {
            node->left = rotationLeft(node->left);
            return rotationRight(node);
        }
    }
    else if (balance < -1)
    {
        if (key > node->right->ID)
            return rotationLeft(node);
        if (key < node->right->ID)
        {
            node->right = rotationRight(node->right);
            return rotationLeft(node);
        }
    }
    return node;
}
template<typename T>
void AVLTree<T>::put(const T& key,string filepath,int line)
{
    root = put(root, key,filepath,line);
}
template<typename T>
Node<T>* AVLTree<T>::del(Node<T>* node, const T& key)
{
    if (node == nullptr)
        return node;
    if (key < node->ID)
        node->left = del(node->left, key);
    else if (key > node->ID)
        node->right = del(node->right, key);
    else
    {
        Node<T>* tmp = nullptr;
        if (node->left != nullptr && node->right != nullptr)
        {
            tmp = successor(node->right);
            node->ID = tmp->ID;
            node->right = del(node->right, tmp->ID);
        }
        else if (node->right == nullptr)
        {
            tmp = node;
            node = node->left;
            delete tmp;
        }
        else if (node->left == nullptr)
        {
            tmp = node;
            node = node->right;
            delete tmp;
        }
        else
        {
            delete node;
        }
    }
    int balance = getBalance(node);
    if (balance > 1)
    {
        if (getBalance(node->left) >= 0)
            return rotationRight(node);
        else
        {
            node->left = rotationLeft(node->left);
            return rotationRight(node);
        }
    }
    else if (balance < -1)
    {
        if (getBalance(node->right) <= 0)
            return rotationLeft(node);
        else
        {
            node->right = rotationRight(node->right);
            return rotationLeft(node);
        }
    }
    return node;
}
template<typename T>
void AVLTree<T>::del(const T& key)
{
    root = del(root, key);
}
template<typename T>
void AVLTree<T>::inOrder(Node<T>* node)
{
    if (node == nullptr)
        return;
    inOrder(node->left);
    cout << "Key = " << node->ID << endl;
    cout << "Filepath = " << node->filepath << endl;
    cout << "Line Number = " << node->linenumber << endl;
    inOrder(node->right);
}
template<typename T>
void AVLTree<T>::value_display(Node<T>* node, int Hash)
{
    if (node == nullptr)
        return;
    value_display(node->left,Hash);
    if (Hash == node->ID)
    {
        string line;
        ifstream myfile("data.txt");
        if (myfile.is_open())
        {
            int count = 1;
            while (getline(myfile, line))
            {
                if (count == node->linenumber)
                {
                    cout << "Value at key :" << line << endl;
                }
                count = count + 1;
            }
            myfile.close();
        }

    }
    value_display(node->right,Hash);

}
template<typename T>
void AVLTree<T>::distributer_inserter(Node<T>* node, circular<T>* obj)
{
    if (node == nullptr)
        return;
    distributer_inserter(node->left,obj);
    obj->insert_data(obj->head->id, node->ID);
    distributer_inserter(node->right,obj);
}
template<typename T>
bool AVLTree<T>::existornot(Node<T>* node, int key)
{
    if (node == nullptr)
        return false;
    existornot(node->left,key);
    if (node->ID == key)
    {
        return true;
    }
    existornot(node->right,key);
}
template<typename T>
void AVLTree<T>::preOrder(Node<T>* node)
{
    if (node == nullptr)
        return;
    cout << node->ID << " ";
    preOrder(node->left);
    preOrder(node->right);
}
class doublylinklist;
template <typename T>
class dnode
{
public:
    int id;
    AVLTree<T> *obj;
    doublylinklist routing;
    int range;
    dnode* next;
public:
    dnode()
    {
        id = -1;
        next = NULL;
        obj = new AVLTree<T>;
        range = -1;
    }
    void insert()
    {
    }
};
class FTP_row
{
public:
    dnode<int> *machine_address;
    FTP_row *next;
    FTP_row *prev;

public:
    FTP_row()
    {
        machine_address = NULL;
        next = NULL;
        prev = NULL;
    }
};
class doublylinklist
{
public:
    FTP_row* head;
    FTP_row* temp1;
    FTP_row* temp2;
    FTP_row* temp3;
public:
    doublylinklist()
    {
        head = NULL;
        temp1 = NULL;
        temp2 = NULL;
        temp3 = NULL;
    }
    void insert(dnode<int>* value)
    {
        if (head == NULL)
        {
            FTP_row* node = new FTP_row;
            node->machine_address = value;
            head = node;
            head->next = NULL;
        }
        else
        {
            temp1 = head;
            while (temp1->next != NULL)
            {
                temp1 = temp1->next;
            }
            FTP_row* node = new FTP_row;
            node->machine_address = value;
            node->next = NULL;
            node->prev = temp1;
            temp1->next = node;
        }
    }
    int get_length()
    {
        temp1 = head;
        int count = 0;
        while (temp1->next != NULL)
        {
            count = count + 1;
            temp1 = temp1->next;
        }
        count = count + 1;
        return count;
    }
    void display()
    {
        temp1 = head;
        int i = 1;
        while (temp1->next != NULL)
        {
            cout <<"|"<< i << "|" << temp1->machine_address->id <<"|"<< endl;
            i = i + 1;
            temp1 = temp1->next;
        }
        cout <<"|"<< i << "|" << temp1->machine_address->id <<"|"<< endl;
    }
};
template <typename T>
class circular
{
public:
    dnode<T>* head;
    dnode<T>* temp1;
    dnode<T>* temp2;
    dnode<T>* temp3;
    int linenumber;
public:
    circular()
    {
        head = NULL;
        linenumber = 1;
    }
    void insert(dnode<T>* value)
    {

        if (head == NULL)
        {
            head = value;
            head->next = head;
        }
        else
        {
            temp1 = head;
            while (temp1->next != head)
            {
                temp1 = temp1->next;
            }
            temp1->next = value;
            value->next = head;
        }
    }
    void delete_machine(int machine_id,int bitspace)
    {
        temp1 = head;
        if (machine_id == head->id)
        {
            temp2 = head;
            while (temp2->next != head)
            {
                temp2 = temp2->next;
            }
        }
        while (temp1->id != machine_id)
        {
            temp2 = temp1;
            temp1 = temp1->next;
        }
        AVLTree<int>* copy = NULL;
        copy = new AVLTree<int>;
        copy->root = temp1->obj->root;
        temp2->next = temp1->next;
        empty_rt();
        make_rt(bitspace);
        set_range();
        copy->distributer_inserter(copy->root, this);
    }
    bool existornot(int val)
    {
        temp1 = head;
        while (temp1->next != head)
        {
            if (temp1->id == val)
            {
                return true;
            }
            temp1 = temp1->next;
        }
        if (temp1->id == val)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int length()
    {
        int count = 0;
        temp1 = head;
        while (temp1->next != head)
        {
            count = count + 1;
            temp1 = temp1->next;
        }
        count = count + 1;
        return count;
    }
    //Avl Tree Checking later
    void sort()
    {
        int temp = 0;
        AVLTree<int>* copy;
        for (temp3 = head; temp3->next != head; temp3 = temp3->next) {
            // start the selection from the temp3->next
            for (temp2 = temp3->next; temp2 != head; temp2 = temp2->next) {
                if (temp3->id > temp2->id) {
                    temp= temp3->id;
                    copy = temp3->obj;
                    temp3->id= temp2->id;
                    temp3->obj = temp2->obj;
                    temp2->id = temp;
                    temp2->obj = copy;
                }
            }//End of outer loop
        }//End of sorting
    }
    void display()
    {
        temp1 = head;
        while (temp1->next != head)
        {
            cout << temp1->id << endl;
            temp1 = temp1->next;
        }
        cout << temp1->id << endl;
    }
    void make_rt(int bits)
    {
        temp1 = head;
        int highid = -1;
        while (temp1->next != head)
        {
            temp1 = temp1->next;
        }
        highid = temp1->id;
        temp1 = head;
        int ftp = -1;
        int len = length();
        for (int i = 0; i < len; ++i)
        {
            for (int j = 1; j <= bits; ++j)
            {
                ftp = temp1->id + pow(2, j - 1);
                if (ftp >= pow(2,bits))
                {
                    int power = pow(2, bits);
                    ftp = ftp % power;
                }
                temp3 = temp1->next;
                temp2 = temp1;
                while (temp3 != temp1)
                {
                    if (ftp > highid || ftp< head->id)
                    {
                        temp1->routing.insert(head);
                        break;
                    }
                    if (ftp <= temp3->id && ftp>temp2->id)
                    {
                        temp1->routing.insert(temp3);
                        break;
                    }
                    if (ftp == temp3->id)
                    {
                        temp1->routing.insert(temp3);
                        break;
                    }
                    temp2 = temp3;
                    temp3 = temp3->next;
                }
            }
            //temp1->routing.display();
            //cout << endl;
            temp1 = temp1->next;
        }
    }
    void insert_data(int machine_id, int hash)
    {
        temp1 = head;
        temp2 = head;
        while (temp2->next != head)
        {
            temp2 = temp2->next;
        }
        while (temp1->id != machine_id)
        {
            temp1 = temp1->next;
        }
        int length = 0;
        cout << "Path\n";
        while (1)
        {
            cout << temp1->id << "->";
            length = temp1->routing.get_length();
            if (hash > temp2->id)
            {
                head->obj->put(hash, "../data.txt", linenumber);
                cout << head->id;
                linenumber = linenumber + 1;
                cout << endl;
                return;
            }
            if ( hash > temp2->id && temp1->id==head->id)
            {
                temp1->obj->put(hash, "../data.txt", linenumber);
                linenumber = linenumber + 1;
                cout << endl;
                return;
            }
            if (hash <= temp1->id && hash >= temp1->range)
            {
                temp1->obj->put(hash, "../data.txt", linenumber);
                linenumber = linenumber + 1;
                cout << endl;
                return;
            }
            if (hash <= head->id && temp1->id == head->id)
            {
                temp1->obj->put(hash, "../data.txt", linenumber);
                linenumber = linenumber + 1;
                cout << endl;
                return;
            }
            temp1->routing.temp1 = temp1->routing.head;
            for (int i = 0; i < length; ++i)
            {
                if (hash <= temp1->routing.temp1->machine_address->id)
                {
                    if (i > 0)
                    {
                       temp1=temp1->routing.temp1->prev->machine_address;
                    }
                    else
                    {
                        temp1 = temp1->routing.temp1->machine_address;
                    }
                    break;
                }
                if (i == length - 1)
                {
                    temp1 = temp1->routing.temp1->machine_address;
                    break;
                }
                temp1->routing.temp1 = temp1->routing.temp1->next;
            }
        }

    }
    void remove_data(int machine_id, int hash)
    {
        temp1 = head;
        temp2 = head;
        while (temp2->next != head)
        {
            temp2 = temp2->next;
        }
        while (temp1->id != machine_id)
        {
            temp1 = temp1->next;
        }
        int length = 0;
        cout << "Path\n";
        while (1)
        {
            cout << temp1->id << "->";
            length = temp1->routing.get_length();
            if (hash > temp2->id)
            {
                head->obj->del(hash);
                cout << head->id;
                linenumber = linenumber + 1;
                cout << endl;
                return;
            }
            if (hash > temp2->id && temp1->id == head->id)
            {
                temp1->obj->del(hash);
                linenumber = linenumber + 1;
                cout << endl;
                return;
            }
            if (hash <= temp1->id && hash >= temp1->range)
            {
                temp1->obj->del(hash);
                linenumber = linenumber + 1;
                cout << endl;
                return;
            }
            if (hash <= head->id && temp1->id == head->id)
            {
                temp1->obj->del(hash);
                linenumber = linenumber + 1;
                cout << endl;
                return;
            }
            temp1->routing.temp1 = temp1->routing.head;
            for (int i = 0; i < length; ++i)
            {
                if (hash <= temp1->routing.temp1->machine_address->id)
                {
                    if (i > 0)
                    {
                        temp1 = temp1->routing.temp1->prev->machine_address;
                    }
                    else
                    {
                        temp1 = temp1->routing.temp1->machine_address;
                    }
                    break;
                }
                if (i == length - 1)
                {
                    temp1 = temp1->routing.temp1->machine_address;
                    break;
                }
                temp1->routing.temp1 = temp1->routing.temp1->next;
            }
        }

    }
    void set_range()
    {
        temp1 = head;
        while (temp1->next != head)
        {
            temp1->next->range = temp1->id + 1;
            temp1 = temp1->next;
        }
        temp1->next->range = temp1->id + 1;
        temp1 = temp1->next;
        
    }
    void display_avl_tree(int machine)
    {
        temp1 = head;
        while (temp1->id != machine)
        {
            temp1 = temp1->next;
        }
        cout << "Machine Id = " << temp1->id << endl;
        temp1->obj->inOrder(temp1->obj->root);
        cout << endl;
    }
    void display_rt(int machine)
    {
        temp1 = head;
        while (temp1->id != machine)
        {
            temp1 = temp1->next;
        }
        temp1->routing.display();
    }
    void empty_rt()
    {
        temp1 = head;
        while (temp1->next != head)
        {
            temp1->routing.head = NULL;
            temp1 = temp1->next;
        }
        temp1->routing.head = NULL;
    }
    void search(int machine_id,int hash)
    {
        temp1 = head;
        temp2 = head;
        while (temp2->next != head)
        {
            temp2 = temp2->next;
        }
        while (temp1->id != machine_id)
        {
            temp1 = temp1->next;
        }
        int length = 0;
        cout << "Path\n";
        while (1)
        {
            cout << temp1->id << "->";
            length = temp1->routing.get_length();
            if (hash > temp2->id)
            {
                if (head->obj->existornot(head->obj->root, hash))
                {
                    cout << endl;
                    cout << "The following key exist in the machine : " << head->id << endl;
                    head->obj->value_display(head->obj->root, hash);
                }
                else
                {
                    cout << endl;
                    cout << "The key does not exist\n";
                }
                cout << endl;
                return;
            }
            if (hash > temp2->id && temp1->id == head->id)
            {
                if (temp1->obj->existornot(temp1->obj->root, hash))
                {
                    cout << endl;
                    cout << "The following key exist in the machine : " << temp1->id << endl;
                    temp1->obj->value_display(temp1->obj->root, hash);
                }
                else
                {
                    cout << endl;
                    cout << "The key does not exist\n";
                }
                cout << endl;
                return;
            }
            if (hash <= temp1->id && hash >= temp1->range)
            {
                if (temp1->obj->existornot(temp1->obj->root, hash))
                {
                    cout << endl;
                    cout << "The following key exist in the machine : " << temp1->id << endl;
                    temp1->obj->value_display(temp1->obj->root, hash);
                }
                else
                {
                    cout << endl;
                    cout << "The key does not exist\n";
                }
                cout << endl;
                return;
            }
            if (hash <= head->id && temp1->id == head->id)
            {
                if (temp1->obj->existornot(temp1->obj->root, hash))
                {
                    cout << endl;
                    cout << "The following key exist in the machine : " << temp1->id << endl;
                    temp1->obj->value_display(temp1->obj->root, hash);
                }
                else
                {
                    cout << endl;
                    cout << "The key does not exist\n";
                }
                cout << endl;
                return;
            }
            temp1->routing.temp1 = temp1->routing.head;
            for (int i = 0; i < length; ++i)
            {
                if (hash <= temp1->routing.temp1->machine_address->id)
                {
                    if (i > 0)
                    {
                        temp1 = temp1->routing.temp1->prev->machine_address;
                    }
                    else
                    {
                        temp1 = temp1->routing.temp1->machine_address;
                    }
                    break;
                }
                if (i == length - 1)
                {
                    temp1 = temp1->routing.temp1->machine_address;
                    break;
                }
                temp1->routing.temp1 = temp1->routing.temp1->next;
            }
        }


    }
    void distribute_data(int machine)
    {
        temp1 = head;
        while (temp1->id != machine)
        {
            temp1 = temp1->next;
        }
        temp2 = temp1->next;
        AVLTree<int>* copy=new AVLTree<int>;
        copy->root = temp2->obj->root;
        temp2->obj = nullptr;
        temp2->obj =new AVLTree<int>;
        copy->distributer_inserter(copy->root, this);
    }
};
int main()
{
    int bitspace = 0;
    int no_of_machines = 0;
    int choice = 0;
    int* id_of_machines;
    circular<int> Ring_DHT;
    cout << "Input the number of Bit-Space\n";
    cin >> bitspace;
    while (1)
    {
        cout << "Enter the number of machines\n";
        cin >> no_of_machines;
        if (no_of_machines <= pow(2, bitspace))
        {
            break;
        }
        cout << "Wrong Input(out of range)\n";
    }
    id_of_machines = new int[no_of_machines];
    while (1)
    {
        cout << "Press 1 to Manually Assign ID's to machines\n";
        cout << "Press 2 to Automatically Assign ID's to machines\n";
        cin >> choice;
        if (choice == 1)
        {
            for (int i = 0; i < no_of_machines; ++i)
            {
                while (1)
                {
                    cout << "Enter the id of " << i + 1 << " machine\n";
                    cin >> id_of_machines[i];
                    if (id_of_machines[i] < pow(2, bitspace) && id_of_machines[i]>=0)
                    {
                        break;
                    }
                    cout<<"Wrong Input(out of range)\n";
                }
                dnode<int>* id;
                id = new dnode<int>;
                id->id = id_of_machines[i];
                Ring_DHT.insert(id);
            }
            Ring_DHT.sort();
            break;
        }
        else if (choice == 2)
        {
            int distributer = 0;
            int distributer_adder = 0;
            distributer = pow(2, bitspace) / no_of_machines;
            distributer_adder = distributer;
            for (int i = 0; i < no_of_machines; ++i)
            {
                id_of_machines[i] = distributer-distributer_adder;
                dnode<int>* id;
                id = new dnode<int>;
                id->id = id_of_machines[i];
                Ring_DHT.insert(id);
                distributer = distributer + distributer_adder;
            }
            break;
        }
        cout << "Wrong Choice\n";
    }
    Ring_DHT.make_rt(bitspace);
    Ring_DHT.set_range();
    //Ring_DHT.display();
    fstream MyFile("data.txt");
    while (1)
    {
        cout << "Press 1 to Insert data\n";
        cout << "Press 2 to Remove data\n";
        cout << "Press 3 to Print routing table of a machine\n";
        cout << "Press 4 to Print AVL tree of a machine\n";
        cout << "Press 5 to Search a key from machine\n";
        cout << "Press 6 to Add a new Machine\n";
        cout << "Press 7 to Remove a  Machine\n";
        cin >> choice;
        if (choice == 1)
        {
            int key;
            string value;
            int machine_id = -1;
            int Hash = -1;
            cout << "Enter the key\n";
            cin >> key;
            cout << "Enter the value\n";
            cin >> value;
           // Hash = SHF(key, bitspace);
            while (1)
            {
                cout << "Enter the Id of the Machine from the given machines\n";
                Ring_DHT.display();
                cin >> machine_id;
                if (Ring_DHT.existornot(machine_id))
                {
                    break;
                }
                cout << "This machines does not exist\n";
            }
            Ring_DHT.insert_data(machine_id, key);           
            // Write to the file
            MyFile << value << endl;
            // Close the file
            //MyFile.close();
        }
        else if (choice == 2)
        {
            int key;
            string value;
            int machine_id = -1;
            int Hash = -1;
            cout << "Enter the key\n";
            cin >> key;
            while (1)
            {
                cout << "Enter the Id of the Machine from the given machines\n";
                Ring_DHT.display();
                cin >> machine_id;
                if (Ring_DHT.existornot(machine_id))
                {
                    break;
                }
                cout << "This machines does not exist\n";
            }
            Ring_DHT.remove_data(machine_id, key);

        }
        else if (choice == 3)
        {
            int machine_id = -1;
            while (1)
            {
                cout << "Enter the Id of the Machine from the given machines\n";
                Ring_DHT.display();
                cin >> machine_id;
                if (Ring_DHT.existornot(machine_id))
                {
                    break;
                }
                cout << "This machines does not exist\n";
            }
            Ring_DHT.display_rt(machine_id);

        }
        else if (choice == 4)
        {
            int machine_id = -1;
            while (1)
            {
                cout << "Enter the Id of the Machine from the given machines\n";
                Ring_DHT.display();
                cin >> machine_id;
                if (Ring_DHT.existornot(machine_id))
                {
                    break;
                }
                cout << "This machines does not exist\n";
            }
            Ring_DHT.display_avl_tree(machine_id);
        }
        else if (choice == 5)
        {
            int machine_id = -1;
            int key = -1;
            while (1)
            {
                cout << "Enter the Id of the Machine from the given machines\n";
                Ring_DHT.display();
                cin >> machine_id;
                if (Ring_DHT.existornot(machine_id))
                {
                    break;
                }
                cout << "This machines does not exist\n";
            }
            while (1)
            {
                cout << "Enter the key to search\n";
                cin >> key;
                if (key < pow(2, bitspace) || key >= 0)
                {
                    break;
                }
                cout << "This key is invalid (Out of Range)\n";
            }
            Ring_DHT.search(machine_id, key);
        }
        else if (choice == 6)
        {
        int machine_id = -1;
            while (1)
            {
                cout << "Enter the id of new machine\n";
                cin >> machine_id;
                if (machine_id < pow(2, bitspace) && !Ring_DHT.existornot(machine_id) && machine_id>=0)
                {
                    break;
                }
                cout << "Machine Already Exist or Out of Range\n";
            }
            dnode<int>* id;
            id = new dnode<int>;
            id->id = machine_id;
            Ring_DHT.insert(id);
            Ring_DHT.sort();
            Ring_DHT.set_range();
            Ring_DHT.empty_rt();
            Ring_DHT.make_rt(bitspace);
            Ring_DHT.distribute_data(machine_id);
        }
        else if (choice == 7)
        {
            int machine_id = -1;
            while (1)
            {
                cout << "Enter the id of Machine\n";
                cin >> machine_id;
                if (machine_id < pow(2, bitspace) && Ring_DHT.existornot(machine_id) && machine_id >= 0)
                {
                    break;
                }
                cout << "Machine Does Not Exist\n";
            }
            Ring_DHT.delete_machine(machine_id, bitspace);

        }
    }
	return 0;
}