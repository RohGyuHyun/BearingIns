//#pragma once
#include <mil.h>
#include <math.h>
#include "share.h"
#include "_FuncModuleBasic.h"
#include "VisionModuleBearing.h"
#include "UMechaProtocol.h"
#include "IniEx.h"
#include <process.h>
#include "btnenh.h"

//#define LIGHT
#ifdef DCF
#define LIGHT
#define RESULT_SAVE
#define RELEAS
#endif

#define RESULT_FILE_NAME			"_Result.csv"

#define TRIGGER_BOARD

#define MAX_STRING_LINE					30000

//#define WIDTH_INSPECTION_SIZE_X	50
#define WIDTH_INSPECTION_SIZE_Y	50

#define HEIGHT_INSPECTION_SIZE_X	50
#define HEIGHT_INSPECTION_SIZE_Y	50

#define ALIGN_REGION				0
#define ALIGN_SEARCH_REGION_1		1
#define INSPECTION_NIDL_NG_REGION	3
#define INSPECTION_NIDL_REGION		4

#define INSPECTION_TYPE_WIDTH		0
#define INSPECTION_TYPE_HEIGHT		1
#define INSPECTION_TYPE_AREA		2
#define INSPECTION_TYPE_SPACE		3
#define INSPECTION_TYPE_NUN_2		4

#define SETUP_MODEL					0
#define SETUP_LIGHT					1
#define SETUP_ALIGN					2
#define SETUP_TEACHING				3
#define SETUP_SYSTEM				4
#define SETUP_TEST_2				5

#define MIN_LIGHT					0
#define MAX_LIGHT					99

#define LIGHT_0						1
#define LIGHT_1						2
#define LIGHT_2						3
#define LIGHT_3						4

#define LINE_LIGHT_COM_PORT			7
#define LINE_LIGHT_COM_BAUDRATE		9600

#define LINE_LIGHT_CNT				2
#define L_LIGHT_PACKET_SIZE			14

#define NG_IMAGE_WIDTH				455
#define NG_IMAGE_HEIGHT				298

#define MAX_INPUT					6
#define MAX_OUTPUT					6

#define COMI_INPUT_START			0
#define COMI_INPUT_NULL_0			1
#define COMI_INPUT_NULL_1			2
#define COMI_INPUT_NULL_2			3

#define COMI_OUTPUT_OK1				0
#define COMI_OUTPUT_NG1				1
#define COMI_OUTPUT_OK2				2
#define COMI_OUTPUT_NG2				3
#define COMI_OUTPUT_NULL1			4
#define COMI_OUTPUT_NULL2			5

#define ALIGN_TYPE_SEARCH			0
#define ALIGN_TYPE_PATTERN			1

#define INSPECTION_FAIL				-1
#define RESULT_TYPE_OK				0
#define RESULT_TYPE_MISS_NG			1
#define RESULT_TYPE_WIDTH_NG		2
#define RESULT_TYPE_CENTER_WIDTH_NG	3
#define RESULT_TYPE_PITCH_NG		4
#define RESULT_TYPE_BROKEN_NG		5
//#define RESULT_TYPE_OK				6
//#define RESULT_TYPE_OK				7
//#define RESULT_TYPE_OK				8
//#define RESULT_TYPE_OK				9
