/*
 *  closest_AVL (augmented with closest-pair AVL) tree implementation.
 *  Author: Akshay Arun Bapat.
 *  Based on materials developed by Anya Tafliovich and F. Estrada.
 */

#include "closest_AVL_tree.h"

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/*
 * Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.  Note: this
 * should be an O(1) operation.
 */
int height(closest_AVL_Node* node){
    if (node == NULL) {
        return 0;
    }
    return node->height;
}
/*
 * Returns the min value in the tree rooted at node 'node'.
 * Returns INT_MAX if 'node' is NULL.  Note: this should be an O(1) operation.
 */
int getMin(closest_AVL_Node* node){
    if(node ==NULL){
        return INT_MAX;
    }
    return node->min;
}
/*
 * Returns the max value in the tree rooted at node 'node'.
 * Returns INT_MIN if 'node' is NULL.  Note: this should be an O(1) operation.
 */
int getMax(closest_AVL_Node* node){
    if(node == NULL){
        return INT_MIN;
    }
    return node->max;
}
/*
 * Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void updateHeight(closest_AVL_Node* node){
    if (node == NULL) {
        return;
    }
    /*int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    if (leftHeight > rightHeight) {
      node->height = leftHeight + 1;
    } else {
      node->height = rightHeight + 1;
   }*/
    int leftHeight = 0;
    int rightHeight = 0;
    if (node->left != NULL) {
        leftHeight = node->left->height;
    }
    if (node->right != NULL) {
        rightHeight = node->right->height;
    }
    if (leftHeight > rightHeight) {
        node->height = leftHeight + 1;
    } else {
        node->height = rightHeight + 1;
    }
}

/*
 * Updates the min value of the tree rooted at node 'node' based on the
 * min value of its children. Note: this should be an O(1) operation.
 */
void updateMin(closest_AVL_Node* node){
    if (node == NULL) {
        return;
    }
    int minValue = node->key;
    if (node->left != NULL && node->left->min < minValue) {
        minValue = node->left->min;
    }
    if (node->right != NULL && node->right->min < minValue) {
        minValue = node->right->min;
    }
    node->min = minValue;
}

/*
 * Updates the max value of the tree rooted at node 'node' based on the
 * max value of its children. Note: this should be an O(1) operation.
 */
void updateMax(closest_AVL_Node* node){
    if (node == NULL) {
        return;
    }
    int maxValue = node->key;
    if (node->left != NULL && node->left->max > maxValue) {
        maxValue = node->left->max;
    }
    if (node->right != NULL && node->right->max > maxValue) {
        maxValue = node->right->max;
    }
    node->max = maxValue;
}
int absDiff(int a, int b) {
    if (a > b) {
        return a - b;
    } else {
        return b - a;
    }
}
/*
 * Updates the closest pair of the tree rooted at node 'node' based on the
 * values from its children. Note: this should be an O(1) operation.
 */
void updateClosestPair(closest_AVL_Node* node) {
    if (node == NULL) {
        return;
    }

    int closestDiff = INT_MAX;
    int closestLower = 0;
    int closestUpper = 0;
    int found = 0;  // Indicator to check if a closest pair is found

    // Check left subtree's closest pair
    if (node->left && node->left->closest_pair) {
        int leftDiff = absDiff(node->left->closest_pair->lower, node->left->closest_pair->upper);
        if (leftDiff < closestDiff) {
            closestDiff = leftDiff;
            closestLower = node->left->closest_pair->lower;
            closestUpper = node->left->closest_pair->upper;
            found = 1;
        }
    }

    // Check right subtree's closest pair
    if (node->right && node->right->closest_pair) {
        int rightDiff = absDiff(node->right->closest_pair->lower, node->right->closest_pair->upper);
        if (rightDiff < closestDiff) {
            closestDiff = rightDiff;
            closestLower = node->right->closest_pair->lower;
            closestUpper = node->right->closest_pair->upper;
            found = 1;
        }
    }

    // Check the pair formed with the node's key and left subtree's max
    if (node->left) {
        int diff = absDiff(node->key, node->left->max);
        if (diff < closestDiff) {
            closestDiff = diff;
            closestLower = (node->key < node->left->max) ? node->key : node->left->max;
            closestUpper = (node->key > node->left->max) ? node->key : node->left->max;
            found = 1;
        }
    }

    // Check the pair formed with the node's key and right subtree's min
    if (node->right) {
        int diff = absDiff(node->key, node->right->min);
        if (diff < closestDiff) {
            closestDiff = diff;
            closestLower = (node->key < node->right->min) ? node->key : node->right->min;
            closestUpper = (node->key > node->right->min) ? node->key : node->right->min;
            found = 1;
        }
    }

    if (found) {
        // Allocate memory for the new closest pair
        pair* newClosest = malloc(sizeof(pair));
        if (newClosest == NULL) {
            // Handle memory allocation failure
            exit(EXIT_FAILURE);
        }
        newClosest->lower = closestLower;
        newClosest->upper = closestUpper;

        // Free the old closest pair if it exists and assign the new one
        if (node->closest_pair) {
            free(node->closest_pair);
        }
        node->closest_pair = newClosest;
    } else {
        // If no closest pair is found, set the closest pair to NULL
        if (node->closest_pair) {
            free(node->closest_pair);
        }
        node->closest_pair = NULL;
    }
    /*if (node == NULL) {
        return;
    }

    int closestDiff = INT_MAX;
    pair* closest = malloc(sizeof(pair));  // Allocate memory for the closest pair

    if (node->left && node->left->closest_pair) {
        int leftDiff = absDiff(node->left->closest_pair->lower, node->left->closest_pair->upper);
        if (leftDiff < closestDiff) {
            closestDiff = leftDiff;
            closest->lower = node->left->closest_pair->lower;
            closest->upper = node->left->closest_pair->upper;
        }
    }

    if (node->right && node->right->closest_pair) {
        int rightDiff = absDiff(node->right->closest_pair->lower, node->right->closest_pair->upper);
        if (rightDiff < closestDiff) {
            closestDiff = rightDiff;
            closest->lower = node->right->closest_pair->lower;
            closest->upper = node->right->closest_pair->upper;
        }
    }

    if (node->left) {
        int diff = absDiff(node->key, node->left->max);
        if (diff < closestDiff) {
            closestDiff = diff;
            closest->lower = node->key;
            closest->upper = node->left->max;
        }
    }

    if (node->right) {
        int diff = absDiff(node->key, node->right->min);
        if (diff < closestDiff) {
            closestDiff = diff;
            closest->lower = node->key;
            closest->upper = node->right->min;
        }
    }

    // Free the old closest pair if it exists and assign the new one
    if (node->closest_pair) {
        free(node->closest_pair);
    }
    node->closest_pair = closest;
     */
}
/*
 * Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 if node is NULL.  Note: this should be
 * an O(1) operation.
 */
int balanceFactor(closest_AVL_Node* node){
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}
/*
 * Returns the result of performing the corresponding rotation in the
 * closest_AVL tree rooted at 'node'.
 */
// single rotations: right/clockwise
closest_AVL_Node* rightRotation(closest_AVL_Node* node) {
    if (node == NULL || node->left == NULL) {
        return node;  // No rotation possible if node or node's left child is NULL
    }

    closest_AVL_Node* newRoot = node->left;       // Node's left child becomes the new root of the subtree
    node->left = newRoot->right;                  // New root's right child becomes old root's left child
    newRoot->right = node;                        // Old root becomes the right child of the new root

    // After rotation, update the heights, min, max, and closest pairs
    // First update the old root because its subtree has changed
    updateHeight(node);
    updateMin(node);
    updateMax(node);
    updateClosestPair(node);

    // Then update the new root because its subtree has also changed
    updateHeight(newRoot);
    updateMin(newRoot);
    updateMax(newRoot);
    updateClosestPair(newRoot);

    return newRoot;  // Return the new root of the subtree
}
// single rotations: left/counter-clockwise
// Function to perform a left (counter-clockwise) rotation on the subtree rooted at 'node'
closest_AVL_Node* leftRotation(closest_AVL_Node* node) {
    if (node == NULL || node->right == NULL) {
        return node;  // No rotation possible if node or node's right child is NULL
    }

    closest_AVL_Node* newRoot = node->right;      // Node's right child becomes the new root of the subtree
    node->right = newRoot->left;                  // New root's left child becomes old root's right child
    newRoot->left = node;                         // Old root becomes the left child of the new root

    // After rotation, update the heights, min, max, and closest pairs
    // First update the old root because its subtree has changed
    updateHeight(node);
    updateMin(node);
    updateMax(node);
    updateClosestPair(node);

    // Then update the new root because its subtree has also changed
    updateHeight(newRoot);
    updateMin(newRoot);
    updateMax(newRoot);
    updateClosestPair(newRoot);

    return newRoot;  // Return the new root of the subtree
}

// double rotation: right/clockwise then left/counter-clockwise
closest_AVL_Node* rightLeftRotation(closest_AVL_Node* node) {
    if (node == NULL || node->right == NULL) {
        return node;  // No rotation possible if node or node's right child is NULL
    }

    // First, perform a right rotation on the right child of the node
    node->right = rightRotation(node->right);

    // Then, perform a left rotation on the node
    return leftRotation(node);
}
// double rotation: left/counter-clockwise then right/clockwise
closest_AVL_Node* leftRightRotation(closest_AVL_Node* node) {
    if (node == NULL || node->left == NULL) {
        return node;  // No rotation possible if node or node's left child is NULL
    }

    // First, perform a left rotation on the left child of the node
    node->left = leftRotation(node->left);

    // Then, perform a right rotation on the node
    return rightRotation(node);
}


// Function to find the successor of a node in an AVL tree
closest_AVL_Node* successor(closest_AVL_Node* node) {
    if (node == NULL) {
        return NULL;  // No successor if node is NULL
    }
    if (node->right != NULL) {
        closest_AVL_Node* temp = node->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }
        return temp;
    }
    return NULL;
}

/*
 * Creates and returns a closest_AVL tree node with key 'key', value 'value',
 * height 1, min and max value 'key', and left, right and closest_pair NULL.
 */
closest_AVL_Node* createNode(int key, void* value) {
    // Allocate memory for the new node
    closest_AVL_Node* node = (closest_AVL_Node*)malloc(sizeof(closest_AVL_Node));
    if (node == NULL) {
        return NULL;  // Memory allocation failed
    }

    // Initialize the node's fields
    node->key = key;             // Set the key
    node->value = value;         // Set the associated value
    node->height = 1;            // A new node is initially at height 1
    node->min = key;             // Min value is the key of the node
    node->max = key;             // Max value is also the key of the node
    node->closest_pair = NULL;   // No closest pair initially
    node->left = NULL;           // No left child
    node->right = NULL;          // No right child

    return node;  // Return the newly created node
}

/*************************************************************************
 ** Provided functions
 *************************************************************************/

void printTreeInorder_(closest_AVL_Node* node, int offset)
{
  if (node == NULL)
  {
    return;
  }
  printTreeInorder_(node->right, offset + 1);
  if (node->left == NULL && node->right == NULL)
  {
    printf("%*s %d [%d / %d / %d / NULL]\n", offset, "",
        node->key, node->height, node->min, node->max);
  }
  else
  {
    printf("%*s %d [%d / %d / %d / (%d, %d)]\n", offset, "",
        node->key, node->height, node->min, node->max,
        node->closest_pair->lower, node->closest_pair->upper);
  }
  printTreeInorder_(node->left, offset + 1);
}

void printTreeInorder(closest_AVL_Node* node)
{
  printTreeInorder_(node, 0);
}

void deleteTree(closest_AVL_Node* node)
{
  if (node == NULL)
  {
    return;
  }
  deleteTree(node->left);
  deleteTree(node->right);
  free(node);
}

/*************************************************************************
 ** Required functions
 ** Must run in O(log n) where n is the number of nodes in a tree rooted
 **  at 'node'.
 *************************************************************************/

closest_AVL_Node* search(closest_AVL_Node* node, int key)
{
    if (node == NULL || node->key == key) {
        return node;
    } else if (key < node->key) {
        return search(node->left, key);
    } else {
        return search(node->right, key);
    }
}
closest_AVL_Node* insert(closest_AVL_Node* node, int key, void* value)
{
    if (node == NULL) {
        return createNode(key, value);
    }
    if (key < node->key) {
        node->left = insert(node->left, key, value);
    } else if (key > node->key) {
        node->right = insert(node->right, key, value);
    } else {
        return node;
    }
    updateHeight(node);
    updateMin(node);
    updateMax(node);
    updateClosestPair(node);
    // 3. Get the balance factor of this node to check whether it has become unbalanced
    int balance = balanceFactor(node);
    // 4. If the node is unbalanced, then there are 4 cases to handle
    // Left Left Case
    if (balance > 1 && key < node->left->key) {
        return rightRotation(node);
    }
    // Right Right Case
    if (balance < -1 && key > node->right->key) {
        return leftRotation(node);
    }
    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }
    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }
    // Return the unchanged node pointer
    return node;
}

// Function to find the minimum node in a subtree
closest_AVL_Node* findMinNode(closest_AVL_Node* node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;  // Move to the leftmost node
    }
    return node;
}

// Function to delete a node from an AVL tree and rebalance it
closest_AVL_Node* delete(closest_AVL_Node* node, int key) {
    if (node == NULL) {
        return node;  // Key not found, return NULL
    }

    // 1. Perform the standard BST delete
    if (key < node->key) {
        // If the key to be deleted is smaller than the node's key, then it lies in the left subtree
        node->left = delete(node->left, key);
    } else if (key > node->key) {
        // If the key to be deleted is greater than the node's key, then it lies in the right subtree
        node->right = delete(node->right, key);
    } else {
        // This is the node to be deleted
        if (node->left == NULL || node->right == NULL) {
            // Node with only one child or no child
            closest_AVL_Node* temp = node->left ? node->left : node->right;
            /*
            closest_AVL_Node* temp;
            if (node->left != NULL)
                temp = node->left;
            else
                temp = node->right;
             */
            if (temp == NULL) {
                // No child case
                temp = node;
                node = NULL;
            } else {
                // One child case
                *node = *temp;  // Copy the contents of the non-empty child
            }
            free(temp);  // Free the memory of the node being removed
        } else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            closest_AVL_Node* temp = findMinNode(node->right);

            // Copy the inorder successor's data to this node
            node->key = temp->key;
            node->value = temp->value;

            // Delete the inorder successor
            node->right = delete(node->right, temp->key);
        }
    }

    // If the tree had only one node then return
    if (node == NULL) {
        return node;
    }

    // 2. Update height and balance of the current node
    updateHeight(node);
    updateMin(node);
    updateMax(node);
    updateClosestPair(node);

    int balance = balanceFactor(node);

    // 3. Rebalance the tree

    // Left Left Case
    if (balance > 1 && balanceFactor(node->left) >= 0) {
        return rightRotation(node);
    }

    // Left Right Case
    if (balance > 1 && balanceFactor(node->left) < 0) {
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }

    // Right Right Case
    if (balance < -1 && balanceFactor(node->right) <= 0) {
        return leftRotation(node);
    }

    // Right Left Case
    if (balance < -1 && balanceFactor(node->right) > 0) {
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }

    return node;  // Return the balanced node
}


/*************************************************************************
 ** Required functions
 ** Must run in O(1)
 *************************************************************************/

pair* getClosestPair(closest_AVL_Node* node)
{
    if (node == NULL) {
        return NULL;  // If the node is NULL, return NULL indicating no data available
    }
    return node->closest_pair;  // Return the pre-computed closest pair
}
