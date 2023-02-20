#include "test_vector.h"
#include "test_string.h"
#include "test_list.h"
#include "test_deque.h"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
