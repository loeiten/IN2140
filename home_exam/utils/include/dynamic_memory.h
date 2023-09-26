#ifndef HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_
#define HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_

struct Route;
struct RoutingTable;

/**
 * @brief Allocate an int array
 *
 * @param intArray The pointer to allocate the array to
 * @param n The size of the array
 * @param name Name of the array
 * @returns 0 on success, 1 on error
 */
int allocateIntArray(int **intArray, const int n, const char *name);

/**
 * @brief Free the memory from a malloced array
 *
 * @param intArray The array to free.
 *                 We're passing the pointer to the pointer as we're
 *                 modifying the original pointer
 */
void freeIntArray(int **intArray);

/**
 * @brief Allocate an int matrix
 *
 * @param intMatrix The pointer to allocate the matrix to
 * @param n The size of the nxn matrix
 * @param name Name of the matrix
 * @returns 0 on success, 1 on error
 */
int allocateIntMatrix(int ***intMatrix, const int n, const char *name);

/**
 * @brief Free the memory from a malloced array
 *
 * @param intMatrix The matrix to free.
 *                  We're passing a triple pointer as we're
 *                  modifying the original pointer
 * @param n The number of rows in the matrix
 */
void freeIntMatrix(int ***intMatrix, int n);

/**
 * @brief Allocate memory to the route array
 *
 * @param routeArray Array of the route to the shortest route from src the the
 *                   other nodes in the graph.
 *                   We're passing the pointer to the pointer as we're modifying
 *                   the original pointer
 * @param n The number of nodes in the graph
 * @param name Name of the route array
 * @returns 0 on success, 1 on error
 */
int allocateRouteArray(struct Route **routeArray, const int n,
                       const char *name);

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

/**
 * @brief Free the routing table and its contents
 *
 * @param routingTable The routing table, one for each node
 * @param n The number of nodes in the graph
 */
void freeRoutingTable(struct RoutingTable **routingTable, int n);

#endif  // HOME_EXAM_UTILS_INCLUDE_DYNAMIC_MEMORY_H_
