#ifndef HOME_EXAM_TEST_INCLUDE_TEST_TOOLS_H_
#define HOME_EXAM_TEST_INCLUDE_TEST_TOOLS_H_

struct Node;

/**
 * @brief Create a Inverted "A" Graph
 *
 * The inverted "A" graph is as follows:
 *
 *   10 [2]   17 [4]
 *   1 |   2    | 1
 *   3 [1] - 5 [3]
 *   1  \     /  3
 *       1 [0]
 *
 * The weight are written on the edges
 * The ids are written on the vertices
 * The indices are written in square brackets
 *
 * @param nodeArray The node array to be filled
 * @return 0 on success, 1 on error
 */
int createInvertedAGraphNodeArray(struct Node** nodeArray);

#endif  // HOME_EXAM_TEST_INCLUDE_TEST_TOOLS_H_
