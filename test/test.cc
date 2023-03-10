#include "test_vector.h"
#include "test_string.h"
#include "test_list.h"
#include "test_deque.h"
#include "test_stack.h"
#include "test_queue.h"
#include "test_priority_queue.h"
#include "test_rb_tree.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
