/******************************************************************************
 * Copyright 2020 The Firmament Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef __MAVLINK_STATUS_H__
#define __MAVLINK_STATUS_H__

#include <firmament.h>

typedef enum {
	CAL_START_GYRO,
	CAL_START_ACC,
	CAL_START_MAG,
	CAL_START_LEVEL,
	CAL_DONE,
	CAL_FAILED,
	CAL_PROGRESS_0,
	CAL_PROGRESS_10,
	CAL_PROGRESS_20,
	CAL_PROGRESS_30,
	CAL_PROGRESS_40,
	CAL_PROGRESS_50,
	CAL_PROGRESS_60,
	CAL_PROGRESS_70,
	CAL_PROGRESS_80,
	CAL_PROGRESS_90,
	CAL_PROGRESS_100,
	CAL_UP_DETECTED,
	CAL_DOWN_DETECTED,
	CAL_LEFT_DETECTED,
	CAL_RIGHT_DETECTED,
	CAL_FRONT_DETECTED,
	CAL_BACK_DETECTED,
	CAL_UP_DONE,
	CAL_DOWN_DONE,
	CAL_LEFT_DONE,
	CAL_RIGHT_DONE,
	CAL_FRONT_DONE,
	CAL_BACK_DONE,
	INIT_WELCOME,
	INIT_INIT_START,
	INIT_INIT_FINISH,
	INIT_HEATING,
	INIT_HEAT_FINISH,
	INIT_HEAT_DISABLE,
	SENSOR_CHECK_START,
	SENSOR_CHECK_ERROR,
	SENSOR_CHECK_FINISH,
	SENSOR_GYRO_UNDETECTED,
	SENSOR_GYRO_UNCALI,
	SENSOR_GYRO_NEED_CALI,
	SENSOR_GYRO_OK,
	SENSOR_ACC_UNCALI,
	SENSOR_ACC_NEED_CALI,
	SENSOR_ACC_OK,
	SENSOR_MAG_UNDETECTED,
	SENSOR_MAG_UNCALI,
	SENSOR_MAG_NEED_CALI,
	SENSOR_MAG_OK,
	SENSOR_BARO_UNDETECTED,
	SENSOR_BARO_OK,
	SENSOR_GPS_UNDETECTED,
	SENSOR_GPS_OK,
	MAV_NOTICE_NUM
} mav_status_type;

typedef struct {
	uint8_t severity;
	char*   string;
} mav_status_t;

mav_status_t mavlink_get_status_content(uint8_t status);

#endif

