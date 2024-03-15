#include "main.h"
#include <list>

extern int MAXSIZE;

struct Customer
{
    string n;
    int e, t;

    Customer *next = nullptr;
    Customer *prev = nullptr;

    Customer()
        : n(""), e(0), t(0) {}

    Customer(string n, int e, int t)
        : n(n), e(e), t(t) {}
    void print()
    {
        cout << n << "-" << e << endl;
    }
};

class List
{
public:
    Customer *head;
    int currSize;

    List()
    {
        head = nullptr;
        currSize = 0;
    }

    ~List()
    {
        clear();
    }

    bool empty()
    {
        return currSize == 0;
    }

    bool full()
    {
        return currSize == MAXSIZE;
    }

    int size()
    {
        return currSize;
    }

    int max_size()
    {
        return MAXSIZE;
    }

    Customer *get_head()
    {
        return head;
    }

    Customer *get_tail()
    {
        return head == nullptr ? nullptr : head->prev;
    }

    void append(string n, int e, int t)
    {
        // Add after tail
        Customer *newNode = new Customer(n, e, t);
        insert(get_tail(), newNode);
    }

    void insert(Customer *position, Customer *newNode)
    {
        // insert new customer after position
        Customer *cus = newNode;
        if (position == nullptr)
        {
            if (empty())
            {
                head = cus;
                head->next = head;
                head->prev = head;
                currSize++;
            }
            return;
        }
        cus->next = position->next;
        cus->prev = position;
        if (position->next != nullptr)
            position->next->prev = cus;
        position->next = cus;
        currSize++;
    }
    //  1-2
    void pop()
    {
        // remove tail
        remove(get_tail());
    }

    void remove(Customer *n)
    {
        // remove node n
        // 1<->2<->3
        if (n == nullptr || empty())
            return;
        if (currSize == 1)
        {
            delete n;
            head = nullptr;
            currSize = 0;
            return;
        }

        n->prev->next = n->next;
        n->next->prev = n->prev;
        currSize--;
        if (n == head)
            head = n->next;
        delete n;
    }

    void clear()
    {
        while (!empty())
            pop();
    }

    virtual int sort()
    {
        return 0;
    }

    // Mode = 1 forwards
    // Mode = 0 backwards
    void print(int mode, Customer *start, Customer *end) const
    {
        if (!(mode == 0 || mode == 1))
            return;

        if (start == nullptr || end == nullptr)
            return;

        Customer *run = start;
        while (run != end)
        {
            run->print();
            run = mode == 1 ? run->next : run->prev;
        }
        run->print();
    }

    void swap(Customer *c1, Customer *c2)
    {
        Customer temp(c1->n, c1->e, c1->t);
        c1->e = c2->e;
        c1->n = c2->n;
        c1->t = c2->t;
        c2->n = temp.n;
        c2->e = temp.e;
        c2->t = temp.t;
    }

public:
    bool containsName(string name)
    {
        if (empty())
            return false;
        Customer *run = head;
        do
        {
            if (run->n == name)
                return true;
            run = run->next;
        } while (run != head);
        return false;
    }
    // mode 0: oan linh
    // mode 1: chu thuat su
    int sum(int mode)
    {

        if (empty())
            return 0;

        Customer *run = get_head();
        int res = 0;

        do
        {
            if (mode == 0 && run->e < 0)
                res += abs(run->e);
            else if (mode == 1 && run->e > 0)
                res += run->e;
            run = run->next;
        } while (run != get_head());
        return res;
    }

    Customer *findMaxTimeCTS()
    {
        if (empty())
            return nullptr;

        Customer *start = head;
        Customer *res = start;

        do
        {
            if (start->e > 0 && start->t > res->t)
                res = start;
            start = start->next;
        } while (start != head);
        if (res->e < 0)
            return nullptr;
        return res;
    }

    Customer *findMaxTimeOL()
    {
        if (empty())
            return nullptr;

        Customer *start = head;
        Customer *res = start;

        do
        {
            if (start->e < 0 && start->t > res->t)
                res = start;
            start = start->next;
        } while (start != head);
        if (res->e > 0)
            return nullptr;
        return res;
    }

    Customer *findMinTimeCTS()
    {
        if (empty())
            return nullptr;

        Customer *start = head;
        Customer *res = nullptr;

        do
        {
            if (start->e > 0)
            {
                if (res == nullptr || start->t < res->t)
                    res = start;
            }
            start = start->next;
        } while (start != head);
        return res;
    }

    Customer *findMinTimeOL()
    {
        if (empty())
            return nullptr;

        Customer *start = head;
        Customer *res = nullptr;

        do
        {
            if (start->e < 0)
            {
                if (res == nullptr || start->t < res->t)
                    res = start;
            }
            start = start->next;
        } while (start != head);
        return res;
    }
};

class CustomerList : public List
{
public:
    Customer *xNode;
    CustomerList()
    {
        xNode = nullptr;
    }
    void insert(Customer *pos, string n, int e, int t)
    {
        Customer *newNode = new Customer(n, e, t);
        // Update xNode
        // call List::insert
        List::insert(pos, newNode);
        xNode = newNode;
    }

    void remove(Customer *n)
    {
        // update xNode
        // call List::remove
        if (n == nullptr)
            return;
        xNode = xNode->e > 0 ? n->next : n->prev;
        List::remove(n);
        if (empty())
            xNode = nullptr;
    }

    Customer *getRes(int e)
    {
        Customer *start = xNode;
        Customer *res = start;

        do
        {
            if (abs(e - start->e) > abs(e - res->e))
                res = start;
            start = start->next;
        } while (start != xNode);
        return res;
    }

    Customer *findMinTime()
    {
        Customer *start = head;
        Customer *res = start;

        do
        {
            if (start->t < res->t)
                res = start;
            start = start->next;
        } while (start != head);
        return res;
    }
};

class Queue : public List
{
public:
    Queue()
    {
    }
    void removeFront()
    {
        remove(get_head());
    }

    Customer *front()
    {
        return get_head();
    }

    int sort() override
    {
        Customer *pos = findMax();
        Customer *run = get_head();
        int n = 1;
        while (run != pos)
        {
            n++;
            run = run->next;
        }
        return shellSort(get_head(), n);
    }

public:
    int compare(Customer *a, Customer *b)
    {
        if (abs(a->e) > abs(b->e))
            return 1;
        if (abs(a->e) == abs(b->e) && a->t < b->t)
            return 1;
        return 0;
    }

    Customer *incrementNode(Customer *n, int k)
    {
        if (n == nullptr || k < 0)
            return nullptr;

        if (k == 0)
            return n;

        while (k--)
            n = n->next;

        return n;
    }

    int shellSort(Customer *begin, int n)
    {
        int count = 0;
        for (int i = n / 2; i > 2; i / 2)
        {
            for (int j = 0; j < i; j++)
            {
                count += insertionSort(incrementNode(begin, j), n - j, i);
            }
        }

        return count + insertionSort(begin, n, 1);
    }

    int insertionSort(Customer *begin, int n, int incr)
    {
        int count = 0;
        for (int i = incr; i < n; i += incr)
        {
            for (int j = i; j >= incr && compare(incrementNode(begin, j), incrementNode(begin, j - incr)); j -= incr)
            {
                swap(incrementNode(begin, j), incrementNode(begin, j - incr));
                count++;
            }
        }
        return count;
    }

    Customer *findMax()
    {
        Customer *run = get_head();
        Customer *res = run;
        do
        {
            if (abs(run->e) >= abs(res->e))
                res = run;
            run = run->next;
        } while (run != head);
        return res;
    }
};

class DumbList : public List
{
public:
    DumbList() {}

    ~DumbList()
    {
        print(0, get_tail(), get_head());
    }

    Customer *choose(Customer *inTable, Customer *inQueue)
    {
        Customer *res = nullptr;

        if (inTable && inQueue)
            res = inTable->t < inQueue->t ? inTable : inQueue;
        else
            res = inTable ? inTable : inQueue;

        append(res->n, res->e, res->t);

        return res;
    }
};

class imp_res : public Restaurant
{
public:
    CustomerList *customers;
    Queue *queue;
    int time = 0;
    int queueSize = 0;

    imp_res()
    {
        customers = new CustomerList;
        queue = new Queue;
    };

    void RED(string name, int energy)
    {
        if (energy == 0)
            return;
        if (customers->containsName(name) || queue->containsName(name))
            return;
        if (customers->full())
        {
            if (!queue->full())
                queue->append(name, energy, time++);

            return;
        }
        if (customers->empty())
        {
            customers->insert(nullptr, name, energy, time++);
            return;
        }
        if (customers->size() < customers->max_size() / 2)
        {
            if (energy >= customers->xNode->e)
                customers->insert(customers->xNode, name, energy, time++);
            else
                customers->insert(customers->xNode->prev, name, energy, time++);
        }
        else
        {
            Customer *res = customers->getRes(energy);
            if (energy - res->e < 0)
                customers->insert(res->prev, name, energy, time++);
            else
                customers->insert(res, name, energy, time++);
        }
    }

    void BLUE(int num)
    {
        while (num-- > 0 && !customers->empty())
        {
            Customer *res = customers->findMinTime();
            customers->remove(res);
        }

        while (!queue->empty() && !customers->full())
        {
            string name = queue->front()->n;
            int energy = queue->front()->e;
            int t = queue->front()->t;
            queue->removeFront();
            REDTIME(name, energy, t);
        }
    }
    void REDTIME(string name, int energy, int t)
    {
        if (energy == 0)
            return;
        if (customers->containsName(name) || queue->containsName(name))
            return;
        if (customers->full())
        {
            if (!queue->full())
                queue->append(name, energy, t);

            return;
        }
        if (customers->empty())
        {
            customers->insert(nullptr, name, energy, t);
            return;
        }
        if (customers->size() < customers->max_size() / 2)
        {
            if (energy >= customers->xNode->e)
                customers->insert(customers->xNode, name, energy, t);
            else
                customers->insert(customers->xNode->prev, name, energy, t);
        }
        else
        {
            Customer *res = customers->getRes(energy);
            if (energy - res->e < 0)
                customers->insert(res->prev, name, energy, t);
            else
                customers->insert(res, name, energy, t);
        }
    }
    void PURPLE()
    {
        if (queue->size() < 2)
            return;

        int numSwap = queue->sort();
        // LIGHT(0);
        BLUE(numSwap % queue->max_size());
    }

    void REVERSAL()
    {
        if (customers->size() < 2)
            return;
        List revPositive;
        List revNegative;
        Customer *start = customers->xNode;

        do
        {
            if (start->e < 0)
                revNegative.append(start->n, start->e, start->t);
            else
                revPositive.append(start->n, start->e, start->t);
            start = start->prev;
        } while (start != customers->xNode);

        start = customers->xNode;
        Customer tempXNode(start->n, start->e, start->t);

        do
        {
            if (start->e < 0)
            {
                customers->swap(start, revNegative.get_tail());
                revNegative.pop();
            }
            else
            {
                customers->swap(start, revPositive.get_tail());
                revPositive.pop();
            }
            start = start->prev;
        } while (start != customers->xNode);

        start = customers->get_head();

        do
        {
            if (tempXNode.n == start->n)
            {
                customers->xNode = start;
                return;
            }
            start = start->next;
        } while (start != customers->get_head());
    }

    void UNLIMITED_VOID()
    {
        int k = 4;

        if (customers->size() < k)
            return;

        int res = 2147483647;
        Customer *resStart = customers->xNode;
        Customer *resEnd = customers->xNode;
        Customer *runOut = customers->xNode;
        do
        {
            Customer *runIn = runOut;
            int sum = 0, count = 0;

            do
            {
                sum += runIn->e;
                ++count;
                if (count >= k && sum <= res)
                {
                    resStart = runOut;
                    resEnd = runIn;
                    res = sum;
                }

                runIn = queue->incrementNode(runIn, 1);
            } while (runIn != runOut);

            runOut = queue->incrementNode(runOut, 1);
        } while (runOut != customers->xNode);

        Customer *run = resStart;
        Customer *min = resStart;
        do
        {
            if (run->e <= min->e)
                min = run;
            run = run->next;
        } while (run != resEnd->next);

        run = min;
        if(min==resStart)
        {
           customers->print(1,resStart,resEnd);
        }
        else if(min==resEnd)
        {
           min->print();
           customers->print(1,resStart,min->prev);
        }
        else
        {
            customers->print(1,min,resEnd);
            customers->print(1,resStart,min->prev);
        }
    }

    void DOMAIN_EXPANSION()
    {
        int sumCTS = customers->sum(1) + queue->sum(1);
        int sumOL = customers->sum(0) + queue->sum(0);
        Customer *minInTable = nullptr;
        Customer *minInQueue = nullptr;
        DumbList dumb;
        if (sumCTS >= sumOL)
        {
            while (true)
            {
                minInTable = customers->findMinTimeOL();
                minInQueue = queue->findMinTimeOL();

                if (!minInTable && !minInQueue)
                    break;

                Customer *res = dumb.choose(minInTable, minInQueue);

                if (res == minInTable)
                    customers->remove(minInTable);
                else
                    queue->remove(minInQueue);
            }
        }
        else
        {
            while (true)
            {
                minInTable = customers->findMinTimeCTS();
                minInQueue = queue->findMinTimeCTS();

                if (!minInTable && !minInQueue)
                    break;

                Customer *res = dumb.choose(minInTable, minInQueue);

                if (res == minInTable)
                    customers->remove(minInTable);
                else
                    queue->remove(minInQueue);
            }
        }
    }

    void LIGHT(int num)
    {
        if (num == 0)
        {
            if (!queue->empty())
                queue->print(1, queue->get_head(), queue->get_tail());
        }
        else
        {
            if (!customers->empty())
            {
                if (num > 0)
                    customers->print(1, customers->xNode, customers->xNode->prev);
                else
                    customers->print(0, customers->xNode, customers->xNode->next);
            }
        }
    }
};
