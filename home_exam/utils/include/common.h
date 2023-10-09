#ifndef HOME_EXAM_UTILS_INCLUDE_COMMON_H_
#define HOME_EXAM_UTILS_INCLUDE_COMMON_H_

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
