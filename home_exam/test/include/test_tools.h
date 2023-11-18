#ifndef HOME_EXAM_TEST_INCLUDE_TEST_TOOLS_H_
#define HOME_EXAM_TEST_INCLUDE_TEST_TOOLS_H_

struct Node;
struct RoutingTableArray;

/**
 * @brief Create an Inverted "A" Graph
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

/**
 * @brief Create the routing table array for an "I" graph
 *
 * The "I" graph is as follows:
 *
 * [0] 7   [1]  22  [2]
 *  o ----- o ----- o
 * 101     15      42
 *
 * The weight are written on the edges
 * The ids are written on the vertices
 * The indices of the \a addressMap are written in square brackets
 *
 * @param routingTableArray The routing table array to be created
 * @param addressMap The address map
 *                  Index 0 of the map refers to the actual index of address 101
 *                  Index 1 of the map refers to the actual index of address 15
 *                  Index 1 of the map refers to the actual index of address 42
 * @param name The
 * @return 0 on success, 1 on error
 */
int createIGraphRoutingTableArray(struct RoutingTableArray* routingTableArray,
                                  const int* const addressMap,
                                  const char* name);

#endif  // HOME_EXAM_TEST_INCLUDE_TEST_TOOLS_H_
