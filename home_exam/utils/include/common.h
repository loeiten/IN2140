#ifndef HOME_EXAM_UTILS_INCLUDE_COMMON_H_
#define HOME_EXAM_UTILS_INCLUDE_COMMON_H_

/**
 * @brief Struct representing the communicated node
 */
struct CommunicatedNode {
  int address;            /**< The address of this node */
  int nNeighbors;         /**< The number of neighbors */
  int* neighborAddresses; /**< Addresses to the other neighbors */
  int* edgeWeights;       /**< Weight to the neighbors */
};

/**
 * @brief Map from index to node id
 */
struct IndexToAddress {
  int n;    /**< Number of elements */
  int* map; /**< The index to id map */
};

/**
 * @brief Get the index from an address
 *
 * @param address The address to get the index from
 * @param indexToAddress The map of index to address
 * @param index The index corresponding to the address
 * @return 0 in success, 1 on error
 */
int getIndexFromAddress(const int address,
                        const struct IndexToAddress* const indexToAddress,
                        int* index);

#endif  // HOME_EXAM_UTILS_INCLUDE_COMMON_H_
