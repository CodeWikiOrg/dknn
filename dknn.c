/**
 * Author(s):           Arda T. Kersu
 * File name:           dknn.h
 * Date:                30th November 2023
 *
 * Description: Header file for the library "dknn.h". Includes declarations of functions that are
                tailored to fit the needs for the supervised classification algorithm "diluted kNN".
                Diluted kNN is a different approach to the original kNN algorithm, and it aims to
                be small sized (under 32 kB), decrease RAM usage, and cut overall hardware requirements
                so that the algorithm could be deployed on microcontrollers with extremely scarce
                resources.
 *
 * Disclaimer: This open-source project is provided "as is" without any warranty, expressed or implied.
 *             The contributors and maintainers disclaim any an all unintended consequences or issues
 *             that may arise from its use. Users are encouraged to review and understand the software's
 *             characteristics before implementation.
 *
 *
 * Copyright @ [Arda T. Kersu]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dknn.h"

#define NUM_OF_CLASSES      (4)

/*int handledBatches[3] = {0, 0, 0}; //resting, training, and panic
dilPar_t RDP, TDP, PDP; //dilution parameters of resting, training and panic cases.
classCenter_t RC, TC, PC;*/

/**
 * @brief Initialize dilution parameters for a data point.
 *
 * This function initializes the dilution parameters for a given data point represented
 * by the 'dilPar_t' structure 'dataPoint'. It sets the 'spread' and 'overconfidence'
 * fields to predefined constants.
 *
 * @param dataPoint A pointer to the 'dilPar_t' structure representing the data point.
 *
 * @note This function initializes the 'spread' and 'overconfidence' fields of the data point.
 *
 * @code
 *   // Example usage:
 *   dilPar_t dataPoint;
 *   initDilutionParameters(&dataPoint);
 *   // The 'spread' and 'overconfidence' fields of 'myDataPoint' are now initialized.
 * @endcode
 */
void initDilutionParameters(dilPar_t *dataPoint)
{
    dataPoint->spread = SPREAD;
    dataPoint->overconfidence = OVERCONFIDENCE;
}

/**
 * @brief Initialize the center coordinates of a class.
 *
 * This function initializes the center coordinates (x and y) of a class represented
 * by the 'classCenter_t' structure 'class' to 0.0.
 *
 * @param class A pointer to the 'classCenter_t' structure representing the class.
 *
 * @note This function sets the x and y coordinates of the class center to 0.0.
 *
 * @code
 *   // Example usage:
 *   classCenter_t class;
 *   initClassCenter(&class);
 *   // The center coordinates of 'myClass' are now initialized to (0.0, 0.0).
 * @endcode
 */
void initClassCenter(classCenter_t *class)
{
    class->xCoord = (float)0.0;
    class->yCoord = (float)0.0;
}

/**
 * @brief Check for and drop an incomplete batch of data points.
 *
 * This function checks if a batch of data points represented by the 'dataPoint_t'
 * structure 'dataPoint' is incomplete.
 *
 * @param dataPoint A pointer to the 'dataPoint_t' structure representing the batch of data points.
 *
 * @return Returns 1 if the batch is incomplete (a dataPoint is NULL), otherwise returns 0.
 *
 * @code
 *   // Example usage:
 *   dataPoint_t* batch = getDataBatch(); // Assuming 'dataPoint_t' represents a data batch.
 *   if (dropIncompleteBatch(batch))
 *   {
 *       // Drop operation.
 *   }
 * @endcode
 */
int dropIncompleteBatch(dataPoint_t *dataPoint)
{
    int retVal;

    retVal = (dataPoint == NULL) ? 1 : 0;
    (void)puts("INVALID BATCH FOUND ******************************");

    return retVal;
}

/**
 * @brief Modify dilution parameters based on class and distance.
 *
 * This function modifies the dilution parameters for a given class represented by the 'dilPar_t'
 * array 'DP' based on the provided 'class' and 'distance' values. The modification depends on the
 * class and the comparison of 'distance' with the overconfidence value of the corresponding class.
 *
 * @param DP       An array of 'dilPar_t' structures representing dilution parameters.
 * @param class    The class identifier (0 for resting pulse, 1 for training pulse, 2 for panic pulse).
 * @param distance The distance value used for comparison.
 *
 * @note The function modifies the 'spread' or 'overconfidence' fields of the dilution parameters
 *       based on the class and the comparison with 'distance'. Constants for modifying parameters
 *       are defined as SPREAD_H, SPREAD_L, OVRCNF_H, and OVRCNF_L.
 *
 * @code
 *   // Example usage:
 *   dilPar_t dilutionParams[2]; // Assuming 'dilPar_t' is a valid data structure.
 *   int classIdentifier = 1; // Class identifier for training pulse.
 *   float distanceValue = 0.03; // Sample distance value.
 *   modifyDilutionPars(dilutionParams, classIdentifier, distanceValue);
 *   // Modify dilution parameters based on class and distance...
 * @endcode
 */
#define SPREAD_H      (0.0100)
#define SPREAD_L      (0.0050)
#define OVRCNF_H      (0.0500)
#define OVRCNF_L      (0.2500)
void modifyDilutionPars(dilPar_t DP[], int class, float distance)
{
    if(class == 0) //if resting pulse data
    {
        if(distance > DP[0].overconfidence)
        {
            DP[0].spread += (float)SPREAD_H;
        }
        else if(distance < DP[0].overconfidence)
        {
            DP[0].overconfidence += (float)OVRCNF_H;
        }
        else
        {
            //
        }
    }
    else if(class == 1) //if training pulse data
    {
        if(distance > DP[1].overconfidence)
        {
            DP[1].spread += (float)SPREAD_H;
        }
        else if(distance < DP[1].overconfidence)
        {
            DP[1].overconfidence += (float)OVRCNF_H;
        }
        else
        {
            //
        }
    }
    else if(class == 2) //if panic pulse data
    {
        if(distance > DP[2].overconfidence)
        {
            DP[2].spread += (float)SPREAD_H;
        }
        else if(distance < DP[2].overconfidence)
        {
            DP[2].overconfidence += (float)OVRCNF_H;
        }
        else
        {
            //
        }
    }
    else
    {
        //
    }
}
#undef SPREAD_H
#undef SPREAD_L
#undef OVRCNF_H
#undef OVRCNF_L

/**
 * @brief Set the center coordinates for different classes based on data points.
 *
 * This function calculates and sets the center coordinates for different classes
 * based on the provided data points and updates the corresponding class centers.
 *
 * @param dataPack    An array of 'dataPoint_t' structures representing data points.
 * @param classCenter An array of 'classCenter_t' structures representing class centers.
 * @param points      An array of integer pointers to track the number of points in each class.
 *
 * @note The function calculates the center coordinates for each class separately based on
 *       the provided data points. It updates the class center coordinates and increments
 *       the count of points in each class.
 *
 * @code
 *   // Example usage:
 *   dataPoint_t dataPoints[BATCH_SIZE]; // Assuming 'dataPoint_t' represents data points.
 *   classCenter_t classCenters[3]; // Assuming 'classCenter_t' represents class centers.
 *   int pointCounts[3] = {0, 0, 0}; // Array to track point counts for each class.
 *   setCircleCenters(dataPoints, classCenters, pointCounts);
 *   // Calculate and set class centers based on data points...
 * @endcode
 */
void setCircleCenters(dataPoint_t dataPack[], classCenter_t classCenter[], int *points[])
{
    float centerCoords[2] = {0, 0};
    float centerWeight[3] = {0, 0, 0};


    for(int loopVar=0; loopVar < BATCH_SIZE; loopVar++) //calculate center
    {
        if(dataPack[loopVar].class == 0)
        {
            if(centerWeight[0] != 0)
            {
                centerCoords[0] = ((centerWeight[0] * classCenter[0].xCoord) + dataPack[loopVar].xCoord) / (centerWeight[0] + 1);
                centerCoords[1] = ((centerWeight[0] * classCenter[0].yCoord) + dataPack[loopVar].yCoord) / (centerWeight[0] + 1);
                centerWeight[0]++;
            }
            else //if(centerWeight == 0)
            {
                centerCoords[0] = dataPack[loopVar].xCoord;
                centerCoords[1] = dataPack[loopVar].yCoord;
                centerWeight[0]++;
            }

            //assign center coords
            classCenter[0].xCoord = centerCoords[0];
            classCenter[0].yCoord = centerCoords[1];
            //clean coordinate buffer
            centerCoords[0] = 0;
            centerCoords[1] = 0;

            *points[0]++;
        }

        if(dataPack[loopVar].class == 1)
        {
            if(centerWeight[1] != 0)
            {
                centerCoords[0] = ((centerWeight[1] * classCenter[1].xCoord) + dataPack[loopVar].xCoord) / (centerWeight[1] + 1);
                centerCoords[1] = ((centerWeight[1] * classCenter[1].yCoord) + dataPack[loopVar].yCoord) / (centerWeight[1] + 1);
                centerWeight[1]++;
            }
            else //if(centerWeight == 0)
            {
                centerCoords[0] = dataPack[loopVar].xCoord;
                centerCoords[1] = dataPack[loopVar].yCoord;
                centerWeight[1]++;
            }

            //assign center coords
            classCenter[1].xCoord = centerCoords[0];
            classCenter[1].yCoord = centerCoords[1];
            //clean coordinate buffer
            centerCoords[0] = 0;
            centerCoords[1] = 0;

            *points[1]++;
        }

        if(dataPack[loopVar].class == 2)
        {
            if(centerWeight[2] != 0)
            {
                centerCoords[0] = ((centerWeight[2] * classCenter[2].xCoord) + dataPack[loopVar].xCoord) / (centerWeight[2] + 1);
                centerCoords[1] = ((centerWeight[2] * classCenter[2].yCoord) + dataPack[loopVar].yCoord) / (centerWeight[2] + 1);
                centerWeight[2]++;
            }
            else //if(centerWeight == 0)
            {
                centerCoords[0] = dataPack[loopVar].xCoord;
                centerCoords[1] = dataPack[loopVar].yCoord;
                centerWeight[2]++;
            }

            //assign center coords
            classCenter[2].xCoord = centerCoords[0];
            classCenter[2].yCoord = centerCoords[1];
            //clean coordinate buffer
            centerCoords[0] = 0;
            centerCoords[1] = 0;

            *points[2]++;
        }
    }
}

/**
 * @brief Calculate the square of a given number.
 *
 * This function calculates the square of the provided 'baseNumber' and returns the result.
 *
 * @param baseNumber The number for which to calculate the square.
 *
 * @return The square of the 'baseNumber'.
 *
 * @code
 *   // Example usage:
 *   float number = 3.0;
 *   float result = square(number);
 *   // 'result' will be 9.0.
 * @endcode
 */
float square(float baseNumber)
{
    return (baseNumber * baseNumber);
}

/**
 * @brief Calculate the Euclidean distance between a data point and a class center.
 *
 * This function calculates the Euclidean distance between a data point represented by
 * 'one' and a class center represented by 'classCenter'. It returns the calculated distance.
 *
 * @param one         A 'dataPoint_t' structure representing a data point.
 * @param classCenter A 'classCenter_t' structure representing a class center.
 *
 * @return The Euclidean distance between the data point and the class center.
 *
 * @code
 *   // Example usage:
 *   dataPoint_t dataPoint; // Assuming 'dataPoint_t' represents a data point.
 *   classCenter_t center; // Assuming 'classCenter_t' represents a class center.
 *   float distance = calcDistance(dataPoint, center);
 *   // Calculate the Euclidean distance between 'dataPoint' and 'center'...
 * @endcode
 */
float calcDistance(dataPoint_t one, classCenter_t classCenter)
{
    float dividend = square(fabsf(one.xCoord - classCenter.xCoord));
    float divisor = square(fabsf(one.yCoord - classCenter.yCoord));

    return sqrtf(dividend + divisor);
}

/**
 * @brief Update the base function value.
 *
 * This function updates the value of the base function based on the provided
 * 'distance' and 'dilutionPars' (dilution parameters).
 *
 * @param distance     The distance value used in the calculation.
 * @param dilutionPars A 'dilPar_t' structure representing dilution parameters.
 *
 * @return The computed value of the base function.
 *
 * @code
 *   // Example usage:
 *   float distanceValue = 0.5; // Sample distance value.
 *   dilPar_t dilutionParameters; // Assuming 'dilPar_t' represents dilution parameters.
 *   float baseFuncValue = baseFunction(distanceValue, dilutionParameters);
 *   // Calculate the base function value...
 * @endcode
 */
float baseFunction(float distance, dilPar_t dilutionPars)
{
    return expf((-1 * fabsf(distance - dilutionPars.overconfidence)) / dilutionPars.spread);
}

/**
 * @brief Check if a given distance falls within the overconfidence circle.
 *
 * This function checks whether a given 'distance' value is less than or equal to
 * the overconfidence radius defined by 'dilutionPars.overconfidence'. It returns
 * 1 if the distance is within the overconfidence circle and 0 otherwise.
 *
 * @param distance          The distance value to be checked.
 * @param dilutionPars      A 'dilPar_t' structure representing dilution parameters.
 *
 * @return Returns 1 if the distance is within the overconfidence circle, otherwise returns 0.
 *
 * @code
 *   // Example usage:
 *   float distanceValue = 0.8; // Sample distance value.
 *   dilPar_t dilutionParameters; // Assuming 'dilPar_t' represents dilution parameters.
 *   int isWithinCircle = checkOverConfidenceCircle(distanceValue, dilutionParameters);
 *   // Check if the distance is within the overconfidence circle...
 * @endcode
 */
int checkOverConfidenceCircle(float distance, dilPar_t dilutionPars)
{
    int retVal;

    retVal = (dilutionPars.overconfidence >= distance) ? 1 : 0;

    return retVal;
}

/**
 * @brief Classify a data point using DkNN, into one of multiple classes based on confidence scores.
 *
 * This function classifies a given 'dataPoint' into one of multiple classes represented by
 * the 'DPs' (dilution parameters) and 'CCs' (class centers) arrays. It calculates confidence
 * scores for each class and determines the class with the highest confidence.
 *
 * @param dataPoint A pointer to the 'dataPoint_t' structure representing the data point to classify.
 * @param DPs       An array of 'dilPar_t' structures representing dilution parameters for each class.
 * @param CCs       An array of 'classCenter_t' structures representing class centers for each class.
 * @param argNum    The number of classes to classify the data point into.
 *
 * @return The index of the class with the highest confidence for the input data point.
 *
 * @code
 *   // Example usage:
 *   dataPoint_t myDataPoint; // Assuming 'dataPoint_t' represents a data point.
 *   dilPar_t dilutionParams[3]; // Array of dilution parameters for each class.
 *   classCenter_t classCenters[3]; // Array of class centers for each class.
 *   int numClasses = 3; // Number of classes to classify the data point into.
 *   int classIndex = classifyDataPoint(&myDataPoint, dilutionParams, classCenters, numClasses);
 *   // Classify the data point and get the index of the class with the highest confidence...
 * @endcode
 */
int classifyDataPoint(dataPoint_t *dataPoint, dilPar_t DPs[], classCenter_t CCs[], int argNum)
{
    int retVal = 0;
    float maxConf = 0;

    (void)printf("results for test data at [%f, %f]:\n", dataPoint->xCoord, dataPoint->yCoord);

    for(int index=0; index<argNum; index++)
    {
        float indexResult;

        float distance = calcDistance(*dataPoint, CCs[index]);
        int checkOCC = checkOverConfidenceCircle(distance, DPs[index]);

        if(checkOCC == 1)
        {
            indexResult = 1;
        }
        else
        {
            indexResult = baseFunction(distance, DPs[index]);
        }

        (void)printf("class %d has confidence value of %f\n", index+1, indexResult);

        if(index == 0)
        {
            maxConf = indexResult;
            retVal = 0;
        }
        else
        {
            retVal = (maxConf < indexResult) ? index : retVal;
            maxConf = (maxConf < indexResult) ? indexResult : maxConf;
        }
    }
    (void)printf("input data belongs to class %d\tconfidence: %f\n", retVal, maxConf);
    (void)puts(" ");

    return retVal;
}
