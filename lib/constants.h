#define RANGE 500  /*!< Range of values in genV*/
#define RRANGE  40 /*!< Range of values in genR. Higher values of rrange will increase the over all security of the encryption*/
#define SCALE 0.04 /*!< Percentage of setting the values to 1 or -1 in Randomline*/
#define DEPTH 2   /*!< Number of times genU traverses the matrix to give more complexity to the unimodular. Increasing depth will increase the time of unimodular creation.*/
#define SIZE 400  /*!< Size of the matrix.*/
/*! \file constants.h
 *  \brief Stores the global constants of the GGH protocol.
 *  \ingroup methods
*/

/* Good Attempts
Attempt 1: 
#define RANGE 500
#define RRANGE  40
#define SIZE 200     
#define SCALE 0.04 
#define DEPTH 2 (lowest possible value)
*/
