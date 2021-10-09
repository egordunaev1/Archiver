#include "../src/Archiver/priority_queue.h"
#include <gtest/gtest.h>
#include <set>

TEST(PriorityQueue, PushAndPopElements) {
    for (int cur_test = 0; cur_test < 100; ++cur_test) {
        PriorityQueue<int> priorityQueue;
        std::set<int>test;

        for (int i = 0; i < rand() % 100 + 1; i++) {
            int cmd = rand() % 2;
            if (cmd == 0) {
                int a = rand();
                test.insert(a);
                priorityQueue.push(a);
            } else {
                int x = -1, y = -1;
                if (test.size()) {
                    x = *test.begin();
                    test.erase(test.begin());
                }
                if (priorityQueue.size()) {
                    y = priorityQueue.pop();
                }
                ASSERT_EQ(x, y);
            }
        }
    }
}