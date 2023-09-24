#ifndef HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_
#define HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_

struct Route;

/**
 * @brief Free the memory from a malloced array
 *
 * @param array The array to free.
 *              We're passing the pointer to the pointer as we're
 *              modifying the original pointer
 */
void freeIntArray(int **array);

/**
 * @brief Free the malloced memory form the route array
 *
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 *                   We're passing the pointer to the pointer as we're modifying
 *                   the original pointer
 * @param n The number of nodes in the graph
 */
void freeRouteArray(struct Route **routeArray, const int n);

#endif  // HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_
