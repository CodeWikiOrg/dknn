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
 				resources. This library is dependent on "dml.h", that is another open-source library
 				that is diluted ML dedicated specifically.
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

#ifndef DML_DKNN_H
#define DML_DKNN_H

// Dilution Parameters -----------------------------------------------------------
#define SPREAD 					(1.442)  	//default value (1/0.69) which is (1/ln(2))
#define OVERCONFIDENCE 			(10.000) 	//default value 1 (bit)
#define MAP_RESOLUTION			(64)		//default value 64
#define DILUTION_RES			(1)		    //default value 1 byte which corresponds to unsigned char


// Hyper Parameters --------------------------------------------------------------
#define EPOCH 					(1000)	    //default value 1000
#define BATCH_SIZE				(50)	    //default value 50

typedef struct dataPointType
{
    float xCoord;
    float yCoord;
    int class;
} dataPoint_t;

typedef struct classCenterType
{
    float xCoord;
    float yCoord;
} classCenter_t;

typedef struct dilutionParameters
{
    float spread;
    float overconfidence;
} dilPar_t;

void initDilutionParameters(dilPar_t *dataPoint);
void initClassCenter(classCenter_t *class);
int dropIncompleteBatch(dataPoint_t *dataPoint);
void modifyDilutionPars(dilPar_t DP[], int class, float distance);
void setCircleCenters(dataPoint_t dataPack[], classCenter_t classCenter[], int *points[]);
void setWeightedCircleCenters(dataPoint_t dataPack[], classCenter_t classCenter[], int *points[]);
float square(float baseNumber);
float calcDistance(dataPoint_t one, classCenter_t classCenter);
float baseFunction(float distance, dilPar_t dilutionPars);
int checkOverConfidenceCircle(float distance, dilPar_t dilutionPars);
int classifyDataPoint(dataPoint_t *dataPoint, dilPar_t DPs[], classCenter_t CCs[], int argNum);

#endif //DML_DKNN_H
